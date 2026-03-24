/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти, за каждую утечку памяти - штраф `-1`.

## Задача № 2 (4 балла)

### № 2.4

Дан отсортированный массив различных целых чисел `A[0..n-1]` и массив целых чисел `B[0..m-1]`. Для каждого элемента массива `B[i]` найдите минимальный индекс
элемента массива `A[k]`, ближайшего по значению к `B[i]`.

**Требования:** Время работы поиска для каждого элемента `B[i]`: $O(\log \mathbf{k})$. Внимание! В этой задаче для каждого `B[i]` сначала нужно определить
диапазон для бинарного поиска размером порядка `k` с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.

`n ≤ 110000, m ≤ 1000`.

| in                                      | out       |
| :-------------------------------------- | :-------- |
| `3`<br>`10 20 30`<br>`3`<br>`9 15 35`   | `0 0 2`   |
| `3`<br>`10 20 30`<br>`4`<br>`8 9 10 32` | `0 0 0 2` |
*/

#include <iostream>

// Время: O(m * log n), Память: O(m)
void FindClosestElements(unsigned int* A, unsigned int n, unsigned int* B, unsigned int m, unsigned int* result) {
  const unsigned int EXPO_MULTIPLIER = 2;

  for (unsigned int i = 0; i < m; ++i) {
    unsigned int left = 0;
    unsigned int right = 1;

    while (right < n && A[right] < B[i]) {
      left = right;
      right *= EXPO_MULTIPLIER;
    }

    if (right >= n) {
      right = n - 1;
    }

    while (left < right) {
      unsigned int middle = left + (right - left) / 2;

      if (A[middle] < B[i]) {
        left = middle + 1;
      } else {
        right = middle;
      }
    }

    if (left > 0 && std::abs(static_cast<int>(A[left]) - static_cast<int>(B[i])) >= std::abs(static_cast<int>(A[left - 1]) - static_cast<int>(B[i]))) {
      result[i] = left - 1;
    } else {
      result[i] = left;
    }
  }
}

int main() {
  unsigned int n{0}, m{0};
  std::cin >> n;

  unsigned int* A = new unsigned int[n];
  for (unsigned int i = 0; i < n; ++i) {
    std::cin >> A[i];
  }

  std::cin >> m;

  unsigned int* B = new unsigned int[m];
  for (unsigned int i = 0; i < m; ++i) {
    std::cin >> B[i];
  }

  unsigned int* result = new unsigned int[m];

  FindClosestElements(A, n, B, m, result);
  for (unsigned int i = 0; i < m; ++i) {
    std::cout << result[i] << " ";
  }
  std::cout << std::endl;

  delete[] A;
  delete[] B;
  delete[] result;

  return 0;
}
