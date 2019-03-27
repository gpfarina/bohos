# put common definitions in here
CC = mipsel-linux-gcc
CCFLAGS = -c -Wall -pedantic  
PRJCFLAGS = -g
LD = mipsel-linux-ld
LDFLAGS = -T /usr/share/uMPS/elf32ltsmip.h.umpscore.x /usr/lib/uMPS/crtso.o /usr/lib/uMPS/libumps.o
ELF2 = umps-elf2umps
RM = rm -f
ECHO = echo
MV = mv -f
MAKEALL = make all
MFLAGS = -s
MKDEV = umps-mkdev
CHMOD = chmod
.SILENT :
