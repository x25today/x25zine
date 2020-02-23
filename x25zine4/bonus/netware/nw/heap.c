#define N_PLAT_WNT4
#define N_ARCH_32
#include <stdlib.h>
#include <stdio.h> 
#include <ntypes.h>


int main(void)
{  
   char *i,*j,*k,*gp;
   char buf[2048];
   unsigned long t=0,g2,g;
   int ii,t0,t1,t2,t3;
   int k0,k1,k2,k3;

   printf("\n Hello World!\n");
   i=malloc(100);
   memset(i,'A',100);
   EnterDebugger();
   j=malloc(100);
   memset(j,'B',100);
   k=malloc(100);
   memset(k,'C',100);


   i=i-8;
   k0=i[0];
   k1=i[1];
   k2=i[2];
   k3=i[3];
   t0=i[4];
   t1=i[5];
   t2=i[6];
   t3=i[7];

   g2=i[7]*256*256*256;
   g2+=i[6]*256*256;
   g2+=i[5]*256;
   g2+=i[4];

   i=i+8;

//   t=i;
//   t=t & 0xFFFFF000;

   g2=g2-1+4;
   gp=g2;
   printf ("memory : %x %x %x %x\n",i,j,k,g2);


//   for(ii=0;ii<3000;ii++){
//          i[ii]=0x41;
//   };


//g2 - adres na desc proc

   j=j-8;

   j[0]=k0;
   j[1]=k1;
   j[2]=k2;
   j[3]=k3;

   j[4]=1;//gp[0];
   j[5]=gp[1];
   j[6]=gp[2];
   j[7]=gp[3];

/*
   j[4]=t0;
   j[5]=t1;
   j[6]=t2;
   j[7]=t3;


   j[4]=0xd3;
   j[5]=0xd3;
   j[6]=0xd3;
   j[7]=0xd3;
*/

   j=j+8;


   k=k-8;
   k[0]=k0;
   k[1]=k1;
   k[2]=k2;
   k[3]=k3;
   k[4]=t0;
   k[5]=t1;
   k[6]=t2;
   k[7]=t3;
   k=k+8;
   
   EnterDebugger();

//memset(i,'A',2048);
// d24c2040

   free(i);
   free(j);
   free(k);

   return(0);
}
