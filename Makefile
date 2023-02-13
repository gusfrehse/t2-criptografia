CFLAGS += -g
EXE = cypher

$(EXE) : cypher.o decrypt.o encrypt.o

decrypt.o : decrypt.h
encrypt.o : encrypt.h

.PHONY: clean

clean :
	-rm *.o $(EXE)

