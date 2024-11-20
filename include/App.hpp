#pragma once
#include "FourierSeries.hpp"

#include <vector>

#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <string>
#include <fstream>

class App
{
  public:
    App(); // init imgui & SDL2 + create window & renderer
    int run(); // run main frame loop
    ~App(); // safely quit imgui & SDL2
  private:
    void show(); // show imgui widgets
    bool m_running = true;

    const int WIDTH {1200};
    const int HEIGHT {900};
    const std::string DATA_PATH {"source.txt"};
    const SDL_Color CLEAR_COLOR {51, 51, 51, 255};
    const SDL_Color CIRCLE_COLOR { 200, 30, 30, 255 };
    const SDL_Color LINE_COLOR { 30, 200, 30, 255 };

    FourierSeries m_series {DATA_PATH, WIDTH, HEIGHT};
    SDL_Texture* m_series_display;
    ImGuiIO m_io;
    SDL_Renderer *m_renderer;
    SDL_Window *m_window;

    // window flags
    const static ImGuiWindowFlags m_app_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

    const static ImGuiWindowFlags m_tools_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

};

