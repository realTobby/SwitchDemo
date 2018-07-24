#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;

#include <switch.h>

#include "./main.hpp"

uint32_t screen_w, screen_h;    // Screen Resolution

#define STAR_COUNT 100
#define CURVE_POINT_COUNT 2000

int main()
{
    bool loop = true;
    uint32_t* gfx_data;             // Framebuffer pixel data
    POINT stars1[STAR_COUNT];
    POINT stars2[STAR_COUNT];
    POINT stars3[STAR_COUNT];
    POINT stars4[STAR_COUNT];


    int16_t* curve_points_x = NULL;
    int16_t* curve_points_y = NULL;

    gfxInitDefault();       // GFX Init
    gfx_data = (uint32_t*)gfxGetFramebuffer(&screen_w, &screen_h);

    srand (time(NULL));

    // Generate Starfield
    GenerateStars(stars1,STAR_COUNT,screen_w, screen_h);
    GenerateStars(stars2,STAR_COUNT,screen_w, screen_h);
    GenerateStars(stars3,STAR_COUNT,screen_w, screen_h);
    GenerateStars(stars4,STAR_COUNT,screen_w, screen_h);

    // Generate Sinuscurve
    curve_points_x = new int16_t[CURVE_POINT_COUNT];
    curve_points_y = new int16_t[CURVE_POINT_COUNT];

    for(int i=0; i<CURVE_POINT_COUNT; i++)
    {
        curve_points_x[i] = sin((i/(float)CURVE_POINT_COUNT)*2*M_PI*2)*250;
        curve_points_x[i] += screen_w/2;

        curve_points_y[i] = sin((i/(float)CURVE_POINT_COUNT)*2*M_PI*10)*250;
        curve_points_y[i] += screen_h/2;
    }

    while(appletMainLoop() && loop)
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) loop = false; // break in order to return to hbmenu

        // get gfx_data pointer
        gfx_data = (uint32_t*)gfxGetFramebuffer(NULL, NULL);

        ClearScreen(gfx_data,screen_w,screen_h,0);

        DrawStarfield(stars1,STAR_COUNT,gfx_data,RGBA8_MAXALPHA(50,50,50));
        DrawStarfield(stars2,STAR_COUNT,gfx_data,RGBA8_MAXALPHA(130,130,130));
        DrawStarfield(stars3,STAR_COUNT,gfx_data,RGBA8_MAXALPHA(200,200,200));
        DrawStarfield(stars4,STAR_COUNT,gfx_data,RGBA8_MAXALPHA(255,255,255));

        ScrollHorStarfield(stars1,STAR_COUNT,screen_w,1);
        ScrollHorStarfield(stars2,STAR_COUNT,screen_w,2);
        ScrollHorStarfield(stars3,STAR_COUNT,screen_w,3);
        ScrollHorStarfield(stars4,STAR_COUNT,screen_w,4);


        uint32_t col = RGBA8_MAXALPHA(255,0,0);
        for(int i=0; i<CURVE_POINT_COUNT; i++)
        {
            SetPixel(gfx_data,curve_points_x[i],curve_points_y[i],col);
        }

        int16_t tmp = curve_points_x[0];
        for(int i=0; i<CURVE_POINT_COUNT-1; i++)
        {
            curve_points_x[i] = curve_points_x[i+1];
        }
        curve_points_x[CURVE_POINT_COUNT-1] = tmp;

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return(0);
}

void ClearScreen(uint32_t* screen, int width, int heigth, uint32_t color)
{
    for(int i=0; i<(width*heigth); i++)
        *screen++=color;
}

void SetPixel(uint32_t* screen, int16_t x, int16_t y, uint32_t color)
{
    if((x < 0) || (y < 0)) return;
    if((x >= (int16_t)screen_w) || (y >= (int16_t)screen_h)) return;

    screen[y*screen_w + x] = color;
}

void GenerateStars(POINT* starfield, int count, int width, int heigth)
{
    for(int i=0; i<count; i++)
    {
        starfield[i].x = rand() % width;
        starfield[i].y = rand() % heigth;
    }
}

void ScrollHorStarfield(POINT* starfield, int count, int width, int step)
{
    for(int i=0; i<count; i++)
    {
        starfield[i].x -= step;
        if(starfield[i].x < 0)
            starfield[i].x += width;
    }
}

void DrawStarfield(POINT* starfield, int count, uint32_t* screen, uint32_t color)
{
    for(int i=0; i<count; i++)
    {
        SetPixel(screen,starfield[i].x,starfield[i].y,color);
    }
}
