CC = gcc
cflags = -std=c11 -O2 -g -Wall -Wextra -Wstrict-aliasing
cflags += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
cflags += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
ldflags += -lm

sources = $(wildcard src/*.c)
objects = $(patsubst src/%, build/%,$(sources:.c=.o))
deps = $(objects:.o=.d)

-include $(deps)
.PHONY: all clean

all: pcp

run: all
	bin/pcp

pcp: $(objects)
	$(CC) -o bin/pcp $^ $(ldflags)

build/%.o: src/%.c
	$(CC) -MMD -o $@ -c $< $(cflags)

clean:
	rm -rf bin/pcp $(objects) build/*.d
