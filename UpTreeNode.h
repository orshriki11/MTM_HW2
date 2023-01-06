//
// Created by tur22 on 05/01/2023.
//

#ifndef MTM_HW2_UPTREENODE_H
#define MTM_HW2_UPTREENODE_H
#include "UnionFind.h"
#include "Team.h"
#include "Player.h"

template<class K,class T>
class UnionFindNode {
    int size;  // size of the up tree
    T data;
    K master;
    //bool isRoot;
    UnionFindNode<K,T>* parent;  // parent in the up tree

    UnionFindNode(T Data);

    UnionFindNode();

    UnionFindNode<K,T> &operator=(const UnionFindNode<K,T> &node) = default;

    ~UnionFindNode() = default;

    void MakeSet(T data);

    void insert(UnionFindNode<K,T>* node);

    K Find(UnionFindNode<K,T>* key);

    void Unite(UnionFindNode<K,T>* x, UnionFindNode<K,T>* y);
};

template<class K,class T>
UnionFindNode<K,T>::UnionFindNode(T Data) : data(Data), parent(nullptr), size(1), master(nullptr) {}

template<class K,class T>
UnionFindNode<K,T>::UnionFindNode() : data(T()), parent(nullptr), size(1),master(nullptr) {}


template<class K,class T>
void UnionFindNode<K,T>::insert(UnionFindNode<K,T>* node) {
    if(root != nullptr)
    {
        root = node;
    }
    else
    {
        node->parent = root;
    }
    size++;
    return;

}




#endif //MTM_HW2_UPTREENODE_H