all: bin/torto

lib/libgameinit.a: src/dicionario.c src/gamelogic.c src/util.c include/dicionario.h include/gamelogic.h include/util.h
	gcc -c src/dicionario.c -Iinclude
	gcc -c src/gamelogic.c -Iinclude
	gcc -c src/util.c -Iinclude
	ar rcs $@ dicionario.o gamelogic.o util.o

bin/torto: src/main.c include/dicionario.h include/gamelogic.h include/util.h lib/libgameinit.a
	gcc -o $@ $< -Llib -lgameinit -lncurses -Iinclude

clean:
	@rm -f lib/* bin/*
	@find . -name '*.o' -exec rm -f {} \;
