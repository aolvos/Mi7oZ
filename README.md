<div align="center">
  <img src="https://github.com/aolvos/Mi7oZ/blob/main/src/res/raw/logo.png"/>
</div>

# Mi7oZ
Mi7oZ is a Simple Lightweight Cross-Platform File2Image Encoder/Decoder. Created using SDL2 libraries. Features GUI and console modes. Mi7oZ is still in development, so stay tuned for new releases on my GitHub page : @aolvos.

## How it works?

### Default
Let's say: you have 2 images and you encode second one into the first one as RGB noise:
<div align="center">
  <img width=174 height=128 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/sample3.png"/>
  <img width=128 height=128 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/sample1.png"/>
</div>

Comparsion between original and encoded:
<div align="center">
  <img width=174 height=128 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/sample3.png"/>
  <img width=174 height=128 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/merged/sample3+1.png"/>
</div>

Magnified:
<div align="center">
  <img width=197 height=146 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/merged/sample3+1_1.png"/>
  <img width=228 height=146 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/merged/sample3+1_2.png"/>
</div>

Every pixel contains 3 encrypted bytes of encoded file. The main thing is that with Mi7oZ you are able to encode any type of file (max. ~15MB) into any image (resolution must be enough to fit a file). Let's encode flowers picture into black square (original and encoded):
<div align="center">
  <img width=180 height=180 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/sample2.png"/>
  <img width=180 height=180 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/merged/sample2+3.png"/>
</div>

If we zoom in, we'll be able to see data pixels:
<div align="center">
  <img width=180 height=180 src="https://github.com/aolvos/Mi7oZ/blob/main/samples/merged/sample2+3_1.png"/>
</div>

## Building

### Dependencies
* SDL2
* SDL2_image
* SDL2_mixer

### Linux
```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
g++ Mi7oZ.cpp -o Mi7oZ -lSDL2 -lSDL2_image -lSDL2_mixer
./Mi7oZ
```

### MacOS
```
brew install sdl2 sdl2_image sdl2_mixer
g++ Mi7oZ.cpp -o Mi7oZ -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_mixer
./Mi7oZ
```

### Windows
* Download `development` libraries:
  * https://www.libsdl.org/download-2.0.php
  * https://www.libsdl.org/projects/SDL_image/
  * https://www.libsdl.org/projects/SDL_mixer/
* Compile source using MinGW or Visual Studio

## Running

### Linux
```
sudo apt install libsdl2 libsdl2-image libsdl2-mixer
./Mi7oZ
```

### MacOS
```
brew install sdl2 sdl2_image sdl2_mixer
./Mi7oZ
```

### Windows
* Download `runtime` libraries:
  * https://www.libsdl.org/download-2.0.php
  * https://www.libsdl.org/projects/SDL_image/
  * https://www.libsdl.org/projects/SDL_mixer/
* Copy .dll's in .exe folder

### Console mode
```
./Mi7oZ -h
-=[ Mi7oZ v0.1alpha ]=-
Copyright (C) 2021 Alexander Olovs

Mi7oZ comes with ABSOLUTELY NO WARRANTY;
This is free software, and you are welcome
to redistribute it under certain conditions;
Read LICENSE for details.

*FileA(.png/.jpg) : source image file.
*FileB : any other file.
-e FileA FileB : encode FileB into FileA.
-d FileA FileB : decode FileB from FileA.
-h Display help.
```

### GUI mode
* Select files A and B in `Options` or by modifying `mi7oz.cfg`
* Run `Encode` or `Decode` operations

<div align="center">
  <img width=582 height=480 src="https://github.com/aolvos/Mi7oZ/blob/main/misc/gui.png"/>
</div>

So, that's it :) Now you can hide your files inside your photos. Have fun!

## License
GNU General Public License v3.0\
Copyright (C) 2021 Alexander Olovs
