#include <iostream>
#include "HashTable.h"

int main() {
    HashTable<int> ht;

    for (int i = 0; i < 1000; i++) {
        ht.insert(i, i);
    }
    ht.print();

    for(int i = 10; i < 1000; i+=11){
        ht.remove(i);
    }



    ht.print();
    ht.remove(0);
    ht.print();
    ht.insert(0, 1);
    ht.print();
////    ht.print();
//
//    HashTable<int> hot_stuff(ht);
//
//    for (int i = 0; i < 8; i++) {
//        ht.remove(i);
////        ht.print();
//    }
//
//    HashTable<int> very_hot_stuff = ht;
////    ht.print();
//
//    int* data = ht.search(0);
//
//    hot_stuff.print();
//    very_hot_stuff.print();


    return 0;
}
