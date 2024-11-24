#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <vector>

using my_clock = std::chrono::high_resolution_clock;

std::random_device rand_dev;
std::mt19937 generator(rand_dev());
std::uniform_int_distribution<> distr(0, 6000);

class ArrayGenerator {
public:
  std::vector<int> random(size_t size) {
    std::vector<int> res(size);
    while (size > 0) {
      res[size--] = distr(generator);
    }
    return res;
  }
  std::vector<int> reversed(size_t size) {
    std::vector<int> res = random(size);
    std::sort(res.begin(), res.end());
    std::reverse(res.begin(), res.end());
    return res;
  }
  std::vector<int> almost_sorted(size_t size) {
    int swap_count = std::uniform_int_distribution<>(
        0, int(std::sqrt(size * 1.0)))(generator);
    std::vector<int> a = random(size);
    std::sort(a.begin(), a.end());
    std::uniform_int_distribution<> select_index(0, size);
    while (swap_count > 0) {
      int i = select_index(generator), j = select_index(generator);
      std::swap(a[i], a[j]);
      swap_count--;
    }
    return a;
  }
};

void insertion_sort(std::vector<int> &arr, int l, int r) {
  for (int i = l + 1; i < r; ++i) {
    int el = arr[i];
    int j = i - 1;
    while (j >= l && arr[j] > el) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = el;
  }
}

void merge(std::vector<int> &arr, int l, int mid, int r) {
  int ll = l, rr = mid;
  std::vector<int> tmp;
  while (ll < mid || rr < r) {
    if (ll >= mid || rr < r && arr[rr] < arr[ll]) {
      tmp.push_back(arr[rr++]);
    } else {
      tmp.push_back(arr[ll++]);
    }
  }
  for (int i = 0; i < tmp.size(); ++i) {
    arr[l + i] = tmp[i];
  }
}

void ultra_sort(std::vector<int> &arr, int l, int r, int threshold = 3) {
  if (r - l <= threshold) {
    insertion_sort(arr, l, r);
    return;
  }
  int mid = (l + r) / 2;
  ultra_sort(arr, l, mid);
  ultra_sort(arr, mid, r);
  merge(arr, l, mid, r);
}

class SortTester {
public:
  std::string mode;
  int threshold = 20;
  void run(std::ostream &out = std::cout, int attempts = 10) {
    for (int attempt = 0; attempt < attempts; ++attempt) {
      std::vector<int> arr;
      arr = arg.random(10000);
      run_with_tag(arr, mode + " random", out);
      arr = arg.reversed(10000);
      run_with_tag(arr, mode + " reversed", out);
      arr = arg.almost_sorted(10000);
      run_with_tag(arr, mode + " almost_sorted", out);
    }
  }

private:
  ArrayGenerator arg;
  void run_with_tag(std::vector<int> &arr, std::string tag, std::ostream &out) {
    for (int n = 500; n < 100000; n += 100) {
      std::vector<int> cp(arr.begin(), arr.begin() + n);

      auto start = std::chrono::high_resolution_clock::now();
      ultra_sort(cp, 0, n, threshold);
      auto elapsed = std::chrono::high_resolution_clock::now() - start;
      long long nsec =
          std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();

      out << n << " " << tag << " " << nsec / 1000 << "\n";
    }
  }
};

int main() {
  auto out = std::ofstream("res2.csv");
  SortTester tester;
  tester.mode = "combined";
  tester.threshold = 20;
  tester.run(out);
  tester.threshold = 1;
  tester.mode = "default";
  tester.run(out);
  out.close();
  return 1;
}
