CC=clang
INCLUDE=include
SRC=src
OBJ=obj
BIN=bin
CCFLAGS=-Iinclude
DEBUG_FLAGS=-g -O0 -Wall
DEBUG ?= false

override DEBUG := $(DEBUG)

ifeq ($(DEBUG), true)
	CCFLAGS += $(DEBUG_FLAGS)
endif

SOURCES=main huffman

OBJECTS=$(foreach source,$(SOURCES),$(OBJ)/$(source).o)

all: $(OBJECTS)
	[ -d $(BIN) ] || mkdir -p $(BIN)
	$(CC) -o $(BIN)/main $^

$(OBJ)/%.o: $(SRC)/%.c
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	$(CC) -c $(CCFLAGS) -o $@ $^


debug: 
	$(MAKE) DEBUG=true all



clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)
