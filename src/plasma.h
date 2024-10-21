/****** plasma.h
 * IndoPanel UI (SDL + ImGui)
 * Mike Teehan <mike.teehan@gmail.com>
 * Copyright 2024
 * License: GPL (see LICENSE for details)
 ******/

#pragma once

#include <SDL2/SDL.h>
#include <math.h>

class Plasma
{
public:
    Plasma();
    SDL_Surface* frame(SDL_Surface* frame);

private:
    static constexpr float DEG = 0.017453292519943296f;
    Uint32 _colourtable[1024];
    int genadd1 = 0, genadd2 = 0;
    float _sintable[2880];
    float _costable[2880];

    void ColorGen();
    void setSurfacePixel(SDL_Surface* surface, const int x, const int y, const Uint32& pixel);
};
