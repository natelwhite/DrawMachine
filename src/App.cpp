#include "App.hpp"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

App::App() {
	const auto toSDL_Color = [](const ImVec4 &imgui_color) -> SDL_Color {
		const SDL_Color result {
			static_cast<Uint8>(imgui_color.x * 255.0f),
			static_cast<Uint8>(imgui_color.y * 255.0f),
			static_cast<Uint8>(imgui_color.z * 255.0f),
			static_cast<Uint8>(imgui_color.w * 255.0f)
		};
		return result;
	};
	m_series.setLineColor(toSDL_Color(m_interface.line_color));
	m_series.setBackgroundColor(toSDL_Color(m_interface.background_color));
	m_series.setPolygonColor(toSDL_Color(m_interface.polygon_color));
	m_series.setSides(m_interface.sides);

	// init SDL2 systems
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error: %s\n", SDL_GetError());
	}

	// create window
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_HIGH_PIXEL_DENSITY);
	m_window = SDL_CreateWindow("Draw Machine", APP_SIZE.x, APP_SIZE.y, window_flags);
	if (m_window == nullptr) {
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
	}

	// create render backend
	m_renderer = SDL_CreateRenderer(m_window, NULL);
	if (m_renderer == nullptr) {
		printf("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
	}

	m_series_display = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, FOURIER_SIZE.x, FOURIER_SIZE.y);
	if (m_series_display == nullptr) {
		printf("Error: SDL_CreateTexture(): %s\n", SDL_GetError());
	}
	// setup dear imgui context
	IMGUI_CHECKVERSION();
	ImGuiContext* context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);

	// link imgui to sdl2
	ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
	ImGui_ImplSDLRenderer3_Init(m_renderer);

	// setup io
	m_io = ImGui::GetIO(); (void)m_io;
	m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// setup dear imgui style
	ImGui::StyleColorsDark();
}

int App::run() {
	// main loop
	SDL_Event event;
	while (m_running) {
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			switch(event.type) {
			case SDL_EVENT_QUIT:
				m_running = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				switch (event.key.key) {
				case SDLK_ESCAPE:
					m_running = false;
					break;
				}
				break;
			}
		}

		// start dear imgui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		// show app window
		this->show();

		ImGui::EndFrame();

		// Rendering
		ImGui::Render();
		SDL_SetRenderScale(m_renderer, m_io.DisplayFramebufferScale.x, m_io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(m_renderer, m_interface.background_color.x, m_interface.background_color.y, m_interface.background_color.z, m_interface.background_color.w);
		SDL_RenderClear(m_renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
		SDL_RenderPresent(m_renderer);
	}
	return 0;
}

App::~App() {
	// cleanup
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
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

	if (ImGui::Begin("Fourier Series", &m_running, m_app_window_flags)) {
		//  draw sdl2 texture as dear imgui image
		ImGuiWindowFlags fourier_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
		ImVec2 fourier_size {viewport_size.x, viewport_size.y - 256.0f};
		ImGui::SetNextWindowSize(fourier_size);
		ImGui::SetNextWindowPos({0.0f, 0.0f});
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f}); // remove window padding
		if (ImGui::Begin("fourier", &m_running, fourier_window_flags)) {
			ImGui::PopStyleVar();
			SDL_FRect src;
			SDL_GetTextureSize(m_series_display, &src.w, &src.h);
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
				ImGui::SliderInt("Time", &m_interface.frame, 0, m_series.getFrames()-1);
			} else {
				if (ImGui::SliderInt("Time", &m_interface.frame, 0, m_series.getFrames()-1)) {
					m_series.setFrame(m_interface.frame);
				}
			}

			if (ImGui::SliderInt("Frequency Sides", &m_interface.sides, 3, 32)) {
				m_series.setSides(m_interface.sides);
			}

				ImGui::End();
			}

		ImGui::End();
	}
}
