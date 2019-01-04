
#####REFERENCE#####
#
#  Automatic Variables:
#  target : dependencies
#       commands
#
#  $^ means all of the dependencies
#  $@ means the target
#  $< means the first dependency
#
#  patsubst
#  $(patsubst pattern,replacement,text)
#  Finds whitespace-separated words in text that match pattern and replaces them with replacement


CC = g++
CFLAGS = -Wall

LINKERS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.cpp) 

INCLUDES = -Iinclude                                                     \
           -ID:/MinGWLibs/SDL2-2.0.8/i686-w64-mingw32/include/SDL2       \
	   -ID:/MinGWLibs/SDL2_image-2.0.3/i686-w64-mingw32/include/SDL2 \
           -ID:/MinGWLibs/SDL2_ttf-2.0.14/i686-w64-mingw32/include/SDL2

LIBS = -LD:/MinGWLibs/SDL2-2.0.8/i686-w64-mingw32/lib        \
       -LD:/MinGWLibs/SDL2_image-2.0.3/i686-w64-mingw32/lib  \
       -LD:/MinGWLibs/SDL2_ttf-2.0.14/i686-w64-mingw32/lib   

OBJFILES = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))


EXENAME = QuietWorld

#Need to put the linkers at the end of the call
$(BINDIR)/$(EXENAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ $(LINKERS)


$(OBJDIR)/%.o: $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/$(EXENAME)

.PHONY: clean
