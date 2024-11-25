#include "FourierSeries.hpp"

FourierSeries::FourierSeries(const std::string &path, const int &t_width, const int &t_height)
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
  }

  // break down path coordinates into vectors (dft)
  // one 'fourier system' for each axis
  if (x_path.size() != y_path.size()) {
    std::cout << "Paths must be exactly the same size" << std::endl;
    return;
  }

  // allocate memory
  m_frames = x_path.size();
  m_xCircles.resize(m_frames);
  m_yCircles.resize(m_frames);

  // dft algorithm
  for (int i = 0; i < m_frames; i++) {
    const double tau {2 * M_PI}; // necessary constant
    Complex x_axis, y_axis; // the complex numbers to be calculated
    // discrete integral over x values to get coordinate points on complex plane
    // using lambdas for readability
    for (int j = 0; j < m_frames; j++) {
      const double phase {tau * i * j / m_frames};

      auto sum = [phase](const Complex prev, const double target) -> Complex {
        Complex result {prev.re + target * cos(phase), prev.im - target * sin(phase)};
        return result;
      };
      x_axis = sum(x_axis, x_path.at(j));
      y_axis = sum(y_axis, y_path.at(j));
    }

    // average a sum that is a complex number
    auto average = [](const Complex sum, const int length) -> Complex {
      Complex result {sum.re / length, sum.im / length};
      return result;
    };
    x_axis = average(x_axis, m_frames);
    y_axis = average(y_axis, m_frames);

    // turn period & complexnum into Frequency
    auto newFrequency = [](const double period, const Complex val) -> Frequency {
      const double amplitude {sqrt(val.re * val.re + val.im * val.im)}; // pythagorean theorem
      const double phase {atan2(val.im, val.re)}; // initial rotation angle
      const Frequency result {period, amplitude, phase};
      return result;
    };
    m_xCircles.at(i) = newFrequency(i, x_axis);
    m_yCircles.at(i) = newFrequency(i, y_axis);
    m_yCircles.at(i).phase -= M_PI / 2.0f; // turn yVectors 90 deg
  }
}

void FourierSeries::draw(SDL_Renderer* renderer, SDL_Texture* tex) {
  SDL_SetRenderTarget(renderer, tex);
  SDL_SetRenderDrawColor(renderer, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
  SDL_RenderClear(renderer);
  // vectors that draw x values
  SDL_FPoint x_point {static_cast<float>(m_width) * 0.2f, 50}, y_point {50, static_cast<float>(m_height) * 0.8f};
  SDL_FPoint prev_x, prev_y;
  for (int i{}; i < m_frames; i++) {
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
    drawPolygon(renderer, prev_x, m_xCircles.at(i).amplitude, m_xCircles.at(i).phase);
    drawPolygon(renderer, prev_y, m_yCircles.at(i).amplitude, m_yCircles.at(i).phase);
  }

  // update last coordinate
  SDL_FPoint result_point {x_point.x, y_point.y};
  m_result.emplace_back(result_point);

  // draw a line from the system to the result
  SDL_SetRenderDrawColor(renderer, m_lineColor.r, m_lineColor.g, m_lineColor.b, m_lineColor.a);
  SDL_RenderDrawLine(renderer, result_point.x, x_point.y, result_point.x, result_point.y);
  SDL_RenderDrawLine(renderer, y_point.x, result_point.y, result_point.x, result_point.y);

  // draw result
  SDL_RenderDrawPointsF(renderer, m_result.data(), m_result.size());
  SDL_SetRenderTarget(renderer, nullptr);
}

void FourierSeries::drawPolygon(SDL_Renderer* renderer, const SDL_FPoint &pos, const float &radius, const float &phase) {
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

int FourierSeries::getFrame() {
  return static_cast<int>(m_time / (2 * M_PI) * m_frames);
}

int FourierSeries::getFrames() {
  return m_frames;
}

void FourierSeries::setFrame(const int &frame) {
  m_time = (2 * M_PI / m_frames) * frame;
}

void FourierSeries::clearResult() {
  m_result.clear();
}

// move vectors to next position
void FourierSeries::update() {
  (m_time <= 2 * M_PI) ? (m_time += 2 * M_PI / m_frames) : (m_time = 0);
}

void FourierSeries::setCircleColor(const SDL_Color &color) {
  m_circleColor = color;
}

void FourierSeries::setLineColor(const SDL_Color &color) { 
  m_lineColor = color;
}
