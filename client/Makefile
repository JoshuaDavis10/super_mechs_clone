CC:=gcc
SRCDIR:=src
OBJDIR:=obj
BIN:=bin/client
BINDIR:=bin
LFLAGS:=-lWs2_32 -lm -lraylib -lopengl32 -lgdi32 -lwinmm

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

$(BIN): $(OBJECTS) | $(BINDIR)
	@echo linking...    
	@$(CC) $^ -o $@ -I include/ -L lib/ $(LFLAGS)

$(BINDIR):
	@mkdir bin

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	@echo compiling $<...
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir obj 

.PHONY: clean
clean:
	@echo removing $(BINDIR) folder...
	@rmdir /Q /S $(BINDIR)
	@echo removing $(OBJDIR) folder...
	@rmdir /Q /S $(OBJDIR)
	@echo cleaned.
