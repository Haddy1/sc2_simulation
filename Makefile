CXX = g++
CXXFLAGS = -std=c++14 -Wall -pedantic -O3
# uncomment for removing debug features
#CXXFLAGS += -DNDEBUG

TARGET = scsim

# enable compiler optimizations
#CXXFLAGS += -march=native -ffast-math #no

SRCD = src
OBJD = obj
INCD = include
INCLUDES = -I$(INCD)

# files
#SRC = $(SRCD)/simulation.cpp $(SRCD)/main.cpp
SRC = $(wildcard src/*.cpp)
INC = $(wildcard include/*.h)
OBJ = $(patsubst $(SRCD)/%.cpp, $(OBJD)/%.o, $(SRC))


.PHONY: all clean

all: $(OBJD) $(TARGET)

$(TARGET): $(OBJ) $(INC) Makefile
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(OBJD)/%.o: $(SRCD)/%.cpp $(INC) Makefile
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


$(OBJD):
	@mkdir $(OBJD)

clean:
	@rm -rf $(TARGET) $(OBJD)

