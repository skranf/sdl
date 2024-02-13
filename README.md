# C/SDL2

## Installation sous Ubuntu

```
sudo apt install libsdl2-2.0-0 libsdl2-dev
sudo apt install libsdl2-ttf-dev libsdl2-image-dev
```

## Compilation des jeux

```
gcc maze.c -o maze -lSDL2 -lSDL2_ttf
gcc runner.c -o runner -lSDL2 -lSDL2_image
```

