/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти, за каждую утечку памяти - штраф `-1`.

## Задача № 4 (4 балла)

**Требование для всех вариантов Задачи 4**:

* Решение всех задач данного раздела предполагает использование кучи, реализованной в виде **шаблонного класса**.
* **Решение должно поддерживать передачу функции сравнения снаружи.**
* *Куча должна быть динамической.*

### № 4.2 Топ `K` пользователей из лога

Имеется лог-файл, в котором хранятся пары для `N` пользователей *(Идентификатор пользователя, посещаемость сайта)*.

Напишите программу, которая выбирает `K` пользователей, которые чаще других заходили на сайт, и выводит их в порядке возрастания посещаемости. Количество
заходов и идентификаторы пользователей не повторяются.

**Требования:** время работы $O(N \log K)$, где `N` - кол-во пользователей. **Ограничение на размер кучи $O(K)$**.

* *Формат входных данных.* Сначала вводятся `N` и `K`, затем пары *(Идентификатор пользователя, посещаемость сайта)*.

* *Формат выходных данных.* Идентификаторы пользователей в порядке возрастания посещаемости

| in                                   | out                  |
| :----------------------------------- | :------------------- |
| `3 3`<br>`100 36`<br>`80 3`<br>`1 5` | `80`<br>`1`<br>`100` |
*/

#include <iostream>

struct User {
  int id;
  int visits;
};

struct UserComparator {
  bool operator()(const User& lhs, const User& rhs) const { return lhs.visits < rhs.visits; }
};

template <class T, class Compare>
class Heap {
 public:
  explicit Heap(int initial_capacity = 10, Compare cmp = Compare()) : capacity(initial_capacity), size(0), comp(cmp) { buffer = new T[capacity]; }

  ~Heap() { delete[] buffer; }
  Heap(const Heap&) = delete;
  Heap& operator=(const Heap&) = delete;

  void Insert(const T& element) {
    if (size == capacity) {
      Grow();
    }
    buffer[size++] = element;
    SiftUp(size - 1);
  }

  T ExtractMin() {
    T min = buffer[0];
    buffer[0] = buffer[--size];
    SiftDown(0);
    return min;
  }

  const T& PeekMin() const { return buffer[0]; }
  int Size() const { return size; }
  bool IsEmpty() const { return size == 0; }

 private:
  T* buffer;
  int capacity;
  int size;
  Compare comp;

  void SiftUp(int index) {
    while (index > 0 && comp(buffer[index], buffer[(index - 1) / 2])) {
      std::swap(buffer[index], buffer[(index - 1) / 2]);
      index = (index - 1) / 2;
    }
  }

  void SiftDown(int index) {
    while (2 * index + 1 < size) {
      int min_child = 2 * index + 1;
      if (2 * index + 2 < size && comp(buffer[2 * index + 2], buffer[2 * index + 1])) {
        min_child = 2 * index + 2;
      }
      if (!comp(buffer[min_child], buffer[index])) {
        break;
      }
      std::swap(buffer[index], buffer[min_child]);
      index = min_child;
    }
  }

  void Grow() {
    capacity *= 2;
    T* new_buffer = new T[capacity];
    for (int i = 0; i < size; ++i) {
      new_buffer[i] = buffer[i];
    }
    delete[] buffer;
    buffer = new_buffer;
  }
};

int main() {
  int n = 0;
  int k = 0;
  if (!(std::cin >> n >> k)) {
    return 0;
  }

  if (k > n) {
    k = n;
  }
  if (k == 0) {
    return 0;
  }

  Heap<User, UserComparator> min_heap(k, UserComparator());

  for (int i = 0; i < k; ++i) {
    User u;
    u.id = 0;
    u.visits = 0;
    std::cin >> u.id >> u.visits;
    min_heap.Insert(u);
  }

  for (int i = k; i < n; ++i) {
    User u;
    u.id = 0;
    u.visits = 0;
    std::cin >> u.id >> u.visits;

    if (u.visits > min_heap.PeekMin().visits) {
      min_heap.ExtractMin();
      min_heap.Insert(u);
    }
  }

  int result_size = min_heap.Size();
  for (int i = 0; i < result_size; ++i) {
    User u = min_heap.ExtractMin();
    std::cout << u.id << " ";
  }
  std::cout << std::endl;

  return 0;
}
