.PHONY: clean distclean default help

CXX=c++
CXXFLAGS=-g -Wall -std=c++14
BINS=ggec
INCLUDE=-I$(PWD)/inc
INCLUDE+= -I$(PWD)
DEPSOURCE=$(wildcard src/*.cpp)
DEPOBJECTS=$(patsubst %.cpp, %.o, $(DEPSOURCE))
GEN?=100
default: $(BINS)

ggec: lexer.o main.o parser.o error.o  tojsonstring.o $(DEPOBJECTS)
	$(CXX) $(INCLUDE) $(CXXFLAGS) -o $@ $^


lexer.cpp: lexer.l ast.hpp
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp parser.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

parser.cpp parser.hpp: parser.y ast.hpp
	bison -dv -o parser.cpp $<

parser.o: parser.cpp parser.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<



%.o: %.cpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $^


lexertest: lexer.o ./tests/lexer/main.o error.o tojsonstring.o parser.o $(DEPOBJECTS)
	$(CXX) $(CXXFLAGS) -o ./tests/lexer/$@ $^

parsertest: lexer.o ./tests/parser/main.o parser.o error.o tojsonstring.o $(DEPOBJECTS)
	$(CXX) $(CXXFLAGS) -o ./tests/parser/$@ $^

test: lexertest parsertest
	@echo "🧪 Running Lexer Suite :"
	@./tests/lexer/runner.sh
	@echo "🧪 Running Parser Suite :"
	@./tests/parser/runner.sh
	@echo "🧪 Running Randomly Generated Edsger Programs :"
	@echo "Generating the input files : ⛏️"
	@$(MAKE) -C examples/syntax_gen generate GEN="$(GEN)"
	@echo "Running the input files on the parser : ⛏️"
	@./tests/parser/runner_gen.sh $(GEN)
	


# lexer.o: lexer.cpp lexer.hpp parser.hpp

# parser.hpp parser.cpp: parser.y
# 	bison -dv -o parser.cpp parser.y

# parser.o: parser.cpp lexer.hpp

# ggec: lexer.o parser.o
# 	$(CXX) $(CXXFLAGS) -o minibasic lexer.o parser.o

clean:
	$(RM) *.o $(BINS) lexer.cpp
	$(RM) parser.hpp parser.output parser.cpp
	$(RM) ./src/*.o
	$(MAKE) -C ./tests/lexer clean
	$(MAKE) -C ./tests/parser clean
	$(MAKE) -C ./examples/syntax_gen clean

distclean: clean
	$(RM) $(BINS)
BLUE=\e[0;34m
RESET=\e[0m
help:
	@echo "$(BLUE)make: $(RESET)"
	@echo "\tbuilds the compiler"
	@echo "$(BLUE)make clean: $(RESET)"
	@echo "\tremoves all automatically generated files except the final executable does it too need to fix it before submitting"
	@echo "$(BLUE)make distclean: $(RESET)"
	@echo "\tremoves all automatically generated files and the final executable"
	@echo "$(BLUE)make test GEN=number: $(RESET)"
	@echo "\t-Run lexer and parser suites "
	@echo "\t-Generate number syntactically correct Edsger programs and passes them through"
	@echo "\t the parsertest"