
CC = g++
CFLAGS = -Wall

LINKERS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SRCS = $(wildcard *.cpp) $(wildcard ObjDefs/*.cpp) 

INCLUDES = -Iinclude                                                     \
           -ID:\MinGWLibs\SDL2-2.0.8\i686-w64-mingw32\include\SDL2       \
	   -ID:\MinGWLibs\SDL2_image-2.0.3\i686-w64-mingw32\include\SDL2 \
           -ID:\MinGWLibs\SDL2_ttf-2.0.14\i686-w64-mingw32\include\SDL2

LIBS = -LD:\MinGWLibs\SDL2-2.0.8\i686-w64-mingw32\lib        \
       -LD:\MinGWLibs\SDL2_image-2.0.3\i686-w64-mingw32\lib  \
       -LD:\MinGWLibs\SDL2_ttf-2.0.14\i686-w64-mingw32\lib \

OBJDIR = obj

OBJFILES = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

EXENAME = QuietWorld

#Need to put the linkers at the end of the call
$(EXENAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ $(LINKERS)


$(OBJDIR)/%.o: $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
