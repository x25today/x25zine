# makefile .. just makefile

send: send.o
	ld -o send send.o
	sstrip send
	rm -rf send.o

send.o: macros.h send.s icmp_packet.h  
	as -W -o send.o macros.h send.s icmp_packet.h  
