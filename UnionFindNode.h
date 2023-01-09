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
    int whenJoined;

    int gamesPlayed_whenBought;
    //bool isRoot;
    UnionFindNode<K,T>* parent;  // parent in the up tree

    explicit UnionFindNode(T Data);

    UnionFindNode();

    UnionFindNode<K,T> &operator=(const UnionFindNode<K,T> &node) = default;

    ~UnionFindNode() = default;

    void MakeSet(T data);

    void insert(UnionFindNode<K,T>* node);

    //UnionFindNode<K,T>* FindRoot(UnionFindNode<K,T>* key);

    UnionFindNode<K,T>* FindRoot(permutation_t *totalSpirit,int *pre_gamesPlayed);

    K Find();

    UnionFindNode<K,T>* FindRootOnly();

    //K Find(UnionFindNode<K,T>* key);

    void Unite(UnionFindNode<K,T>* x);

    K FindWithSpirit(permutation_t *totalSpirit);

    //void FindSpiritLinks(permutation_t *totalSpirit);

    int FindGamesPlayed();


    //void Unite(UnionFindNode<K,T>* x, UnionFindNode<K,T>* y);

    //K getMaster();
};

template<class K,class T>
UnionFindNode<K,T>* UnionFindNode<K,T>::FindRootOnly() {
    auto node = this;
    while (node->parent != nullptr) {
        node = node->parent;
    }
    return node;
}

template<class K,class T>
UnionFindNode<K,T>::UnionFindNode(T Data) :
        size(1),
        data(Data),
        master(nullptr),
        link_gamesPlayed(0),
        whenJoined(0),
        gamesPlayed_whenBought(0),
        parent(nullptr)
          {
              linkSpirit = permutation_t::neutral();
          }

template<class K,class T>
UnionFindNode<K,T>::UnionFindNode() : size(1),
                                    data(T()),
                                    master(nullptr),
                                    link_gamesPlayed(0),
                                    whenJoined(0),
                                    gamesPlayed_whenBought(0),
                                    parent(nullptr) {
    linkSpirit = permutation_t::neutral();
                                    }


template<class K,class T>
void UnionFindNode<K,T>::insert(UnionFindNode<K,T>* node) {
    if(this != NULL)
    {
        node->parent = this;
        this->size++;
        node->whenJoined = size;
        //initNode = false;
    }
    else
    {
        node->parent = nullptr;
        node->master = master;
        //initNode = false;

    }

}

template<class K,class T>
void UnionFindNode<K,T>::MakeSet(T data) {
    auto *new_node = new UnionFindNode<K,T>(data);
    if(this != nullptr)
    {
        new_node->parent = this;
        size++;
    }
    else
    {
        new_node->parent = nullptr;
    }
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
    permutation_t totalSpirit = permutation_t::neutral();
    int wow = 0;
    UnionFindNode<K,T>* root = this->FindRoot(&totalSpirit,&wow);
    return root->master;
}

template<class K,class T>
UnionFindNode<K,T>* UnionFindNode<K,T>::FindRoot(permutation_t *totalSpirit, int *pre_gamesPlayed) {
    if(parent != nullptr)
    {
        UnionFindNode<K,T>* key_root = parent->FindRoot(totalSpirit,pre_gamesPlayed);
        *totalSpirit = (*totalSpirit) * linkSpirit;

        *pre_gamesPlayed -= link_gamesPlayed;
        *pre_gamesPlayed += gamesPlayed_whenBought;
        parent = key_root;
        linkSpirit = *totalSpirit;
        link_gamesPlayed = *pre_gamesPlayed;
        return key_root;
    }
    else
    {
        *pre_gamesPlayed += gamesPlayed_whenBought;
        return this;
    }
}

template<class K,class T>
K UnionFindNode<K,T>::FindWithSpirit(permutation_t *totalSpirit) {
    //permutation_t totalSpirit = permutation_t::neutral();
    int wow = 0;
    UnionFindNode<K,T>* root = this->FindRoot(totalSpirit,&wow);
    return root->master;
}

/*template<class K,class T>
void UnionFindNode<K,T>::FindSpiritLinks(permutation_t *totalSpirit)
{
    permutation_t sumSpirit = permutation_t::neutral();
    int gamesPlayed = 0;
    this->FindRoot(&sumSpirit,gamesPlayed);
    *totalSpirit = *totalSpirit * sumSpirit;
}*/

template<class K,class T>
int UnionFindNode<K,T>::FindGamesPlayed()
{
    int gamesToSub = 0;
    permutation_t totalSpirit = permutation_t::neutral();
    this->FindRoot(&totalSpirit,&gamesToSub);
    return gamesToSub;
}


template<class K,class T>
void UnionFindNode<K,T>::Unite(UnionFindNode<K,T>* x) {

    permutation_t totalSpirit1 = permutation_t::neutral();
    permutation_t totalSpirit2 = permutation_t::neutral();
    int gamesPlayed1 = 0;
    int gamesPlayed2 = 0;
    UnionFindNode<K,T>* x_Root = x->FindRoot(&totalSpirit1,&gamesPlayed1);
    UnionFindNode<K,T>* y_Root = this->FindRoot(&totalSpirit2,&gamesPlayed2);
    if(x_Root == y_Root)
        return;
//    if(y_Root->initNode)
//    {
//        x_Root->parent = nullptr;
//        x_Root->master = y_Root->master;
//        y_Root = x_Root;
//    }
    // FIXME: this lacks the needed logic for correct spirit/num_games calculations
    if (y_Root->size < x_Root->size) {
        y_Root->whenJoined = x_Root->size + 1;
        x_Root->size += y_Root->size;
        y_Root->parent = x_Root;

        x_Root->master = y_Root->master;
    }
    else
    {
        x_Root->whenJoined = y_Root->size + 1;
        y_Root->size += x_Root->size;
        x_Root->parent = y_Root;
    }
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

/*template<class K, class T>
K UnionFindNode<K,T>::getMaster() {
    return master;
}*/


#endif //MTM_HW2_UNIONFINDNODE_H
