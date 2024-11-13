#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

struct Circle {
  float frequency;
  float amplitude;
  float phase;
  SDL_FPoint getPoint(const float &time) {
    SDL_FPoint result;
    result.x = cos(frequency * time + phase) * amplitude;
    result.y = sin(frequency * time + phase) * amplitude;
    return result;
  }
};

class FourierSeries {
  public:
    FourierSeries(const std::vector<float> &x_path, const std::vector<float> &y_path, const int &t_height);
    void draw(SDL_Renderer*);
    void update();
    SDL_FPoint getPoint();
    float getTime();
    void setCircleColor(const SDL_Color &color);
    void setLineColor(const SDL_Color &color);
  private:
    void drawCircle(SDL_Renderer*, const SDL_FPoint &pos, const float &radius);
    std::vector<Circle> m_xCircles, m_yCircles;
    float m_lastX, m_lastY, m_size;
    int m_height;
    SDL_Color m_circleColor {255, 255, 255, 255}, m_lineColor {255, 0, 0, 255};
    float m_time { };
};

