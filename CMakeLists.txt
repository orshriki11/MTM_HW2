cmake_minimum_required(VERSION 3.24)
project(wet2mivne)

set(CMAKE_CXX_STANDARD 11)

add_executable(hash_table_test main_ht.cpp HashTable.h)

add_executable(avl_tree_test main_avl.cpp AVLTree.h)