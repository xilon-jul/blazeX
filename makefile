OBJS = adserver
CC = gcc
DEBUG = -gstabs
SRC_INCLUDE = ./src/
OUTPUT_DIR = ./build/
LIBS = event
CFLAGS = -Wall -std=c11 -c $(DEBUG) -I $(SRC_INCLUDE)
LFLAGS = -Wall $(DEBUG) -l$(LIBS)
BINARY = ./bin/blazex
TEST_BINARY = ./bin/test_blazex

SOURCES := $(shell find ./src/ -iname '*.c')
OBJS := $(patsubst %.c,%.o,$(SOURCES))

%.o: %.c
	@echo Compiling "$@" using "$<"
	$(CC) $(CFLAGS) -o "$@" "$<"

test: $(OBJS)
	$(CC) $(LFLAGS)) -o $(TEST_BINARY)) $(OBJS)
	
all: $(OBJS)
	$(CC) $(LFLAGS) -o $(BINARY) $(OBJS)

clean:
	@echo "Sources = $(SOURCES)"
	@echo "Objects = $(OBJS)"
	rm -Rf $(BINARY) $(OBJS) $(TEST_BINARY)
	



