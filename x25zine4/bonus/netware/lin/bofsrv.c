#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PASSOK "NETWARE"

int authit(char *p){
    char pass[32];
    int res=0;

    strcpy(pass,p+5);
    printf("PASS: %s",pass);
    if (strncmp(pass,PASSOK,strlen(PASSOK))==0){res=1;};

    return res;
};



int main() { 
    struct sockaddr_in addr;
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
    addr.sin_family=AF_INET;

    addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);    //inet_aton("172.17.1.250");
    addr.sin_port=htons(100);
    
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

