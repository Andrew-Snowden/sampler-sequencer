export BINARYNAME = main
export BUILDDIR_A7 = build/corea7
export BUILDDIR_M4 = build/corem4

all: a7 m4

a7:
	@$(MAKE) --no-print-directory -f makefile_a7.mk all
	
m4:
	@$(MAKE) --no-print-directory -f makefile_m4.mk all
	
clean:
	rm -rf $(BUILDDIR_A7)
	rm -rf $(BUILDDIR_M4)