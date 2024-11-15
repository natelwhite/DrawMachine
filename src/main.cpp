#include <SDL2/SDL.h>
#include "FourierSeries.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

const int WIDTH {1200}, HEIGHT {900};
const int FOURIER_START {0};
const SDL_Color CIRCLE_COLOR { 200, 30, 30, 255 };
const SDL_Color LINE_COLOR { 30, 200, 30, 255 };

int main(int argc, char *args[] )
{
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Error: %s\n", SDL_GetError());
  }

  // enable native IME
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

  std::vector<float> x_path, y_path; // x & y coordinates
  std::vector<SDL_FPoint> path; // the path that has been drawn so far

  // build arr of x vals and y vals seperately
  std::ifstream source ("source.txt"); // a list of points
  SDL_Event e;
  bool quit {false};

  // store data found in source
  int size {};
  if (source.is_open()) {
    int i{};
    std::string line;
    while (std::getline(source, line)) {
      int start = line.find(':') + 2;
      int end = line.find(',');
      std::string x = line.substr(start, end - start);
      start = end + 2;
      end = line.length() - 1;
      std::string y = line.substr(start, end - start);
      x_path.emplace_back(std::stof(x));
      y_path.emplace_back(std::stof(y));
      i++;
    }
    size = i; // number of points in source
  } else {
    std::cout << "could not open source.txt" << std::endl;
    quit = true;
  }

  FourierSeries series {x_path, y_path, HEIGHT, WIDTH};
  series.setLineColor(LINE_COLOR);
  series.setCircleColor(CIRCLE_COLOR);

  while (!quit) {
    // event handling
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        quit = true;
      }
    }

    // draw background
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderClear(renderer);

    // draw fourier + path created from tail of fourier
    series.draw(renderer);
    path.emplace_back(series.getPoint());

    SDL_RenderDrawLinesF(renderer, path.data(), path.size());
    series.update();
    SDL_RenderPresent(renderer);
    SDL_Delay(50);
  }
  return 0;
}
