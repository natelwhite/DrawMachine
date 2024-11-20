#include "App.hpp"
#include <iostream>

App::App() {
  m_series.setLineColor(LINE_COLOR);
  m_series.setCircleColor(CIRCLE_COLOR);

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
  m_window = SDL_CreateWindow("Palettable", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  if (m_window == nullptr) {
    printf("Error: SDL_CreateWindow0(): %s\n", SDL_GetError());
  }

  // create render backend
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (m_renderer == nullptr) {
    SDL_Log("Error creating SDL_Renderer!");
  }

  m_series_display = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
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
    m_series.update();
    m_series.draw(m_renderer, m_series_display);
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

  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void App::show() {
  // make window fill viewport
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);
  
  if (ImGui::Begin("Fourier Series", &m_running, m_app_window_flags)) {
    // calculate window positions and sizes
    ImVec2 app_size = ImGui::GetWindowSize();

    //  draw sdl2 texture as dear imgui image
    int width, height;
    SDL_QueryTexture(m_series_display, nullptr, nullptr, &width, &height);
    ImGui::Image((ImTextureID)(intptr_t)m_series_display, ImVec2(static_cast<float>(width), static_cast<float>(height)));
    ImGui::End();
  }
}

