/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 1. Хеш-таблица (6 баллов)

**Обязательная задача.**

Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с **открытой адресацией**. Хранимые строки непустые и состоят из строчных
латинских букв.

Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.

Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает
3/4.

Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

### № 1.1

Для разрешения коллизий используйте квадратичное пробирование. $i$-ая проба
$g(k, i)=g(k, i-1) + i (\mod m)$. $m$ - степень двойки.
*/

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

enum class State { EMPTY, KEY, DELETED };

struct HashNode {
  std::string key;
  State state = State::EMPTY;
};

class HashTable {
 public:
  HashTable();
  ~HashTable() = default;

  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;

  bool Add(const std::string& key);
  bool Remove(const std::string& key);
  bool Has(const std::string& key) const;

 private:
  std::vector<HashNode> table_;
  size_t keys_count_ = 0;
  size_t deleted_count_ = 0;
  static const size_t kHashParameter = 31;  // Константа для метода Горнера

  void Rehash();
  size_t Hash(const std::string& key, size_t m) const;
  size_t GetNextProbe(size_t current_probe, size_t i, size_t m) const;
};

HashTable::HashTable() : table_(8), keys_count_(0) {}

size_t HashTable::Hash(const std::string& key, size_t m) const {
  size_t hash = 0;
  for (char c : key) {
    hash = (hash * kHashParameter + static_cast<size_t>(c)) % m;
  }
  return hash;
}

void HashTable::Rehash() {
  std::vector<HashNode> new_table(table_.size() * 2);

  for (const auto& node : table_) {
    if (node.state == State::KEY) {
      size_t probe = Hash(node.key, new_table.size());
      size_t i = 1;

      while (new_table[probe].state == State::KEY) {
        probe = GetNextProbe(probe, i, new_table.size());
        i++;
      }
      new_table[probe].key = node.key;
      new_table[probe].state = State::KEY;
    }
  }
  table_ = std::move(new_table);
  deleted_count_ = 0;
}

bool HashTable::Add(const std::string& key) {
  if ((keys_count_ + deleted_count_) * 4 >= table_.size() * 3) {
    Rehash();
  }

  size_t probe = Hash(key, table_.size());
  size_t i = 1;
  int first_deleted = -1;

  while (table_[probe].state != State::EMPTY) {
    if (table_[probe].state == State::KEY && table_[probe].key == key) {
      return false;
    }
    if (table_[probe].state == State::DELETED && first_deleted == -1) {
      first_deleted = static_cast<int>(probe);
    }
    probe = GetNextProbe(probe, i, table_.size());
    i++;
  }

  if (first_deleted != -1) {
    table_[first_deleted].key = key;
    table_[first_deleted].state = State::KEY;
    deleted_count_--;
    keys_count_++;
  } else {
    table_[probe].key = key;
    table_[probe].state = State::KEY;
    keys_count_++;
  }

  return true;
}

size_t HashTable::GetNextProbe(size_t current_probe, size_t i, size_t m) const { return (current_probe + i) % m; }

bool HashTable::Has(const std::string& key) const {
  size_t probe = Hash(key, table_.size());
  size_t i = 1;

  while (table_[probe].state != State::EMPTY) {
    if (table_[probe].state == State::KEY && table_[probe].key == key) {
      return true;
    }
    probe = GetNextProbe(probe, i, table_.size());
    i++;
  }
  return false;
}

bool HashTable::Remove(const std::string& key) {
  size_t probe = Hash(key, table_.size());
  size_t i = 1;

  while (table_[probe].state != State::EMPTY) {
    if (table_[probe].state == State::KEY && table_[probe].key == key) {
      table_[probe].state = State::DELETED;
      keys_count_--;
      deleted_count_++;
      return true;
    }
    probe = GetNextProbe(probe, i, table_.size());
    i++;
  }
  return false;
}

int main() {
  HashTable hash_table;
  char operation;
  std::string key;

  while (std::cin >> operation >> key) {
    bool result = false;
    switch (operation) {
      case '+':
        result = hash_table.Add(key);
        break;
      case '-':
        result = hash_table.Remove(key);
        break;
      case '?':
        result = hash_table.Has(key);
        break;
      default:
        assert(false && "Unknown operation");
    }

    if (result) {
      std::cout << "OK" << std::endl;
    } else {
      std::cout << "FAIL" << std::endl;
    }
  }

  return 0;
}
