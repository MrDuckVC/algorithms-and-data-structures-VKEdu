/*
Задано $N$ точек на плоскости. Указать $(N-1)$-звенную несамопересекающуюся замкнутую ломаную, проходящую через все эти точки.
Предполагается, что никакие три точки не лежат на одной прямой.
Стройте ломаную от точки, имеющей наименьшую координату x. Если таких точек несколько, то используйте точку с наименьшей координатой y. Точки на ломаной
расположите в порядке убывания углов лучей от начальной точки до всех остальных точек.
**Для сортировки точек реализуйте алгоритм сортировки вставками.**
*/

#include <iostream>

struct Point {
  long long x;
  long long y;
};

struct PolarAngleComparator {
  Point p0;

  explicit PolarAngleComparator(Point p0) : p0(p0) {}

  bool operator()(const Point& a, const Point& b) const {
    long long dx1 = a.x - p0.x;
    long long dy1 = a.y - p0.y;
    long long dx2 = b.x - p0.x;
    long long dy2 = b.y - p0.y;

    return (dx1 * dy2 - dy1 * dx2) < 0;
  }
};

template <typename T, class Compare>
void InsertionSort(T* arr, int n, Compare cmp) {
  for (int i = 1; i < n; ++i) {
    T tmp = arr[i];
    int j = i - 1;

    while (j >= 0 && cmp(tmp, arr[j])) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = tmp;
  }
}

int main() {
  int n = 0;
  if (!(std::cin >> n)) {
    return 0;
  }

  Point* points = new Point[n];

  std::cin >> points[0].x >> points[0].y;
  int p0_index = 0;

  for (int i = 1; i < n; ++i) {
    std::cin >> points[i].x >> points[i].y;

    if (points[i].x < points[p0_index].x || (points[i].x == points[p0_index].x && points[i].y < points[p0_index].y)) {
      p0_index = i;
    }
  }

  Point temp = points[0];
  points[0] = points[p0_index];
  points[p0_index] = temp;

  PolarAngleComparator cmp(points[0]);

  InsertionSort(points + 1, n - 1, cmp);

  for (int i = 0; i < n; ++i) {
    std::cout << points[i].x << " " << points[i].y << std::endl;
  }

  delete[] points;

  return 0;
}
