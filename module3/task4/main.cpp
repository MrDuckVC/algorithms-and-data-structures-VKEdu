/*
# Модуль №3

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 4. «Пятнашки» (8 баллов)

Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:

```text
[ 1  2  3  4 ]
[ 5  6  7  8 ]
[ 9  10 11 12]
[ 13 14 15 0 ]
```

где `0` задает пустую ячейку.

Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.

* *Формат входных данных.*

    Начальная расстановка.

* *Формат выходных данных.*

    Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек, которое нужно сделать, чтобы достичь выигрышной
конфигурации, а во второй строке выведите соответствующую последовательность ходов: `L` означает, что костяшка сдвинулась влево, `R` – вправо, `U` – вверх, `D`
– вниз. Если таких последовательностей несколько, то выведите любую из них. Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число
`−1`.

| in                                                     | out        |
| :----------------------------------------------------- | :--------- |
| `1 2 3 4`<br>`5 6 7 8`<br>`9 10 11 0`<br>`13 14 15 12` | `1`<br>`U` |
*/

#pragma GCC optimize("O3")
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

typedef uint64_t uint64;

// Целевое состояние:
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0
// В 64-битном числе (по 4 бита на ячейку, LSB - это индекс 0)
const uint64 TARGET_BOARD = 0x0FEDCBA987654321ULL;

int manhattan[16][16];

void init_manhattan() {
  for (int val = 1; val < 16; ++val) {
    int target_r = (val - 1) / 4;
    int target_c = (val - 1) % 4;
    for (int pos = 0; pos < 16; ++pos) {
      int r = pos / 4;
      int c = pos % 4;
      manhattan[val][pos] = std::abs(r - target_r) + std::abs(c - target_c);
    }
  }
}

inline int get_val(uint64 board, int pos) { return (board >> (pos << 2)) & 0xF; }

inline uint64 swap_vals(uint64 board, int pos1, int pos2) {
  uint64 val1 = (board >> (pos1 << 2)) & 0xF;
  uint64 val2 = (board >> (pos2 << 2)) & 0xF;
  board &= ~(0xFULL << (pos1 << 2));
  board &= ~(0xFULL << (pos2 << 2));
  board |= (val1 << (pos2 << 2));
  board |= (val2 << (pos1 << 2));
  return board;
}

int linear_conflict(uint64 board) {
  int conflict = 0;
  for (int r = 0; r < 4; ++r) {
    for (int c1 = 0; c1 < 3; ++c1) {
      int val1 = get_val(board, r * 4 + c1);
      if (val1 == 0 || (val1 - 1) / 4 != r) continue;
      for (int c2 = c1 + 1; c2 < 4; ++c2) {
        int val2 = get_val(board, r * 4 + c2);
        if (val2 == 0 || (val2 - 1) / 4 != r) continue;
        if (val1 > val2) conflict += 2;
      }
    }
  }
  for (int c = 0; c < 4; ++c) {
    for (int r1 = 0; r1 < 3; ++r1) {
      int val1 = get_val(board, r1 * 4 + c);
      if (val1 == 0 || (val1 - 1) % 4 != c) continue;
      for (int r2 = r1 + 1; r2 < 4; ++r2) {
        int val2 = get_val(board, r2 * 4 + c);
        if (val2 == 0 || (val2 - 1) % 4 != c) continue;
        if (val1 > val2) conflict += 2;
      }
    }
  }
  return conflict;
}

struct State {
  uint64 board;
  int g;
  int manhattan_h;
  int z;
  int f;

  State(uint64 b, int g_val, int m_h, int z_val, int f_val) : board(b), g(g_val), manhattan_h(m_h), z(z_val), f(f_val) {}

  bool operator>(const State& o) const {
    if (f != o.f) return f > o.f;
    return g < o.g;
  }
};

struct FastHash {
  size_t operator()(uint64 k) const {
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    return k;
  }
};

bool is_solvable(const std::vector<int>& v) {
  int inv = 0;
  int row_0 = 0;
  for (int i = 0; i < 16; ++i) {
    if (v[i] == 0) {
      row_0 = i / 4;
      continue;
    }
    for (int j = i + 1; j < 16; ++j) {
      if (v[j] != 0 && v[i] > v[j]) {
        inv++;
      }
    }
  }
  int row_from_bottom = 4 - row_0;
  return (inv + row_from_bottom) % 2 != 0;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  init_manhattan();

  std::vector<int> start_v(16);
  uint64 start_board = 0;
  int start_z = 0;

  for (int i = 0; i < 16; ++i) {
    if (!(std::cin >> start_v[i])) return 0;
    if (start_v[i] == 0) start_z = i;
    start_board |= (static_cast<uint64>(start_v[i]) << (i << 2));
  }

  if (start_board == TARGET_BOARD) {
    std::cout << 0 << std::endl << std::endl;
    return 0;
  }

  if (!is_solvable(start_v)) {
    std::cout << -1 << std::endl;
    return 0;
  }

  std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
  std::unordered_map<uint64, int, FastHash> g_score;
  std::unordered_map<uint64, uint64, FastHash> came_from;

  int start_manhattan = 0;
  for (int i = 0; i < 16; ++i) {
    int val = start_v[i];
    if (val != 0) start_manhattan += manhattan[val][i];
  }

  g_score[start_board] = 0;
  came_from[start_board] = start_board;

  const int W = 5;
  int initial_f = 0 + W * (start_manhattan + linear_conflict(start_board));
  pq.push(State(start_board, 0, start_manhattan, start_z, initial_f));

  int moves[4] = {-4, 4, -1, 1};

  while (!pq.empty()) {
    State curr = pq.top();
    pq.pop();

    if (curr.board == TARGET_BOARD) {
      break;
    }

    auto g_it = g_score.find(curr.board);
    if (g_it != g_score.end() && curr.g > g_it->second) {
      continue;
    }

    int z = curr.z;

    for (int dz : moves) {
      if (z % 4 == 0 && dz == -1) continue;
      if (z % 4 == 3 && dz == 1) continue;
      if (z / 4 == 0 && dz == -4) continue;
      if (z / 4 == 3 && dz == 4) continue;

      int nz = z + dz;
      int val = get_val(curr.board, nz);

      uint64 nb = swap_vals(curr.board, z, nz);

      int new_manhattan = curr.manhattan_h - manhattan[val][nz] + manhattan[val][z];
      int ng = curr.g + 1;

      auto it = g_score.find(nb);
      if (it == g_score.end() || ng < it->second) {
        g_score[nb] = ng;
        came_from[nb] = curr.board;

        int nf = ng + W * (new_manhattan + linear_conflict(nb));
        pq.push(State(nb, ng, new_manhattan, nz, nf));
      }
    }
  }

  if (came_from.find(TARGET_BOARD) == came_from.end()) {
    std::cout << -1 << std::endl;
    return 0;
  }

  std::string path = "";
  uint64 curr = TARGET_BOARD;

  while (curr != start_board) {
    uint64 parent = came_from[curr];

    int c_z = -1, p_z = -1;
    for (int i = 0; i < 16; ++i) {
      if (get_val(curr, i) == 0) c_z = i;
      if (get_val(parent, i) == 0) p_z = i;
    }

    if (c_z == p_z - 4)
      path += 'D';
    else if (c_z == p_z + 4)
      path += 'U';
    else if (c_z == p_z - 1)
      path += 'R';
    else if (c_z == p_z + 1)
      path += 'L';

    curr = parent;
  }

  std::reverse(path.begin(), path.end());

  std::cout << path.length() << std::endl << path << std::endl;

  return 0;
}
