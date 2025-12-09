# FLAGS = -lGL -lglfw -Iinclude -lm

CXXFLAGS = -Iinclude -Wall -std=c++17
LDFLAGS = -lGL -lglfw -lm

OBJ = bin/main.o bin/glad.o 
DEPS = src/camera.hpp src/cube.hpp src/material.hpp src/material.hpp src/obj.hpp src/renderable.hpp src/shader.hpp src/window.hpp src/aabb.hpp src/noise.hpp

all: $(OBJ) bin/app

bin/app: $(OBJ)
	g++ $(OBJ) $(LDFLAGS) -o bin/app

bin/%.o: src/%.cpp $(DEPS)
	g++ -c $< $(CXXFLAGS) -o $@

bin/%.o: src/%.c $(DEPS)
	g++ -c $< $(CXXFLAGS) -o $@

.PHONY: test
test: bin/app
	bin/app

.PHONY: clean
clean:
	rm -rf bin/*.o bin/app
# app: main.o glad.o camera.o shader.o window.o
# 	g++ bin/main.o bin/glad.o $(FLAGS) -o bin/app

# main.o: src/main.cpp
# 	g++ -c src/main.cpp $(FLAGS) -o bin/main.o

# glad.o: src/glad.c
# 	g++ -c src/glad.c $(FLAGS) -o bin/glad.o

# camera.o: src/camera.hpp
# 	g++ -c src/camera.hpp $(FLAGS) -o bin/camera.o

# shader.o: src/shader.hpp
# 	g++ -c src/shader.hpp $(FLAGS) -o bin/shader.o

# window.o: src/window.hpp
# 	g++ -c src/window.hpp $(FLAGS) -o bin/window.o
	
# .PHONY: test clean

# test: app
# 	bin/app

# clean:
# 	rm -r bin/*.o
# 	rm bin/app