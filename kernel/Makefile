include ../Makefile.inc

ALLDEPS = threadpq.o common.o main.o localFunc.o threadq.o asl.o tcb.o syscall.o sys[1-9].o interrupts.o p2test.o devices.o scheduler.o timer.o /usr/include/uMPS/*
INCLUDES = -I ../h/ -I ../e/ -I /usr/include/uMPS/ -I ../e/syscalls/

all: $(ALLDEPS)

tcb.o:
	$(CC) $(CCFLAGS) $(INCLUDES) tcb.c

threadq.o:
	$(CC) $(CCFLAGS) $(INCLUDES) threadq.c

threadpq.o:
	$(CC) $(CCFLAGS) $(INCLUDES) threadpq.c

common.o:
	$(CC) $(CCFLAGS) $(INCLUDES) common.c

asl.o:
	$(CC) $(CCFLAGS) $(INCLUDES) asl.c

main.o:
	$(CC) $(CCFLAGS) $(INCLUDES) main.c

syscall.o:
	$(CC) $(CCFLAGS) $(INCLUDES) syscall.c

sys[1-9].o:
	$(CC) $(CCFLAGS) $(INCLUDES) syscalls/sys[1-8].c


interrupts.o:
	$(CC) $(CCFLAGS) $(INCLUDES) interrupts.c

devices.o:
	$(CC) $(CCFLAGS) $(INCLUDES) devices.c

p2test.o:
	$(CC) $(CCFLAGS) $(INCLUDES) p2test.c

localFunc.o:
	$(CC) $(CCFLAGS) $(INCLUDES) localFunc.c

scheduler.o:
	$(CC) $(CCFLAGS) $(INCLUDES) scheduler.c

timer.o:
	$(CC) $(CCFLAGS) $(INCLUDES) timer.c

clean:
	-$(RM) *.o


