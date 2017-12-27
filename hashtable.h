//
// Created by andrei on 10.12.2017.
//

#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <climits>
#include <iostream>
using namespace std;

#define TRESHOLD 0.75f // pt resize

template<typename Tkey, typename Tvalue>
struct entry {
  entry(Tkey key, Tvalue value) : key(key), value(value) {}

  Tkey key;
  Tvalue value;

};

template<typename Tkey, typename Tvalue>
class Hashtable{
private:

  std::list<entry<Tkey, Tvalue>> *H;
  unsigned int capacity;
  unsigned int size;
  unsigned int (*hash) (const Tkey &);
  std::hash<Tkey> func;

public:
  Hashtable(unsigned int cap, unsigned int(*h) (const Tkey &)){
    capacity = cap; // dimensiunea
    hash = h; // functia de hash
    H = new std::list<entry<Tkey, Tvalue>> [capacity]; // bucketurile
    size = 0;
  }

  ~Hashtable(){
    delete[] H;
  }

  float current_factor(){
    return (float)(size) / (float)(capacity);
  }

  void modify(const Tkey &k, const Tkey &k2, const Tvalue &v2){
    if(has_key(k)){
      remove(k);
      put(k2, v2);
    }

    // in cazul in care valoarea ce se modifica nu exista, nu o adaug nici pe a 2a

  }

  void put(const Tkey &k, const Tvalue &v){
    unsigned int idx = get_index(k);
    typename list<entry<Tkey, Tvalue>>::iterator it;
    // caut daca exista un element cu cheia k
    it = find_if(H[idx].begin(), H[idx].end(),[&]( entry<Tkey, Tvalue> A){ return A.key==k; });
    if(it != H[idx].end()){
      //inseamna ca elementul deja exista, modific valoarea
      it->value = v;
    }
    else{
      H[idx].emplace_back(k, v);
      size ++;
      if (current_factor() > TRESHOLD)
        resize();
    }
  }

  void remove(const Tkey &k){
//    cout << "remove\n";
    unsigned int idx = get_index(k);
    typename list<entry<Tkey, Tvalue>>::iterator it;
    // caut daca exista un element cu cheia k
    it = find_if(H[idx].begin(), H[idx].end(),[&]( entry<Tkey, Tvalue> A){ return A.key==k; });
    if(it != H[idx].end())
      H[idx].erase(it);
  }

  Tvalue get(const Tkey &k){
    if(!has_key(k)) {
//      cout << " nu exista elementul cu cheia: " << k;
      return INT_MIN;
    }
    unsigned int idx = get_index(k);
    typename list<entry<Tkey, Tvalue>>::iterator it;
    // caut daca exista un element cu cheia k
    it = find_if(H[idx].begin(), H[idx].end(),[&]( entry<Tkey, Tvalue> A){ return A.key==k; });
    return it->value;
  }

  void resize() {
//    cout << size << " " << capacity << " resize\n";
    capacity *= 2;
//    cout << "resize: " << capacity << "/n";
    Tkey k;
    Tvalue v;
    unsigned int idx;
    list<entry<Tkey, Tvalue>> *old = H;
    H = new std::list<entry<Tkey, Tvalue>> [capacity]; // bucketurile
    for(unsigned int i = 0 ; i < capacity / 2; i ++)
      for (typename list<entry<Tkey, Tvalue>>::iterator it = old[i].begin(); it != old[i].end(); it++){
        k = it->key;
        v = it->value;
        idx = get_index(k);
        H[idx].emplace_back(k, v);
      }
    delete[] old;
  }

  Hashtable<Tkey, Tvalue> get_union(const Hashtable<Tkey, Tvalue> &a){
    Hashtable<Tkey, Tvalue> newMap = Hashtable<Tkey, Tvalue> (capacity, hash);
    for (unsigned int i = 0 ; i < capacity ; i++)
      for(typename list<entry<Tkey, Tvalue>>::iterator it = H[i].begin();
          it != H[i].end(); it++)
        newMap.put(it->key, it->value);

    for (unsigned int i = 0 ; i < a.getCapacity() ; i++)
      for(typename list<entry<Tkey, Tvalue>>::iterator it = a.getH()[i].begin();
          it != a.getH()[i].end(); it++)
        newMap.put(it->key, it->value);
    return newMap;
  }

  Hashtable<Tkey, Tvalue> get_intersection(const Hashtable<Tkey, Tvalue> &a) {
    Hashtable<Tkey, Tvalue> newMap = Hashtable<Tkey, Tvalue> (capacity, hash);
    for (unsigned int i = 0 ; i < capacity ; i++)
      for(typename list<entry<Tkey, Tvalue>>::iterator it = H[i].begin(); it != H[i].end(); it++)
        if(a.has_key(it->key))
          newMap.put(it->key, it->value);
    return newMap;
  }

  Hashtable<Tkey, Tvalue> get_difference(const Hashtable<Tkey, Tvalue> &a) {
    Hashtable<Tkey, Tvalue> newMap = Hashtable<Tkey, Tvalue> (capacity, hash);
    for (unsigned int i = 0 ; i < capacity ; i++)
      for(typename list<entry<Tkey, Tvalue>>::iterator it = H[i].begin(); it != H[i].end(); it++)
        if(!a.has_key(it->key))
          newMap.put(it->key, it->value);
    return newMap;
  }

  void print_all(){
    cout << "size = " << size << endl;
    cout << "capacity = " << capacity << endl;
    cout << "perc = " << current_factor() << endl;
    for (unsigned int i = 0 ; i < capacity ; i++)
      for(typename list<entry<Tkey, Tvalue>>::iterator it = H[i].begin(); it != H[i].end(); it++)
        cout << it->key << " " << it->value << endl;
    cout << endl;
  }

  bool has_key(const Tkey &k)const {
    unsigned int idx = get_index(k);
    typename list<entry<Tkey, Tvalue>>::iterator it;
    // caut daca exista un element cu cheia k
    it = find_if(H[idx].begin(), H[idx].end(),[&]( entry<Tkey, Tvalue> A){ return A.key==k; });
    if(it != H[idx].end())
      return true;
    return false;
  }

  list <entry<Tkey, Tvalue>> *getH() const {
      return H;
    }

  unsigned int getCapacity()const {
    return capacity;
  }

  unsigned int getSize() const {
    return size;
  }

  unsigned int get_index(const Tkey &k)const {
    return hash(k) % capacity;
//      return func(k) % capacity;
  }

  unsigned int get_collisions(){
    // imi va returna practic cate au intalnit coliziuni la adaugare
    // adica la daca h[1] = 0 1 2 h[2] = 4 h[3] = 5 6
    // va returna 3, pentru ca 1 si 2 sunt unde e si 0 si 6 unde e 5
    int s = 0;
    for (unsigned int i = 0 ; i < capacity; i ++)
      s += H[i].size() - 1;
    return s;
  }

};


#endif //HASHTABLE_HASHTABLE_H
