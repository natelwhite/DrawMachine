#pragma once
#include "FourierSeries.hpp"

#include <vector>

#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <string>
#include <fstream>

struct SeriesInterface {
  bool play;
};

class App
{
  public:
    App(); // init imgui & SDL2 + create window & renderer
    int run(); // run main frame loop
    ~App(); // safely quit imgui & SDL2
  private:
    void show(); // show imgui widgets
    bool m_running = true;

    const ImVec2 APP_SIZE {1200.0f, 900.0f};
    const ImVec2 INTERFACE_SIZE {APP_SIZE.x, 64.0f};
    const ImVec2 FOURIER_SIZE {APP_SIZE.x, APP_SIZE.y - INTERFACE_SIZE.y};
    /*const int WIDTH {1200};*/
    /*const int HEIGHT {900};*/
    const std::string DATA_PATH {"source.txt"};
    const SDL_Color CLEAR_COLOR {51, 51, 51, 255};
    const SDL_Color CIRCLE_COLOR { 200, 30, 30, 255 };
    const SDL_Color LINE_COLOR { 30, 200, 30, 255 };

    FourierSeries m_series {DATA_PATH, static_cast<int>(FOURIER_SIZE.x), static_cast<int>(FOURIER_SIZE.y)};
    SeriesInterface m_series_interface {false};
    SDL_Texture* m_series_display;
    ImGuiIO m_io;
    SDL_Renderer *m_renderer;
    SDL_Window *m_window;

    // window flags
    const static ImGuiWindowFlags m_app_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

    const static ImGuiWindowFlags m_tools_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

};

