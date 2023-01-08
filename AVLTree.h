//
// Created by hilla on 11/27/2022.
//

#ifndef MTM_HW2_AVLTREE_H
#define MTM_HW2_AVLTREE_H

#include <cmath>
#include <iostream>


typedef enum {
    AVL_TREE_ALREADY_EXISTS, AVL_TREE_DOES_NOT_EXIST, AVL_TREE_SUCCESS, AVL_TREE_RANK_OUT_OF_RANGE
} AVLTreeResult;

template<class K, class T>
class Node {
public:
    int height;
    K key;
    T data;
    Node<K, T> *parent;
    Node<K, T> *left_son;
    Node<K, T> *right_son;
    int subtree_size;

    explicit Node(const K &key, const T &data) : height(0), key(key), data(data), parent(NULL), left_son(NULL),
                                                 right_son(NULL), subtree_size(1) {}

    Node() : height(0), parent(NULL), left_son(NULL),
             right_son(NULL), subtree_size(1) {}

    Node(const Node<K, T> &to_copy) = default;

    Node<K, T> &operator=(const Node<K, T> &node) = default;

    ~Node<K, T>() = default;

    void update_height_and_subtree_size();

    int balance_factor();

    static int get_subtree_size(Node *);
};

/*use this member function to update the height of a node. (has meaning in context of a tree)*/
template<class K, class T>
void Node<K, T>::update_height_and_subtree_size() {
    if (left_son != NULL && right_son != NULL) {
        int max = right_son->height > left_son->height ? right_son->height : left_son->height;
        height = max + 1;
    } else if (left_son != NULL) {
        height = left_son->height + 1;
    } else if (right_son != NULL) {
        height = right_son->height + 1;
    } else {
        height = 0;
    }

    subtree_size = 1 + get_subtree_size(left_son) + get_subtree_size(right_son);
}

/*use this function to calculate the balance factor of a node. (has meaning when the node is part of a tree)*/
template<class K, class T>
int Node<K, T>::balance_factor() {
    int bf;
    if (left_son != NULL && right_son != NULL) {
        bf = left_son->height - right_son->height;
    } else if (left_son != NULL) {
        bf = left_son->height - -1;
    } else if (right_son != NULL) {
        bf = -1 - right_son->height;
    } else {
        bf = 0;
    }

    return bf;
}

template<class K, class T>
int Node<K, T>::get_subtree_size(Node *node) {
    if (node == NULL) {
        return 0;
    } else {
        return node->subtree_size;
    }
}

/*Our generic AVL tree!*/
template<class K, class T>
class AVLTree {
private:
    Node<K, T> *dummy_root;
    int size;

    static Node<K, T> *copy_tree(Node<K, T> *root, Node<K, T> *new_parent);

    static void destroy_tree(Node<K, T> *root);

    static Node<K, T> *LL(Node<K, T> *v);

    static Node<K, T> *RR(Node<K, T> *v);

    static Node<K, T> *LR(Node<K, T> *c);

    static Node<K, T> *RL(Node<K, T> *c);

    static Node<K, T> *find_aux(Node<K, T> *root, const K &key);

    static Node<K, T> *find_last_node_in_search_path(Node<K, T> *root, const K &key);

    void balance_nodes_in_search_path(Node<K, T> *last_in_path); //we want access to root

    static Node<K, T> *remove_leaf(Node<K, T> *to_remove);

    static Node<K, T> *remove_with_one_child(Node<K, T> *to_remove);

    static Node<K, T> *remove_with_two_children(Node<K, T> *to_remove);

    static void print_inorder_with_bf_aux(Node<K, T> *root);

    static Node<K, T> *complete_tree(int height, Node<K, T> *parent);

    static bool is_power_of_two(int n);

    static int find_minimal_complete_tree_height(int size);

    static void make_almost_complete_tree(Node<K, T> *root, int *current_size_ptr, int final_size);

    static void populate_empty_tree(Node<K, T> *root, K key_array[], T data_array[], int *iptr);

    static void inorder_tree_to_array(Node<K, T> *root, K key_array[], T data_array[], int *iptr);

    static void inrange_order_tree_to_array(Node<K, T> *root, K key_array[], T *const data_array, int *iptr,
                                            K minRange, K maxRange, int *count);

    static void inorder_tree_data_to_array(Node<K, T> *root, T data_array[], int *iptr);

    static void
    merge_key_arrays_and_data_arrays(K key_array1[], K key_array2[], T data_array1[], T data_array2[], int size1,
                                     int size2,
                                     K merged_key_array[], T merged_data_array[]);

    Node<K, T> *root() const {
        return dummy_root->left_son;
    }

public:
    AVLTree() : dummy_root(new Node<K, T>()), size(0) {}

    AVLTree(K sorted_key_array[], T sorted_data_array[], int size);

    AVLTree(const AVLTree<K, T> &);

    ~AVLTree();

    AVLTree<K, T> &operator=(const AVLTree<K, T> &);

    AVLTreeResult insert(const K &key, const T &data);

    AVLTreeResult remove(const K &key);

    AVLTreeResult find(const K &key, T *found_data);

    bool contains(const K &key);

    void print_inorder_with_bf();

    int get_size() const {
        return this->size;
    }

    void build_from_two_merged_trees(const AVLTree<K, T> &tree1, const AVLTree<K, T> &tree2);

    K *get_prev_inorder(const K &);

    K *get_next_inorder(const K &);

    void to_sorted_keys_and_data(K key_array[], T data_array[]) const;

    void to_sorted_ranged_keys_and_data(K key_array[], T data_array[], K minRange, K maxRange, int *count) const;

    AVLTreeResult get_ith_ranked_element(int rank, T *data);
};

/*helper function that copies a tree and returns pointer to the copied root*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::copy_tree(Node<K, T> *root, Node<K, T> *new_parent) {
    if (root == NULL) {
        return NULL;
    }
    Node<K, T> *new_root = new Node<K, T>(*root);
    new_root->parent = new_parent;
    new_root->left_son = copy_tree(root->left_son, new_root);
    new_root->right_son = copy_tree(root->right_son, new_root);

    return new_root;
}

/*helper function that destroys a tree post order*/
template<class K, class T>
void AVLTree<K, T>::destroy_tree(Node<K, T> *root) {
    if (root == NULL) {
        return;
    }
    destroy_tree(root->left_son);
    destroy_tree(root->right_son);
    delete root;
}

/*copy c'tor*/
template<class K, class T>
AVLTree<K, T>::AVLTree(const AVLTree<K, T> &to_copy) :
        dummy_root(new Node<K, T>()), size(to_copy.size) {
    this->dummy_root->left_son = copy_tree(to_copy.root(), dummy_root);
}

template<class K, class T>
AVLTree<K, T> &AVLTree<K, T>::operator=(const AVLTree<K, T> &tree) {
    if (this == &tree) {
        return *this;
    }
    destroy_tree(this->root()); //destroy the tree that begins at this->root()
    this->dummy_root->left_son = copy_tree(tree.root(), dummy_root);
    size = tree.size;

    return *this;
}

/*d'tor*/
template<class K, class T>
AVLTree<K, T>::~AVLTree<K, T>() {
    destroy_tree(dummy_root);
}

/*helper function - LL rotation*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::LL(Node<K, T> *v) {
    Node<K, T> *v_l = v->left_son;
    v->left_son = v_l->right_son;
    v_l->right_son = v;

    v->update_height_and_subtree_size(); //v is now the son, so update first
    v_l->update_height_and_subtree_size();

    v_l->parent = v->parent;
    v->parent = v_l;
    if (v->left_son) {
        v->left_son->parent = v;
    }

    if (v_l->parent->left_son == v) {
        v_l->parent->left_son = v_l;
    } else {
        v_l->parent->right_son = v_l;
    }

    return v_l;
}

/*helper function - RR rotation*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::RR(Node<K, T> *v) {
    Node<K, T> *v_r = v->right_son;
    v->right_son = v_r->left_son;
    v_r->left_son = v;

    v->update_height_and_subtree_size(); //update v first since now he is the son
    v_r->update_height_and_subtree_size();

    v_r->parent = v->parent;
    v->parent = v_r;
    if (v->right_son) {
        v->right_son->parent = v;
    }

    if (v_r->parent->left_son == v) {
        v_r->parent->left_son = v_r;
    } else {
        v_r->parent->right_son = v_r;
    }

    return v_r;
}

/*helper function - LR rotation*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::LR(Node<K, T> *c) {
    RR(c->left_son);
    return LL(c);
}

/*helper function - RL rotation*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::RL(Node<K, T> *c) {
    LL(c->right_son);
    return RR(c);
}

/*helper function - finds node that contains specific key and returns a pointer to that node*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::find_aux(Node<K, T> *root, const K &key) {
    if (root == NULL) {
        return NULL;
    }

    if (root->key == key) {
        return root;
    } else if (root->key > key) {
        return find_aux(root->left_son, key);
    } else {
        return find_aux(root->right_son, key);
    }
}

/*will return a pointer to the leaf which we need to attach next element to*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::find_last_node_in_search_path(Node<K, T> *root, const K &key) {

    if (root == NULL) {
        return NULL;
    }

    if (root->key == key) {
        return root; //if the key exists then the last node in the search path is the node containing said key
    }

    if (root->key > key) {
        if (root->left_son == NULL) {
            return root;
        }
        return find_last_node_in_search_path(root->left_son, key);
    } else {
        if (root->right_son == NULL) {
            return root;
        }
        return find_last_node_in_search_path(root->right_son, key);
    }
}

/*finds key in the tree, and puts the matching data inside the ptr found_data*/
template<class K, class T>
AVLTreeResult AVLTree<K, T>::find(const K &key, T *found_data) {
    Node<K, T> *found_node = find_aux(this->root(), key);
    if (found_node == NULL) {
        return AVL_TREE_DOES_NOT_EXIST;
    }

    *found_data = found_node->data;
    return AVL_TREE_SUCCESS;
}


template<class K, class T>
bool AVLTree<K, T>::contains(const K &key) {
    Node<K, T> *found_node = find_aux(this->root(), key);
    return found_node != NULL;
}

/*helper function who's purpose is to balance our tree from the bottom up starting from the parent of the last inserted or removed node*/
template<class K, class T>
void AVLTree<K, T>::balance_nodes_in_search_path(Node<K, T> *last_in_path) {
    //last_node = last node in search path (not the inserted/ removed node)
    if (last_in_path == NULL) {
        return;
    }

    last_in_path->update_height_and_subtree_size();

    if (last_in_path->balance_factor() == 2) {
        if (last_in_path->left_son->balance_factor() >= 0) {
            last_in_path = LL(last_in_path);
        } else {
            last_in_path = LR(last_in_path);
        }
    }
    if (last_in_path->balance_factor() == -2) {
        if (last_in_path->right_son->balance_factor() == 1) {
            last_in_path = RL(last_in_path);
        } else {
            last_in_path = RR(last_in_path);
        }
    }

    if (last_in_path == this->root()) {
        return;
    }

    /*
    if (last_in_path->parent->height >= last_in_path->height + 1) {
        return;
    }*/

    balance_nodes_in_search_path(last_in_path->parent);
}

/*classic insert member function for an AVL tree. takes key and data to be inserted*/
template<class K, class T>
AVLTreeResult AVLTree<K, T>::insert(const K &key, const T &data) {
//check whether key already exists in the tree
    if (find_aux(this->root(), key) != NULL) {
        return AVL_TREE_ALREADY_EXISTS;
    }

    this->size++;

    Node<K, T> *parent_of_new_node = find_last_node_in_search_path(this->root(), key);

    Node<K, T> *new_node = new Node<K, T>(key, data);

    if (parent_of_new_node == NULL) { //epmty tree
        this->dummy_root->left_son = new_node;
        new_node->parent = this->dummy_root;
    } else {
        if (parent_of_new_node->key > key) {
            parent_of_new_node->left_son = new_node;
        } else {
            parent_of_new_node->right_son = new_node;
        }

        new_node->parent = parent_of_new_node;

        this->balance_nodes_in_search_path(parent_of_new_node);
    }

    return AVL_TREE_SUCCESS;
}

/*helper function that removes a leaf and returns a pointer to parent of removed node*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::remove_leaf(Node<K, T> *to_remove) {
    Node<K, T> *parent = to_remove->parent;

    if (parent->left_son == to_remove) {
        parent->left_son = NULL;
    } else {
        parent->right_son = NULL;
    }

    delete to_remove;
    return parent;
}

/*helper function that removes node with one child. returns ptr to parent of the removed*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::remove_with_one_child(Node<K, T> *to_remove) {
    Node<K, T> *parent = to_remove->parent;
    Node<K, T> *child = to_remove->left_son != NULL ? to_remove->left_son : to_remove->right_son;

    if (parent->left_son == to_remove) {
        parent->left_son = child;
    } else {
        parent->right_son = child;
    }
    child->parent = parent;

    delete to_remove;
    return parent;
}

/*helper function that removes node with two children. returns ptr to parent of the removed*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::remove_with_two_children(Node<K, T> *to_remove) {
    Node<K, T> *next_inorder = to_remove->right_son; //no need to check for null since node for sure has two sons
    while (next_inorder->left_son != NULL) {
        next_inorder = next_inorder->left_son;
    }

    to_remove->key = next_inorder->key;
    to_remove->data = next_inorder->data;

    if (next_inorder->right_son != NULL) {
        return remove_with_one_child(next_inorder); //returning the parent of the node we actually
        // remove, so that we can make necessary updates and rotations upward
    } else {
        return remove_leaf(next_inorder);
    }
}

/*removes element from tree given key*/
template<class K, class T>
AVLTreeResult AVLTree<K, T>::remove(const K &key) {
//find the node to remove
    Node<K, T> *node_to_remove = find_aux(this->root(), key);
    if (node_to_remove == NULL) {
        return AVL_TREE_DOES_NOT_EXIST;
    }

    Node<K, T> *parent_of_removed;
    if (node_to_remove->left_son == NULL && node_to_remove->right_son == NULL) {
        parent_of_removed = remove_leaf(node_to_remove);
    } else if (node_to_remove->left_son != NULL && node_to_remove->right_son != NULL) {
        parent_of_removed = remove_with_two_children(node_to_remove);
    } else {
        parent_of_removed = remove_with_one_child(node_to_remove);
    }

    this->size--;

    if (parent_of_removed != this->dummy_root) {
        this->balance_nodes_in_search_path(parent_of_removed);
    }

    return AVL_TREE_SUCCESS;
}

/*recursive helper function for print_in_order_with_bf*/
template<class K, class T>
void AVLTree<K, T>::print_inorder_with_bf_aux(Node<K, T> *root) {
    if (root == NULL) {
        return;
    }
    print_inorder_with_bf_aux(root->left_son);
    std::cout << "K:" << root->key << " h:" << root->height << " bf:" << root->balance_factor() << " sts:"
              << root->subtree_size << std::endl;
    print_inorder_with_bf_aux(root->right_son);
}

/*prints tree keys inorder with balance factors*/
template<class K, class T>
void AVLTree<K, T>::print_inorder_with_bf() {
    std::cout << "Now printing tree in order " << std::endl;
    print_inorder_with_bf_aux(this->root());
    std::cout << std::endl;
}

/*helper function that creates a complete tree*/
template<class K, class T>
Node<K, T> *AVLTree<K, T>::complete_tree(int height, Node<K, T> *parent) {
    if (height < 0) {
        return NULL;
    }

    Node<K, T> *new_node = new Node<K, T>();
    new_node->left_son = complete_tree(height - 1, new_node);
    new_node->right_son = complete_tree(height - 1, new_node);
    new_node->parent = parent;

    new_node->update_height_and_subtree_size();

    return new_node;
}

/*helper function that checks if a number is a power of 2*/
template<class K, class T>
bool AVLTree<K, T>::is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

/*helper function that finds the smallest height of complete tree with number of nodes as close as possible to size*/
template<class K, class T>
int AVLTree<K, T>::find_minimal_complete_tree_height(int size) {
    do {
        size++;
    } while (!is_power_of_two(size));

    return log2(size) - 1;
}

/*helper function that turns a complete tree to an almost complete tree*/
template<class K, class T>
void AVLTree<K, T>::make_almost_complete_tree(Node<K, T> *root, int *current_size_ptr, int final_size) {
    //root is of complete tree, and we are erasing elements from right to left
    if (root == NULL || *current_size_ptr == final_size) {
        return;
    }

    make_almost_complete_tree(root->right_son, current_size_ptr, final_size);

    if (root->left_son == NULL && root->right_son == NULL) {
        if (root->parent->right_son == root) {
            root->parent->right_son = NULL;
        } else {
            root->parent->left_son = NULL;
        }
        root->parent->update_height_and_subtree_size();

        delete root;
        (*current_size_ptr)--;
    } else {
        make_almost_complete_tree(root->left_son, current_size_ptr, final_size);
    }
}

/*helper function that populates a tree that is effectively empty (no intentional data has yet been placed inside)*/
template<class K, class T>
void AVLTree<K, T>::populate_empty_tree(Node<K, T> *root, K key_array[], T data_array[], int *iptr) {
    if (root == NULL) {
        return;
    }

    populate_empty_tree(root->left_son, key_array, data_array, iptr);

    root->key = key_array[*iptr];
    root->data = data_array[*iptr];
    (*iptr)++;

    populate_empty_tree(root->right_son, key_array, data_array, iptr);
}

/*c'tor for a tree that uses sorted key array and sorted data array to initialize new tree*/
template<class K, class T>
// this c'tor will help us in unite teams, where will build a united tree out of a sorted array of the union of players from both teams
AVLTree<K, T>::AVLTree(K sorted_key_array[], T sorted_data_array[], int size) : dummy_root(new Node<K, T>()), size(0) {
    int height = find_minimal_complete_tree_height(size);
    dummy_root->left_son = complete_tree(height, dummy_root);
    int current_size = pow(2, height + 1) - 1;
    make_almost_complete_tree(this->root(), &current_size, size);

    this->size = size;

    int i = 0;
    populate_empty_tree(this->root(), sorted_key_array, sorted_data_array, &i);
}

/*helper function that traverses a tree inorder and puts keys and data into array (both inorder according to key) within
 * desired range.*/
template<class K, class T>
void AVLTree<K, T>::inrange_order_tree_to_array(Node<K, T> *root, K key_array[], T *const data_array, int *iptr,
                                                K minRange, K maxRange, int *count) {
    if (root == NULL) {
        return;
    }
    if (root->key < minRange) {
        if (root->right_son == NULL) {
            return;
        } else {
            inrange_order_tree_to_array(root->right_son, key_array, data_array, iptr, minRange, maxRange, count);
        }

    } else if (root->key > maxRange) {
        if (root->left_son == NULL) {
            return;
        } else {
            inrange_order_tree_to_array(root->left_son, key_array, data_array, iptr, minRange, maxRange, count);
        }
    } else if (root->key >= minRange && root->key <= maxRange) {
        inrange_order_tree_to_array(root->left_son, key_array, data_array, iptr, minRange, maxRange, count);
        if (key_array != nullptr) {
            key_array[*iptr] = root->key;
        }
        (*count)++;
        data_array[*iptr] = root->data;
        (*iptr)++;
        inrange_order_tree_to_array(root->right_son, key_array, data_array, iptr, minRange, maxRange, count);
    }


}

/*helper function that traverses a tree inorder and puts keys and data into array (both inorder according to key)*/
template<class K, class T>
void AVLTree<K, T>::inorder_tree_to_array(Node<K, T> *root, K key_array[], T *const data_array, int *iptr) {
    if (root == NULL) {
        return;
    }

    inorder_tree_to_array(root->left_son, key_array, data_array, iptr);
    if (key_array != nullptr) {
        key_array[*iptr] = root->key;
    }
    data_array[*iptr] = root->data;
    (*iptr)++;

    inorder_tree_to_array(root->right_son, key_array, data_array, iptr);
}

/*helper function that traverses a tree inorder and puts keys and data into array (both inorder according to key)*/
template<typename K, typename T>
void AVLTree<K, T>::inorder_tree_data_to_array(Node<K, T> *root, T *const data_array, int *iptr) {
    if (root == NULL) {
        return;
    }

    inorder_tree_data_to_array(root->left_son, data_array, iptr);
    data_array[*iptr] = root->data;
    (*iptr)++;

    inorder_tree_data_to_array(root->right_son, data_array, iptr);
}

//copies tree data and keys into respective arrays
template<class K, class T>
void AVLTree<K, T>::to_sorted_keys_and_data(K key_array[], T *const data_array) const {
    int i = 0;
    if (key_array != nullptr) {
        inorder_tree_to_array(this->root(), key_array, data_array, &i);
    } else {
        inorder_tree_data_to_array(this->root(), data_array, &i);
    }
}

//copies tree data and keys into respective arrays considering they are in the desired range
template<class K, class T>
void AVLTree<K, T>::to_sorted_ranged_keys_and_data(K key_array[], T *const data_array, K minRange, K maxRange,
                                                   int *count) const {
    int i = 0;
    if (key_array != nullptr) {
        inrange_order_tree_to_array(this->root(), key_array, data_array, &i, minRange, maxRange, count);
    }
}

/*helper function*/
//takes two sorted data arrays and merges them into a single sorted data array
//takes two sorted key arrays and merges them into a single sorted key array
template<class K, class T>
void
AVLTree<K, T>::merge_key_arrays_and_data_arrays(K key_array1[], K key_array2[], T data_array1[], T data_array2[],
                                                int size1, int size2,
                                                K merged_key_array[], T merged_data_array[]) {
    int i1 = 0;
    int i2 = 0;
    int i3 = 0;

    while (i1 < size1 && i2 < size2) {
        if (key_array1[i1] < key_array2[i2]) {
            merged_key_array[i3] = key_array1[i1];
            merged_data_array[i3] = data_array1[i1];
            i1++;
        } else {
            merged_key_array[i3] = key_array2[i2];
            merged_data_array[i3] = data_array2[i2];
            i2++;
        }
        i3++;
    }
    while (i1 < size1) {
        merged_key_array[i3] = key_array1[i1];
        merged_data_array[i3] = data_array1[i1];
        i1++;
        i3++;
    }
    while (i2 < size2) {
        merged_key_array[i3] = key_array2[i2];
        merged_data_array[i3] = data_array2[i2];
        i2++;
        i3++;
    }
}

/*takes two trees and merges them into a new one using the O(size of merged tree) algorithm from class*/
template<class K, class T>
void AVLTree<K, T>::build_from_two_merged_trees(const AVLTree<K, T> &tree1, const AVLTree<K, T> &tree2) {
    //put trees 1 and 2 into arrays
    K *key_array1 = new K[tree1.size];
    T *data_array1 = new T[tree1.size];

    K *key_array2 = new K[tree2.size];
    T *data_array2 = new T[tree2.size];

    tree1.to_sorted_keys_and_data(key_array1, data_array1);
    tree2.to_sorted_keys_and_data(key_array2, data_array2);

    //merge the arrays

    K *merged_key_array = new K[tree1.size + tree2.size];
    T *merged_data_array = new T[tree1.size + tree2.size];

    merge_key_arrays_and_data_arrays(key_array1, key_array2, data_array1, data_array2, tree1.size, tree2.size,
                                     merged_key_array, merged_data_array);

    //call c'tor that takes an array and build a tree
    //uses actual operator=()
    *this = AVLTree<K, T>(merged_key_array, merged_data_array, tree1.size + tree2.size);

    //return that tree
    delete[] key_array1;
    delete[] data_array1;
    delete[] key_array2;
    delete[] data_array2;
    delete[] merged_key_array;
    delete[] merged_data_array;
}

/*gets previous element in tree according to key inorder arrangement. if the key is the smallest in the tree NULL is returned*/
template<class K, class T>
K *AVLTree<K, T>::get_prev_inorder(const K &key) {
    if (this->size < 2) {
        return NULL;
    }
    Node<K, T> *found_node = find_aux(this->root(), key);
    if (found_node == NULL) {
        return NULL;
    }

    //if found_node has a left son
    if (found_node->left_son != NULL) {
        found_node = found_node->left_son;
        while (found_node->right_son != NULL) {
            found_node = found_node->right_son;
        }
        return &found_node->key;
    } else {//found_node has no left son. So the only place we might find someone smaller is above found node.
        //so we'll keep going upward until we turn left for the first time

        while (found_node != this->root()) {
            if (found_node == found_node->parent->right_son) {
                return &found_node->parent->key;
            }
            found_node = found_node->parent;
        }
    }

    //if we are here we didnt find anyone smaller above us or below us, therefore we are the smallest
    return NULL;
}

/*gets next element in tree according to key inorder arrangement. if the key is the greatest in the tree NULL is returned*/
template<class K, class T>
K *AVLTree<K, T>::get_next_inorder(const K &key) {
    //if k has right son then we go right once then left all the way
    //if k has no right son but has parent that is bigger than k (meaning k is a left son of parent)
    // then next in order is parent
    //if k has no right son AND  k has no parent that is bigger than k (meaning k is right son of parent or k is the root)
    // then next in order is NULL.
    if (this->size < 2) {
        return NULL;
    }
    Node<K, T> *found_node = find_aux(this->root(), key);
    if (found_node == NULL) {
        return NULL;
    }

    if (found_node->right_son != NULL) {
        found_node = found_node->right_son;
        while (found_node->left_son != NULL) {
            found_node = found_node->left_son;
        }
        return &found_node->key;
    } else {
        //we want to find the first ancestor that is greater than key
        while (found_node != this->root()) {
            if (found_node == found_node->parent->left_son) {
                return &found_node->parent->key;
            }
            found_node = found_node->parent;
        }
    }
    //if we are here we didn't find an ancestor that is greater, and we have no descendants that are greater
    //therefore we are the greatest so there is no next

    return NULL;
}


template<class K, class T>
AVLTreeResult AVLTree<K, T>::get_ith_ranked_element(int rank, T *data) {
    if (Node<K, T>::get_subtree_size(root()) < rank || rank < 1) {
        return AVL_TREE_RANK_OUT_OF_RANGE;
    }

    Node<K, T> *current = root();
    int r = 1 + Node<K, T>::get_subtree_size(current->left_son);

    while (r != rank) {
        if (rank > r) {
            current = current->right_son;
            r = r + Node<K, T>::get_subtree_size(current->left_son) + 1;
        } else if (rank < r) {
            current = current->left_son;
            r = r - 1 - Node<K, T>::get_subtree_size(current->right_son);
        }
    }
    *data = current->data;
    return AVL_TREE_SUCCESS;
}


#endif //MTM_HW2_AVLTREE_H
