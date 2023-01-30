CC:=clang++
OPTS:=-Wall -O3 -Wno-unused-function
STD:=-std=c++17
EXT:=cpp
CPPFLAGS:=-c $(OPTS) $(STD)
LDFLAGS:=-lm
HEADERS:=$(wildcard *.h *.hpp)
SOURCES:=$(wildcard *.$(EXT))
TARGET_ARM:=arm64-apple-macos13
TARGET_X86:=x86_64-apple-macos12
OBJECTS_X86:=$(SOURCES:.$(EXT)=.x86.o)
OBJECTS_ARM:=$(SOURCES:.$(EXT)=.arm.o)
EXECUTABLES:=psn-level-calculator-v1 psn-level-calculator-v2 psn-points-to-level-v1 psn-points-to-level-v2 psn-level-to-points-v1 psn-level-to-points-v2
EXECUTABLES_X86:=$(EXECUTABLES:=.x86)
EXECUTABLES_ARM:=$(EXECUTABLES:=.arm)

.PHONY: all debug obj clean cleanObj cleanIntermediateExe cleanExe rebuild

all: $(EXECUTABLES)

obj: $(OBJECTS_X86) $(OBJECTS_ARM)

rebuild: clean all

clean: cleanObj cleanIntermediateExe cleanExe

cleanObj:
	rm -f -- $(OBJECTS_X86) $(OBJECTS_ARM)

cleanIntermediateExe:
	rm -f -- $(EXECUTABLES_X86) $(EXECUTABLES_ARM)

cleanExe:
	rm -f -- $(EXECUTABLES)

$(OBJECTS_X86): %.x86.o: %.$(EXT)
	$(CC) $(CPPFLAGS) -target $(TARGET_X86) $< -o $@

$(OBJECTS_ARM): %.arm.o: %.$(EXT)
	$(CC) $(CPPFLAGS) -target $(TARGET_ARM) $< -o $@

$(EXECUTABLES_X86): %.x86: %.x86.o $(filter-out $(patsubst %.x86, %.x86.o, $(EXECUTABLES_X86)), $(OBJECTS_X86))
	$(CC) $(LDFLAGS) $^ -target $(TARGET_X86) -o $@

$(EXECUTABLES_ARM): %.arm: %.arm.o $(filter-out $(patsubst %.arm, %.arm.o, $(EXECUTABLES_ARM)), $(OBJECTS_ARM))
	$(CC) $(LDFLAGS) $^ -target $(TARGET_ARM) -o $@

$(EXECUTABLES): %: %.x86 %.arm
	lipo -create -output $@ $^
