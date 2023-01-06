cmake_minimum_required(VERSION 3.24)
project(wet2mivne)

set(CMAKE_CXX_STANDARD 11)

add_executable(hash_table_test main_ht.cpp HashTable.h)

add_executable(avl_tree_test main_avl.cpp AVLTree.h)

add_executable(union_find_test main_uf.cpp HashTable.h UnionFindHillah.h main_uf.cpp)

add_executable(world_cup_test main23a2.cpp HashTable.h UnionFindHillah.h AVLTree.h Player.h Team.h worldcup23a2.h worldcup23a2.cpp)