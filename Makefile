CC = gcc
CFLAGS = -I$(shell pg_config --includedir-server) -fPIC -lm
LNAME = libentropy.so
OBJS = entropy.o

default: $(LNAME)

$(LNAME): $(OBJS)
	$(CC) -shared -o $@ $< $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: install uninstall clean

install: 
	cp -v $(LNAME) $(shell pg_config --pkglibdir)
create:
	psql < createFunctions.sql


uninstall:
	rm -v `pg_config --pkglibdir`/$(LNAME)

clean:
	$(RM) $(OBJS)

cleanall: clean
	$(RM) $(LNAME)
