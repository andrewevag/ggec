CXX=c++
CXXFLAGS=-Wall

# We want to compile with the original lexer and link it to local main
BINS=lexertest

default: main.o

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^
%.o: %.cpp


clean: 
	$(RM) $(BINS) *.o