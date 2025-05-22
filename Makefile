CC = gcc
CFLAGS = -Wall -Werror -g -I modules
LDFLAGS = -lncurses
SRC = $(wildcard modules/*.c)
OBJ = $(notdir $(SRC:.c=.o))
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: modules/%.c
	$(CC) $(CFLAGS) -c $< -o $@

depend: .depend

.depend: $(SRC)
	rm -f .depend
	$(CC) $(CFLAGS) -M $(SRC) > .depend

include .depend

clean:
	rm -f $(OBJ) $(TARGET) .depend

debug: CFLAGS += -DDEBUG
debug: clean $(TARGET)

.PHONY: all clean depend debug