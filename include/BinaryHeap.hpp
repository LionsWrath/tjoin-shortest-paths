#ifndef _BINARY_HEAP_HPP
#define _BINARY_HEAP_HPP

#include <deque>
#include <stdexcept>
#include <functional>
#include <unordered_map>

#include <iostream>

template <class T, class I, class C=std::greater<T>>
class BinaryHeap {
    std::deque<std::pair<I, T>> *vec;
    std::unordered_map<I, long long> *index_map;

    void swap(int a, int b) {
        index_map->operator[](vec->at(a).first) = b;
        index_map->operator[](vec->at(b).first) = a;
        std::swap(vec->at(a), vec->at(b));
    }

public:
    BinaryHeap() {
        vec = new std::deque<std::pair<I, T>>(); 
        index_map = new std::unordered_map<I, long long>();
    }

    ~BinaryHeap() {
        delete vec;
        delete index_map;
    }

    // Heapify a subtree beginning at a index
    // O(log n)
    void heapify(int idx) {
        int r = this->get_right(idx);
        int l = this->get_left(idx);
        int s = idx;

        if (l < vec->size() && C()(vec->at(s), vec->at(l))) s = l;
        if (r < vec->size() && C()(vec->at(s), vec->at(r))) s = r;

        if (s != idx) {
            this->swap(idx, s);
            this->heapify(s);
        }
    }

    // Check if heap is empty
    // O(1)
    bool is_empty() {
        return 0 == size();
    }

    // Get number of elements in heap
    // O(1)
    int size() {
        return vec->size();
    }

    // Get parent of a node
    // O(1)
    int get_parent(int idx) {
        return (idx - 1)/2;
    }

    // Get the left child element of a node
    // O(1)
    int get_left(int idx) {
        return 2*idx + 1;
    }

    // Get the right child element of a node
    // O(1)
    int get_right(int idx) {
        return 2*idx + 2;
    }

    // Get the actual minimal element of the heap
    // O(1)
    std::pair<I, T> get_min() {
        return vec->front();
    }

    // Get the actual minimal and extract him of the heap
    // O(log n)
    std::pair<I, T> extract_min() {
        if (0 == vec->size())
            throw std::logic_error("The heap is empty!");

        std::pair<I, T> root = this->get_min();

        // Switch the last and the first
        this->swap(0, this->size() - 1);

        // Remove the element
        vec->pop_back();
        index_map->erase(root.first);

        // Reconfigure subtree
        this->heapify(0);

        return root;
    }

    // Insert an element in the heap
    // O(log n)
    void insert_key(I value, T key) {
        vec->push_back(std::make_pair(value, key));
        index_map->operator[](value) = vec->size() - 1;

        int idx = index_map->operator[](value);
        while (idx != 0 && C()(vec->at(this->get_parent(idx)), vec->at(idx))) {
            this->swap(idx, this->get_parent(idx)); 
            idx = this->get_parent(idx);
        }
    }

    // Decrease a value of a key T on the heap with value K
    // O(log n)
    void decrease_key(I value, T key) {
        long long idx = index_map->operator[](value);

        if (C()(std::make_pair(value, key), vec->at(idx)))
            throw std::logic_error("The new key is greater than the last!");

        vec->operator[](idx) = std::make_pair(value, key);

        // Bubble up
        while (idx != 0 && C()(vec->at(this->get_parent(idx)), vec->at(idx))) {
            this->swap(idx, this->get_parent(idx)); 
            idx = this->get_parent(idx);
        }
    }
};

#endif
