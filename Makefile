# ==============================================================================
# Makefile for Flappy Bird Clone (C++ with Allegro 5)
# Designed for clarity and robustness against common makefile issues.
#
# IMPORTANT: All command lines (lines starting with @, $ or a variable)
#            MUST be indented with a single TAB character, NOT spaces.
# ==============================================================================

# --- Configuration ---

# C++ Compiler
CXX = g++

# Compiler Flags
# -Wall: Enable all standard warnings
# -g: Include debugging information for debuggers
# -std=c++17: Use C++17 standard (adjust as needed: c++11, c++14, etc.)
CXXFLAGS = -Wall -g -std=c++17

# Include Directories (relative to the Makefile's location)
# Order matters: more specific includes often come before general ones
INCLUDES = -Iinclude \
           -Iinclude/dinamic \
           -Iinclude/elements \
           -Iinclude/register

# Allegro 5 Libraries (Manual Setup)
# Adjust these paths/names based on your Allegro installation.
# If you're on Linux/macOS and pkg-config works, you can use:
# ALLEGRO_LIBS = $(shell pkg-config --libs allegro_monolith-5.2)
# Or for individual components:
# ALLEGRO_LIBS = $(shell pkg-config --libs allegro_image-5.2 allegro_font-5.2 allegro_ttf-5.2 allegro_primitives-5.2 allegro_dialog-5.2 allegro-5.2)
#
# For manual setup (e.g., Windows or if pkg-config fails):
# You might need to add -L/path/to/allegro/lib and -I/path/to/allegro/include
# if your Allegro installation is not in standard system paths.
ALLEGRO_LIBS = -lallegro \
               -lallegro_image \
               -lallegro_font \
               -lallegro_ttf \
               -lallegro_primitives \
               -lallegro_dialog

# --- Directory Definitions ---

# Output directory for the final executable and copied assets
BUILD_DIR = bin

# Output directory for intermediate object files (.o)
OBJ_DIR = obj

# Name of the final executable
EXECUTABLE_NAME = flappy_bird_clone

# Full path to the final executable
TARGET = $(BUILD_DIR)/$(EXECUTABLE_NAME)

# --- Source File Collection ---

# Collect all .cpp files from src/ and its immediate subdirectories
SRCS = $(wildcard src/*.cpp) \
       $(wildcard src/dinamic/*.cpp) \
       $(wildcard src/elements/*.cpp) \
       $(wildcard src/register/*.cpp)

# Define corresponding object files for each source file
# Transforms 'src/dir/file.cpp' into 'obj/dir/file.o'
OBJS = $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# ==============================================================================
# --- Makefile Rules ---
# ==============================================================================

# Default target: builds the executable and copies assets
.PHONY: all
all: $(TARGET) assets_copy

# Rule to link all object files into the final executable
# This rule depends on all object files being compiled.
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	# Ensure the build output directory exists before linking
	@mkdir -p $(BUILD_DIR)
	# The linking command: compile object files into the target executable
	$(CXX) $(OBJS) -o $@ $(ALLEGRO_LIBS)

# Rule to compile each .cpp file into an .o object file
# This is a pattern rule: it applies to any .cpp file under src/
# and generates its corresponding .o file under obj/.
$(OBJ_DIR)/%.o: src/%.cpp
	@echo "Compiling $< to $@"
	# Create the necessary subdirectory in obj/ if it doesn't exist
	@mkdir -p $(dir $@)
	# The compilation command: takes source, includes, and creates object file
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to copy assets to the build directory
# This ensures that the executable finds its necessary resources at runtime.
.PHONY: assets_copy
assets_copy:
	@echo "Copying assets to $(BUILD_DIR)/assets/..."
	# Create the assets subdirectory within the build directory
	@mkdir -p $(BUILD_DIR)/assets
	# Copy all contents from the source assets/ folder to the build assets/ folder
	@cp -r assets/* $(BUILD_DIR)/assets/

# Rule to clean all build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	# Remove the entire object files directory
	@rm -rf $(OBJ_DIR)
	# Remove the entire build output directory (which contains the executable and copied assets)
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."

# Ensure initial object file subdirectories are created at the start of the build
# This helps in preventing 'No such file or directory' errors for obj/ subfolders
$(shell mkdir -p $(OBJ_DIR)/dinamic $(OBJ_DIR)/elements $(OBJ_DIR)/register)
