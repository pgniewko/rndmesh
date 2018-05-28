RM          := rm -f
MKDIR	    := mkdir -p
# C compiler
C	     	:= g++
CXX	     	:= g++
F90 		:= gfortran

STATIC		:=librndmesh.a
EXE			:=run_rndmesh

SRC			:=./src
INCLUDE     :=/Users/pawel/include
LD_LIBS     :=/usr/local/Cellar/gcc/7.1.0/lib/gcc/7

CFLAGS 		:= -O3 -I$(INCLUDE)
CXXFLAGS	:= -O3 -std=gnu++11 -L$(LD_LIBS) -I$(INCLUDE) -DFAST
LDLIBS   	:= -lrndmesh -lnblists -lgfortran

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
	$(CXX) -o $@ $^ $(CXXFLAGS) -I$(SRC) $(LDLIBS)

%.o: %.cpp %.h
	@echo [Compile C++ files] $<
	$(CXX) -c $(CFLAGS) $< -o $@

%.o: %.c %.h
	@echo [Compile C files] $<
	$(C) -c $(CFLAGS) $< -o $@

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
