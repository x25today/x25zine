#define N_PLAT_WNT4
#define N_ARCH_32
#include <stdlib.h>
#include <stdio.h> 
#include <ntypes.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PASSOK "NETWARE"

int res=0;

int authit(char *p){
    char pass[32];

    strcpy(pass,p+5);
    if (strncmp(pass,PASSOK,strlen(PASSOK))==0){res=1;};

//    EnterDebugger();
    return res;
};



int main() { 
    struct sockaddr addr;
    int addrlen;
    int s,s2; 
    char auth[]="AUTH PLZ!\n";
    char buf[256];


    if((s=socket(AF_INET, SOCK_STREAM, 0))<0) {
        printf("socket#1");
        return -1;
    }

    addrlen=sizeof(addr);
    memset(&addr, 0, addrlen);
    addr.sa_family=AF_INET;

    memset(addr.sa_data,0,sizeof(addr.sa_data));
    addr.sa_data[0]=0x07; //  port 2000
    addr.sa_data[1]=0xd0;
    
    printf("do bind\n");

    if(bind(s, (struct sockaddr *)&addr, addrlen)){
        printf("bind#1\n");
	return -1;
    };

    printf("do listen\n");

    if ( listen(s, 5) ) {
        printf("listen");
	return -1;
    };

    printf("do accept\n");


while(1){

    if((s2=accept(s, (struct sockaddr *)&addr, &addrlen))<0) {
        printf("accept");
	return -1;
    }

    printf("client connected\n");

    memset(buf,0,sizeof(buf));

    send(s2,auth,sizeof(auth),0);
    recv(s2,buf,256,0);

    printf("USER DATA: %s",buf);    
    
    if (strncmp(buf,"AUTH ",5)==0){
        if (authit(buf)){ 
	    printf("AUTH PASSED !\n");
	    send(s2,"AUTH PASSED !\n",14,0);
	}else {
	    printf("AUTH FAILED !\n");
	    send(s2,"AUTH FAILED !\n",14,0);
	};
    }else{
	printf("BAD protocol \n");
	send(s2,"BAD protocol \n",14,0);
    };
        
    close(s2); 
};
    close(s); 

    return 0;
}

