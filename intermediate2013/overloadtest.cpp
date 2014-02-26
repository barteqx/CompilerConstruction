#include <iostream>


int foo(bool, double, int, double){
  return 5;
}

int foo(int, double, double, int){
  return 10;
}

int main(){
  std::cout << foo(0, 1, 1, 1);


}