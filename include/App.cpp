#include "App.hpp"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

App::App() {
  m_series.setLineColor(LINE_COLOR);
  m_series.setPolygonColor(CIRCLE_COLOR);
  m_series.setSides(m_interface.x_sides, m_interface.y_sides);

  // init SDL2 systems
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    printf("Error: %s\n", SDL_GetError());
  }

  // enable native IME
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // create window
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  m_window = SDL_CreateWindow("Draw Machine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, APP_SIZE.x, APP_SIZE.y, window_flags);
  if (m_window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
  }

  // create render backend
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (m_renderer == nullptr) {
    printf("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
  }

  m_series_display = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, APP_SIZE.x, APP_SIZE.y);
  if (m_series_display == nullptr) {
    printf("Error: SDL_CreateTexture(): %s\n", SDL_GetError());
  }
  // setup dear imgui context
  IMGUI_CHECKVERSION();
  ImGuiContext* context = ImGui::CreateContext();
  ImGui::SetCurrentContext(context);

  // link imgui to sdl2
  ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
  ImGui_ImplSDLRenderer2_Init(m_renderer);

  // setup io
  m_io = ImGui::GetIO(); (void)m_io;
  m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // setup dear imgui style
  ImGui::StyleColorsDark();
}

int App::run() {
  // main loop
  SDL_Event event;
  while (m_running) {
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      switch(event.type) {
        case SDL_QUIT:
          m_running = false;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              m_running = false;
              break;
          }
      }
    }

    // start dear imgui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // show app window
    this->show();

    ImGui::EndFrame();

    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(m_renderer, m_io.DisplayFramebufferScale.x, m_io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(m_renderer, CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
    SDL_RenderClear(m_renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
    SDL_RenderPresent(m_renderer);
  }
  return 0;
}

App::~App() {
  // cleanup
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyTexture(m_series_display);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void App::show() {
  // make window fill viewport
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImVec2 viewport_size = {viewport->Size};
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport_size);
  ImGui::SetNextWindowViewport(viewport->ID);
  
  if (ImGui::Begin("Fourier Series", &m_running, m_app_window_flags)) {
    //  draw sdl2 texture as dear imgui image
    ImGuiWindowFlags fourier_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
    ImVec2 fourier_size {viewport_size.x, viewport_size.y - 256.0f};
    ImGui::SetNextWindowSize(fourier_size);
    ImGui::SetNextWindowPos({0.0f, 0.0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f}); // remove window padding
    if (ImGui::Begin("fourier", &m_running, fourier_window_flags)) {
      ImGui::PopStyleVar();
      SDL_Rect src;
      SDL_QueryTexture(m_series_display, nullptr, nullptr, &src.w, &src.h);
      if (m_interface.play) {
        m_series.update();
      }
      m_series.draw(m_renderer, m_series_display);
      ImGui::Image((ImTextureID)(intptr_t)m_series_display, ImVec2(src.w, src.h));
      ImGui::End();
    }

    // create buttons and sliders for manipulating the series render
    ImGuiWindowFlags interface_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
    ImVec2 interface_size {viewport_size.x, 256.0f};
    ImGui::SetNextWindowSize(interface_size);
    ImGui::SetNextWindowPos({0.0f, viewport_size.y - interface_size.y});
    if (ImGui::Begin("interface", &m_running, interface_flags)) {

      // convert from ImVec4 to SDL_Color
      const auto toSDL_Color = [](const ImVec4 &imgui_color) -> SDL_Color {
        const SDL_Color result {
          static_cast<Uint8>(imgui_color.x * 255.0f),
          static_cast<Uint8>(imgui_color.y * 255.0f),
          static_cast<Uint8>(imgui_color.z * 255.0f),
          static_cast<Uint8>(imgui_color.w * 255.0f)
        };
        return result;
      };
      if (ImGui::ColorEdit4("Background Color", &m_interface.background_color.x)) {
        SDL_Color color = toSDL_Color(m_interface.background_color);
        m_series.setBackgroundColor(color);
      }
      if (ImGui::ColorEdit4("Line Color", &m_interface.line_color.x)) {
        SDL_Color color = toSDL_Color(m_interface.line_color);
        m_series.setLineColor(color);
      }
      if (ImGui::ColorEdit4("Polygon Color", &m_interface.polygon_color.x)) {
        SDL_Color color = toSDL_Color(m_interface.polygon_color);
        m_series.setPolygonColor(color);
      }

      // if stopped, show play button, show stop button otherwise
      if (!m_interface.play) {
        if (ImGui::Button("Play")) {
          m_interface.play = true;
        }
      } else {
        if (ImGui::Button("Stop")) {
          m_interface.play = false;
        }
      }

      if (ImGui::Button("Clear")) {
        m_series.clearResult();
      }

      if (m_interface.play) {
        m_interface.frame = m_series.getFrame();
        ImGui::SliderInt("Time", &m_interface.frame, 0, m_series.getFrames());
      } else {
        if (ImGui::SliderInt("Time", &m_interface.frame, 0, m_series.getFrames())) {
          m_series.setFrame(m_interface.frame);
        }
      }

      if (ImGui::SliderInt("X Axis Sides", &m_interface.x_sides, 3, 32)) {
        m_series.setSides(m_interface.x_sides, m_interface.y_sides);
      }
      if (ImGui::SliderInt("Y Axis Sides", &m_interface.y_sides, 3, 32)) {
        m_series.setSides(m_interface.x_sides, m_interface.y_sides);
      }

      ImGui::End();
    }

    ImGui::End();
  }
}
