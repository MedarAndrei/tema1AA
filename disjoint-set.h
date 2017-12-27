//
// Created by andrei on 11.12.2017.
//

#ifndef UNION_FIND_DISJOINT_SET_H
#define UNION_FIND_DISJOINT_SET_H

#include <vector>
#include <iostream>
using namespace std;

template<typename T> struct setEntry{
  int index;
  int parent;
  int rank;
  T info;
  setEntry(T x, int idx){
    index = parent = idx;
    rank = 1;
    info = x;
  }
};

template <typename T> struct disjointSet{
public:
  vector<setEntry<T>> v; // elementele memorate
  int size;

  disjointSet(){
    size = 0;
  }

  void add(T x){
    v.emplace_back(setEntry<T>(x, size));
    size ++;
  }

  void show(){
    for(int i = 0 ; i < size; i++)
      cout << i << " -> " << find(i) << "\n";
  }

  int parent(int x){
    return v[x].parent;
  }

  int rank(int x){
    return v[x].rank;
  }

  int find(int a){
    int b = a;

    while(parent(b) != b)
      b = parent(b);
    // acum la b este parintele
    // le parcurg pe celelalte prin care am trecut pentru a face compresia drumurilor

    int aux;
    int c = a;
    while(parent(c) != c){
      aux = parent(c);
      v[c].parent = b;
      c = aux;
    }

    return b;
  }

  void uni(int a, int b){
    int x = find(a);
    int y = find(b);
    if(x == y) // sunt deja unite
      return;
    if(rank(x) < rank(y))
      v[x].parent = y;
    else{
      if(rank(x) > rank(y))
        v[y].parent = x;
      else { // sunt egale
        v[x].parent = y;
        v[y].rank = v[x].rank + 1;
      }
    }
  }

  bool does_belong(int a, int b){
    // verifica daca a si b apartin aceleasi multimi;
    return find(a) == find(b);
  }

};


#endif //UNION_FIND_DISJOINT_SET_H
