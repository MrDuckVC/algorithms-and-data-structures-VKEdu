/*
Дана последовательность, состоящая из символов ’(’ и ’)’. Требуется определить, является ли данная последовательность правильной скобочной последовательностью
(каждой открывающей скобке соответствует закрывающая). Длина исходной последовательности ≤ 800000.
*/

#include <iostream>
#include <string>

class Stack {
 public:
  explicit Stack(int capacity) : buffer_size(capacity), top(-1) { buffer = new char[buffer_size]; }

  ~Stack() { delete[] buffer; }

  Stack(const Stack&) = delete;
  Stack& operator=(const Stack&) = delete;

  void Push(char value) { buffer[++top] = value; }

  char Pop() { return buffer[top--]; }

  bool IsEmpty() const { return top == -1; }

 private:
  char* buffer;
  int buffer_size;
  int top;
};

bool IsValidBracketSequence(const std::string& sequence) {
  int n = sequence.length();

  if (n == 0) {
    return true;
  }

  Stack stack(n);

  for (int i = 0; i < n; ++i) {
    if (sequence[i] == '(') {
      stack.Push('(');
    } else if (sequence[i] == ')') {
      if (stack.IsEmpty()) {
        return false;
      }
      stack.Pop();
    }
  }

  return stack.IsEmpty();
}

int main() {
  std::string sequence;
  if (!(std::cin >> sequence)) {
    return 0;
  }

  if (IsValidBracketSequence(sequence)) {
    std::cout << "YES" << std::endl;
  } else {
    std::cout << "NO" << std::endl;
  }

  return 0;
}
