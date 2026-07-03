/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 5. Алгоритм сжатия данных Хаффмана (6 баллов и более)

Напишите две функции для создания архива из одного файла и извлечения файла из архива.

**Требования**: коэффициент сжатия $< 1$.

```cpp
// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);
```

где:

```cpp
typedef unsigned char byte;
#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};
```

В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных.
Дерево Хаффмана требуется хранить эффективно - не более `10` бит на каждый `8`-битный символ.

В контест необходимо отправить `.cpp` файл содержащий функции `Encode`, `Decode`, а также включающий файл `Huffman.h`. Тестирующая программа выводит размер
сжатого файла в процентах от исходного.

**Лучшие 3 решения с коэффициентом сжатия < 1 из каждой группы оцениваются в 10, 7 и 4 баллов соответственно.**

Пример минимального решения:

```cpp
#include "Huffman.h"

static void copyStream(IInputStream& input, IOutputStream& output)
{
    byte value;
    while (input.Read(value))
    {
        output.Write(value);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed)
{
    copyStream(original, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original)
{
    copyStream(compressed, original);
}
```
*/

#include <algorithm>
#include <cstdint>
#include <vector>

#include "Huffman.h"

namespace {
typedef unsigned char ubyte;

const int BLOCK_SIZE = 65536;

class BitWriter {
  IOutputStream& out_;
  uint64_t buf_ = 0;
  int count_ = 0;

 public:
  explicit BitWriter(IOutputStream& out) : out_(out) {}

  inline void WriteBit(ubyte bit) {
    buf_ = (buf_ << 1) | (bit & 1u);
    if (++count_ == 8) {
      out_.Write(static_cast<byte>(buf_));
      buf_ = 0;
      count_ = 0;
    }
  }

  inline void WriteCode(uint64_t packed_msb, int n) {
    if (n == 0) return;
    buf_ = (buf_ << n) | (packed_msb >> (64 - n));
    count_ += n;
    while (count_ >= 8) {
      count_ -= 8;
      out_.Write(static_cast<byte>(buf_ >> count_));
    }
    buf_ &= (1ull << count_) - 1;
  }

  void Flush() {
    if (count_ > 0) {
      out_.Write(static_cast<byte>(buf_ << (8 - count_)));
      buf_ = 0;
      count_ = 0;
    }
  }
};

class BitReader {
  IInputStream& in_;
  ubyte buf_ = 0;
  int count_ = 0;

 public:
  explicit BitReader(IInputStream& in) : in_(in) {}

  inline int& count() { return count_; }
  inline ubyte& buf() { return buf_; }
  inline IInputStream& stream() { return in_; }

  inline bool ReadBit(bool& bit) {
    if (count_ == 0) {
      byte raw;
      if (!in_.Read(raw)) return false;
      buf_ = static_cast<ubyte>(raw);
      count_ = 8;
    }
    bit = (buf_ >> (count_ - 1)) & 1;
    --count_;
    return true;
  }

  inline bool ReadByte(ubyte& b) {
    b = 0;
    for (int i = 0; i < 8; ++i) {
      bool bit;
      if (!ReadBit(bit)) return false;
      b = static_cast<ubyte>((b << 1) | (bit ? 1u : 0u));
    }
    return true;
  }
};

struct Node {
  uint32_t freq = 0;
  ubyte symbol = 0;
  int left = -1;
  int right = -1;
  int height = 0;
  bool is_leaf = false;
};

struct CompareNode {
  const std::vector<Node>& pool;
  explicit CompareNode(const std::vector<Node>& p) : pool(p) {}
  bool operator()(int a, int b) const {
    if (pool[a].freq != pool[b].freq) return pool[a].freq > pool[b].freq;
    if (pool[a].height != pool[b].height) return pool[a].height > pool[b].height;
    return a > b;
  }
};

struct FastCode {
  uint64_t bits = 0;
  uint8_t length = 0;
};

static void BuildCodes(int root, const std::vector<Node>& pool, uint64_t cur_bits, int cur_len, std::vector<FastCode>& codes) {
  if (root == -1) return;

  if (pool[root].is_leaf) {
    codes[pool[root].symbol] = {cur_bits, static_cast<uint8_t>(cur_len)};
    return;
  }

  BuildCodes(pool[root].left, pool, cur_bits, cur_len + 1, codes);
  BuildCodes(pool[root].right, pool, cur_bits | (1ull << (63 - cur_len)), cur_len + 1, codes);
}

static void SerializeTreeDFS(int root, const std::vector<Node>& pool, BitWriter& bw) {
  if (root == -1) return;
  if (pool[root].is_leaf) {
    bw.WriteBit(1);
    bw.WriteCode(static_cast<uint64_t>(pool[root].symbol) << 56, 8);
  } else {
    bw.WriteBit(0);
    SerializeTreeDFS(pool[root].left, pool, bw);
    SerializeTreeDFS(pool[root].right, pool, bw);
  }
}

static int DeserializeTreeDFS(BitReader& br, std::vector<Node>& pool) {
  bool bit;
  if (!br.ReadBit(bit)) return -1;

  int idx = static_cast<int>(pool.size());
  pool.push_back(Node());

  if (bit) {
    ubyte sym;
    if (!br.ReadByte(sym)) return -1;
    pool[idx].symbol = sym;
    pool[idx].is_leaf = true;
  } else {
    pool[idx].is_leaf = false;
    pool[idx].left = DeserializeTreeDFS(br, pool);
    pool[idx].right = DeserializeTreeDFS(br, pool);
  }
  return idx;
}
}  // namespace

void Encode(IInputStream& original, IOutputStream& compressed) {
  BitWriter bw(compressed);

  std::vector<ubyte> block;
  block.reserve(BLOCK_SIZE);
  std::vector<Node> pool;
  pool.reserve(511);
  std::vector<int> initial_nodes;
  initial_nodes.reserve(256);
  std::vector<FastCode> codes(256);

  while (true) {
    block.clear();
    pool.clear();
    initial_nodes.clear();
    std::fill(codes.begin(), codes.end(), FastCode{0, 0});

    for (int i = 0; i < BLOCK_SIZE; ++i) {
      byte b;
      if (!original.Read(b)) break;
      block.push_back(static_cast<ubyte>(b));
    }

    if (block.size() == BLOCK_SIZE) {
      bw.WriteBit(1);
    } else if (!block.empty()) {
      bw.WriteBit(0);
      bw.WriteBit(1);
      bw.WriteCode(static_cast<uint64_t>(block.size()) << 48, 16);
    } else {
      bw.WriteBit(0);
      bw.WriteBit(0);
      break;
    }

    uint32_t freq[256] = {};
    for (ubyte c : block) freq[c]++;

    for (int i = 0; i < 256; ++i) {
      if (freq[i] > 0) {
        int idx = static_cast<int>(pool.size());
        pool.push_back({freq[i], static_cast<ubyte>(i), -1, -1, 0, true});
        initial_nodes.push_back(idx);
      }
    }

    CompareNode cmp(pool);
    std::make_heap(initial_nodes.begin(), initial_nodes.end(), cmp);

    while (initial_nodes.size() > 1) {
      std::pop_heap(initial_nodes.begin(), initial_nodes.end(), cmp);
      int r = initial_nodes.back();
      initial_nodes.pop_back();

      std::pop_heap(initial_nodes.begin(), initial_nodes.end(), cmp);
      int l = initial_nodes.back();
      initial_nodes.pop_back();

      int idx = static_cast<int>(pool.size());
      int h = std::max(pool[l].height, pool[r].height) + 1;
      pool.push_back({pool[l].freq + pool[r].freq, 0, l, r, h, false});

      initial_nodes.push_back(idx);
      std::push_heap(initial_nodes.begin(), initial_nodes.end(), cmp);
    }

    int root = initial_nodes.empty() ? -1 : initial_nodes.front();

    BuildCodes(root, pool, 0ull, 0, codes);
    SerializeTreeDFS(root, pool, bw);

    for (ubyte c : block) {
      const FastCode& fc = codes[c];
      bw.WriteCode(fc.bits, fc.length);
    }
  }

  bw.Flush();
}

void Decode(IInputStream& compressed, IOutputStream& original) {
  BitReader br(compressed);
  std::vector<Node> pool;
  pool.reserve(511);

  while (true) {
    bool bit1;
    if (!br.ReadBit(bit1)) break;

    uint32_t block_size = 0;
    if (bit1) {
      block_size = BLOCK_SIZE;
    } else {
      bool bit2;
      if (!br.ReadBit(bit2)) break;
      if (!bit2) {
        break;
      } else {
        ubyte b1, b2;
        if (!br.ReadByte(b1) || !br.ReadByte(b2)) break;
        block_size = (static_cast<uint32_t>(b1) << 8) | b2;
      }
    }

    pool.clear();
    int root = DeserializeTreeDFS(br, pool);
    if (root == -1) break;

    if (pool[root].is_leaf) {
      for (uint32_t i = 0; i < block_size; ++i) {
        original.Write(static_cast<byte>(pool[root].symbol));
      }
    } else {
      int curr = root;
      int count = br.count();
      ubyte buf = br.buf();
      IInputStream& in_stream = br.stream();

      for (uint32_t decoded = 0; decoded < block_size;) {
        if (count == 0) {
          byte raw;
          if (!in_stream.Read(raw)) break;
          buf = static_cast<ubyte>(raw);
          count = 8;
        }

        bool bit = (buf >> (count - 1)) & 1;
        --count;

        curr = bit ? pool[curr].right : pool[curr].left;
        if (curr == -1) break;

        if (pool[curr].is_leaf) {
          original.Write(static_cast<byte>(pool[curr].symbol));
          ++decoded;
          curr = root;
        }
      }

      br.count() = count;
      br.buf() = buf;
    }
  }
}
