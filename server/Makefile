CC:=gcc
SRCDIR:=src
OBJDIR:=obj
BIN:=bin/server
BINDIR:=bin
CFLAGS:= -fsanitize=address -std=gnu99 -Wall -Werror
LFLAGS:= -fsanitize=address

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

$(BIN): $(OBJECTS) | $(BINDIR) 
	@echo linking...
	@$(CC) $^ -o $@ $(LFLAGS)

$(BINDIR):
	@mkdir bin

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo compiling $<...
	@$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR):
	@mkdir obj

.PHONY: clean
clean:
	@rm -rf obj
	@rm -rf bin
