stater:	stater.o process.o ftree.o copy.o
	cc -g -o stater stater.o process.o ftree.o copy.o -L /opt/pgpro/ent-15/lib -lpq

process.o: process.c
	cc -g -c process.c -I/opt/pgpro/ent-15/include/ -L /opt/pgpro/ent-15/lib -lpq

stater.o: stater.c
	cc -g -c stater.c -I/opt/pgpro/ent-15/include/ -L /opt/pgpro/ent-15/lib -lpq

ftree.o: ftree.c
	cc -g -c ftree.c -I/opt/pgpro/ent-15/include/ -L /opt/pgpro/ent-15/lib -lpq

copy.o:	copy.c
	cc -g -c copy.c -I/opt/pgpro/ent-15/include/ -L /opt/pgpro/ent-15/lib -lpq

clean:
	rm stater stater.o process.o ftree.o copy.o
