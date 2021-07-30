CPP := g++
CPPFLAGS := -Wall -Werror -I include/

SOURCES := $(wildcard src/*.cxx)
OBJECTS := $(patsubst %.cxx, %.o, $(SOURCES))

EXE := jay

all: $(EXE)

%.o: %.cxx
	$(CPP) $(CPPFLAGS) -o $@ -c $<

$(EXE): $(OBJECTS)
	$(CPP) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXE)
