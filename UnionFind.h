#ifndef MTM_HW2_UNIONFIND_H
#define MTM_HW2_UNIONFIND_H

template<class T>
struct UpTreeNode {
    int size;  // size of the up tree
    T* data;
    //bool isRoot;
    UpTreeNode<T>* parent;  // parent in the up tree
    UnionFind* UF;

    UpTreeNode(T* Data);

    UpTreeNode();

    ~UpTreeNode() = default;
};

template<class T>
UpTreeNode<T>::UpTreeNode(T* Data) : data(Data), isRoot(false), parent(nullptr), size(1), UF(nullptr) {}

template<class T>
UpTreeNode<T>::UpTreeNode() : data(T()), isRoot(false), parent(nullptr), size(1), UF(nullptr) {}

/*template<class T>
UpTreeNode<T>::~UpTreeNode(){
    free(parent);
}*/






template<class K, class T>
class UnionFind
{
private:
    int size;
    K* Master;
    UpTreeNode<T>* root;

public:
    UnionFind<K,T>() : size(0), Master(nullptr), root(nullptr){}

    UnionFind<K,T>(K* master) : size(0), Master(master), root(nullptr){}

    ~UnionFind<K,T>() = default;

    void MakeSet(T data);

    void insert(UpTreeNode<T>* node);

    K* Find(UpTreeNode<T>* key);

    UpTreeNode<T>* FindRoot(UpTreeNode<T>* key);

    void Unite(UpTreeNode<T>* x, UpTreeNode<T>* y);
};

//TODO: Check viability of function.

template<class T>
void UnionFind<K,T>::insert(UpTreeNode<T>* node) {
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

//TODO: Check viability of function.
template<class T>
void UnionFind<K,T>::MakeSet(T data) {
    UpTreeNode<T> *new_node = new UpTreeNode<T>(data);
    if(root != nullptr)
    {
        root = new_node->parent;
    }
    else
    {
        new_node->parent = root;
    }

}

template<class T, Class K>
K* UnionFind<K,T>::Find(UpTreeNode<T>* key) {
    UpTreeNode<T>* root = findRoot(key);
    return root.UF;
}

template<class T>
UpTreeNode<T>* UnionFind<K,T>::FindRoot(UpTreeNode<T>* key) {
    if(key->parent != nullptr)
    {
        UpTreeNode<T>* key_root = findRoot(key->parent);
        key->parent = key_root;
        return key_root;
    }
    else
    {
        return key;
    }
}

template<class T, Class K>
void UnionFind<K,T>::Unite(UpTreeNode<T>* x, UpTreeNode<T>* y) {
    UpTreeNode<T>* x_Root = findRoot(x);
    UpTreeNode<T>* y_Root = findRoot(y);
    if(x_Root = y_Root)
        return;
    if(x_Root.size >= y_Root.size)
    {
        y_Root.parent = x_Root;
    }
    else
    {
        x_Root.parent = y_Root;
    }
}



#endif //MTM_HW2_UNIONFIND_H
