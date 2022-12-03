.PHONY: clean distclean default help

## Normal Build Variables
CXX=c++
CXXFLAGS=-Wall -std=c++14 -g `$(LLVMCONFIG) --cxxflags` -fexceptions
LDFLAGS=$(shell $(LLVMCONFIG) --ldflags --system-libs --libs all)
BINS=ggec main
INCLUDE=-I$(PWD)/inc
INCLUDE+= -I$(PWD)
DEPSOURCE=$(wildcard src/*.cpp)
DEPOBJECTS=$(patsubst %.cpp, %.o, $(DEPSOURCE))
LLVMCONFIG=llvm-config-11
##

## Test dependencies
PYTHON3?=/opt/homebrew/bin/python3
ERL?=/opt/homebrew/bin/erl
ERL_LIBS?=/opt/homebrew/opt/proper/proper-1.4
## needs python3 deepdiff module to run tests
## pip3 install deepdiff
##

## Test variables
GEN?=100
##

default: $(BINS)

main: ggec
	cp $< $@

ggec: lexer.o main.o parser.o error.o ast.o tojsonstring.o general.o symbol.o semantical.o codegen.o $(DEPOBJECTS)
	$(CXX) $(INCLUDE) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


lexer.cpp: lexer.l ast.hpp
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp parser.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

parser.cpp parser.hpp: parser.y ast.hpp
	bison -dv -o parser.cpp $<

parser.o: parser.cpp parser.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

general.o: general.cpp error.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

error.o: error.cpp general.hpp error.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

symbol.o: symbol.cpp symbol.hpp general.hpp error.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

semantical.o: semantical.cpp ast.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

codegen.o: codegen.cpp symbol.hpp ast.hpp error.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<


ast.hpp: symbol.hpp llvmhead.hpp

%.o: %.cpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $^


lexertest: lexer.o ./tests/lexer/main.o error.o ast.o tojsonstring.o parser.o general.o symbol.o semantical.o $(DEPOBJECTS)
	$(CXX) $(CXXFLAGS) -o ./tests/lexer/$@ $^

parsertest: lexer.o ./tests/parser/main.o parser.o error.o ast.o tojsonstring.o general.o symbol.o semantical.o $(DEPOBJECTS)
	$(CXX) $(CXXFLAGS) -o ./tests/parser/$@ $^

semanticstest: ./tests/semantics/main.o lexer.o parser.o error.o ast.o tojsonstring.o general.o symbol.o semantical.o $(DEPOBJECTS)
	$(CXX) $(CXXFLAGS) -o ./tests/semantics/$@ $^

test: lexertest parsertest semanticstest
	@echo "🧪 Running Lexer Suite :"
	@$(PYTHON3) ./tests/lexer/runner.sh
	@echo "🧪 Running Parser Suite :"
	@$(PYTHON3) ./tests/parser/runner.sh
	@echo "🧪 Running Randomly Generated Edsger Programs :"
	@echo "Generating the input files : ⛏️"
	@export ERL
	@export ERL_LIBS
	@$(MAKE) -C examples/syntax_gen generate GEN="$(GEN)"
	@echo "Running the input files on the parser : ⛏️"
	@$(PYTHON3) ./tests/parser/runner_gen.sh $(GEN) 
	@echo "🧪 Running Semantics Suite :"
	@$(PYTHON3) ./tests/semantics/runner.sh
	


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
	$(MAKE) -C ./tests/semantics clean

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
	@echo "$(BLUE)make test [GEN=number] [PYTHON3=python3 path] [ERL=erl path] [ERL_LIBS=proper path]: $(RESET)"
	@echo "\t-Run lexer and parser suites "
	@echo "\t-Generate number syntactically correct Edsger programs and passes them through"
	@echo "\t the parsertest"
	@echo "\n\t requires python3 deepdiff module"
	@echo "\t\t (pip3 install deepdiff)"