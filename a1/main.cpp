#include <algorithm>
#include <iostream>
#include <random>

using std::min;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Point {
  double x, y;

  double distance(const Point &other) {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
  }
};

struct Circle {
  Point center;
  double r;

  bool contains(const Point &point) { return center.distance(point) <= r; }
};

struct Rectangle {
  Point p1; // left-bottom
  Point p2; // right-upper

  double square() { return (p2.x - p1.x) * (p2.y - p1.y); }
};

double accurate() { return 0.25 * M_PI + 1.25 * std::asin(0.8) - 1; }

double approximate(Circle c1, Circle c2, Circle c3, Rectangle bounds, int n) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_real_distribution<> x_gen(bounds.p1.x, bounds.p2.x);
  std::uniform_real_distribution<> y_gen(bounds.p1.y, bounds.p2.y);

  int m = 0;

  for (int i = 0; i < n; ++i) {
    double xx = x_gen(generator), yy = y_gen(generator);
    Point p = {xx, yy};
    bool contains = c1.contains(p) && c2.contains(p) && c3.contains(p);
    if (contains) {
      m++;
    }
  }
  return (m * 1.0 / n) * bounds.square();
}

double approximate_default(Rectangle bounds, int n) {
  Circle c1 = {{1, 1}, 1}, c2 = {{1.5, 2}, std::sqrt(5) / 2},
         c3 = {{2, 1.5}, std::sqrt(5) / 2};
  return approximate(c1, c2, c3, bounds, n);
}

Rectangle fit_circles(Circle c1, Circle c2, Circle c3) {
  double l =
      min(c1.center.x - c1.r, min(c2.center.x - c2.r, c3.center.x - c3.r));
  double r =
      min(c1.center.x + c1.r, min(c2.center.x + c2.r, c3.center.x + c3.r));
  double b =
      min(c1.center.y - c1.r, min(c2.center.y - c2.r, c3.center.y - c3.r));
  double u =
      min(c1.center.y + c1.r, min(c2.center.y + c2.r, c3.center.y + c3.r));
  return {{l, b}, {r, u}};
}

int main() {
  Rectangle mega = {{0, 0}, {2 + sqrt(5) / 2, 2 + sqrt(5) / 2}};
  Rectangle mini = {{0.8, 0.8}, {2.1, 2.1}};
  for (int n = 100; n < 100000; n += 500) {
    double s_mega = approximate_default(mega, n);
    double s_mini = approximate_default(mini, n);
    std::cout << n << " " << s_mega << " " << s_mini << "\n";
  }
}
