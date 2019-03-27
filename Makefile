include Makefile.inc

SRCDIR=kernel
SUCCMSG=build complete. To run BohOS, now launch \`umps\' in the $(OUTDIR)/ directory.
OUTDIR=builds
OUTFILE=kernel
ALLDEPS=$(SRCDIR)/Makefile

all :	$(ALLDEPS)
	$(ECHO) $(LD) $(LDFLAGS) $(SRCDIR)/*.o -o $(OUTDIR)/$(OUTFILE)
	$(LD) $(LDFLAGS) $(SRCDIR)/*.o -o $(OUTDIR)/$(OUTFILE)
	$(ELF2) -k $(OUTDIR)/$(OUTFILE)
	$(ECHO) $(MKDEV) -d $(OUTDIR)/disk0.umps
	$(MKDEV) -d $(OUTDIR)/disk0.umps
	$(ECHO) $(CHMOD) 755 $(OUTDIR)/disk0.umps
	$(CHMOD) 755 $(OUTDIR)/disk0.umps
	$(ECHO) $(MKDEV) -t $(OUTDIR)/tape0.umps $(OUTDIR)/kernel.core.umps
	$(MKDEV) -t $(OUTDIR)/tape0.umps $(OUTDIR)/kernel.core.umps
	$(ECHO) 
	$(ECHO) $(SUCCMSG)
	$(ECHO)

$(SRCDIR)/Makefile : force_look
	$(ECHO) looking into $(SRCDIR)
	cd $(SRCDIR); make all $(MFLAGS)

clean :
	$(ECHO)
	$(ECHO) cleaning up objects file in . $(SRCDIR)/ and umps files in $(OUTDIR)/
	$(ECHO)
	-$(RM) $(EXE) $(OBJS) $(OBJLIBS)
	-for d in $(SRCDIR)/; do (cd $$d; $(MAKE) clean ); done
	-$(RM) $(OUTDIR)/*.umps
	-$(RM) $(OUTDIR)/kernel.* $(OUTDIR)/kernel
	$(ECHO) clean complete
	$(ECHO)

force_look :
	true

