# What compiler to use
CC=gcc

# What linker to use
CXX=g++

# -fno-default-inline
# -fcheck-new -fpermissive -fno-strict-aliasing
CFLAGS=-DMAGICK_HAS_EXCEPTIONS -DASM_WEAKEN_LABEL -O3 -fno-default-inline -I/usr/local/include -I$(TOPDIR)/include -g

# Linking flags to give the compiler
LFLAGS=-L/usr/local/lib -lACE -lpthread -lm -lstdc++ -lz

# What third-party libraries to include in link
SUBLIBS=./src/magick.a ./src/antlr/antlr.a ./src/des/des.a ./src/rx-1.5/rx/librx.a

# What are source and object files
.SUFFIXES:	.o .obj .cpp .c

# What extension do objects have
OBJ=o
