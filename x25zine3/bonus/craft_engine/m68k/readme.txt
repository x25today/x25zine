
  CraftPack
  by madcr (m68k)


send.s - main program (open socket,sendto,cicle if it need).

*_packet.h - packets

macros.h - net engine ;) : 
  ip_checksum_F                   - calculate ip checksum & ip total length
  ip_source_generate_F            - random ip source generate v1
  ip_src_dev_random_generator_F   - random ip source generate v2
  icmp_checksum_F                 - calculate icmp checksum 
  udp_checksum_F                  - calculate udp checksum
  print_F                         - just print symbols on stdout 
  exit_F                          - just exit from programm.
  