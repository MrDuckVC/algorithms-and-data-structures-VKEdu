#pragma once
#include <vector>

struct Point {
  double x, y;
};

double GetDistance(const Point& a, const Point& b);
Point GenerateNormalPoint();
std::vector<Point> GeneratePoints(int n);
