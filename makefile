OBJS = adserver
CC = gcc
DEBUG = -gstabs
SRC_INCLUDE = ./src/
LIB_INCLUDE = ./lib/
OUTPUT_DIR = ./build/
LIBS = event
CFLAGS = -Wall -c $(DEBUG) -I $(SRC_INCLUDE)
LFLAGS = -Wall $(DEBUG) -l$(LIBS)
BINARY = ./bin/blazeo

SOURCES :=  $(wildcard $(LIB_INCLUDE)*.c) $(wildcard $(SRC_INCLUDE)*.c)
OBJS := $(patsubst %.c,%.o,$(SOURCES))

%.o: %.c
	@echo Compiling "$@" using "$<"
	$(CC) $(CFLAGS) -o "$@" "$<"

all: $(OBJS)
	$(CC) $(LFLAGS) -o $(BINARY) $(OBJS)

clean:
	@echo "Sources = $(SOURCES)"
	@echo "Objects = $(OBJS)"
	rm -Rf $(BINARY) $(OBJS)



