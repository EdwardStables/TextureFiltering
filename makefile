BUILD_DIR ?= ./build
EMBUILD_DIR ?= ./embuild
SRC_DIR ?= ./src
HEADER_DIR ?= ./headers
FLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
EMFLAGS = -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 
BINARY = game
CMD = $(CXX)
EMCMD = em++
EMSHELL = $(SRC_DIR)/basic_template.html

_OBJS = filtering.o olcPixelGameEngine.o

OBJS = $(patsubst %,$(BUILD_DIR)/%,$(_OBJS))
EMOBJS = $(patsubst %,$(EMBUILD_DIR)/%,$(_OBJS))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_DIR)/%.h
	$(CMD) -c $< $(FLAGS) -o $@

$(EMBUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_DIR)/%.h
	$(EMCMD) -c $< $(EMFLAGS) -o $@

build: $(SRC_DIR)/main.cpp $(OBJS)
	$(CMD) $^ $(FLAGS) -o $(BINARY)

embuild: $(SRC_DIR)/main.cpp $(EMOBJS)
	$(EMCMD) $^ $(EMFLAGS) -o $(EMBUILD_DIR)/index.html --shell-file $(EMSHELL) --preload-file media

run:
	./$(BINARY)

mrun:
	emrun $(EMBUILD_DIR)/index.html

.PHONEY: clean
clean:
	rm -rf $(BUILD_DIR)/* $(BINARY)* $(EMBUILD_DIR)/*

package:
	zip $(EMBUILD_DIR)/game $(EMBUILD_DIR)/index.{html,wasm,js,data} 
