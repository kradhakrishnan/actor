include iocore/cmake/DefaultMakefile

all: prep base-all

clean: base-clean
	@cd iocore && make -j`nproc` clean

test: base-test

prep:
	@echo 'ENV-SETUP'
	@ls include/iocore || ln -s `pwd`/iocore/include include/iocore
	@cd iocore && make all
	@ls ${OBJDIR} || (mkdir -p $(OBJDIR) && cp `find iocore | grep \.a$$` $(OBJDIR))

.DEFAULT_GOAL := all
