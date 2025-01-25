#include "Machine.hpp"
#include <fstream>
#include <iostream>

Machine::Machine(const std::string &path, const int &t_width, const int &t_height)
 : m_width(t_width), m_height(t_height){
  // create series
  std::vector<double> x_path, y_path; // x & y coordinates

  // build arr of x vals and y vals seperately
  std::ifstream source ("source.txt"); // a list of points

  // store data found in source
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
      x_path.emplace_back(std::stod(x));
      y_path.emplace_back(std::stod(y));
      i++;
    }
  } else {
    std::cout << "could not open source.txt" << std::endl;
    return;
  }

  if (x_path.size() != y_path.size()) {
    std::cout << "Paths must be exactly the same size" << std::endl;
    return;
  }

  // allocate memory
  m_frames = x_path.size();
  m_series.resize(m_frames * 2); // two dimensional

  // break down path coordinates into vectors (dft)
  for (int i = 0; i < m_frames; i++) {
    const double tau {2 * M_PI}; // necessary constant
    Complex x_axis, y_axis; // the complex numbers to be calculated
    // discrete integral over x values to get coordinate points on complex plane
    // using lambdas for readability
    for (int j = 0; j < m_frames; j++) {
      const double phase {tau * i * j / m_frames};

      auto sum = [phase](const Complex &prev, const double &target) -> Complex {
        Complex result {prev.re + target * cos(phase), prev.im - target * sin(phase)};
        return result;
      };
      x_axis = sum(x_axis, x_path.at(j));
      y_axis = sum(y_axis, y_path.at(j));
    }

    // average a sum that is a complex number
    auto average = [](const Complex &sum, const int &length) -> Complex {
      Complex result {sum.re / length, sum.im / length};
      return result;
    };
    x_axis = average(x_axis, m_frames);
    y_axis = average(y_axis, m_frames);

    // turn period & complexnum into Frequency
    auto newFrequency = [](const double &period, const Complex &val) -> Frequency {
      const double amplitude {sqrt(val.re * val.re + val.im * val.im)}; // pythagorean theorem
      const double phase {atan2(val.im, val.re)}; // initial rotation angle
      const Frequency result {period, amplitude, phase};
      return result;
    };
    Frequency x = newFrequency(i, x_axis);
    Frequency y = newFrequency(i, y_axis);
    y.phase -= M_PI * 0.5f;
    m_series.emplace_back(x);
    m_series.emplace_back(y);
  }
}

void Machine::draw(SDL_Renderer* renderer, SDL_Texture* tex) {
  SDL_SetRenderTarget(renderer, tex);
  SDL_SetRenderDrawColor(renderer, m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a);
  SDL_RenderClear(renderer);
  // vectors that draw x values
  SDL_FPoint prev_x, prev_y;
  SDL_FPoint current {static_cast<float>(m_width) * 0.2f, static_cast<float>(m_height) * 0.5f};
  SDL_FPoint prev {current};
  for (int i{}; i < m_series.size(); i++) {
    SDL_FPoint next = m_series.at(i).getPoint(m_time);
    prev = current;
    current.x += next.x;
    current.y += next.y;

    // draw a line from one circle to the next
    SDL_SetRenderDrawColor(renderer, m_line_color.r, m_line_color.g, m_line_color.b, m_line_color.a);
    SDL_RenderDrawLineF(renderer, prev.x, prev.y, current.x, current.y);

    // draw frequency
    SDL_SetRenderDrawColor(renderer, m_polygon_color.r, m_polygon_color.g, m_polygon_color.b, m_polygon_color.a);
    drawPolygon(renderer, m_sides, prev, m_series.at(i).amplitude, m_series.at(i).phase);
  }

  // update last coordinate
  SDL_FPoint result_point = current;
  m_result.emplace_back(current);

  // draw result
  SDL_SetRenderDrawColor(renderer, m_line_color.r, m_line_color.g, m_line_color.b, m_line_color.a);
  SDL_RenderDrawLinesF(renderer, m_result.data(), m_result.size());
  SDL_SetRenderTarget(renderer, nullptr);
}

void Machine::drawPolygon(SDL_Renderer* renderer, const int &sides, const SDL_FPoint &pos, const float &radius, const float &phase) {
  // calculate coordinate of tangent points along the circumfrence
  std::vector<SDL_FPoint> points;
  for (double i{}; i < 2 * M_PI; i += 2 * M_PI / static_cast<double>(sides)) {
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

int Machine::getFrame() {
  return static_cast<int>(m_time / (2 * M_PI) * m_frames);
}

int Machine::getFrames() {
  return m_frames;
}

void Machine::setFrame(const int &frame) {
  m_time = (2 * M_PI / m_frames) * frame;
}

void Machine::setSides(const int &sides) {
  m_sides = sides;
}

void Machine::clearResult() {
  m_result.clear();
}

// move vectors to next position
void Machine::update() {
  (m_time <= 2 * M_PI) ? (m_time += 2 * M_PI / m_frames) : (m_time = 0);
}

void Machine::setBackgroundColor(const SDL_Color &color) {
  m_background_color = color;
}

void Machine::setPolygonColor(const SDL_Color &color) {
  m_polygon_color = color;
}

void Machine::setLineColor(const SDL_Color &color) { 
  m_line_color = color;
}
