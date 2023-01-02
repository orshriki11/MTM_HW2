//
// Created by hilla on 12/31/2022.
//

#ifndef WET2MIVNE_HASHTABLE_H
#define WET2MIVNE_HASHTABLE_H

#include <cassert>

template<class T>
class HashTable {
private:
    enum CellStatus {
        EMPTY, DIRTY, OCCUPIED
    };

    int *keys_array;
    T *data_array;
    CellStatus *cell_status_array;
    int size;
    int count_occupied;
    int count_dirty;

    template<class S>
    static S *allocateAndCopy(int new_size, int to_copy_size, S *array_to_copy);

    void growSize();

    void shrinkSize();

    int iterativeHash(int key, int iter_num);

    void rehash();

    int findKeyIndex(int key);

    void checkGrowSize();

    void checkShrinkSize();

    void checkRehash();

    static const int INITIAL_ARRAY_SIZE = 7;

    explicit HashTable(int size);

public:
    HashTable();

    HashTable(const HashTable<T> &to_copy);

    ~HashTable();

    HashTable &operator=(const HashTable<T> &other);

    void insert(int key, const T &element);

    void remove(int key);

    T *search(int key);

    int getNumElements();

    class KeyNotFoundException {
    };

    void print();
};

template<class T>
HashTable<T>::HashTable(int size) : size(size), count_occupied(0), count_dirty(0) {
    keys_array = new int[size];
    data_array = new T[size];
    cell_status_array = new CellStatus[size];
    for (int i = 0; i < size; i++) {
        cell_status_array[i] = EMPTY;
    }
}

template<class T>
template<class S>
S *HashTable<T>::allocateAndCopy(int new_size, int to_copy_size, S *array_to_copy) {
    S *copied_array = new S[new_size];
    for (int i = 0; i < to_copy_size; i++) {
        copied_array[i] = array_to_copy[i];
    }
    return copied_array;
}

template<class T>
HashTable<T>::HashTable() : size(INITIAL_ARRAY_SIZE), count_occupied(0), count_dirty(0) {
    keys_array = new int[size];
    data_array = new T[size];
    cell_status_array = new CellStatus[size];
    for (int i = 0; i < size; i++) {
        cell_status_array[i] = EMPTY;
    }
}

template<class T>
HashTable<T>::HashTable(const HashTable<T> &to_copy) : size(to_copy.size), count_occupied(to_copy.count_occupied),
                                                       count_dirty(to_copy.count_dirty) {
    keys_array = allocateAndCopy(size, to_copy.size, to_copy.keys_array);
    data_array = allocateAndCopy(size, to_copy.size, to_copy.data_array);
    cell_status_array = allocateAndCopy(size, to_copy.size, to_copy.cell_status_array);
}

template<class T>
HashTable<T>::~HashTable() {
    delete[] keys_array;
    delete[] data_array;
    delete[] cell_status_array;
}

template<class T>
HashTable<T> &HashTable<T>::operator=(const HashTable<T> &other) {
    if (this == &other) {
        return *this;
    }
    int *keys_array_copy = allocateAndCopy(other.size, other.size, other.keys_array);
    T *data_array_copy = allocateAndCopy(other.size, other.size, other.data_array);
    CellStatus *cell_status_array_copy = allocateAndCopy(other.size, other.size, other.cell_status_array);

    delete[] keys_array;
    delete[] data_array;
    delete[] cell_status_array;

    keys_array = keys_array_copy;
    data_array = data_array_copy;
    cell_status_array = cell_status_array_copy;

    size = other.size;
    count_occupied = other.count_occupied;
    count_dirty = other.count_dirty;
    return *this;
}

template<class T>
int HashTable<T>::iterativeHash(int key, int iter_num) {
    int h = key % size;
    int r = 1 + (key % (size - 1));
    return (h + iter_num * r) % size;
}

template<class T>
void HashTable<T>::growSize() {
    int new_size = (size + 1) * 2 - 1;

    int *doubled_keys_array = allocateAndCopy(new_size, size, keys_array);
    T *doubled_data_array = allocateAndCopy(new_size, size, data_array);
    CellStatus *doubled_cell_status_array = allocateAndCopy(new_size, size, cell_status_array);

    for (int i = size; i < new_size; i++) {
        doubled_cell_status_array[i] = EMPTY;
    }

    delete[] keys_array;
    delete[] data_array;
    delete[] cell_status_array;

    keys_array = doubled_keys_array;
    data_array = doubled_data_array;
    cell_status_array = doubled_cell_status_array;
    size = new_size;
}

template<class T>
void HashTable<T>::insert(int key, const T &element) {
    checkGrowSize();
    checkRehash();

    int h_i;
    for (int i = 0; i < size; i++) {
        h_i = iterativeHash(key, i);
        if (cell_status_array[h_i] != OCCUPIED) {
            break;
        }
    }
    assert(cell_status_array[h_i] != OCCUPIED);

    if (cell_status_array[h_i] == DIRTY) {
        count_dirty--;
    }

    keys_array[h_i] = key;
    data_array[h_i] = element;
    cell_status_array[h_i] = OCCUPIED;

    count_occupied++;
}

template<class T>
void HashTable<T>::rehash() {
    //new hash table of same size
    HashTable<T> new_hash_table(size);
    //go thru our hash table and insert into new one the relevant data
    for (int i = 0; i < size; i++) {
        if (cell_status_array[i] == OCCUPIED) {
            new_hash_table.insert(keys_array[i], data_array[i]);
        }
    }
    //use overloaded = operator - old hash = new hash
    *this = new_hash_table;
    int x = 2;
}

template<class T>
void HashTable<T>::shrinkSize() {
    //new hash table
    int new_size = (size + 1) / 2 - 1;
    if (new_size < INITIAL_ARRAY_SIZE) {
        new_size = INITIAL_ARRAY_SIZE;
    }
    HashTable<T> new_hash_table(new_size);
    //go thru our hash table and insert into new one the relevant data
    for (int i = 0; i < size; i++) {
        if (cell_status_array[i] == OCCUPIED) {
            new_hash_table.insert(keys_array[i], data_array[i]);
        }
    }
    //use overloaded = operator - old hash = new hash
    *this = new_hash_table;
}

template<class T>
int HashTable<T>::findKeyIndex(int key) {
    int h_i;
    for (int i = 0; i < size; i++) {
        h_i = iterativeHash(key, i);
        if (keys_array[h_i] == key && cell_status_array[h_i] == OCCUPIED) {
            return h_i;
        }
    }
    return -1;
}

template<class T>
void HashTable<T>::remove(int key) {
    checkShrinkSize();
    checkRehash();

    int key_index = findKeyIndex(key);
    if (key_index == -1) {
        throw KeyNotFoundException();
    }

    cell_status_array[key_index] = DIRTY;
    count_occupied--;
    count_dirty++;
}

template<class T>
T *HashTable<T>::search(int key) {
    int key_index = findKeyIndex(key);
    if (key_index == -1) {
        return nullptr;
    }
    return &data_array[key_index];
}

template<class T>
int HashTable<T>::getNumElements() {
    return count_occupied;
}

template<class T>
void HashTable<T>::checkGrowSize() {
    if (count_occupied >= 0.75 * size) {
        growSize();
    }
}

template<class T>
void HashTable<T>::checkShrinkSize() {
    if (count_occupied <= size / 4 && size > INITIAL_ARRAY_SIZE) {
        shrinkSize();
    }
}

template<class T>
void HashTable<T>::checkRehash() {
    if (count_dirty > size / 4) {
        rehash();
    }
}

template<class T>
void HashTable<T>::print() {
    std::cout << "size: " << size << " count_occupied: " << count_occupied << " count_dirty: " << count_dirty
              << std::endl;

    std::cout << "Occupied: {";
    // loop over occupied and print in k:d, format
    for (int i = 0; i < size; i++) {
        if (cell_status_array[i] == OCCUPIED)
            std::cout << keys_array[i] << ":" << data_array[i] << ", ";
    }
    std::cout << "}" << std::endl;

    std::cout << "Dirty: {";
    // loop over dirty and print in k:d, format
    for (int i = 0; i < size; i++) {
        if (cell_status_array[i] == DIRTY)
            std::cout << keys_array[i] << ":" << data_array[i] << ", ";
    }
    std::cout << "}" << std::endl;
}


#endif //WET2MIVNE_HASHTABLE_H
