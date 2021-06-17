CXX 		:= g++
CXX_FLAGS	:= -ggdb3 -Og -Bstatic -static-libgcc -static-libstdc++ -std=c++17

BIN			:= bin
INC			:= include
LIB			:= lib
OBJ			:= obj
SRC			:= src

INC_FLAG	:= -I$(INC)
LIBRARIES	:= -lssl -lcrypto -lX11 -lGL -lpthread -lstdc++fs -lpng

LIB_FLAG	:= -Llib

SOURCES		:= $(wildcard $(SRC)/*.cpp)
OBJECTS		:= $(patsubst $(SRC)/%,$(OBJ)/%,$(SOURCES:.cpp=.cpp.o))

.PHONY: clean all

all: $(BIN)/mwaExample

clean:
	rm -rf obj/*.o

# Compile only
$(OBJ)/%.cpp.o : $(SRC)/%.cpp $(DEPENDENCIES)
	$(CXX) $(CXX_FLAGS) $(INC_FLAG) -c -o $@ $<

# Link the object files and libraries
$(BIN)/mwaExample : $(OBJECTS)
	$(CXX) $(CXX_FLAGS) -o $(BIN)/mwaExample $^ $(LIBRARIES) $(LIB_FLAG)
