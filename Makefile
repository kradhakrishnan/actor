ifndef OBJDIR
OBJDIR=$(PWD)/obj
endif

dev-setup:
	git submodule add git@github.com:kradhakrishnan/core.git

dev-update:
	git submodule update

all:
	mkdir -p $(OBJDIR)
	cd $(OBJDIR) && cmake .. && make -j`nproc`

clean:
	rm -r -f $(OBJDIR)
