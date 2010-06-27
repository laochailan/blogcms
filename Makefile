CFLAGS  = -Wall -g

OBJ = blog.o read.o article.o
BIN = index.fcgi
# INSTALLPATH for laochailans blog; you may have to change this.
INSTALLPATH = /srv/http/blog 

%.o: %.c
	gcc -c $< $(CFLAGS)
	
blog: $(OBJ)
	gcc -o $(BIN) $(OBJ) $(CFLAGS)

install: blog
	cp $(BIN) $(INSTALLPATH)

.PHONY: clean
clean:
	rm $(BIN) $(OBJ)