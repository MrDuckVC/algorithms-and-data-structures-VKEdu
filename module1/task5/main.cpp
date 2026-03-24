/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти, за каждую утечку памяти - штраф `-1`.

## Задача № 5 (4 балла)

**Требование для всех вариантов Задачи 5:**

* Во всех задачах данного раздела необходимо реализовать и использовать **сортировку слиянием в виде шаблонной функции**.
* **Решение должно поддерживать передачу функции сравнения снаружи.**
* Общее время работы алгоритма $O(n \log n)$.

### № 5.2 Современники

Группа людей называется современниками если был такой момент, когда они могли собраться вместе. Для этого в этот момент каждому из них должно было уже
исполниться `18` лет, но ещё не исполниться `80` лет.

Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников. В день `18`-летия человек уже может принимать участие в собраниях, а
в день `80`-летия и в день смерти уже не может.

Замечание. Человек мог не дожить до `18`-летия, либо умереть в день `18`-летия. В этих случаях принимать участие в собраниях он не мог.

| in                                                                         | out |
| :------------------------------------------------------------------------- | :-- |
| `3`<br>`2 5 1980 13 11 2055`<br>`1 1 1982 1 1 2030`<br>`2 1 1920 2 1 2000` | `3` |
*/

#include <algorithm>
#include <iostream>

// Структура хранит валидный отрезок пребывания на собраниях
struct LifeTime {
  int arrival;
  int departure;
};

struct IntComparator {
  bool operator()(int lhs, int rhs) const { return lhs < rhs; }
};

template <typename T, class Compare>
void Merge(T* arr, int left, int mid, int right, T* buffer, Compare cmp) {
  int it1 = left;
  int it2 = mid;
  int k = left;

  while (it1 < mid && it2 < right) {
    if (cmp(arr[it1], arr[it2])) {
      buffer[k++] = arr[it1++];
    } else {
      buffer[k++] = arr[it2++];
    }
  }

  while (it1 < mid) {
    buffer[k++] = arr[it1++];
  }

  while (it2 < right) {
    buffer[k++] = arr[it2++];
  }

  for (int i = left; i < right; ++i) {
    arr[i] = buffer[i];
  }
}

template <typename T, class Compare>
void MergeSort(T* arr, int size, Compare cmp = Compare()) {
  if (size <= 1) {
    return;
  }

  T* buffer = new T[size];

  for (int step = 1; step < size; step *= 2) {
    for (int i = 0; i < size - step; i += 2 * step) {
      int left = i;
      int mid = i + step;
      int right = std::min(i + 2 * step, size);

      Merge(arr, left, mid, right, buffer, cmp);
    }
  }

  delete[] buffer;
}

// Конвертация даты в число YYYYMMDD
int ConvertDate(int day, int month, int year) { return year * 10000 + month * 100 + day; }

int GetMaxContemporaries(LifeTime* lifetimes, int count) {
  if (count == 0) {
    return 0;
  }

  int* arrivals = new int[count];
  int* departures = new int[count];

  for (int i = 0; i < count; ++i) {
    arrivals[i] = lifetimes[i].arrival;
    departures[i] = lifetimes[i].departure;
  }

  MergeSort(arrivals, count, IntComparator());
  MergeSort(departures, count, IntComparator());

  int max_contemporaries = 0;
  int current_contemporaries = 0;

  int i = 0;
  int j = 0;

  while (i < count && j < count) {
    if (arrivals[i] < departures[j]) {
      current_contemporaries++;
      max_contemporaries = std::max(max_contemporaries, current_contemporaries);
      i++;
    } else {
      current_contemporaries--;
      j++;
    }
  }

  delete[] arrivals;
  delete[] departures;

  return max_contemporaries;
}

int main() {
  int n = 0;
  if (!(std::cin >> n)) {
    return 0;
  }

  LifeTime* lifetimes = new LifeTime[n];
  int valid_count = 0;

  for (int i = 0; i < n; ++i) {
    int b_d = 0, b_m = 0, b_y = 0;
    int d_d = 0, d_m = 0, d_y = 0;
    std::cin >> b_d >> b_m >> b_y >> d_d >> d_m >> d_y;

    int birth = ConvertDate(b_d, b_m, b_y);
    int death = ConvertDate(d_d, d_m, d_y);

    int arrival = birth + 180000;
    int departure = std::min(death, birth + 800000);

    if (arrival < departure) {
      lifetimes[valid_count].arrival = arrival;
      lifetimes[valid_count].departure = departure;
      valid_count++;
    }
  }

  int result = GetMaxContemporaries(lifetimes, valid_count);
  std::cout << result << std::endl;

  delete[] lifetimes;

  return 0;
}
