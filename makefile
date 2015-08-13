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

SOURCES := $(shell find ./ -iname '*.c')
# Objects for main
OBJS := $(patsubst %.c,%.o,$(subst ./test/test.c,,$(SOURCES)))
TEST_OBJS := $(patsubst %.c,%.o,$(subst ./src/main.c,,$(SOURCES)))

# Objects for test binary


%.o: %.c
	@echo Compiling "$@" using "$<"
	$(CC) $(CFLAGS) -o "$@" "$<"

test: $(TEST_OBJS)
	$(CC) $(LFLAGS) -o $(TEST_BINARY) $(TEST_OBJS)
	
all: $(OBJS)
	$(CC) $(LFLAGS) -o $(BINARY) $(OBJS)

clean:
	@echo "Sources = $(SOURCES)"
	@echo "Objects = $(OBJS)"
	rm -Rf $(BINARY) $(OBJS) $(TEST_OBJS) $(TEST_BINARY)
	



