#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <SDL3/SDL.h>
#include <vector>
#include <string>

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

class Machine {
  public:
    Machine(const std::string &path, const std::string &filename, const int &t_width, const int &t_height);
    void draw(SDL_Renderer* renderer, SDL_Texture* tex); // draw series to sdl texture
    void update(); // advance one frame

    int getFrames(); // the number of frames in the animation
    int getFrame(); // the current frame

    void setFrame(const int &frame);
    void setSides(const int &sides);
    void setBackgroundColor(const SDL_Color &color);
    void setPolygonColor(const SDL_Color &color);
    void setLineColor(const SDL_Color &color);
    void clearResult();
  private:
    std::vector<Frequency> m_series;
    std::vector<SDL_FPoint> m_result;
    int m_frames;
    int m_width, m_height;
    int m_sides {12}; // number of sides to display for each frequency
    SDL_Color m_polygon_color {255, 255, 255, 255}, m_line_color {255, 0, 0, 255}, m_background_color {0, 0, 0, 255};
    float m_time { };

    void drawPolygon(SDL_Renderer*, const int &sides, const SDL_FPoint &pos, const float &radius, const float &phase);
};

