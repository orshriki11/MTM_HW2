#ifndef MTM_HW2_UNIONFIND_H
#define MTM_HW2_UNIONFIND_H

template<class E>
class UpTreeNode {
public:
    E element;
    UpTreeNode<E> *parent;  // parent in the up tree

    explicit UpTreeNode(E element) : element(element), parent(nullptr) {}
};

template<class G, class E>
class GroupNode;

template<class G, class E>
class UpTree{
private:
    UpTreeNode<E>* root;
    GroupNode<G, E>* group_node;
    int size;
};

template<class G, class E>
class GroupNode{
public: 
    G group;
    UpTree<G, E>* group_tree;
};

template<class K, class E>
class UnionFindHillah {
private:
    int size;
    K *Master;
    UpTreeNode<E> *root;

public:
    UnionFindHillah<K, E>() : size(0), Master(nullptr), root(nullptr) {}

    UnionFindHillah<K, E>(K *master) : size(0), Master(master), root(nullptr) {}

    ~UnionFindHillah<K, E>() = default;

    void MakeSet(E data);

    void insert(UpTreeNode<E> *node);

    K *Find(UpTreeNode<E> *key);

    UpTreeNode<E> *FindRoot(UpTreeNode<E> *key);

    void Unite(UpTreeNode<E> *x, UpTreeNode<E> *y);
};

//TODO: Check viability of function.

template<class E>
void UnionFindHillah<K, E>::insert(UpTreeNode<E> *node) {
    if (root != nullptr) {
        root = node;
    } else {
        node->parent = root;
    }

}

//TODO: Check viability of function.
template<class E>
void UnionFindHillah<K, E>::MakeSet(E data) {
    UpTreeNode<E> *new_node = new UpTreeNode<E>(data);
    if (root != nullptr) {
        root = new_node->parent;
    } else {
        new_node->parent = root;
    }

}

template<class T, Class K>
K *UnionFindHillah<K, E>::Find(UpTreeNode<E> *key) {
    UpTreeNode<E> *root = findRoot(key);
    return root.UF;
}

template<class E>
UpTreeNode<E> *UnionFindHillah<K, E>::FindRoot(UpTreeNode<E> *key) {
    if (key->parent != nullptr) {
        UpTreeNode<E> *key_root = findRoot(key->parent);
        key->parent = key_root;
        return key_root;
    } else {
        return key;
    }
}

template<class T, Class K>
void UnionFindHillah<K, E>::Unite(UpTreeNode<E> *x, UpTreeNode<E> *y) {
    UpTreeNode<E> *x_Root = findRoot(x);
    UpTreeNode<E> *y_Root = findRoot(y);
    if (x_Root = y_Root)
        return;
    if (x_Root.size >= y_Root.size) {
        y_Root.parent = x_Root;
    } else {
        x_Root.parent = y_Root;
    }
}


#endif //MTM_HW2_UNIONFIND_H
