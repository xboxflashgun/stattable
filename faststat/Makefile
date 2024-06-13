obj = stater.o ftree.o copy.o process.o getlangs.o
CFLAGS = -g -O0 -I/opt/pgpro/ent-15/include/
# CFLAGS = -O -I/opt/pgpro/ent-15/include/

stater:	$(obj)
	cc -o stater $(obj) -L /opt/pgpro/ent-15/lib -lpq

process.o: stater.h

stater.o: stater.h

ftree.o: stater.h

copy.o:	stater.h

getlangs.o: stater.h

clean:
	rm stater $(obj)
