#pragma once

#include <vector>

template <class T_Type, template<class> class T_Op > class SegmentTree {
public:
	SegmentTree() : m_Size{ 0 } {}
	SegmentTree(const std::vector<T_Type>& values) {
		m_Size = values.size();
		m_STree.resize(4 * m_Size);
		_build(0, 0, m_Size - 1, values);
	}
	~SegmentTree() {}

	void build(const std::vector<T_Type>& values) { 
		m_Size = values.size();
		m_STree.resize(4 * m_Size);
		_build(0, 0, m_Size - 1, values);
	}
	void update(int idx, int v) { _update(0, 0, m_Size - 1, idx, v); }
	T_Type query(int i, int j) { return _query(0, 0, m_Size - 1, i, j); }

private:
	inline int _parent(int i) const { return (i - 1) / 2; }
    inline int _leftChild(int i) const { return i * 2 + 1; }
    inline int _rightChild(int i) const { return i * 2 + 2; }
    
	void _build(int vertex, int L, int R, const std::vector<T_Type>& values) {
		if (L == R)
			m_STree[vertex] = values[L];
		else {
			int M = (L + R) / 2;
			_build(_leftChild(vertex), L, M, values);
			_build(_rightChild(vertex), M + 1, R, values);
			m_STree[vertex] = T_Op<T_Type>()(m_STree[_leftChild(vertex)], m_STree[_rightChild(vertex)]);
		}
	}

	void _update(int vertex, int L, int R, int idx, int v) {
		if (L == R)
			m_STree[vertex] = v;
		else {
			int M = (L + R) / 2;
			if (M < idx) _update(_rightChild(vertex), M + 1, R, idx, v);
			else _update(_leftChild(vertex), L, M, idx, v);
			m_STree[vertex] = T_Op<T_Type>()(m_STree[_leftChild(vertex)], m_STree[_rightChild(vertex)]);
		}
	}

	T_Type _query(int vertex, int L, int R, int i, int j) {
		if (i > R || j < L) return 0;
		if (L >= i && R <= j) return m_STree[vertex];
		int M = (L + R) / 2;
		T_Type p1 = _query(_leftChild(vertex), L, M, i, j);
		T_Type p2 = _query(_rightChild(vertex), M + 1, R, i, j);

		return T_Op<T_Type>()(p1, p2);
	}

private:
	std::vector<T_Type> m_STree;
	int m_Size;
};