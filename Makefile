CFLAGS := -g -Wall
CFLAGS += -O3 -ggdb -DDEBUG -fno-inline
ASFLAGS = $(CFLAGS)

ASMS := $(wildcard *.S)  # wildcard *.S
SRCS := $(wildcard *.c)  # wildcard *.c

PROG := $(patsubst %.S,%,$(ASMS))  # patsubst %.S → %
PROG += $(patsubst %.c,%,$(SRCS))  # patsubst %.c → %
PROG += tp

all: $(PROG)

tp: ant_engine.c artist_ant.c
	$(CC) $(CFLAGS) $^ -o $@

tp.mips: paint.S ant_engine.c artist_ant.c palette_size.S ant_advance.S
	$(CC) $(ASFLAGS) -D ASSEMBLY $^ -o $@

indent: .clang-files .clang-format
	xargs -r clang-format -i <$<

clean:
	rm -f $(PROG) *.o core

.PHONY: clean all

