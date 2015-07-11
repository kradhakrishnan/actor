ifndef OBJDIR
OBJDIR=$(PWD)/obj
endif

all:
	mkdir -p $(OBJDIR)
	cd $(OBJDIR) && cmake .. && make -j`nproc`

clean:
	rm -r -f $(OBJDIR)
