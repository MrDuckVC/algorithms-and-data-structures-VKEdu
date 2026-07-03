/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти, за каждую утечку памяти - штраф `-1`.

## Задача № 3 (4 балла)

Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды `push*` и `pop*`.

* *Формат входных данных.*
  В первой строке количество команд `n`. `n ≤ 1000000`.

  Каждая команда задаётся как `2` целых числа: `a b`.

  ```cpp
  a = 1 // push front
  a = 2 // pop front
  a = 3 // push back
  a = 4 // pop back
  ```

  Команды добавления элемента `1` и `3` заданы с неотрицательным параметром `b`.

  Для очереди используются команды `2` и `3`. Для дека используются все четыре команды.

  Если дана команда `pop*`, то число `b` - ожидаемое значение. Если команда `pop` вызвана для пустой структуры данных, то ожидается "-1".

* *Формат выходных данных.*
  Требуется напечатать `YES` - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать `NO`.

### № 3.3

Реализовать очередь с помощью двух стеков.

**Требования:** Очередь должна быть реализована в виде класса. Стек тоже должен быть реализован в виде класса (на основе динамического массива).

| in                                | out   |
| :-------------------------------- | :---- |
| `3`<br>`3 44`<br>`3 50`<br>`2 44` | `YES` |
| `2`<br>`2 -1`<br>`3 10`           | `YES` |
| `2`<br>`3 44`<br>`2 66`           | `NO`  |
*/

#include <iostream>

class Stack {
 public:
  explicit Stack(int initial_size = 1) : buffer_size(initial_size), top(-1) { buffer = new int[buffer_size]; }

  ~Stack() { delete[] buffer; }
  Stack(const Stack&) = delete;
  Stack& operator=(const Stack&) = delete;

  void Push(int value) {
    Resize();
    buffer[++top] = value;
  }
  int Pop() {
    if (IsEmpty()) {
      return -1;
    }
    return buffer[top--];
  }
  bool IsEmpty() const { return top == -1; }

 private:
  int* buffer;
  int buffer_size;
  int top;

  void Resize() {
    if (top + 1 >= buffer_size) {
      buffer_size *= 2;
      int* new_buffer = new int[buffer_size];
      for (int i = 0; i <= top; ++i) {
        new_buffer[i] = buffer[i];
      }
      delete[] buffer;
      buffer = new_buffer;
    }
  }
};

class Queue {
 public:
  Queue() = default;
  Queue(const Queue&) = delete;
  Queue& operator=(const Queue&) = delete;

  void Push(int value) { stack_in.Push(value); }
  int Pop() {
    if (stack_out.IsEmpty()) {
      while (!stack_in.IsEmpty()) {
        stack_out.Push(stack_in.Pop());
      }
    }
    return stack_out.Pop();
  }
  bool IsEmpty() const { return stack_out.IsEmpty() && stack_in.IsEmpty(); }

 private:
  Stack stack_out, stack_in;
};

int main() {
  int n = 0;
  std::cin >> n;

  Queue q;
  bool is_correct = true;

  for (int i = 0; i < n; ++i) {
    int command = 0;
    int value = 0;
    std::cin >> command >> value;

    if (command == 3) {
      q.Push(value);
    } else if (command == 2) {
      int result = q.Pop();

      if (result != value) {
        is_correct = false;
      }
    }
  }

  if (is_correct) {
    std::cout << "YES" << std::endl;
  } else {
    std::cout << "NO" << std::endl;
  }

  return 0;
}
