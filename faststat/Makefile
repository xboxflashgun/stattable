stater:	stater.o read-dicts.o process.o
	cc -o stater stater.o read-dicts.o process.o -L /opt/pgpro/ent-15/lib -lpq

process.o: process.c
	cc -c process.c -I/opt/pgpro/ent-15/include/ -L /opt/pgpro/ent-15/lib -lpq

stater.o: stater.c
	cc -c stater.c -I/opt/pgpro/ent-15/include/ -L /opt/pgpro/ent-15/lib -lpq

read-dicts.o: read-dicts.c
	cc -c read-dicts.c -I/opt/pgpro/ent-15/include/ -L /opt/pgpro/ent-15/lib -lpq

clean:
	rm stater stater.o read-dicts.o
