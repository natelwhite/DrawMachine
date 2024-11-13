#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

struct Circle {
  double frequency;
  double amplitude;
  double phase;
};

class FourierSeries {
  public:
    FourierSeries(const std::vector<float> &x_path, const std::vector<float> &y_path, const int &t_height);
    void draw(SDL_Renderer*);
    void update();
    float getY();
    float getX();
    float getTime();
    void setCircleColor(const int&, const int&, const int&, const int&);
    void setLineColor(const int&, const int&, const int&, const int&);
  private:
    void drawCircle(SDL_Renderer*, int, int, int);
    std::vector<Circle> m_xCircles;
    std::vector<Circle> m_yCircles;
    float m_lastX, m_lastY, m_size;
    int m_height;
    int circleColor[4] {255, 255, 255, 255}, lineColor[4] {255, 0, 0, 255};
    float time { };
};
