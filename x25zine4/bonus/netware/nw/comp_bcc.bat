rem comp.bat lala.c lala.def

@echo off

SET WATCOM=C:\WATCOM
SET PATH=C:\WATCOM\BINNT;C:\WATCOM\BINW;
SET EDPATH=C:\WATCOM\EDDAT
SET INCLUDE=C:\WATCOM\H;C:\WATCOM\MFC\INCLUDE;C:\WATCOM\H\NT;

set bcc55=c:\bcc55\
%BCC55%bin\bcc32.exe -c -u- -I%BCC55%include\nlm %1 
wlink @%2
