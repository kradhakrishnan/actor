ifndef OBJDIR
OBJDIR=$(PWD)/obj
endif

setup: cleanup
	git submodule init
	ln -s `pwd`/iocore/include include/iocore

update:
	git submodule update

cleanup:
	rm -f include/iocore

all:
	mkdir -p $(OBJDIR)
	cd $(OBJDIR) && cmake .. && make -j`nproc`

clean:
	rm -r -f $ (OBJDIR)
