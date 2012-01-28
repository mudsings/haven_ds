
SRC=math.c develop.c matrix.c parser.c external.c contrib.c sockets.c mudlib_stats.c 
OBJ=math.$(O) develop.$(O) matrix.$(O) parser.$(O) external.$(O) contrib.$(O) sockets.$(O) mudlib_stats.$(O) 

.c.$(O):
	$(CC) $(CFLAGS) -I.. -c $*.c

all: $(OBJ)
	ar rcu packages.$(A) $(OBJ)
	$(RANLIB) packages.$(A)

db.$(O): db.c
	$(CC) $(CFLAGS) -I.. -I/usr/include/mysql -c $*.c

clean:
	rm -f *.$(O)
	rm -f packages.$(A)
