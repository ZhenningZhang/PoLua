# Compiler
CXX := clang++

# Compiler flags
CXXFLAGS := -std=c++17 -O3

# Include directories
INCLUDES := -I"./Third Party" \
            -I"./src" \
            -I"./Third Party/SDL/include" \
            -I"./Third Party/SDL Image/include" \
            -I"./Third Party/SDL Mixer/include" \
            -I"./Third Party/SDL TTF/include" \
            -I"./Third Party/Lua" \
            -I"./Third Party/LuaBridge" \
            -I"./box2d/src"

# Library flags
LDFLAGS := -L"Third Party/SDL/lib" -lSDL2 \
           -L"Third Party/SDL Image/lib" -lSDL2_image \
           -L"Third Party/SDL Mixer/lib" -lSDL2_mixer \
           -L"Third Party/SDL TTF/lib" -lSDL2_ttf \
           -L"Third Party/Lua" -llua5.4

# Source files
SOURCES := src/*.cpp \
           box2d/src/collision/*.cpp \
           box2d/src/common/*.cpp \
           box2d/src/dynamics/*.cpp \
           box2d/src/rope/*.cpp \

# Output target executable
TARGET := game_engine_linux

# Rule to build the target
main: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $(TARGET)
