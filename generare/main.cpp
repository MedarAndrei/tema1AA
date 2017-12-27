#include <iostream>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string alphabet = "0123456789!@#$%^&*ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int size = alphabet.size();
int cuv_len, i;
string s;


void randstr(){
  s.erase();
  cuv_len = rand() % 30 + 1; // cuvinte de maxim 30
  for (i = 0 ; i < cuv_len; i++)
    s += alphabet[rand() % size];
}

int main(int argc, char *argv[]) {
  int n;
//  cout << argc << "\n";

  if(argc != 2){
    cout << "INCORRECT NUMBER OF ARGUMENTS\n";
    return 0;
  }

  n = atoi(argv[1]);
  // genereaza n perechi de tipul string int
  cout << n << "\n";
  //cin >> n;
  for(int i = 0 ; i < n ; i ++){
    randstr();
//    printf("%s %d \n", s, rand() % 1000000);
    cout << s << " " << rand() % 1000000 << "\n";
  }

  return 0;
}
