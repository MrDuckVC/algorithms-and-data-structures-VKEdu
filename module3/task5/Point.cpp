#include "Point.h"

#include <cmath>
#include <cstdlib>
#include <vector>

double GetDistance(const Point& a, const Point& b) { return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }

Point GenerateNormalPoint() {
  double u1 = (std::rand() + 1.0) / (RAND_MAX + 2.0);
  double u2 = (std::rand() + 1.0) / (RAND_MAX + 2.0);

  double r = std::sqrt(-2.0 * std::log(u1));
  double theta = 2.0 * M_PI * u2;

  return {r * std::cos(theta), r * std::sin(theta)};
}

std::vector<Point> GeneratePoints(int n) {
  std::vector<Point> points(n);
  for (int i = 0; i < n; ++i) {
    points[i] = GenerateNormalPoint();
  }
  return points;
}
