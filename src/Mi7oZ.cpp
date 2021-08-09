/*     
    Copyright (C) 2021 Alexander Olovs
    Mi7oZ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mi7oZ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mi7oZ.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <unistd.h>
#include <cmath>

#include "res/res.h"

const int imgnum = 3;

SDL_Window *win;
SDL_Renderer *win_r;
SDL_Surface *images[imgnum];
SDL_Texture *tex[imgnum];

SDL_RWops *rw;

SDL_Event e;
int event = 0;

const char fontmap[] = " !'#():?@[].-,|0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const int fscale = 2;
const int fontsize = 8 * fscale;

const float maxangle = 6.275 * 2;
const float astep = 0.05 / 4;
const int aval = maxangle / astep;
float sinetable[aval];
float cosinetable[aval];

int scene = 0;
float angle = 0;
int selector = 0;
int xscroll = 0;

int opstatus = 0;
int inputstatus = 0;
int guistatus = 0;

const char *scrlmsg = "Welcome to Mi7oZ : Simple Lightweight Cross-Platform File2Image Encoder released by SATAGAX in AUG 2021. Mi7oZ works using SDL2 libraries. Avaliable on Windows (x86 | x64), MacOS (ARM) and Linux (x86 | x64 | ARM). Features GUI and console modes. Mi7oZ is still in development, so stay tuned for new releases on my GitHub page : @aolvos. Have a great day :)   | CREDITS |    Code, Graphics and Music by aolvos.    | GREETINGS |    - ALCATRAZ | CLASS | DESIRE | DEVIANCE | F4CG | FAIRLIGHT | HOODLUM | PARADOX | RAZOR 1911 | SCOOPEX | SKIDROW | TITAN - AND MANY OTHERS.    - Forever in progress! -";
const int flen = 64;
char filea[flen + 2] = "";
char fileb[flen + 2] = "";

const char *config = "mi7oz.cfg";

Mix_Music *music;

float sine(float angle)
{
    int pos = angle / astep;
    float res = sinetable[pos];
    return res;
}

float cosine(float angle)
{
    int pos = angle / astep;
    float res = cosinetable[pos];
    return res;
}

void incangle(float step)
{
    angle += step;
    if (angle >= maxangle) angle -= maxangle;
}

void SDL_PutImage(int x, int y, SDL_Rect *clip, SDL_Renderer *dest, SDL_Texture *src)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    if (clip == 0) SDL_QueryTexture(src, NULL, NULL, &rect.w, &rect.h);
    else 
    {
        int cw = clip->w;
        int ch = clip->h;
        rect.w = cw;
        rect.h = ch;
    }
    SDL_RenderCopy(dest, src, clip, &rect);
}

int findletter(char letter)
{
    int num = 0;
    int len = strlen(fontmap);
    while (true)
    {
        if (num >= len)
        {
            num = 7;
            break;
        }
        else if (fontmap[num] == letter) break;
        num++;
    }
    return num;
}

void SDL_Print(const char string[], int x, int y, SDL_Texture *src, SDL_Renderer *dest, int r, int g, int b)
{
    int len = strlen(string);
    SDL_Rect clip;
    clip.w = clip.h = fontsize;
    SDL_SetTextureColorMod(src, r, g, b);
    for (int i = 0; i < len; i++)
    {
        int num = findletter(string[i]);
        int c = num / 16;
        clip.y = c * fontsize;
        clip.x = (num - c * 16) * fontsize;
        SDL_PutImage(x, y, &clip, dest, src);
        x+=fontsize;
    }
}

void SDL_Scroll(const char string[], int x, int y, SDL_Texture *src, SDL_Renderer *dest)
{
    int len = strlen(string);
    SDL_Rect clip;
    clip.w = 1;
    clip.h = fontsize;
    float temp = angle;
    for (int i = 0; i < len; i++)
    {
        int num = findletter(string[i]);
        int c = num / 16;
        clip.y = c * fontsize;
        clip.x = (num - c * 16) * fontsize;
        for (int i = 0; i < fontsize; i++)
        {
            SDL_SetTextureColorMod(src, abs(255 * sine(angle / 2)), abs(128 + 25 * cosine(angle)), abs(204 + 51 * cosine(angle / 2)));
            SDL_PutImage(x, y + sine(angle) * 24, &clip, dest, src);
            clip.x += 1;
            x++;
            incangle(astep);
        }
    }
    angle = temp;
}

SDL_Surface *SDL_ScaleImage(SDL_Surface *src, int ratio)
{
    SDL_Surface *dest = SDL_CreateRGBSurface(0, src->w * ratio, src->h * ratio, src->format->BitsPerPixel,
        src->format->Rmask, src->format->Gmask, src->format->Bmask, src->format->Amask);
    SDL_BlitScaled(src, NULL, dest, NULL);
    SDL_FreeSurface(src);
    return dest;
}

void drawsc()
{
    SDL_RenderClear(win_r);
    SDL_PutImage(0, 0, NULL, win_r, tex[0]);
    SDL_PutImage(193, 10, NULL, win_r, tex[2]);
    if (scene == 0)
    {
        const int opnum = 4;
        if (selector < 0) selector = opnum - 1;
        else if (selector > opnum - 1) selector = 0;
        const char *set[] = {"Encode", "Decode", "Options", "Go away"};
        for (int i = 0; i < opnum; i++)
        {
            int j = 0;
            if (i == selector) j = 2;
            SDL_Print(set[i], 120 + j * fontsize, 165 + i * (fontsize + 8), tex[1], win_r, 255, 255, 255);
        }
        SDL_Print("|", 120, 165 + selector * (fontsize + 8), tex[1], win_r, 250, 100, 168);
    }
    if (scene == 1)
    {
        SDL_Print("Processing: [          ]", 120, 165, tex[1], win_r, 255, 255, 255);
        if (opstatus == -1) SDL_Print("Error!", 120, 165 + (fontsize + 8), tex[1], win_r, 250, 100, 168);
        for (int i = 0; i < opstatus; i++) SDL_Print("#", 120 + (13 + i) * fontsize, 165, tex[1], win_r, 255, 255, 255);
        opstatus++;
        if (opstatus >= 11) SDL_Print("Exporting...", 120, 165 + (fontsize + 8), tex[1], win_r, 255, 255, 255);
    }
    if (scene == 2)
    {
        const int opnum = 4;
        if (selector < 0) selector = opnum - 1;
        else if (selector > opnum - 1) selector = 0;
        const char *set[] = {"File A:", "File B:", "Method:", "Return"};
        for (int i = 0; i < opnum; i++)
        {
            int j = 0;
            if (i == selector) j = 2;
            SDL_Print(set[i], 120 + j * fontsize, 165 + i * (fontsize + 8), tex[1], win_r, 255, 255, 255);

            char bufa[flen + 2] = "";
            strcpy(bufa, filea);
            if (strlen(filea) > 15)
            {
                bufa[12] = 0;
                strcat(bufa, "...");
            }

            char bufb[flen + 2] = "";
            strcpy(bufb, fileb);
            if (strlen(fileb) > 15)
            {
                bufb[12] = 0;
                strcat(bufb, "...");
            }

            if (i == 0) SDL_Print(bufa, 120 + (j + 8) * fontsize, 165 + i * (fontsize + 8), tex[1], win_r, 255, 255, 128);
            if (i == 1) SDL_Print(bufb, 120 + (j + 8) * fontsize, 165 + i * (fontsize + 8), tex[1], win_r, 255, 255, 128);
            if (i == 2) SDL_Print("Default", 120 + (j + 8) * fontsize, 165 + i * (fontsize + 8), tex[1], win_r, 80, 100, 200);
        }
        SDL_Print("|", 120, 165 + selector * (fontsize + 8), tex[1], win_r, 250, 100, 168);
    }
    SDL_Print("| SATAGAX, 2021 |", 184, 305, tex[1], win_r, 80, 100, 200);
    SDL_Scroll(scrlmsg, 650 - xscroll, 400, tex[1], win_r);
    SDL_RenderPresent(win_r);
}

void events()
{
     while (SDL_PollEvent(&e))
     {
         switch (e.type)
         {
            case SDL_QUIT:
                 event = 1;
                 break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        event = 1;
                        break;
                    case SDLK_RETURN:
                        if (scene == 0)
                        {
                            if (selector == 0) event = 2;
                            else if (selector == 1) event = 3;
                            else if (selector == 2) event = 4;
                            else if (selector == 3) event = 1;
                        }
                        if (scene == 2)
                        {
                            if (selector == 0) event = 6;
                            else if (selector == 1) event = 7;
                            else if (selector == 2) event = 8;
                            else if (selector == 3) event = 5;
                        }
                        break;
                    case SDLK_UP:
                        if (inputstatus == 0) selector--;
                        break;
                    case SDLK_DOWN:
                        if (inputstatus == 0) selector++;
                        break;
                    case SDLK_BACKSPACE:
                        if (inputstatus == 1)
                        {
                            int currpos = strlen(filea) - 2;
                            filea[currpos] = 0;
                            strcat(filea, "#");
                        }
                        else if (inputstatus == 2)
                        {
                            int currpos = strlen(fileb) - 2;
                            fileb[currpos] = 0;
                            strcat(fileb, "#");
                        } 
                        break;
                }
                break;
            case SDL_TEXTINPUT:
                if (inputstatus == 1)
                {
                    int currpos = strlen(filea) - 1;
                    if (currpos < flen)
                    {
                        char buf[32];
                        strcpy(buf, e.text.text);
                        filea[currpos] = buf[0];
                        strcat(filea, "#");
                    }
                }
                else if (inputstatus == 2)
                {
                    int currpos = strlen(fileb) - 1;
                    if (currpos < flen)
                    {
                        char buf[32];
                        strcpy(buf, e.text.text);
                        fileb[currpos] = buf[0];
                        strcat(fileb, "#");
                    }
                }
                break;
        }
    }
}

void errscript()
{
    if (guistatus != 0)
    {
        opstatus = -1;
        drawsc();
        SDL_Delay(500);
    }
    else
    {
        printf("Error!\n");
    }
}

void encode()
{
    SDL_Surface *source_image = IMG_Load(filea);
    if (!source_image)
    {
        errscript();
        SDL_FreeSurface(source_image);
        return;
    }
    Uint8 *data = (Uint8 *)source_image -> pixels;
    int bpp = source_image -> format -> BytesPerPixel;
    int width = source_image -> w;
    int height = source_image -> h;
    int pit = source_image -> pitch;

    if (access(fileb, F_OK) != 0)
    {
        errscript();
        SDL_FreeSurface(source_image);
        return;
    }
    FILE *in = fopen(fileb, "rb");
    fseek(in, 0, SEEK_END);
    int size = ftell(in);
    rewind(in);

    if (size >= width * height * 3)
    {
        errscript();
        SDL_FreeSurface(source_image);
        fclose(in);
        return;
    }

    data[0] = size / 65536;
    data[1] = (size - (data[0] * 65536)) / 256;
    data[2] = size - (data[0] * 65536 + data[1] * 256);

    int s = 1;
    int spr = sqrt(width * height * 3 / size);

    int num = size / 33;
    int counter = 0;

    for (int y = 0; y < height; y+=spr)
    {
        if (y == spr) s = 0;
        for (int x = 0 + s * spr; x < width; x+=spr)
        {
            unsigned char byte[3] = {0, 0, 0};
            if (fread(byte, 1, 3, in) == 0) goto skip;
            
            data[x * bpp + y * pit] = byte[0] - 4 * (x + 1);
            data[x * bpp + y * pit + 1] = byte[1] - 8 * (y + 1);
            data[x * bpp + y * pit + 2] = byte[2] - 4 * (x + 1) * (y + 1);

            counter++;
            if (counter == num)
            {
                if (guistatus != 0) drawsc();
                counter = 0;
            }
        }
    }

    skip:
    fclose(in);
    IMG_SavePNG(source_image, "exported.png");
    SDL_FreeSurface(source_image);
}

void decode()
{
    SDL_Surface *source_image = IMG_Load(filea);
    if (!source_image)
    {
        errscript();
        SDL_FreeSurface(source_image);
        return;
    }
    Uint8 *data = (Uint8 *)source_image -> pixels;
    int bpp = source_image -> format -> BytesPerPixel;
    int width = source_image -> w;
    int height = source_image -> h;
    int pit = source_image -> pitch;

    FILE *out = fopen(fileb, "wb");

    int size = data[0] * 65536 + data[1] * 256 + data[2];
    int s = 1;
    int spr = sqrt(width * height * 3 / size);

    int num = size / 33;
    int counter = 0;

    for (int y = 0; y < height; y+=spr)
    {
        if (y == spr) s = 0;
        for (int x = 0 + s * spr; x < width; x+=spr)
        {
            unsigned char byte[3] = {0, 0, 0};
            
            byte[0] = data[x * bpp + y * pit] + 4 * (x + 1);
            byte[1] = data[x * bpp + y * pit + 1] + 8 * (y + 1);
            byte[2] = data[x * bpp + y * pit + 2] + 4 * (x + 1) * (y + 1);

            for (int i = 0; i < 3; i++)
            {
                fwrite(byte + i, 1, 1, out);
                size--;
                if (size == 0) goto skip;
            }

            counter++;
            if (counter == num)
            {
                if (guistatus != 0) drawsc();
                counter = 0;
            }
        }
    }

    skip:
    fclose(out);
    SDL_FreeSurface(source_image);
}

void trim()
{
    int pos = strlen(filea) - 1;
    filea[pos] = 0;
    pos = strlen(fileb) - 1;
    fileb[pos] = 0;
}

int main(int argc, const char *argv[])
{
    printf("-=[ Mi7oZ v0.1alpha ]=-\nCode, Graphics & Music by aolvos/SATAGAX, 2021.\n");

    if (argc > 1)
    {
        if (argc > 4)
        {
            printf("Too many arguments!\nType -h for help.\n");
            return 0;
        }
        if (strcmp(argv[1], "-e") == 0)
        {
            if (argc < 4) goto err;
            printf("Processing...\n");
            strcpy(filea, argv[2]);
            strcpy(fileb, argv[3]);
            encode();
        }
        else if (strcmp(argv[1], "-d") == 0)
        {
            if (argc < 3) goto err;
            printf("Processing...\n");
            strcpy(filea, argv[2]);
            decode();
        }
        else if (strcmp(argv[1], "-h") == 0)
        {
            printf("*FileA(.png/.jpg) : source image file.\n*FileB : any other file.\n-e FileA FileB : encode FileB into FileA.\n-d FileA : decode FileB from FileA.\n-h Display help.\n");
        }
        else
        {
            err:
            printf("Incorrect arguments.\nType -h for help.\n");
        }
        return 0;
    }

    guistatus = 1;
    printf("No arguments specified. Running in GUI mode.\n");
    if (access(config, F_OK) != 0)
    {
        printf("Generating config file...\n");
        FILE *cfg = fopen(config, "w");
        fprintf(cfg, "filea.png\nfileb.png\n");
        fclose(cfg);
    }

    printf("Loading config file...\n");
    FILE *cfg = fopen(config, "r");
    fgets(filea, flen, cfg);
    fgets(fileb, flen, cfg);
    trim();
    fclose(cfg);

    for (int i = 0; i < aval; i++)
    {
        sinetable[i] = sin(astep * i);
        cosinetable[i] = cos(astep * i);
    }

    printf("Starting up!\n");
    SDL_Init(SDL_INIT_EVERYTHING);

    rw = SDL_RWFromMem(menu_png, menu_png_len);
    images[0] = IMG_Load_RW(rw, 1);
    rw = SDL_RWFromMem(font_png, font_png_len);
    images[1] = SDL_ScaleImage(IMG_Load_RW(rw, 1), fscale);
    rw = SDL_RWFromMem(logo_png, logo_png_len);
    images[2] = IMG_Load_RW(rw, 1);

    win = SDL_CreateWindow("Mi7oZ v0.1alpha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    win_r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(win_r, 640, 480);

    for (int i = 0; i < imgnum; i++)
    {
        tex[i] = SDL_CreateTextureFromSurface(win_r, images[i]);
        SDL_FreeSurface(images[i]);
    }

    int xlen = strlen(scrlmsg);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    rw = SDL_RWFromMem(mus_xm, mus_xm_len);
    music = Mix_LoadMUS_RW(rw, 1);
    Mix_PlayMusic(music, -1);
    
    while (true)
    {
        drawsc();

        events();
        if (event != 0)
        {
            if (event == 1) goto exit;
            else if (event == 2)
            {
                scene = 1;
                encode();
                opstatus = 0;
                scene = 0;
            }
            else if (event == 3)
            {
                scene = 1;
                decode();
                opstatus = 0;
                scene = 0;
            }
            else if (event == 4)
            {
                scene = 2;
                selector = 0;
            }
            else if (event == 5)
            {
                cfg = fopen(config, "w");
                fprintf(cfg, "%s\n%s\n", filea, fileb);
                fclose(cfg);
                scene = 0;
                selector = 2;
            }
            else if (event == 6)
            {
                if (inputstatus != 0)
                {
                    int currpos = strlen(filea) - 1;
                    filea[currpos] = 0;
                    inputstatus = 0;
                }
                else
                {
                    strcpy(filea, "#");
                    inputstatus = 1;
                }
            }
            else if (event == 7)
            {
                if (inputstatus != 0)
                {
                    int currpos = strlen(fileb) - 1;
                    fileb[currpos] = 0;
                    inputstatus = 0;
                }
                else
                {
                    strcpy(fileb, "#");
                    inputstatus = 2;
                }
            }
            event = 0;
        }
        incangle(astep * 4);

        xscroll+=2;
        if (xscroll >= 650 + xlen * fontsize) xscroll = 0;
    }

    exit:
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    for (int i = 0; i < imgnum; i++) SDL_DestroyTexture(tex[i]);
    SDL_DestroyRenderer(win_r);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}