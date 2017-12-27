//
// Created by andrei on 11.12.2017.
//

#ifndef TESTARE_TREAP_H
#define TESTARE_TREAP_H

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#define SIZE 700000000 //
#define PRIO rand() % SIZE

using namespace std;

template <typename T> struct Treap {
  T key;
  int priority;
  Treap<T> *left, *right;
  bool nil;


  // Pentru a rezolva problema 3 trebuie ca fiecare nod sa retina numarul de
  // noduri din subarborle sau
  int nr_nodes;

  // Creaza un nod nil
  Treap() : priority(-1), left(NULL), right(NULL), nil(true), nr_nodes(0) {}

  // Adaugam date, transformand un nod nil intr-un nod obisnuit
  void addData(T key, int priority) {
    this->nil = false;
    this->key = key;
    this->priority = priority;
    this->nr_nodes = 1;
    this->left = new Treap();
    this->right = new Treap();
  }

  // Stergem un nod obisnuit, transformandu-l intr-unul nil
  void delData() {
    this->nil = true;
    this->priority = -1;
    delete this->left;
    delete this->right;
    this->left = NULL;
    this->right = NULL;
    this->nr_nodes = 0;
  }

  bool isNil()const { return this->nil; }

  bool find(T key) {
    if (this->isNil()) {
      return false;
    }
    if (this->key == key)
      return true;
    if (this->key > key) {
      if (left != nullptr) {
        return this->left->find(key);
      } else
        return false;
    } else {
      if (right != nullptr) {
        return this->right->find(key);
      } else
        return false;
    }
  }

  /* Atat insert cat si erase au nevoie de o referinta catre un fatherPointer,
   adica de pointerul left sau right din parinte care pointeaza catre obiectul
   curent. De ce?
   Sa presupunem ca avem urmatoarea configuratie:
                 a
                / \
               b   c
                  / \
                 d   e

   si facem o rotatie dreapta in nodul c. Configuratia care rezulta este:
                 a
                / \
               b   d
                    \
                     c
                      \
                       e

   Dupa cum se poat vedea pointerul right a lui a trebuie sa pointeze in urma
   rotirii catre d. De aceea, o referinta a acelui pointer trebuie pasata catre
   fiul care reprezinta acel pointer, pentru ca nodul a sa vada eventualele
   inlocuiri ce pot aparea in urma unor rotiri.
   Atentie! - desi s-ar putea sa spunem ca putem folosi pointerul this pentru
   a rezolva problema de mai sus, acest lucru este gresit, deoarece this este un
   pointer constant, asupra caruia nu se pot face asignari de forma this = ...
  */

  void rotateRight() {
    swap(this->key, this->left->key);
    swap(this->priority, this->left->priority);
    Treap *C = this->right;
    this->right = this->left;
    this->left = this->right->left;
    this->right->left = this->right->right;
    this->right->right = C;
    this->right->nr_nodes =
            this->right->right->nr_nodes + this->right->left->nr_nodes + 1;
    this->nr_nodes = this->right->nr_nodes + this->left->nr_nodes + 1;
  }

  void rotateLeft() {
    swap(this->key, this->right->key);
    swap(this->priority, this->right->priority);
    Treap *C = this->left;
    this->left = this->right;
    this->right = this->left->right;
    this->left->right = this->left->left;
    this->left->left = C;
    this->left->nr_nodes =
            this->left->left->nr_nodes + this->left->right->nr_nodes + 1;
    this->nr_nodes = this->right->nr_nodes + this->left->nr_nodes + 1;

  }

  void modify(const T &key, const T &key2){
    if(this->find(key)){
      this->remove(key);
      this->insert(key2, PRIO);
    }

    // in cazul in care valoarea ce se modifica nu exista, nu o adaug nici pe a 2a
  }

  void add(const T &key, const int &priority){
    if(this->find(key)){
      //DEJA EXISTA
      return;
    }
    else
      this->insert(key, priority);
  }

  void insert(const T &key, const int &priority) {
    if (this->isNil()) {
      this->addData(key, priority);

      return;
    }

    // TODO: Completati inserarea
    this->nr_nodes++;
    if (key < this->key) { // mergem in stanga
      if (this->left->isNil())
        this->left = new Treap<T>();
      this->left->insert(key, priority);
    } else { // mergem in dreapta
      if (this->right->isNil())
        this->right = new Treap<T>();
      this->right->insert(key, priority);
    }

    // TODO: Incrementati dimensiunea subraborelui curenti

    if (this->left->priority > this->priority) {
      this->rotateRight();
    } else if (this->right->priority > this->priority) {
      this->rotateLeft();
    }
  }

  void remove(const T &key){
    if(!this->find(key)){
      //NU EXISTA
      return;
    }
    else{
      this->erase(key);
      //erase vector
    }
  }

  void erase(const T &key) {
    if (this->isNil()) {
      return;
    }

    // TODO: Decrementati dimensiunea subarborelui curent

    if (key < this->key) {
      this->nr_nodes--;

      this->left->erase(key);
    } else if (key > this->key) {
      this->nr_nodes--;

      this->right->erase(key);
    } else if (this->left->isNil() && this->right->isNil()) {
      this->delData();
    } else {
      if (this->left->priority > this->right->priority) {
//        cout << "rotate right " << this->left->key << " -> " << this->key
//             << endl;
        this->rotateRight();
        this->nr_nodes--;
        this->right->erase(key);
      } else {
//        cout << "rotate left " << this->right->key << " -> " << this->key
//             << endl;

        this->rotateLeft();
        this->nr_nodes--;
        this->left->erase(key);
      }
    }
  }

  void inOrder() {
    if (this->isNil()) {
      return;
    }
    if (left)
      this->left->inOrder();
    cout << this->key << " ";
    if (right)
      this->right->inOrder();
    // TODO: Afisati parcurgerea inordine a cheilor
  }

  void preOrder(int level = 0) {
    if (this->isNil()) {
      return;
    }
    for (int i = 0; i < level; i++) {
      cout << "  ";
    }
    cout << key << " -> " << this->priority << " -> " << this->nr_nodes << endl;
    if (left != nullptr)
      this->left->preOrder(level + 1);
    if (right != nullptr)
      this->right->preOrder(level + 1);
    // TODO: Afisati parcurgerea preordine a prioritatilor
  }

  T findK(int k) {
    // TODO: Pe baza nr_nodes determinati cea de-a k cheie in ordinea sortarii
    if (k == this->left->nr_nodes + 1)
      return this->key;
    if (k < this->left->nr_nodes + 1) {
      if (left != nullptr) {
        return this->left->findK(k);
      }
    } else {
      if (right != nullptr) {
        return this->right->findK(k - this->left->nr_nodes - 1);
      }
    }
    return 0;
  }

  vector<T> get_elements(){
    // intoarce un vector cu toate elementele
    // il folosesc pentru reuniune intersectie si diferenta
    vector<T> curr, left_elements, right_elements;
    if(left && !left->isNil()){
      left_elements = this->left->get_elements();
      curr = left_elements;
    }
    curr.push_back(this->key);
    if(right && !right->isNil()){
      right_elements = this->right->get_elements();
      curr.insert(curr.end(), right_elements.begin(), right_elements.end());
    }

    return curr;

  }

  Treap<T>* get_union(Treap<T> *a){
    Treap<T> *tr = new Treap<T>();
    if(this->isNil() && a->isNil())
      return tr;
    if(this->isNil())
      return a;
    if(a->isNil())
      return this;

    vector<T> m = this->get_elements();
    vector<T> n = a->get_elements();

    for(const auto &it : m)
      tr->add(it, PRIO);
    for(const auto &it : n)
      if(!tr->find(it))
        tr->add(it, PRIO);


    return tr;
  }


  Treap<T>* get_intersection(Treap<T> *a) {
    Treap<T> *tr = new Treap<T>();
    if (this->isNil() || a->isNil())
      return tr;

    vector<T> m = this->get_elements();
    vector<T> n = a->get_elements();

    for (const auto &it : m) {
      if(std::binary_search(n.begin(), n.end(), it))
      tr->add(it, PRIO);
    }
    return tr;
  }


  Treap<T>* get_difference(Treap<T> *a){
    Treap<T> *tr = new Treap<T>();
    if(this->isNil() || a->isNil())
      return tr;

    vector<T> m = this->get_elements();
    vector<T> n = a->get_elements();

    for (const auto &it : m){
      if(!std::binary_search(n.begin(), n.end(), it))
        tr->add(it, PRIO);
    }


    return tr;
  }



};





#endif //TESTARE_TREAP_H
