
#include <iostream>
#include "AVLTree.h"

int main(){
    AVLTree<int, int> tree;
    for(int i = 1; i < 1000; i++){
        tree.insert(i, i);
    }
    int* data;
    AVLTreeResult res = tree.get_ith_ranked_element(248, data);

    std::cout << "data: " << *data << std::endl;
    std::cout << "res: " << res << std::endl;

    return 0;
}