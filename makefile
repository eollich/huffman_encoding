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

SOURCES=main hashtable

OBJECTS=$(foreach source,$(SOURCES),$(OBJ)/$(source).o)

all: $(OBJECTS)
	[ -d $(BIN) ] || mkdir -p $(BIN)
	$(CC) -o $(BIN)/main $^

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
