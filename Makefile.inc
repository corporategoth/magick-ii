# What compiler to use
CC=gcc

# What linker to use
CXX=g++

# Compilation flags to give the compiler
CFLAGS=-O2 -I/usr/local/include -I$(TOPDIR)/include

# Linking flags to give the compiler
LFLAGS=-L/usr/local/lib -lACE -lpthread

# What third-party libraries to include in link
LIBS=./src/magick.a $(LIBACE) $(LIBCRYPT) $(LIBZ) ./src/bob/bob.a ./src/antlr/antlr.a ./src/des/des.a

# What are source and object files
.SUFFIXES:	.o .obj .cpp .c

# What extension do objects have
OBJ=o
