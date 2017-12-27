#include <iostream>
#include "disjoint-set.h"
#include "hashtable.h"
#include "treap.h"
#include <chrono>
#include <unordered_map>
#include <utility>
using namespace std;

#define TEST 0



std::hash<string> hash_str;
vector<pair<string, int>> v; // datele de testare
int size; // marimea
std::chrono::time_point<std::chrono::high_resolution_clock> begin_clock;
std::chrono::time_point<std::chrono::high_resolution_clock> end_clock;
//std::chrono::duration<double> elapsed;

void start(){
  begin_clock = std::chrono::high_resolution_clock::now();
}

void stop(){
  end_clock = std::chrono::high_resolution_clock::now();
}

long result(){
  auto duration = chrono::duration_cast<chrono::milliseconds>(end_clock - begin_clock);
//  auto duration = chrono::duration_cast<chrono::nanoseconds>(end_clock - begin_clock);
  return duration.count();
}

long resultns(){
//  auto duration = chrono::duration_cast<chrono::milliseconds>(end_clock - begin_clock);
  auto duration = chrono::duration_cast<chrono::nanoseconds>(end_clock - begin_clock);
  return duration.count();
}


//PT HASHTABLE
unsigned int hash_stl(const string &str){
  return hash_str(str);
}

unsigned int hash_bun(const string &str) {
  unsigned int hash = 5381;
  for(unsigned int i=0;i<str.size();i++)
    hash = ((hash << 5) + hash) + str[i] ;
  return hash;
}

unsigned int hash_prost(const string &str){
  return 0;
}


void test_union_find(){
  cout << "\n\nDisjoint Set : \n\n";

  disjointSet<int> d1 = disjointSet<int>();

  start();
  for(int i = 0; i < size - 1; i ++)
    d1.add(i);
  stop();
  cout << "Adaugarea a n-1 elemente: " << result() << "\n";


    start();
    d1.add(size - 1);
    stop();
    if(TEST)
      cout << "Adaugarea unui singur element(ns): " << resultns() << "\n";
  


  for (int i = 0; i < size; i++)
    d1.add(size + i); // fac un set de 2 * n

//start();
  for(int i = 0; i < size; i ++)
    d1.uni(0, i);
//  cout << "a\n";

  for(int i = size; i < 2 * size; i++)
    d1.uni(2 * size, i);

  start();
  for(int i = 0; i < 2 * size; i = i + 2)
    d1.does_belong(i, rand() % (2 * size));
  stop();
  cout << "Verificarea pentru fiecare element daca apartine unei multimi: " << result() << "\n";


  start();
  	




  disjointSet<int> d2 = disjointSet<int>();
  // fac un set de 2 multimi de marime n

  for(int i = 0; i < 2 * size; i ++)
    d2.add(i);

  for (int i = 0; i < size; i++)
    d2.add(size + i);

  // creez cele 2 multimi de marime n

  for(int i = 0; i < size; i ++)
    d2.uni(0, i);


  for(int i = size; i < 2 * size; i++)
    d2.uni(2 * size, i);

  // le unesc

  start();
    d2.uni(rand() % size, rand() % size + size);
  stop();
  if(TEST)
    cout << "Reuniunea a 2 multimi de marime n(ns): " << resultns() << "\n";


}


void test_treap(){
  cout << "\n\nTREAP\n\n";

  start();
  Treap<int> *t1 = new Treap<int>;
  for (int i = 0 ; i < size - 1 ; i ++)
    t1->add(v[i].second, PRIO);
  stop();
  cout << "Adaugare n-1 elemente: " << result() << "\n";

  start();
  t1->add(v[size - 1].second, PRIO);
  stop();
  if(TEST)
    cout << "Adaugarea unui singur element(ns): " << resultns() << "\n";


//  start();
//  Treap<int> *t2 = new Treap<int>;
//  for (int i = 0 ; i < size - 1 ; i ++)
//    t2->add(i, size - i);
//  stop();
//  cout << "Adaugare n-1 elemente(inlantuire): " << result() << "\n";
//
//  start();
//  t2->add(size - 1, 0);
//  stop();
//  cout << "Adaugarea unui singur element(inlantuire): " << result() << "\n";


  // generez un alt treap random dar care sa aiba si cateva elemente comune
  Treap<int> *t2 = new Treap<int>;
  for(int i = 0 ; i < size - 1; i++){
    if(i % 10 == 0)
      t2->add(v[i].second, PRIO);
    else
      t2->add(rand() % 1000000, PRIO);
  }

  start();
  t1->get_union(t2);
  stop();
  cout << "Reuniunea a 2 treap-uri cu n elemente: " << result() << "\n";

  start();
  t1->get_intersection(t2);
  stop();
  cout << "Intersectia a 2 treap-uri cu n elemente: " << result() << "\n";

  start();
  t1->get_difference(t2);
  stop();
  cout << "Diferenta a 2 treap-uri cu n elemente: " << result() << "\n";



  Treap<int> *t3 = new Treap<int>;
  for (int i = 0 ; i < size - 1 ; i ++)
    t3->add(v[i].second, PRIO);

//  t3->inOrder();

  start();
  for (int i = 0 ; i < size - 1; i ++)
    t3->modify(v[i].second, v[i].second + 1);
  stop();
  cout << "Modificarea fiecarui element: " << result() << "\n";

//  t3->inOrder();

  int rand1 = rand() % size;
  start();
  if(t3 && !t3->isNil())
    t3->modify(v[rand1].second + 1, v[rand1].second + 2);
  else
    cout << "e gol\n";
  stop();
  if(TEST)
    cout << "Modificarea unui singur element(ns): " << resultns() << "\n";



  start();
  t1->remove(v[rand1].second);
  stop();
  if(TEST)
    cout << "Stergerea unui singur element(ns): " << resultns() << "\n";

  start();
  for(int i = 0 ; i < size ; i ++)
    if(!t1->isNil())
      t1->remove(v[i].second);
  stop();
  cout << "Stergerea a n-1 elemente: " << result() << "\n";

}


void test_hash(){
  cout << "\nHASHTABLE\n\n";
  // Hashtable normal : realocare + hash ok
  // masurare adaugarea tuturor, adaugarea unui element
  // extragere, modificare
  // reuniune, intersectie, diferenta
  start();
  Hashtable<string, int> h1 = Hashtable<string, int>(10, hash_stl);
  for (int i = 0 ; i < size - 1; i++)
    h1.put(v[i].first, v[i].second);
  stop();
  cout << "Adaugare n-1 elemente(cu realocare) : " << result() << "\n";

  start();
  Hashtable<string, int> h4 = Hashtable<string, int>(size * 10 / 7, hash_stl);
  for (int i = 0 ; i < size - 1; i++)
    h4.put(v[i].first, v[i].second);

  stop();
  cout << "Adaugare n-1 elemente(fara realocare) : " << result() << "\n";


  start();
  unordered_map<string,int> mp;
  for (int i = 0 ; i < size - 1; i++)
    mp[v[i].first] = v[i].second;
  stop();
  cout << "Adaugare n-1 elemente STL " << result() <<  "\n";



  start();
  h1.put(v[size - 1].first, v[size - 1].second);
  stop();
  if(TEST)
    cout << "Adaugare un singur element(ns): " << resultns() << "\n";


  // union, intersect, difference
  Hashtable<string, int> h2 = Hashtable<string, int>(10, hash_stl);
  for (int i = 0 ; i < size - 1; i++) {
    if( i % 10 == 0)
      h2.put(v[i].first, v[i].second);
    else
      h2.put(v[i].first + 'a', v[i].second >> 2);
    // incerc sa ma asigur ca sunt si elemente comune
  }

  start();
  h1.get_union(h2);
  stop();
  cout << "Reuniune 2 hashtableuri de marime n: " << result() << "\n";

  start();
  h1.get_intersection(h2);
  stop();
  cout << "Intersectie 2 hashtableuri de marime n: " << result() << "\n";

  start();
  h1.get_difference(h2);
  stop();
  cout << "Diferenta 2 hashtableuri de marime n: " << result() << "\n";


  Hashtable<string, int> h3 = Hashtable<string, int>(size * 2, hash_stl);
  for (int i = 0 ; i < size - 1; i++)
    h3.put(v[i].first, v[i].second);

  start();
  for(int i = 0 ; i < size ; i ++)
  h3.modify(v[i].first, v[i].first + 'a', v[i].second + 1);
  stop();
  cout << "Modificare fiecarui element: " << result() << "\n";

  int rand1 = rand() % size;
  start();
  h3.modify(v[rand1].first + 'a', v[rand1].first + "aa", v[rand1].second + 2);
  stop();
  if(TEST)
    cout << "Modificare un singur element(ns): " << resultns() << "\n";

  start();
  h1.remove(v[size - 1].first);
  stop();
  if(TEST)
    cout << "Stergere un singur element(ns): " << resultns() << "\n";

//  h1.print_all();
  start();
  for(int i = 0 ; i < size - 1; i ++)
    h1.remove(v[i].first);
  stop();
  cout << "Stergerea a n - 1 elemente: " << result() << "\n";




//  start();
//  Hashtable<string, int> h5 = Hashtable<string, int>(10, hash_prost);
//  for (int i = 0 ; i < size - 1; i++)
//    h5.put(v[i].first, v[i].second);
//  stop();
//  cout << "Adaugare n-1 elemente(hash prost) : " << result() << "\n";


}



int main(int argc, char *argv[]) {

  if(argc != 2){
    cout << "INCORRECT NUMBER OF ARGUMENTS\n";
    return 0;
  }


  ifstream f(argv[1]);
  if(!f){
    cout << "PROBLEMA LA FISIER\n";
    return 0;
  }

  f >> size;
  string aux1;
  int aux2;
  for(int i = 0 ; i < size ; i ++){
    f >> aux1 >> aux2;
    v.emplace_back(aux1, aux2);
  }

  cout << "-------------------------------------\nTESTARE : " << size << " ELEMENTE";

  test_hash();
  test_treap();
  test_union_find();




  f.close();
  return 0;
}
