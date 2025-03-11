#pragma once
#include "Machine.hpp"
#include <SDL3/SDL.h>
#include "imgui.h"
#include <string>

struct InterfaceData {
	bool play {true};
	int frame {0};
	int sides {12};
	ImVec4 background_color {0.2f, 0.2f, 0.2f, 1.0f};
	ImVec4 line_color {0.78f, 0.12f, 0.12f, 1.0f};
	ImVec4 polygon_color {0.12f, 0.78, 0.12f, 1.0f};
};

class App {
	public:
		App(); // init imgui & SDL2 + create window & renderer
		int run(); // run main frame loop
		~App(); // safely quit imgui & SDL2
	private:
		void show(); // show imgui widgets
		bool m_running = true;

		// window size
		const ImVec2 APP_SIZE {900.0f, 900.0f};
		const ImVec2 FOURIER_SIZE {900.0f, 900.0f};

		Machine m_series {"data/source.txt", static_cast<int>(FOURIER_SIZE.x), static_cast<int>(FOURIER_SIZE.y)};
		InterfaceData m_interface;
		SDL_Texture* m_series_display;
		ImGuiIO m_io;
		SDL_Renderer *m_renderer;
		SDL_Window *m_window;

		// window flags
		const static ImGuiWindowFlags m_app_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
};

