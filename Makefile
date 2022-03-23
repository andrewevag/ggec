.PHONY: clean distclean default help

CXX=c++
CXXFLAGS=-Wall

default: lexer

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

lexer: lexer.o
	$(CXX) $(CXXFLAGS) -o $@ $^
# lexer.o: lexer.cpp lexer.hpp parser.hpp

# parser.hpp parser.cpp: parser.y
# 	bison -dv -o parser.cpp parser.y

# parser.o: parser.cpp lexer.hpp

# ggec: lexer.o parser.o
# 	$(CXX) $(CXXFLAGS) -o minibasic lexer.o parser.o

clean:
	$(RM) lexer.cpp lexer *.o

distclean: clean
	$(RM) 

help:
	@echo "make: "
	@echo "\tbuilds the compiler"
	@echo "make clean: "
	@echo "\tremoves all automatically generated files except the final executable"
	@echo "make distclean: "
	@echo "\tremoves all automatically generated files and the final executable"