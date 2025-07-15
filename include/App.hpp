#pragma once
#include "Machine.hpp"
#include <SDL3/SDL.h>
#include "imgui.h"
#include <string>

struct InterfaceData {
	bool play {true};
	int frame {0};
	int sides {12};
	ImVec4 background_color {0.0f, 0.0f, 0.0f, 1.0f}; // black
	ImVec4 line_color {1.0f, 1.0f, 1.0f, 1.0f}; // white
	ImVec4 polygon_color {1.0f, 1.0f, 1.0f, 0.5f}; // semi-transparent white
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
		const ImVec2 APP_SIZE {1920.0f, 1080.0f};
		const ImVec2 INTERFACE_SIZE {1920.0f, 256.0f};

		Machine m_series {"data/source.txt"};
		InterfaceData m_interface;
		SDL_Texture* m_series_display;
		ImGuiIO m_io;
		SDL_Renderer *m_renderer;
		SDL_Window *m_window;

		// window flags
		const static ImGuiWindowFlags m_app_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
};

