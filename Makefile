CC := g++
INCLUDE := -Iinclude/ -Isrc/
FLAGS := -std=c++17 -g -Wall -Wextra -Werror $(INCLUDE)

LD      := ld
LDFLAGS :=

srcdir := src
src    := $(wildcard $(srcdir)/*.cpp) $(wildcard $(srcdir)/*/*.cpp $(srcdir)/*/*/*.cpp) $(wildcard $(srcdir)/*/*.c)
obj    := $(src:%.cpp=%.o)

libs_src := $(wildcard include/*.cpp include/*/*.cpp include/*/*/*.cpp)
libs_obj := $(libs_src:%.cpp=%.o)

bin := bin
OUT := $(bin)/binary


$(shell mkdir -p $(bin)/)

run: build
	./$(OUT) $(args)

gdb: build
	gdb -batch -ex "file $(OUT)" -ex "$(cmd)"

build: $(obj)
	$(CC) $(FLAGS) $(flags) $(wildcard $(bin)/*.o) $(wildcard $(bin)/libs/*.o) -o $(OUT)

libs: $(libs_obj)

.PHONY: build all clean run

%.o: %.cpp
	$(CC) $(FLAGS) $< -c -o $(bin)/$(@F)

include/%.o: include/%.cpp
	-@mkdir -p $(bin)/libs/
	$(CC) $(FLAGS) $< -c -o $(bin)/libs/$(@F)

clean:
	@rm -r $(bin)/
	@rm $(OUT)

