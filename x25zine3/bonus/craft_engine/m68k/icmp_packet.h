packet:
       /* ethernet - 14 bytes       */
/* .ascii "\x00\x10\xa4\x10\xe8\x1d" */  /* mac dest    00:10:a4:10:e8:1d (1.1)*/
.ascii "\x00\x80\xad\x90\x5f\x64"  /* mac dest    00:80:ad:90:5f:64 (1.3)*/
.ascii "\x00\x80\xad\xc6\x18\xbb"   /* mac source  00:80:ad:c6:18 :bb (1.2)*/
.ascii "\x08\x00"                   /* mac options 0806 - ARP, 0800 - IP  */
       /* ip header - 20 bytes      */
.ascii "\x45"                       /* version:4, ihl:5                   */
.ascii "\x00"                       /* type of service:                   */
.ascii "\x00\x00"               /* total length ip header + data (ip_len_F) */
.ascii "\x00\x00"                   /* id       	     	          */
.ascii "\x00\x00"                   /* flags                              */
.ascii "\x80"                       /* TTL                   : 255        */
.ascii "\x01"                       /* protocol (1=ICMP)                  */	         
.ascii "\x00\x00"                   /* checksum  (ip_checksum_F)          */
.ascii "\xc0\xa8\x01\x02"           /* ip source       : 192.168.1.2      */
.ascii "\xc0\xa8\x01\x03"           /* ip destination  : 192.168.1.1      */
       /* icmp header - 8 bytes     */
.ascii "\x08"                       /* 08 - echo request, 00 - echo reply */
.ascii "\x00"                       /* CODE = 0                           */
.ascii "\x00\x00"                   /* checksum (icmp_checksum_F)         */
.ascii "\x01\x00"                   /* Identifer - if CODE 0, so 0 too    */
.ascii "\x00\x00"                   /* seq.num - if CODE 0, so 0 too      */
       /* icmp data - any size */
.ascii "\x21\x12\x32\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x11"

.space 100,0x61  
/*
.ascii "\x67\x67\x67\x67\x67\x67\x67\x67\x67\x67" 
*/

size_packet = .-packet


