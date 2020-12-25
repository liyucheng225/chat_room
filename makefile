SECTIONS = ./Server/  ./Client/   
all:
	for dir in $(SECTIONS);do $(MAKE) -C $$dir || exit "$$?";done
.PHONY:clean
clean:
	@for dir in $(SECTIONS);do $(MAKE) -C $$dir clean || exit "$$";done
