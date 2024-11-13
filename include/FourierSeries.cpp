#include "FourierSeries.hpp"
FourierSeries::FourierSeries(std::vector<float> x_path, std::vector<float> y_path, int t_height)
  : m_height(t_height) {
  // break down path coordinates into vectors (dft)
  // one 'fourier system' for each axis
  if (x_path.size() != y_path.size()) {
    std::cout << "Paths must be exactly the same size" << std::endl;
    return;
  }

  m_size = x_path.size();
  m_xCircles.resize(m_size);
  m_yCircles.resize(m_size);

  for (int i = 0; i < m_size; i++) {
    double xRe{}, xIm{}; // real, imaginary | complex num | represents coordinate point
    double yRe{}, yIm{};
    double tau {2 * M_PI}; // circumfrence to radius ratio

    // discrete integral over x values to get coordinate points on complex plane
    for (int j = 0; j < m_size; j++) {
      const double phi {tau * i * j / m_size}; // phase
      xRe += x_path.at(j) * cos(phi); // real x value      | terminal x val of vector
      xIm -= x_path.at(j) * sin(phi); // imaginary y value | terminal y val of vector
      yRe += y_path.at(j) * cos(phi);
      yIm -= y_path.at(j) * sin(phi);
    }
    xRe /= m_size; // average out all values
    xIm /= m_size;
    yRe /= m_size;
    yIm /= m_size;
    m_xCircles.at(i).frequency = i; // num of rotations per unit time
    m_xCircles.at(i).amplitude = sqrt(xRe * xRe + xIm * xIm); // a^2 + b^2 = c^2 | c^2 = length/amplitude of circle
    // ^ this is also the radius of the circle formed around the circle
    m_xCircles.at(i).phase = atan2(xIm, xRe); // starting rotation for circle

    m_yCircles.at(i).frequency = i;
    m_yCircles.at(i).amplitude = sqrt(yRe * yRe + yIm * yIm);
    m_yCircles.at(i).phase = atan2(yIm, yRe);
    m_yCircles.at(i).phase -= M_PI / 2; // turn yVectors 90 deg
  }
}

void FourierSeries::draw(SDL_Renderer* renderer) {
  // vectors that draw x values
  float xAxis[2] {0, 0};
  float yAxis[2] {0, 0};
  yAxis[1] = m_height / 2.0;
  for (int i{}; i < m_size; i++) {
    float prevX[2] {xAxis[0], xAxis[1]}; // coordinate pair for xVector[i]
    float prevY[2] {yAxis[0], yAxis[1]}; // coordinate pair for yVector[i]
    xAxis[0] += cos(m_xCircles.at(i).frequency * time + m_xCircles.at(i).phase) * m_xCircles.at(i).amplitude; // x = cos(frequency over time plus the initial angle) * strength of vector
    xAxis[1] += sin(m_xCircles.at(i).frequency * time + m_xCircles.at(i).phase) * m_xCircles.at(i).amplitude; // same but y, so sin()
    yAxis[0] += cos(m_yCircles.at(i).frequency * time + m_yCircles.at(i).phase) * m_yCircles.at(i).amplitude;
    yAxis[1] += sin(m_yCircles.at(i).frequency * time + m_yCircles.at(i).phase) * m_yCircles.at(i).amplitude;
    SDL_SetRenderDrawColor(renderer, lineColor[0], lineColor[1], lineColor[2], lineColor[3]); // draw lines in red

    // draw a line from one vector to the next
    SDL_RenderDrawLine(renderer, prevX[0], prevX[1], xAxis[0], xAxis[1]);
    SDL_RenderDrawLine(renderer, prevY[0], prevY[1], yAxis[0], yAxis[1]);

    // draw circle to represent vector
    drawCircle(renderer, prevX[0], prevX[1], m_xCircles.at(i).amplitude);
    drawCircle(renderer, prevY[0], prevY[1], m_yCircles.at(i).amplitude);
  }

  // give access to vector result
  m_lastX = xAxis[0];
  m_lastY = yAxis[1];

  // draw a line from the system to the result
  int xVectorLastY = xAxis[1];
  int yVectorLastX = yAxis[0];

  SDL_SetRenderDrawColor(renderer, lineColor[0], lineColor[1], lineColor[2], lineColor[3]);
  SDL_RenderDrawLine(renderer, m_lastX, xVectorLastY, m_lastX, m_lastY);
  SDL_RenderDrawLine(renderer, yVectorLastX, m_lastY, m_lastX, m_lastY);
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
  return m_lastY;
}

// get y-axis tail (x-coordinate of path)
float FourierSeries::getX() {
  return m_lastX;
} 

// returns 0 when the series repeats
float FourierSeries::getTime() {
  return time;
}

// move vectors to next position
void FourierSeries::update() {
  (time <= 2 * M_PI) ? (time += 2 * M_PI / m_size) : (time = 0);
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
