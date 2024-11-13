#include "FourierSeries.hpp"
FourierSeries::FourierSeries(const std::vector<float> &x_path, const std::vector<float> &y_path, const int &t_height)
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
  float yAxis[2] {0, static_cast<float>(m_height)};
  for (int i{}; i < m_size; i++) {
    float prevX[2] {xAxis[0], xAxis[1]}; // coordinate pair for xVector[i]
    float prevY[2] {yAxis[0], yAxis[1]}; // coordinate pair for yVector[i]
    xAxis[0] += cos(m_xCircles.at(i).frequency * m_time + m_xCircles.at(i).phase) * m_xCircles.at(i).amplitude; // x = cos(frequency over time plus the initial angle) * strength of vector
    xAxis[1] += sin(m_xCircles.at(i).frequency * m_time + m_xCircles.at(i).phase) * m_xCircles.at(i).amplitude; // same but y, so sin()
    yAxis[0] += cos(m_yCircles.at(i).frequency * m_time + m_yCircles.at(i).phase) * m_yCircles.at(i).amplitude;
    yAxis[1] += sin(m_yCircles.at(i).frequency * m_time + m_yCircles.at(i).phase) * m_yCircles.at(i).amplitude;
    SDL_SetRenderDrawColor(renderer, m_lineColor.r, m_lineColor.g, m_lineColor.b, m_lineColor.a); // draw lines in red

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

  SDL_SetRenderDrawColor(renderer, m_lineColor.r, m_lineColor.g, m_lineColor.b, m_lineColor.a);
  SDL_RenderDrawLine(renderer, m_lastX, xVectorLastY, m_lastX, m_lastY);
  SDL_RenderDrawLine(renderer, yVectorLastX, m_lastY, m_lastX, m_lastY);
}

void FourierSeries::drawCircle(SDL_Renderer* renderer, const float &xPos, const float &yPos, const float &radius) {
  SDL_SetRenderDrawColor(renderer, m_circleColor.r, m_circleColor.g, m_circleColor.b, m_circleColor.a);
  float x{}, y{};
  // calculate coordinate of tangent points along the circumfrence
  for (double i{}; i < 2 * M_PI; i += M_PI / (radius)) {
    x = cos(i) * radius + xPos;
    y = sin(i) * radius + yPos;
    SDL_RenderDrawPointF(renderer, x, y);
  }
}

// get tail of x & y axis
SDL_FPoint FourierSeries::getPoint() {
  return SDL_FPoint {m_lastX, m_lastY};
}

// returns 0 when the series repeats
float FourierSeries::getTime() {
  return m_time;
}

// move vectors to next position
void FourierSeries::update() {
  (m_time <= 2 * M_PI) ? (m_time += 2 * M_PI / m_size) : (m_time = 0);
}

void FourierSeries::setCircleColor(const SDL_Color &color) {
  m_circleColor = color;
}

void FourierSeries::setLineColor(const SDL_Color &color) { 
  m_lineColor = color;
}
