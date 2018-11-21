RM          := rm -f
MKDIR	    := mkdir -p
# C compiler
CC	     	:= g++-7
CXX	     	:= g++-7
F90 		:= gfortran

STATIC		:=librndmesh.a
EXE			:=run_rndmesh

SRC			:=./src
PREFIX      :=/usr/local
INCLUDE     :=$(PREFIX)/include
LIBS        :=$(PREFIX)/lib

FASTFLAG    := -DFAST
FFLAGS 		:= -O3 -I$(INCLUDE) $(FASTFLAG)
CFLAGS 		:= -O3 -std=gnu++11 -I$(INCLUDE) $(FASTFLAG)
CXXFLAGS	:= -O3 -std=gnu++11 -L$(LIBS) -I$(INCLUDE) $(FASTFLAG)
LDLIBS   	:= -lrndmesh -lnblists -lgfortran

SOURCES	     := $(shell find $(SRC) -type f -name "*.cpp" -or -name "*.c" -or -name "*.f90")

HEADERS	     := $(shell find $(SRC) -type f -name "*.h" -not -name "fastmath.h" )

OBJECTS      := $(patsubst %.cpp,%.o,$(patsubst %.c,%.o, $(patsubst %.f90,%.o, $(SOURCES) )))

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
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.f90
	@echo [Compile f90 files] $<
	$(F90) -c $(FFLAGS) $< -o $@

PHONY: exe build clean install

exe: $(EXE)

build: $(STATIC)
	@echo [Building]

clean:
	@echo [Cleaning]
	$(RM) $(STATIC) $(EXE) $(OBJECTS) 

install: $(STATIC)
	@echo [Installing] $<
	sudo install -m 755 $(STATIC) $(LIBS) 
	sudo install -m 755 $(HEADERS) $(INCLUDE) 
