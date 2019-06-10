INFILE	=	\
	a.txt	\
	b.txt	\
	c.txt

CONVFILE	=	$(INFILE:c.txt=)

all: echo

echo:
	echo $(CONVFILE)

conv: $(CONVFILE)
	nkf -em0 --overwrite $<
	
clean: $(CONVFILE)
	nkf -wm0 --overwrite $<
