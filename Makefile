# A simple Makefile for compiling small SDL projects
CC := gcc
LDLIBS := -ggdb3 -O0 --std=c99 -Wall -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
EXEC := CyberHighway
RES :=

###windows replacement
#remove #'s bellow to build Windows 64 bit ON Linux
#    CC := x86_64-w64-mingw32-gcc
#    RES := CyberHighway64Icon.res
#remove # bellow to build Windows 32 bit ON Linux
#    CC := i686-w64-mingw32-gcc
#    RES := CyberHighway32Icon.res
##remove #'s bellow to build Windows ON ALL
#    LDLIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 
#    EXEC := CyberHighway.exe
    

# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags `

# add source files here
SRCS := CyberHighway.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# default recipe
all: $(EXEC)


# recipe for building the final executable
$(EXEC): $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) $(RES) $(CFLAGS) $(LDLIBS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
