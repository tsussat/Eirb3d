TARGET 		= eirb3d

CC 		= g++

CFLAGS 		= -W -Wall -fPIC -fopenmp

LINKER 		= g++ -o

LFLAGS 		= -Wall -I. -lm -lSDL2

SRCDIR 		= src
OBJDIR 		= obj
BINDIR 		= bin

SOURCES 	:= $(wildcard $(SRCDIR)/*.c)
INCLUDES 	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm 		= rm -f

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS) -fopenmp

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
