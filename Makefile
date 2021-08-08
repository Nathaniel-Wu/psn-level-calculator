CC:=clang++
OPTS:=-Wall -O3 -Wno-unused-function
STD:=-std=c++17
EXT:=cpp
CPPFLAGS:=-c $(OPTS) $(STD)
LDFLAGS:=-lm
HEADERS:=$(wildcard *.h *.hpp)
SOURCES:=$(wildcard *.$(EXT))
OBJECTS:=$(SOURCES:.$(EXT)=.o)
EXECUTABLES:=psn-level-calculator

.PHONY: all debug obj clean cleanObj cleanExe rebuild

all: obj $(EXECUTABLES)

obj: $(OBJECTS)

rebuild: clean all

clean: cleanObj cleanExe

cleanObj:
	rm -f $(OBJECTS)

cleanExe:
	rm -f $(EXECUTABLES)

$(OBJECTS): %.o: %.$(EXT) $(HEADERS)
	$(CC) $(CPPFLAGS) $< -o $@

$(EXECUTABLES): %: $(filter-out $(EXECUTABLES:%=%.o), $(OBJECTS)) %.o
	$(CC) $^ $(LDFLAGS) -o $@
