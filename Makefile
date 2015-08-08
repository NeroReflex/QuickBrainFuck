CXX			=	g++
LIBS		=	
CXXFLAGS 	=	-std=c++0x -Wall -Wextra -g
TARGET		=	BrainFuck

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) -o $(TARGET) $(LIBS) $^

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
	rm -f *.exe