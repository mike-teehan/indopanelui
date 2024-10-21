/****** plasma.cpp
 * IndoPanel UI (SDL + ImGui)
 * Mike Teehan <mike.teehan@gmail.com>
 * Copyright 2024
 * License: GPL (see LICENSE for details)
 ******/

#include "plasma.h"

Plasma::Plasma()
{
        this->ColorGen();
}

SDL_Surface* Plasma::frame(SDL_Surface* frame)
{
    int loopx, loopy;// Loop variables.
    int cv1, cv2, ca1, ca2;// Used in calculations.
//    Uint32 draw_point = 0;
//    this->genadd1 += 4;// Change these additions
//    this->genadd2 += 3;// For different speed X+Y.
    this->genadd1 += 3;// Change these additions
    this->genadd2 += 7;// For different speed X+Y.
//    char *pix;

    if(this->genadd1 > 360)
        this->genadd1 = this->genadd1 - 360;
    if(this->genadd2 > 360)
        this->genadd2 = this->genadd2 - 360;

    ca1 = 50 + 49 * this->_costable[this->genadd1];// You can play with these vars.
    ca2 = 50 + 75 * this->_sintable[this->genadd2];// Strange things may happen though.

    SDL_LockSurface(frame);
    for(loopy=0; loopy < frame->h; loopy++)
    {
        cv1 = 205 + (ca1 * this->_sintable[this->genadd2 + loopy]);
        for(loopx=0; loopx < frame->w; loopx++)
        {
            cv2 = 400+(ca2*this->_costable[this->genadd1+loopx]);
//            pix = static_cast<char *>(frame->pixels) + draw_point;
//            auto pix = ((Uint32*)frame->pixels) + draw_point;
            this->setSurfacePixel(frame, loopx, loopy, this->_colourtable[cv1 + cv2]);
//            *pix = this->_colourtable[cv1 + cv2];
//            draw_point += 4;
        }
    }
    SDL_UnlockSurface(frame);

    return frame;
}

void Plasma::setSurfacePixel(SDL_Surface* surface, const int x, const int y, const Uint32& pixel)
{
    Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}

void Plasma::ColorGen()
{
    int colourload;
    int basered=0, basegreen=0, baseblue=255;
    int redbounce=0, greenbounce=0, bluebounce=0;

    for(int a = 0; a < 2880; a++)
    {
        this->_sintable[a] = sin(a * DEG);
        this->_costable[a] = cos(a * DEG);
    }
    for(colourload=0; colourload<1024; colourload++)
    {
        if(redbounce==0)
        {
            basered++;
            if(basered>100)
            {
                basered=255;
                redbounce=1;
            }
        }
        if(greenbounce==0 && colourload>512)
        {
            basegreen+=2;
            if(basegreen>255)
            {
                basegreen= 255;
                greenbounce=1;
            }
        }
        if(bluebounce==0)
        {
            baseblue--;
            if(baseblue<0)
            {
                baseblue = 255;
                bluebounce=1;
            }
        }
        if(redbounce==1)
        {
            basered-=3;
            if(basered<0)
            {
                basered = 0;
                redbounce=0;
            }
        }
        if(greenbounce==1)
        {
            basegreen-=1;
            if(basegreen<0)
            {
                basegreen=0;
                greenbounce=0;
            }
        }
        if(bluebounce==1)
        {
            baseblue++;
            if(baseblue>255)
            {
                baseblue = 0;
                bluebounce=0;
            }
        }

        this->_colourtable[colourload] = ((basered << 8) + basegreen << 8) + baseblue;
    }
}

/*
int main(int argc, char* argv[])
{
   if(SDL_Init(SDL_INIT_VIDEO)!=0) return 1;
   screen = SDL_SetVideoMode(GRAPHICS_WIDTH, GRAPHICS_HEIGHT, 32, 0);
   if(screen == NULL) return 1;

   int a;
   for(a=0; a<2880; a++){sintable[a]=sin(a*DEG); costable[a]=cos(a*DEG);}

   ColourGen();
   while(1)
   {
       Plasma();
       while(SDL_PollEvent(&event))
           if(event.type==SDL_KEYDOWN)
               return 0;
       if(SDL_Flip(screen)==-1) return 1;
   }
}*/

