
CC = g++
CFLAGS = -std=c++11 -Wall -Wextra

LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SRCS = $(wildcard *.cpp)

INCLUDES = -I/include     
# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .cpp of all words in the macro SRCS
# with the .o suffix
#

#OBJFILES = $(SRCS:.cpp=.o)

OBJDIR = obj

OBJFILES = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))


EXENAME = QuietWorld


$(EXENAME): $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ 


$(OBJDIR)/%.o: $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@


