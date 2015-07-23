include iocore/Makefile

prep:
	@echo 'ENV-SETUP'
	@git submodule init
	@rm -f include/iocore
	@ln -s `pwd`/iocore/include include/iocore
	@cd iocore && make clean all
	@mkdir -p $(OBJDIR) && cp `find iocore | grep \.a$$` $(OBJDIR)

update:
	@echo 'ENV-UPDATE'
	@git submodule update
	@make -C iocore all
	@cd iocore && make all
	@mkdir -p $(OBJDIR) && cp `find iocore | grep \.a$$` $(OBJDIR)

trash:
	@echo 'ENV-CLEANUP'
	@rm -f include/iocore
