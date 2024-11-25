#pragma once
#include "FourierSeries.hpp"

#include <vector>

#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <string>
#include <fstream>

struct InterfaceData {
  bool play {true};
  int frame {0};
  int x_sides {3};
  int y_sides {3};
  ImVec4 background_color {0.2f, 0.2f, 0.2f, 1.0f};
  ImVec4 line_color {0.78f, 0.12f, 0.12f, 1.0f};
  ImVec4 polygon_color {0.12f, 0.78, 0.12f, 1.0f};
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

    // window sizes
    const ImVec2 APP_SIZE {900.0f, 900.0f};

    const std::string DATA_PATH {"source.txt"};
    const SDL_Color CLEAR_COLOR {51, 51, 51, 255};
    const SDL_Color CIRCLE_COLOR { 200, 30, 30, 255 };
    const SDL_Color LINE_COLOR { 30, 200, 30, 255 };

    FourierSeries m_series {DATA_PATH, 800, 800};
    InterfaceData m_interface;
    SDL_Texture* m_series_display;
    ImGuiIO m_io;
    SDL_Renderer *m_renderer;
    SDL_Window *m_window;

    // window flags
    const static ImGuiWindowFlags m_app_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
};

