CC:=gcc
SRCDIR:=src
OBJDIR:=obj
BIN:=bin/client.exe
BINDIR:=bin
LFLAGS:=-lWs2_32 -lm -lraylib -lopengl32 -lgdi32 -lwinmm

SRCDIRS = $(sort $(dir $(wildcard $(SRCDIR)/*/)))
OBJDIRS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCDIRS))
SOURCES=$(foreach dir, $(SRCDIRS),$(wildcard $(dir)*.c))
OBJECTS=$(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SOURCES))

$(BIN): $(OBJECTS) | $(BINDIR)
	@echo linking...    
	@$(CC) $^ -o $@ -I include/ -L lib/ $(LFLAGS)

$(BINDIR):
	@mkdir bin
	@echo created bin folder

obj/tools/%.o: src/tools/%.c | obj/tools
	@echo compiling $<...
	@$(CC) $(CFLAGS) -c $< -o $@

obj/game/%.o: src/game/%.c | obj/game 
	@echo compiling $<...
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIRS):
	@mkdir $(OBJDIR) 
	@mkdir $(OBJDIR)\tools
	@mkdir $(OBJDIR)\game
	@echo created obj folders

.PHONY: clean
clean:
	@echo removing $(BINDIR) folder...
	@rmdir /s /q $(BINDIR) 
	@echo removing $(OBJDIR) folder...
	@rmdir /s /q $(OBJDIR)
	@echo cleaned.
