#pragma once

#include <vector>
#include <map>
#include <utility>

template <class TPriority, class TKey> class UpdatableMaxHeap {
public:
    UpdatableMaxHeap()
        : m_Size{ 0 } {}
    ~UpdatableMaxHeap() {}

    int getSize() const { return m_Size; }
    bool isInserted(const TKey& k) const {
        auto it = m_Keys.find(k);
        return (it != m_Keys.end() ? true : false);
    }
    std::pair<TPriority, TKey> top() const {
        if (m_Size == 0)
            return std::pair<TPriority, TKey>();
        return m_Data[0];
    }
    void pop() {
        if (m_Size == 1) {
            m_Keys.erase(m_Data[0].second);
            m_Data.pop_back();
            m_Size--;
        }
        else if (m_Size == 2) {
            std::swap(m_Data[0], m_Data[1]);
            m_Keys[m_Data[0].second] = 0;
            m_Keys.erase(m_Data[1].second);
            m_Data.pop_back();
            m_Size--;
        }
        else {
            std::swap(m_Data[0], m_Data[--m_Size]);
            m_Keys[m_Data[0].second] = 0;
            m_Keys.erase(m_Data[m_Size].second);
            m_Data.pop_back();

            heapify(0);
        }
    }
    void erase(const TKey& k) {
        if (isInserted(k)) {
            int i = m_Keys[k];
            std::swap(m_Data[i], m_Data[--m_Size]);
            m_Keys[m_Data[i].second] = i;
            m_Keys.erase(k);
            m_Data.pop_back();

            heapify(i);
        }
    }
    void insertOrUpdate(const TPriority& p, const TKey& k) {
        if (m_Size == 0) {
            m_Data.push_back(std::pair<TPriority, TKey>(p, k));
            m_Keys[k] = 0;
            m_Size++;
        }
        else {
            int i;
            auto it = m_Keys.find(k);
            if (it != m_Keys.end()) { // Update
                i = it->second;
                m_Data[i].first = p;
            }
            else { // Insert
                i = m_Size++;
                m_Data.push_back(std::pair<TPriority, TKey>(p, k));
                m_Keys[k] = i;
            }

            heapify(i);
        }
    }

private:
    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return i * 2 + 1; }
    int rightChild(int i) const { return i * 2 + 2; }
    void heapify(int i) { // MaxHeapify
        int p = parent(i);
        if (m_Data[i].first > m_Data[p].first) { // Up
            swapData(i, p);
            heapify(p);
        }
        else if (m_Data[i].first == m_Data[p].first) {
            if (m_Data[i].second > m_Data[p].second) { // Up
                swapData(i, p);
                heapify(p);
            }
            else { // Down
                downHeapify(i);
            }
        }
        else { // Down
            downHeapify(i);
        }
    }
    void downHeapify(int i) {
        int l = leftChild(i);
        int r = rightChild(i);
        int largest;

        if (l < m_Size) {
            if (m_Data[l].first > m_Data[i].first) { largest = l; }
            else if (m_Data[l].first == m_Data[i].first) {
                if (m_Data[l].second > m_Data[i].second) { largest = l; }
                else { largest = i; }
            }
            else { largest = i; }

            if (r < m_Size) {
                if (m_Data[r].first > m_Data[largest].first) { largest = r; }
                else if (m_Data[r].first == m_Data[largest].first) {
                    if (m_Data[r].second > m_Data[largest].second) { largest = r; }
                }
            }

            if (largest != i) {
                swapData(i, largest);
                heapify(largest);
            }
        }
    }
    void swapData(int i, int j) {
        m_Keys[m_Data[i].second] = j;
        m_Keys[m_Data[j].second] = i;
        std::swap(m_Data[i], m_Data[j]);
    }

private:
    std::vector<std::pair<TPriority, TKey>> m_Data;
    std::map<TKey, int> m_Keys;
    int m_Size;
};