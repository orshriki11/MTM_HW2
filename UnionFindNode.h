//
// Created by tur22 on 06/01/2023.
//

#ifndef MTM_HW2_UNIONFINDNODE_H
#define MTM_HW2_UNIONFINDNODE_H

#include "Team.h"
#include "Player.h"
#include "wet2util.h"

template<class K,class T>
class UnionFindNode {
public:
    int size;  // size of the up tree
    T data;
    K master;
    permutation_t linkSpirit;
    int link_gamesPlayed;
    //bool isRoot;
    UnionFindNode<K,T>* parent;  // parent in the up tree

    UnionFindNode(T Data);

    UnionFindNode();

    UnionFindNode<K,T> &operator=(const UnionFindNode<K,T> &node) = default;

    ~UnionFindNode() = default;

    void MakeSet(T data);

    void insert(UnionFindNode<K,T>* node);

    //UnionFindNode<K,T>* FindRoot(UnionFindNode<K,T>* key);

    UnionFindNode<K,T>* FindRoot();

    K Find();

    //K Find(UnionFindNode<K,T>* key);

    void Unite(UnionFindNode<K,T>* x);

    //void Unite(UnionFindNode<K,T>* x, UnionFindNode<K,T>* y);

    K getMaster();
};

template<class K,class T>
UnionFindNode<K,T>::UnionFindNode(T Data) : data(Data), parent(nullptr), size(1), master(nullptr) {}

template<class K,class T>
UnionFindNode<K,T>::UnionFindNode() : data(T()), parent(nullptr), size(1),master(nullptr) {}


template<class K,class T>
void UnionFindNode<K,T>::insert(UnionFindNode<K,T>* node) {
    if(this != nullptr)
    {
        node->parent = this;
        size++;
    }
    else
    {
        node->parent = nullptr;
    }
    node->size++;
    return;

}

template<class K,class T>
void UnionFindNode<K,T>::MakeSet(T data) {
    UnionFindNode<K,T> *new_node = new UnionFindNode<K,T>(data);
    if(this != nullptr)
    {
        new_node->parent = this;
        size++;
    }
    else
    {
        new_node->parent = nullptr;
    }
    new_node->size++;
    return;

}

/*template<class K,class T>
K UnionFindNode<K,T>::Find(UnionFindNode<K,T>* key) {
    UnionFindNode<K,T>* root = findRoot(key);
    return root->master;
}*/

/*template<class K,class T>
UnionFindNode<K,T>* UnionFindNode<K,T>::FindRoot(UnionFindNode<K,T>* key) {
    if(key->parent != nullptr)
    {
        UnionFindNode<K,T>* key_root = findRoot(key->parent);
        key->parent = key_root;
        return key_root;
    }
    else
    {
        return key;
    }
}*/

template<class K,class T>
K UnionFindNode<K,T>::Find() {
    UnionFindNode<K,T>* root = this->FindRoot();
    return root->master;
}

template<class K,class T>
UnionFindNode<K,T>* UnionFindNode<K,T>::FindRoot() {
    if(parent != nullptr)
    {
        UnionFindNode<K,T>* key_root = parent->FindRoot();
        parent = key_root;
        return key_root;
    }
    else
    {
        return this;
    }
}



template<class K,class T>
void UnionFindNode<K,T>::Unite(UnionFindNode<K,T>* x) {
    UnionFindNode<K,T>* x_Root = x->FindRoot();
    UnionFindNode<K,T>* y_Root = this->FindRoot();
    if(x_Root = y_Root)
        return;
    x_Root->parent = y_Root;
    return;
}

/*template<class K,class T>
void UnionFindNode<K,T>::Unite(UnionFindNode<K,T>* x, UnionFindNode<K,T>* y) {
    UnionFindNode<K,T>* x_Root = findRoot(x);
    UnionFindNode<K,T>* y_Root = findRoot(y);
    if(x_Root = y_Root)
        return;
    if(x_Root->size >= y_Root->size)
    {
        y_Root->parent = x_Root;
    }
    else
    {
        x_Root->parent = y_Root;
    }
}*/

template<class K, class T>
K UnionFindNode<K,T>::getMaster() {
    return master;
}


#endif //MTM_HW2_UNIONFINDNODE_H
