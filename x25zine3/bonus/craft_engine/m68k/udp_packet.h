packet:
       /* ethernet - 14 bytes       */
.ascii "\x00\x10\xa4\x10\xe8\x1d"   /* mac dest    00:10:a4:10:e8:1d (1.1)*/
/* .ascii "\x00\x80\xad\x90\x5f\x64" */  /* mac dest    00:80:ad:90:5f:64 (1.3)*/
.ascii "\x00\x80\xad\xc6\x18\xbb"   /* mac source  00:80:ad:c6:18 :bb (1.2)*/
.ascii "\x08\x00"                   /* mac options 0806 - ARP, 0800 - IP  */
       /* ip header - 20 bytes      */
.ascii "\x45"                       /* version:4, ihl:5                   */
.ascii "\x00"                       /* type of service:                   */
.ascii "\x00\x1c"                   /* total length ip header + data(auto) */
.ascii "\x00\x00"                   /* ID       	     	          */
.ascii "\x00\x00"                   /* flags                              */
.ascii "\x80"                       /* TTL                   : 255        */
.ascii "\x11"                       /* protocol (17(0x11)=UDP)            */	         
.ascii "\xb7\x7e"                   /* checksum (auto)                    */
.ascii "\xc0\xa8\x01\x03"           /* ip source       : 192.168.1.1      */
.ascii "\xc0\xa8\x01\x01"           /* ip destination  : 192.168.1.1      */
       /* udp header - 8 bytes      */
.ascii "\x04\x00"                   /* source port 2000                     */
.ascii "\x00\x35"                   /* destantion port 53 (dns)             */
.ascii "\x00\x00"                   /* total length udp header + data(auto) */
.ascii "\x00\x00"                   /* checksum (auto)                      */
     /* ---- udp data ---- */
         /* dns header */
.ascii "\x00\x00"                /* ID                                       */ 

.byte 00000001                   /* 1 bit (QR) - 0 query, 1 - answer         */
                                 /* 2,3,4,5 (OPCODE)  : 0 - standart query   */
				 /*                     1 - an inversy query */
				 /*                     2 - server status    */
				 /*                  3-15 - _reserved_ ? ;)  */
				 /* 6 bit (AA) - 0 - query, 1 - answer       */
				 /* 7 bit (TC) - 0 if not trucated, 1 if yes */
				 /* 8 bit (RD) - 1- recursive, 0 -not        */

.byte 00000000                   /* 1 bit (RA) - answer about recurse    */
				 /* 2 bit (Z)  - must be 0?;) (reserve)  */
				 /* 3,4,5,6 (RCODE) - error code. 0-good */
				 /* 7,8 - reserved ? ;)                  */

.ascii "\x00\x01"                /* QDCOUNT - how many domains in my packet */
.ascii "\x00\x00"                /* ANCOUNT - how many field about domen    */
.ascii "\x00\x00"                /* NSCOUNT - how many nameservers          */
.ascii "\x00\x00"                /* ARCOUNT - how many nameserver ips       */  
        /* dns data - QUSTION section */			  				       
.ascii "\x03" "www" "\x06" "phrack" "\x03" "org" "\x00" 

.ascii "\x00\x01"
.ascii "\x00\x01"
size_packet = .-packet
