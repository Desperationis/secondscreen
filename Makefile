CXX = g++
CXXFLAGS = -std=c++20 -Wall -g
TARGET = out
SRCS = src/main.cpp src/X11Overlay.cpp
OBJS = $(patsubst src/%.cpp,build/%.o,$(SRCS))
BUILD_DIR = build

# Add the required libraries and flags
LIBS = -lX11 -lXext -lXcomposite -lXfixes -lcairo
INCLUDES = -I/usr/X11R6/include -I include/
LDFLAGS = -L/usr/X11R6/lib

# Add pkg-config for Cairo
CAIRO_FLAGS = $(shell pkg-config --cflags --libs cairo)

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) $(CAIRO_FLAGS)

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

