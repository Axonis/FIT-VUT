  # the compiler
CC = gcc

  # compiler flags:
CFLAGS  = -std=gnu99 -pthread -Wall -Wextra -Werror -pedantic

  # the build target executable:
TARGET = proj2

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clear:
	$(RM) $(TARGET)
