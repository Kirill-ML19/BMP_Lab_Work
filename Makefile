CXX :=g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

SOURCES:=$(wildcard	*.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)
TARGET:=image_tool

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run_rotate:
	./$(TARGET) input.bmp output_rotated.bmp --rotate $(ANGLE)

run_blur: 
	./$(TARGET) input.bmp output_blurred.bmp --blur 

clean:
	rm -f $(OBJECTS) $(TARGET)