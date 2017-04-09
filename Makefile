RM          := rm -f
MKDIR	    := mkdir -p
# C compiler
CC	     	:= g++-4.9
F90 		:= gfortran-4.9

STATIC		:=librndmesh.a
EXE			:=run_rndmesh

SRC			:=./src

CFLAGS 		:= -O3
CXXFLAGS	:= -O3 -std=gnu++11
LDLIBS   	:= -lrndmesh -lgfortran

SOURCES	     := $(shell find $(SRC) -type f -name "*.cpp" -or -name "*.c" -or -name "*.f90")

HEADERS	     := $(shell find $(SRC) -type f -name "*.h" -not -name "fastmath.h" )

OBJECTS      := $(patsubst %.cpp,%.o,$(patsubst %.c,%.o, $(patsubst %.f90,%.o, $(SOURCES) )))

PREFIX   := /usr/local

$(STATIC): $(OBJECTS)
	@echo "[Link (Static)]"
	ar -rcs $@ $^
	ranlib $@

$(EXE): main.cpp
	@echo [Building exe]
	$(CC) -o $@ $^ $(CXXFLAGS) -I$(SRC) $(LDLIBS)

%.o: %.cpp %.h
	@echo [Compile C++ files] $<
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.c %.h
	@echo [Compile C files] $<
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.f90
	@echo [Compile f90 files] $<
	$(F90) -c $(CFLAGS) $< -o $@

PHONY: exe build clean install

exe: $(EXE)

build: $(STATIC)
	@echo [Building]

clean:
	@echo [Cleaning]
	$(RM) $(STATIC) $(EXE) $(OBJECTS) 

install: $(STATIC)
	@echo [Installing] $<
	sudo install -m 755 $(STATIC) $(PREFIX)/lib
	sudo install -m 755 $(HEADERS) $(PREFIX)/include