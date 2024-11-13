#include "FourierSeries.hpp"
FourierSeries::FourierSeries(double* xArr, double* yArr, int count, int t_height) {
  m_height = t_height;

  // size of array | num of vectors to find for each arr
  size = count; 

  // allocate memory for 2 arrs of vectors. arr[0] will be the fundamental frequency. arr[count] will be the output
  xVectors = new vector[count];
  yVectors = new vector[count];

  // break down path coordinates into vectors (dft)
  // one 'fourier system' for each axis
  for (int i{}; i < count; i++)
  {
    double xRe{}, xIm{}; // real, imaginary | complex num | represents coordinate point
    double yRe{}, yIm{};
    double tau {2 * M_PI}; // circumfrence to radius ratio

    // discrete integral over x values to get coordinate points on complex plane
    for (int j{}; j < count; j++)
    {
      const double phi {tau * i * j / size}; // phase
      xRe += xArr[j] * cos(phi); // real x value      | terminal x val of vector
      xIm -= xArr[j] * sin(phi); // imaginary y value | terminal y val of vector
      yRe += yArr[j] * cos(phi);
      yIm -= yArr[j] * sin(phi);
    }
    xRe /= size; // average out all values
    xIm /= size;
    yRe /= size;
    yIm /= size;
    xVectors[i].frequency = i; // num of rotations per unit time
    xVectors[i].amplitude = sqrt(xRe * xRe + xIm * xIm); // a^2 + b^2 = c^2 | c^2 = length/amplitude of vector
    // ^ this is also the radius of the circle formed around the vector
    xVectors[i].phase = atan2(xIm, xRe); // starting rotation for vector
    yVectors[i].frequency = i;
    yVectors[i].amplitude = sqrt(yRe * yRe + yIm * yIm);
    yVectors[i].phase = atan2(yIm, yRe);
    yVectors[i].phase -= M_PI / 2; // turn yVectors 90 deg
  }
}

void FourierSeries::draw(SDL_Renderer* renderer) {
  // vectors that draw x values
  float xAxis[2] {0, 0};
  float yAxis[2] {0, 0};
  yAxis[1] = m_height / 2.0;
  for (int i{}; i < size; i++) {
    float prevX[2] {xAxis[0], xAxis[1]}; // coordinate pair for xVector[i]
    float prevY[2] {yAxis[0], yAxis[1]}; // coordinate pair for yVector[i]
    xAxis[0] += cos(xVectors[i].frequency * time + xVectors[i].phase) * xVectors[i].amplitude; // x = cos(frequency over time plus the initial angle) * strength of vector
    xAxis[1] += sin(xVectors[i].frequency * time + xVectors[i].phase) * xVectors[i].amplitude; // same but y, so sin()
    yAxis[0] += cos(yVectors[i].frequency * time + yVectors[i].phase) * yVectors[i].amplitude;
    yAxis[1] += sin(yVectors[i].frequency * time + yVectors[i].phase) * yVectors[i].amplitude;
    SDL_SetRenderDrawColor(renderer, lineColor[0], lineColor[1], lineColor[2], lineColor[3]); // draw lines in red

    // draw a line from one vector to the next
    SDL_RenderDrawLine(renderer, prevX[0], prevX[1], xAxis[0], xAxis[1]);
    SDL_RenderDrawLine(renderer, prevY[0], prevY[1], yAxis[0], yAxis[1]);

    // draw circle to represent vector
    drawCircle(renderer, prevX[0], prevX[1], xVectors[i].amplitude);
    drawCircle(renderer, prevY[0], prevY[1], yVectors[i].amplitude);
  }

  // give access to vector result
  lastX = xAxis[0];
  lastY = yAxis[1];

  // draw a line from the system to the result
  int xVectorLastY = xAxis[1];
  int yVectorLastX = yAxis[0];

  SDL_SetRenderDrawColor(renderer, lineColor[0], lineColor[1], lineColor[2], lineColor[3]);
  SDL_RenderDrawLine(renderer, lastX, xVectorLastY, lastX, lastY);
  SDL_RenderDrawLine(renderer, yVectorLastX, lastY, lastX, lastY);
}

void FourierSeries::drawCircle(SDL_Renderer* renderer, int xPos, int yPos, int radius) {
  SDL_SetRenderDrawColor(renderer, circleColor[0], circleColor[1], circleColor[2], circleColor[3]);
  float x{}, y{};
  // calculate coordinate of tangent points along the circumfrence
  for (double i{}; i < 2 * M_PI; i += M_PI / 36) 
  {
    x = cos(i) * radius + xPos;
    y = sin(i) * radius + yPos;
    SDL_RenderDrawPointF(renderer, x, y);
  }
}

// get x-axis tail (y-coordinate of path)
float FourierSeries::getY() { 
  return lastY;
}

// get y-axis tail (x-coordinate of path)
float FourierSeries::getX() {
  return lastX;
} 

// returns 0 when the series repeats
float FourierSeries::getTime() {
  return time;
}

// move vectors to next position
void FourierSeries::update() {
  (time <= 2 * M_PI) ? (time += 2 * M_PI / size) : (time = 0);
}

void FourierSeries::setCircleColor(const int &r, const int &g, const int &b, const int &a) {
  circleColor[0] = r;
  circleColor[1] = g;
  circleColor[2] = b;  
  circleColor[3] = a;
}

void FourierSeries::setLineColor(const int &r, const int &g, const int &b, const int &a) { 
  lineColor[0] = r;
  lineColor[1] = g;
  lineColor[2] = b;  
  lineColor[3] = a; 
}
