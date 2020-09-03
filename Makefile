CC = gcc
FLGS_OBJ = -c -o
FLGS_PROG = -o
LIBS = -lz -lrt -lpthread
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
OBJS := $(filter-out Launcher.o Server.o Miner.o MinerFucker.o,$(OBJS))
PROG := Launcher.out Server.out Miner.out MinerFucker.out 

all: CLEAN BUILD

BUILD: START $(OBJS) $(PROG) END

%.o: %.c
	$(CC) $(FLGS_OBJ) $@ $^ $(LIBS)
%.out: %.c
	$(CC) $(FLGS_PROG) $@ $^ $(OBJS) $(LIBS) 
START:
	@echo Compilationing started...
END:
	@echo Compilationing finished...
RUN: $(PROG)
	@echo Runing program....
	./Launcher.out 
.PHONY: CLEAN
CLEAN:
	@echo Cleaning....
	rm -vf $(OBJS) $(PROG)
