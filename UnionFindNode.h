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
    permutation_t spirit_whenBought;
    int link_gamesPlayed;
    int whenJoined;
    int gamesPlayed_whenBought;

    UnionFindNode<K,T>* parent;  // parent in the up tree

    explicit UnionFindNode(T Data);

    UnionFindNode();

    UnionFindNode(const UnionFindNode<K,T> &node);

    UnionFindNode<K,T> &operator=(const UnionFindNode<K,T> &node);

    ~UnionFindNode() = default;

    void MakeSet(T data);

    void insert(UnionFindNode<K,T>* node);

    UnionFindNode<K,T>* FindRoot(permutation_t *totalSpirit,int *pre_gamesPlayed,
                                 permutation_t *root_spirit,int *root_gamesPlayed);

    K Find();

    UnionFindNode<K,T>* FindRootOnly();

    void Unite(UnionFindNode<K,T>* x);

    K FindWithSpirit(permutation_t *totalSpirit,permutation_t *rootSpirit);

    int FindGamesPlayed();


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
              spirit_whenBought = permutation_t::neutral();
          }

template<class K,class T>
UnionFindNode<K,T>::UnionFindNode() : size(1),
                                    data(T()),
                                    master(nullptr),
                                    link_gamesPlayed(0),
                                    whenJoined(0),
                                    gamesPlayed_whenBought(0),
                                    parent(nullptr)
{
    linkSpirit = permutation_t::neutral();
    spirit_whenBought = permutation_t::neutral();
}



template<class K,class T>
UnionFindNode<K,T>::UnionFindNode(const UnionFindNode<K,T> &node)
{
    this->size = node.size;
    this->parent = node.parent;
    this->master = node.master;
    this->whenJoined = node.whenJoined;
    this->gamesPlayed_whenBought = node.gamesPlayed_whenBought;
    this->spirit_whenBought = node.spirit_whenBought;
    this->link_gamesPlayed = node.link_gamesPlayed;
    this->linkSpirit = node.linkSpirit;
    this->data = node.data;
}

template<class K,class T>
UnionFindNode<K,T> &UnionFindNode<K,T>::operator=(const UnionFindNode<K,T> &node)
{
    this->size = node.size;
    this->parent = node.parent;
    this->master = node.master;
    this->whenJoined = node.whenJoined;
    this->gamesPlayed_whenBought = node.gamesPlayed_whenBought;
    this->spirit_whenBought = node.spirit_whenBought;
    this->link_gamesPlayed = node.link_gamesPlayed;
    this->linkSpirit = node.linkSpirit;
    this->data = node.data;
    return *this;
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


template<class K,class T>
K UnionFindNode<K,T>::Find() {
    permutation_t totalSpirit = permutation_t::neutral();
    permutation_t rootSpirit = permutation_t::neutral();
    int gamesPlayed = 0;
    int root_gamesPlayed = 0;
    UnionFindNode<K,T>* root = this->FindRoot(&totalSpirit,&gamesPlayed,&rootSpirit,&root_gamesPlayed);
    return root->master;
}

template<class K,class T>
UnionFindNode<K,T>* UnionFindNode<K,T>::FindRoot(permutation_t *totalSpirit, int *pre_gamesPlayed,
                                                 permutation_t *root_spirit, int *root_gamesPlayed) {
    if(parent != nullptr)
    {
        UnionFindNode<K,T>* key_root = parent->FindRoot(totalSpirit,pre_gamesPlayed,root_spirit,root_gamesPlayed);
        permutation_t forLink = linkSpirit;
        linkSpirit = *totalSpirit * linkSpirit;
        *totalSpirit = (*totalSpirit) * (forLink * spirit_whenBought);

        *pre_gamesPlayed -= link_gamesPlayed;
        parent = key_root;
        //linkSpirit = *totalSpirit;
        link_gamesPlayed = -(*pre_gamesPlayed);
        *pre_gamesPlayed += gamesPlayed_whenBought;
        return key_root;
    }
    else
    {

        *root_gamesPlayed = gamesPlayed_whenBought;
        *root_spirit = spirit_whenBought;
        return this;
    }
}

template<class K,class T>
K UnionFindNode<K,T>::FindWithSpirit(permutation_t *totalSpirit,permutation_t *rootSpirit) {
    //permutation_t totalSpirit = permutation_t::neutral();
    int gamesPlayed = 0;
    int root_gamesPlayed = 0;
    UnionFindNode<K,T>* root = this->FindRoot(totalSpirit,&gamesPlayed, rootSpirit,&root_gamesPlayed);
    //totalSpirit = (*totalSpirit);
    return root->master;
}


template<class K,class T>
int UnionFindNode<K,T>::FindGamesPlayed()
{
    int gamesToSub = 0;
    int root_gamesPlayed = 0;
    permutation_t totalSpirit = permutation_t::neutral();
    permutation_t rootSpirit = permutation_t::neutral();
    this->FindRoot(&totalSpirit,&gamesToSub,&rootSpirit,&root_gamesPlayed);
    return gamesToSub + root_gamesPlayed;
}


template<class K,class T>
void UnionFindNode<K,T>::Unite(UnionFindNode<K,T>* x) {

    permutation_t totalSpirit1 = permutation_t::neutral();
    permutation_t totalSpirit2 = permutation_t::neutral();
    permutation_t rootSpirit1 = permutation_t::neutral();
    permutation_t rootSpirit2 = permutation_t::neutral();
    int gamesPlayed1 = 0;
    int gamesPlayed2 = 0;
    int rootGamesPlayed1 = 0;
    int rootGamesPlayed2 = 0;

    UnionFindNode<K,T>* x_Root = x->FindRoot(&totalSpirit1,&gamesPlayed1,
                                             &rootSpirit1,&rootGamesPlayed1);
    UnionFindNode<K,T>* y_Root = this->FindRoot(&totalSpirit2,&gamesPlayed2,
                                                &rootSpirit2,&rootGamesPlayed2);
    if(x_Root == y_Root)
        return;
//    if(y_Root->initNode)
//    {
//        x_Root->parent = nullptr;
//        x_Root->master = y_Root->master;
//        y_Root = x_Root;
//    }

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



#endif //MTM_HW2_UNIONFINDNODE_H
