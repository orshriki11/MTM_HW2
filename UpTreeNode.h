//
// Created by tur22 on 05/01/2023.
//

#ifndef MTM_HW2_UPTREENODE_H
#define MTM_HW2_UPTREENODE_H
#include "UnionFind.h"

template<class T>
struct UpTreeNode {
    int size;  // size of the up tree
    T data;
    //bool isRoot;
    UpTreeNode<T>* parent;  // parent in the up tree

    //UnionFind<T,T> UF = new UnionFind();

    UpTreeNode(T Data);

    UpTreeNode();

    UpTreeNode<T> &operator=(const UpTreeNode<T> &node) = default;

    ~UpTreeNode() = default;
};

template<class T>
UpTreeNode<T>::UpTreeNode(T Data) : data(Data), parent(nullptr), size(1), UF(nullptr) {}

template<class T>
UpTreeNode<T>::UpTreeNode() : data(T()), parent(nullptr), size(1), UF(nullptr) {}
#endif //MTM_HW2_UPTREENODE_H
