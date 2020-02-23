packet:
       /* ethernet - 14 bytes       */
/* first - mac DEST, second - mac SOURCE */
.ascii "\x00\x80\xad\x90\x5f\x64"   /* mac  00:80:ad:90:5f:64 (1.3)*/
#.ascii "\x00\x80\xad\xc6\x18\xbb"  /* mac  00:80:ad:c6:18:bb (1.2)*/
.ascii "\x00\x10\xa4\x10\xe8\x1d"   /* mac  00:10:a4:10:e8:1d (1.1)*/
.ascii "\x08\x00"                   /* mac options 0806 - ARP, 0800 - IP  */
       /* ip header - 20 bytes      */
.ascii "\x45"                       /* version:4, ihl:5                   */
.ascii "\x00"                       /* type of service:                   */
.ascii "\x00\x1c"                   /* total length ip header + data      */
.ascii "\x00\x00"                   /* id       	     	          */
.ascii "\x00\x00"                   /* flags                              */
.ascii "\x80"                       /* TTL                   : 255        */
.ascii "\x01"                       /* protocol (1=ICMP)                  */	         
.ascii "\x00\x00"                   /* checksum  (ip_checksum_F)          */
.ascii "\xc0\xa8\x01\x02"           /* ip source       : 192.168.1.1      */
.ascii "\xc0\xa8\x01\x03"           /* ip destination  : 192.168.1.2      */
       /* icmp header - 8 bytes     */
.ascii "\x08"                       /* 08 - echo request, 00 - echo reply */
.ascii "\x00"                       /* CODE = 0                           */
.ascii "\xf7\xff"                   /* checksum 0xf7ff (63487)            */
.ascii "\x00\x00"                   /* Identifer - if CODE 0, so 0 too    */
.ascii "\x00\x00"                   /* seq.num - if CODE 0, so 0 too      */
       /* icmp data - any size      */
.ascii "\x61\x62\x63\x64\x65\x66"   /* DATA                               */
.ascii "\x61\x62\x63\x64\x65\x66"   /* DATA                               */
.ascii "\x61\x62\x63\x64\x65\x66"   /* DATA                               */
.ascii "\x61\x62\x63\x64\x65\x66"   /* DATA                               */
.ascii "\x61\x62\x63\x64\x65\x66"   /* DATA                               */
.ascii "\x61\x62\x63\x64\x65\x66"   /* DATA                               */

size_packet = .-packet
