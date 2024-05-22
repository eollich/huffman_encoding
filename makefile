CC=clang
INCLUDE=include
SRC=src
OBJ=obj
BIN=bin
CCFLAGS=-Iinclude
DEBUG_FLAGS= -g -O0 -Wall
DEBUG ?= false

ifeq ($(DEBUG), true)
	CFLAGS += $(CCFLAGS) $(DEBUG_FLAGS)
else
	CFLAGS = $(CCFLAGS)
endif

SOURCES=huffman
OBJECTS=$(foreach source,$(SOURCES), $(OBJ)/$(SOURCES).o)

all: $(OBJECTS)
	[ -d $(BIN) ] || mkdir -p $(BIN)
	$(CC) -o $(BIN)/huffman $^

$(OBJ)/%.o: $(SRC)/%.c
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	$(CC) -c $(CFLAGS) -o $@ $^

override DEBUG=true
debug: all

test: $(BIN)/huffman
	./$^

clean:
	rm -rf bin
	rm -rf obj
