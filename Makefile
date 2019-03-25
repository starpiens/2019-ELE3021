# Compiler and Compile options.
CC = gcc
CFLAGS = -g -Wall

# Macros specifying path for compile.
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)
BIN = ./bin/
INC = ./include/
LIB = ./lib/ -lpthread

# Pre-Processor.
CFLAGS += -I$(INC)

# Compile command.
TARGET = shell
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)$(TARGET) $(OBJS) -L$(LIB)

# Delete binary & object files
clean:
	$(RM) $(BIN)$(TARGET) $(OBJS)
