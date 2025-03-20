CC:=gcc
SRCDIR:=src
OBJDIR:=obj
BIN:=bin/server
CFLAGS:= -fsanitize=address -std=c99 -Wall -Werror
LFLAGS:= -fsanitize=address

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

$(BIN): $(OBJECTS) 
	@echo linking...
	@$(CC) $^ -o $@ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo compiling $<...
	@$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR):
	@mkdir obj

.PHONY: clean
clean:
	@rm -rf obj
