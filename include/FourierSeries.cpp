#include "FourierSeries.hpp"

FourierSeries::FourierSeries(const std::string &path, const int &t_width, const int &t_height)
 : m_width(t_width), m_height(t_height){
  // create series
  std::vector<float> x_path, y_path; // x & y coordinates

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

void FourierSeries::draw(SDL_Renderer* renderer, SDL_Texture* tex) {
  SDL_SetRenderTarget(renderer, tex);
  SDL_SetRenderDrawColor(renderer, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
  SDL_RenderClear(renderer);
  // vectors that draw x values
  SDL_FPoint x_point {static_cast<float>(m_width) / 2.0f, 50}, y_point {50, static_cast<float>(m_height) * 0.66f};
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
    drawCircle(renderer, prev_x, m_xCircles.at(i).amplitude, m_xCircles.at(i).phase);
    drawCircle(renderer, prev_y, m_yCircles.at(i).amplitude, m_yCircles.at(i).phase);
  }

  // update last coordinate
  SDL_FPoint result_point {x_point.x, y_point.y};
  m_result.emplace_back(result_point);

  // draw a line from the system to the result
  SDL_SetRenderDrawColor(renderer, m_lineColor.r, m_lineColor.g, m_lineColor.b, m_lineColor.a);
  SDL_RenderDrawLine(renderer, result_point.x, x_point.y, result_point.x, result_point.y);
  SDL_RenderDrawLine(renderer, y_point.x, result_point.y, result_point.x, result_point.y);

  // draw result
  SDL_RenderDrawLinesF(renderer, m_result.data(), m_result.size());
  SDL_SetRenderTarget(renderer, nullptr);
}

void FourierSeries::drawCircle(SDL_Renderer* renderer, const SDL_FPoint &pos, const float &radius, const float &phase) {
  // calculate coordinate of tangent points along the circumfrence
  std::vector<SDL_FPoint> points;
  for (double i{}; i < 2 * M_PI; i += 2 * M_PI / 3.0f) {
    SDL_FPoint point = {
      static_cast<float>(cos(i + phase)) * radius + pos.x,
      static_cast<float>(sin(i + phase)) * radius + pos.y
    };
    points.emplace_back(point);
  }
  // duplicate first point to the end of the list to complete shape
  points.emplace_back(points.at(0));
  SDL_RenderDrawLinesF(renderer, points.data(), points.size());
}

// returns 0 when the series repeats
float FourierSeries::getTime() {
  return m_time;
}

void FourierSeries::clearResult() {
  m_result.clear();
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
