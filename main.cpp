#include <SDL2/SDL.h>
#include "FourierSeries.hpp"
#include <vector>
#include <string>
#include <fstream>

const int WIDTH {1920}, HEIGHT {1080};
const int FOURIER_START {50};
int size {};
Uint32 getPixel(SDL_Surface*, int, int);

int main(int argc, char *args[] ){
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    std::vector<double> xArr; // x vals
    std::vector<double> yArr; // y vals
    std::vector<SDL_FPoint> path; // the path that has been drawn so far

    // build arr of x vals and y vals seperately
    std::ifstream source ("source.txt");
    SDL_Event e;
    bool quit {false};
    if (source.is_open())
    {
        int i{};
        std::string line;
        while (std::getline(source, line))
        {
            xArr.push_back(std::stod(line.substr(line.find(':') + 2, 6)));
            yArr.push_back(std::stod(line.substr(line.find(',') + 2, 6)));
            i++;
        }
        size = i;
    }
    else
    {
        quit = true;
    }

    FourierSeries series {xArr.data(), yArr.data(), size, FOURIER_START, WIDTH, FOURIER_START, HEIGHT};
    int circleColor[4] { 255, 0, 255, 255 };
    int lineColor[4] { 255, 255, 255, 255 };
    series.setLineColor(lineColor, lineColor + 1, lineColor + 2, lineColor + 3);
    series.setCircleColor(circleColor, circleColor + 1, circleColor + 2, circleColor + 3);
    // game loop
    while (!quit){
        // event handling
        while (SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 78, 0, 78, 255);
        SDL_RenderClear(renderer);

        series.draw(renderer);
        path.push_back(SDL_FPoint {series.getX(), series.getY()});
        if (series.getTime() == 0)
        {
            path.clear();
        }
        SDL_RenderDrawLinesF(renderer, path.data(), path.size());
        series.update();
        SDL_RenderPresent(renderer);
        SDL_Delay(23);
    }
    return 0;
}

Uint32 getPixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8* p {(Uint8*)surface->pixels + y * surface->pitch + x * bpp};

    switch(bpp)
    {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;
        case 4:
            return *(Uint16 *)p;
            break;
        default:
            return 0;
    }
}
