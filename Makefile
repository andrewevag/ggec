.PHONY: clean distclean default help

CXX=c++
CXXFLAGS=-Wall
BINS=ggec lexertest

default: $(BINS)

ggec: lexer.o main.o parser.o
	$(CXX) $(CXXFLAGS) -o $@ $^


lexer.cpp: lexer.l ast.hpp
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp parser.hpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

parser.cpp parser.hpp: parser.y ast.hpp
	bison -dv -o parser.cpp $<

parser.o: parser.cpp parser.hpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<



%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^


lexertest: lexer.o 
	$(MAKE) -C ./tests/lexer
	$(CXX) $(CXXFLAGS) -o ./tests/lexer/$@ ./tests/lexer/main.o $^

test: lexertest
	./tests/lexer/runner.sh


# lexer.o: lexer.cpp lexer.hpp parser.hpp

# parser.hpp parser.cpp: parser.y
# 	bison -dv -o parser.cpp parser.y

# parser.o: parser.cpp lexer.hpp

# ggec: lexer.o parser.o
# 	$(CXX) $(CXXFLAGS) -o minibasic lexer.o parser.o

clean:
	$(RM) *.o $(BINS) lexer.cpp
	$(RM) parser.hpp parser.output parser.cpp
	$(MAKE) -C ./tests/lexer clean
	

distclean: clean
# $(RM) ??

help:
	@echo "make: "
	@echo "\tbuilds the compiler"
	@echo "make clean: "
	@echo "\tremoves all automatically generated files except the final executable"
	@echo "make distclean: "
	@echo "\tremoves all automatically generated files and the final executable"