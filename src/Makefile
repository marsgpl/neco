CC= gcc
LD= gcc
CCFLAGS= -c -O2 -std=c2x -march=native -fno-ident \
    -Wall -Wextra -Wshadow -Wstrict-aliasing -Werror -pedantic \
    -Wno-gnu-zero-variadic-macro-arguments
LDFLAGS=
INCS=
LIBS=

build: encode decode

clean:
	rm -f *.o encode decode debug

encode: encode.o file.o wav.o
	$(LD) $(LDFLAGS) -o $@ $^

decode: decode.o file.o wav.o
	$(LD) $(LDFLAGS) -o $@ $^

debug: debug.o file.o wav.o
	$(LD) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CCFLAGS) -o $@ $<

encode.o: encode.c encode.h file.h wav.h error.h
decode.o: decode.c decode.h file.h wav.h error.h
debug.o: debug.c debug.h file.h wav.h error.h
file.o: file.c file.h error.h
wav.o: wav.c wav.h error.h

.PHONY: build clean
