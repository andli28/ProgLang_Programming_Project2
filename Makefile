CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -g -I.
PARSER = bison
PARSERFLAGS = -Wall -Werror -d
LEXER = flex
LEXERFLAGS =

# Targets
TARGET = parser
PARSER_SRC = parser.tab.cpp
PARSER_HDR = parser.tab.hpp
LEXER_SRC = lex.yy.c
OBJS = main.o scanner.o parser.o astnode.o

# Default build (normal)
all: $(TARGET)

# Debug build with debug outputs
debug: PARSERFLAGS += -t -v
debug: LEXERFLAGS += -d
debug: CXXFLAGS += -DYYDEBUG=1
debug: $(TARGET)

$(TARGET): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

parser.tab.cpp parser.tab.hpp: parser.y
	@$(PARSER) $(PARSERFLAGS) -o $(PARSER_SRC) parser.y

lex.yy.c: lexer.l
	@$(LEXER) $(LEXERFLAGS) -o $(LEXER_SRC) lexer.l

parser.o: parser.tab.cpp parser.tab.hpp astnode.hpp
	@$(CXX) $(CXXFLAGS) -c -o $@ $(PARSER_SRC)

scanner.o: lex.yy.c parser.tab.hpp astnode.hpp
	@$(CXX) $(CXXFLAGS) -c -o $@ $(LEXER_SRC)

astnode.o: astnode.cpp astnode.hpp
	@$(CXX) $(CXXFLAGS) -c -o $@ astnode.cpp

main.o: main.cpp parser.tab.hpp astnode.hpp
	@$(CXX) $(CXXFLAGS) -c -o $@ main.cpp

clean:
	@rm -f $(TARGET) $(PARSER_SRC) $(PARSER_HDR) $(LEXER_SRC) *.o parser.output

.PHONY: all debug clean