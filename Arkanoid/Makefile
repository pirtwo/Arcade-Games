TARGET  	:= Arkanoid
SRC_DIR 	:= ./src
INC_DIR 	:= ./include
BLD_DIR 	:= ./build
CPP_STD 	:= c++2a
BLD_OPT		:= -g -Wall

CXX       	:= g++
CXXFLAGS  	:= $(BLD_OPT) -I$(INC_DIR) -std=$(CPP_STD)
LDFLAGS   	:= 
LDLIBS    	:= -lsfml-window -lsfml-system -lsfml-graphics

SRCFILES 	:= $(SRC_DIR)/*.cpp
OBJFILES 	:= $(patsubst $(SRC_DIR)/%.cpp, $(BLD_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))

$(BLD_DIR)/$(TARGET): $(OBJFILES)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)	

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BLD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(BLD_DIR):
	mkdir $(BLD_DIR)

.PHONY: clean
clean:
	rm -r $(BLD_DIR) 