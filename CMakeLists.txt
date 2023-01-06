cmake_minimum_required(VERSION 3.24)
project(MTM_HW2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hash_table_test main_ht.cpp HashTable.h)

add_executable(avl_tree_test main_avl.cpp AVLTree.h)

add_executable(MTM_HW2
        main23a2.cpp
        Player.h
        Team.h
        wet2util.h
        worldcup23a2.cpp
        worldcup23a2.h UnionFindNode.h)

add_executable(union_find_test main_uf.cpp HashTable.h UnionFindHillah.h main_uf.cpp)
