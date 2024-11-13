#include <SDL2/SDL.h>
#include "FourierSeries.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

const int WIDTH {1200}, HEIGHT {900};
const int FOURIER_START {0};
const int CIRCLE_COLOR[4] { 200, 30, 30, 255 };
const int LINE_COLOR[4] { 30, 200, 30, 255 };

int main(int argc, char *args[] )
{
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

  std::vector<double> xArr; // x vals
  std::vector<double> yArr; // y vals
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
      xArr.push_back(std::stod(x));
      yArr.push_back(std::stod(y));
      i++;
    }
    size = i; // number of points in source
  } else {
    std::cout << "could not open source.txt" << std::endl;
    quit = true;
  }

  FourierSeries series {xArr.data(), yArr.data(), size, HEIGHT};
  series.setLineColor(LINE_COLOR[0], LINE_COLOR[1], LINE_COLOR[2], LINE_COLOR[3]);
  series.setCircleColor(CIRCLE_COLOR[0], CIRCLE_COLOR[1], CIRCLE_COLOR[2], CIRCLE_COLOR[3]);

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
    path.push_back(SDL_FPoint {series.getX(), series.getY()});

    SDL_RenderDrawLinesF(renderer, path.data(), path.size());
    series.update();
    SDL_RenderPresent(renderer);
  }
  return 0;
}
