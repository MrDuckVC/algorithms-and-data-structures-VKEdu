/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти, за каждую утечку памяти - штраф `-1`.

## Задача № 6 (3 балла)

**Обязательная задача.**

Дано множество целых чисел из $[0..10^9]$ размера `n`.

Используя алгоритм поиска `k`-ой порядковой статистики, требуется найти следующие параметры множества:

1. $10\%$ перцентиль
2. медиана
3. $90\%$ перцентиль

**Требования:**

* К дополнительной памяти: $O(n)$.
* Среднее время работы: $O(n)$.
* Должна быть отдельно выделенная функция `partition`.
* Рекурсия запрещена.
* Решение должно поддерживать передачу функции сравнения снаружи.

Функцию `Partition` следует реализовывать методом прохода двумя итераторами в одном направлении. Описание для случая прохода от начала массива к концу:

* Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
* Во время работы `Partition` в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива
лежат нерассмотренные элементы. Последним элементом лежит опорный.
* Итератор (индекс) `i` указывает на начало группы элементов, строго бОльших опорного.
* Итератор `j` больше `i`, итератор `j` указывает на первый нерассмотренный элемент.
* Шаг алгоритма. Рассматривается элемент, на который указывает `j`. Если он больше опорного, то сдвигаем `j`.

  Если он не больше опорного, то меняем `a[i]` и `a[j]` местами, сдвигаем `i` и сдвигаем `j`.
* В конце работы алгоритма меняем опорный и элемент, на который указывает итератор `i`.

### № 6.2

Реализуйте стратегию выбора опорного элемента "медиана трёх". Функцию `Partition` реализуйте методом прохода двумя итераторами от конца массива к началу.
*/

#include <algorithm>
#include <iostream>
#include <utility>

struct IntComparator {
  bool operator()(int lhs, int rhs) const { return lhs < rhs; }
};

template <typename T, class Compare>
int Partition(T* arr, int left, int right, Compare cmp) {
  if (left >= right) return left;

  int mid = left + (right - left) / 2;

  if (cmp(arr[mid], arr[left])) std::swap(arr[left], arr[mid]);
  if (cmp(arr[right], arr[left])) std::swap(arr[left], arr[right]);
  if (cmp(arr[right], arr[mid])) std::swap(arr[mid], arr[right]);

  std::swap(arr[left], arr[mid]);
  T pivot = arr[left];

  int i = right;
  int j = right;

  while (j > left) {
    if (cmp(pivot, arr[j])) {
      std::swap(arr[i], arr[j]);
      i--;
    }
    j--;
  }

  std::swap(arr[left], arr[i]);
  return i;
}

template <typename T, class Compare>
T KthStatistic(T* arr, int left, int right, int k, Compare cmp) {
  while (left <= right) {
    int pivot_pos = Partition(arr, left, right, cmp);

    if (pivot_pos == k) {
      return arr[k];
    } else if (pivot_pos > k) {
      right = pivot_pos - 1;
    } else {
      left = pivot_pos + 1;
    }
  }
  return arr[k];
}

int main() {
  int n = 0;
  if (!(std::cin >> n)) return 0;

  int* arr = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }

  IntComparator cmp;

  int perc10_idx = n / 10;
  int median_idx = n / 2;
  int perc90_idx = 9 * n / 10;

  int median = KthStatistic(arr, 0, n - 1, median_idx, cmp);

  int perc10 = KthStatistic(arr, 0, median_idx - 1, perc10_idx, cmp);

  int perc90 = KthStatistic(arr, median_idx + 1, n - 1, perc90_idx, cmp);

  std::cout << perc10 << std::endl << median << std::endl << perc90 << std::endl;

  delete[] arr;

  return 0;
}
