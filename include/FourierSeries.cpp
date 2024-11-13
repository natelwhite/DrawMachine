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
  SDL_FPoint x_point, y_point;
  y_point.y = m_height;
  SDL_FPoint prev_x, prev_y;
  for (int i{}; i < m_size; i++) {
    SDL_FPoint next_x = m_xCircles.at(i).getPoint(m_time);
    prev_x = x_point;
    x_point.x += next_x.x;
    x_point.y += next_x.y;

    SDL_FPoint next_y = m_yCircles.at(i).getPoint(m_time);
    prev_y = y_point;
    y_point.x += next_y.x;
    y_point.y += next_y.y;

    // draw a line from one circle to the next
    SDL_SetRenderDrawColor(renderer, m_lineColor.r, m_lineColor.g, m_lineColor.b, m_lineColor.a);
    SDL_RenderDrawLineF(renderer, prev_x.x, prev_x.y, x_point.x, x_point.y);
    SDL_RenderDrawLineF(renderer, prev_y.x, prev_y.y, y_point.x, y_point.y);


    // draw circle
    SDL_SetRenderDrawColor(renderer, m_circleColor.r, m_circleColor.g, m_circleColor.b, m_circleColor.a);
    drawCircle(renderer, prev_x, m_xCircles.at(i).amplitude);
    drawCircle(renderer, prev_y, m_yCircles.at(i).amplitude);
  }

  // update last coordinate
  m_result = {x_point.x, y_point.y};

  // draw a line from the system to the result
  SDL_SetRenderDrawColor(renderer, m_lineColor.r, m_lineColor.g, m_lineColor.b, m_lineColor.a);
  SDL_RenderDrawLine(renderer, m_result.x, x_point.y, m_result.x, m_result.y);
  SDL_RenderDrawLine(renderer, y_point.x, m_result.y, m_result.x, m_result.y);
}

void FourierSeries::drawCircle(SDL_Renderer* renderer, const SDL_FPoint &pos, const float &radius) {
  float x{}, y{};
  // calculate coordinate of tangent points along the circumfrence
  for (double i{}; i < 2 * M_PI; i += M_PI / (radius)) {
    x = cos(i) * radius + pos.x;
    y = sin(i) * radius + pos.y;
    SDL_RenderDrawPointF(renderer, x, y);
  }
}

// get tail of x & y axis
SDL_FPoint FourierSeries::getPoint() {
  return m_result;
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
