/*
Дан массив, который получен путем нескольких циклических сдвигов исходного отсортированного массива. Нужно найти индекс элемента, с которого начинался исходный
массив. Массив состоит из уникальных элементов.

Примеры:

[12, 14, 16, 17, 1, 3, 5, 7] (исходный массив [1, 3, 5, 7, 12, 14, 16, 17])

Ответ: 4

[7, 9, 11, 1, 3, 5] (исходный массив [1, 3, 5, 7, 9, 11])

Ответ: 3
*/

#include <iostream>

int FindShiftIndex(const int* arr, int n) {
  if (n <= 0) {
    return -1;
  }

  int left = 0;
  int right = n - 1;

  if (arr[left] <= arr[right]) {
    return 0;
  }

  while (left < right) {
    int mid = left + (right - left) / 2;

    if (arr[mid] > arr[right]) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }

  return left;
}

int main() {
  int n = 0;
  if (!(std::cin >> n)) {
    return 0;
  }

  int* arr = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }

  int result = FindShiftIndex(arr, n);

  std::cout << result << std::endl;

  delete[] arr;

  return 0;
}
