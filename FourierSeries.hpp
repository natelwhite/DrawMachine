#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>

struct vector
{
    double frequency{};
    double amplitude{};
    double phase{};
};

class FourierSeries
{
    public:
        FourierSeries(double*, double*, int, int, int, int, int);
        void draw(SDL_Renderer*);
        void update();
        float getY();
        float getX();
        float getTime();
        void setCircleColor(int*, int*, int*, int*);
        void setLineColor(int*, int*, int*, int*);
    private:
        void drawCircle(SDL_Renderer*, int, int, int);
        vector* xVectors;
        vector* yVectors;
        float lastX{}, lastY{};
        float size;
        int pixWidth{}, pixHeight{}, xPixStart{}, yPixStart{};
        int circleColor[4] {255, 255, 255, 255}, lineColor[4] {255, 0, 0, 255};
        float time {};
};