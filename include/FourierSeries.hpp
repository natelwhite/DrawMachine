#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <fstream>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

struct Complex {
  double re, im;
};

struct Frequency {
  double period, amplitude, phase;
  // returns point tangential to circle according to frequency, amplitude, phase, and time
  SDL_FPoint getPoint(const float &time) {
    SDL_FPoint result;
    result.x = cos(period * time + phase) * amplitude;
    result.y = sin(period * time + phase) * amplitude;
    return result;
  }
};

class FourierSeries {
  public:
    FourierSeries(const std::string &path, const int &t_width, const int &t_height);
    void draw(SDL_Renderer* renderer, SDL_Texture* tex);
    void update(); // advance one frame
    int getFrames(); // the number of frames in the animation
    int getFrame(); // the current frame
    void setFrame(const int &frame);
    void setCircleColor(const SDL_Color &color);
    void setLineColor(const SDL_Color &color);
    void clearResult();
  private:
    std::vector<Frequency> m_xCircles, m_yCircles;
    std::vector<SDL_FPoint> m_result;
    int m_frames;
    int m_width, m_height;
    SDL_Color m_circleColor {255, 255, 255, 255}, m_lineColor {255, 0, 0, 255}, m_backgroundColor {0, 0, 0, 255};
    float m_time { };

    void drawPolygon(SDL_Renderer*, const SDL_FPoint &pos, const float &radius, const float &phase);
};

