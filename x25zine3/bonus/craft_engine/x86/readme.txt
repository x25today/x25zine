
  CraftPack
  by madcr (x86)

  im too laze for rename instructions ;) so, if you realy need full engine - 
  just read m68k version.

send.s - main program (open socket,sendto,cicle if it need).

*_packet.h - packets

macros.h - network engine ;) : 
  ip_checksum_F                   - calculate ip checksum & ip total length
  ip_source_generate_F            - random ip source generate
  print_F                         - just print symbols on stdout 
  exit_F                          - just exit from programm.
  