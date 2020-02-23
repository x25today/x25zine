rem comp.bat lala

@echo off

SET WATCOM=C:\WATCOM
SET PATH=C:\WATCOM\BINNT;C:\WATCOM\BINW;
SET EDPATH=C:\WATCOM\EDDAT
SET INCLUDE=C:\WATCOM\H;C:\WATCOM\H\NT;c:\watcom\h\nlm;

c:\watcom\binnt\wcc386 %1.c -bt=netware
c:\watcom\binnt\wlink name %1 d all SYS netware d novell op maxe=25 op symf FIL %1.obj
