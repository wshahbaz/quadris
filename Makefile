#Build Macros
CXX=g++ -std=c++14
CXXFLAGS= -Wall -MMD
GTKFLAGS = `pkg-config gtkmm-3.0 --cflags --libs`
SOURCES = $(wildcard *.cpp) # list of all .cc files in the current directory
OBJECTS = ${SOURCES:.cpp=.o} # .o files depend upon .cc files with same names
DEPENDS=${OBJECTS:.o=.d}
#substitute .o with .d for dependency files
EXEC=quadris
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)  $(GTKFLAGS)
%.o: %.cpp 
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(GTKFLAGS)

-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC)
