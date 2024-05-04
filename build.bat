g++ src/snake.cpp -o game.exe -O2 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
game.exe
del game.exe