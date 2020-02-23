#  Makefile for send

send: send.o
	ld -o send send.o
#	sstrip send
	rm -rf send.o

send.o:  macros.h send.s udp_packet.h   
	as -o send.o macros.h send.s udp_packet.h   
