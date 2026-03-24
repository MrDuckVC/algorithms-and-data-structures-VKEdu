/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти, за каждую утечку памяти - штраф `-1`.

## Задача № 7 (3 балла)

### № 7.2 `LSD` для `long long`

Дан массив неотрицательных целых `64`-битных чисел. Количество чисел не больше $10^6$. Отсортировать массив методом поразрядной сортировки `LSD` по байтам.

| in                   | out           |
| :------------------- | :------------ |
| `3`<br>`4 1000000 7` | `4 7 1000000` |
*/

#include <iostream>

void CountingSortByByte(unsigned long long* arr, int n, int byte_index, unsigned long long* buffer) {
  const int kAlphabetSize = 256;
  int count[kAlphabetSize] = {0};

  for (int i = 0; i < n; ++i) {
    int current_byte = (arr[i] >> (byte_index * 8)) & 255;
    count[current_byte]++;
  }

  for (int i = 1; i < kAlphabetSize; ++i) {
    count[i] += count[i - 1];
  }

  for (int i = n - 1; i >= 0; --i) {
    int current_byte = (arr[i] >> (byte_index * 8)) & 255;
    buffer[--count[current_byte]] = arr[i];
  }

  for (int i = 0; i < n; ++i) {
    arr[i] = buffer[i];
  }
}

void LSDSort(unsigned long long* arr, int n) {
  unsigned long long* buffer = new unsigned long long[n];

  for (int byte_index = 0; byte_index < 8; ++byte_index) {
    CountingSortByByte(arr, n, byte_index, buffer);
  }

  delete[] buffer;
}

int main() {
  int n = 0;
  if (!(std::cin >> n)) {
    return 0;
  }

  unsigned long long* arr = new unsigned long long[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }

  LSDSort(arr, n);

  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  delete[] arr;

  return 0;
}
