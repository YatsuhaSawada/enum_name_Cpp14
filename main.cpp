
#include "nameof.hpp"

#include <iostream>

enum class Fruit1 {
  BANANA1,
  BANANA2,
};

enum class Fruit2 {
  BANANA1,
  BANANA2,
  BANANA3,
};


int main() {
  
  std::cout << CONV2NAME(Fruit1::BANANA1) << std::endl;
  std::cout << CONV2NAME(Fruit1::BANANA2) << std::endl;
  std::cout << CONV2NAME(Fruit2::BANANA2) << std::endl;
  std::cout << CONV2NAME(Fruit2::BANANA2) << std::endl;
  std::cout << CONV2NAME(Fruit2::BANANA2) << std::endl;

  {
    auto c = Fruit1::BANANA1;
    std::cout << CONV2NAME(c) << std::endl;
  }
  {
    auto c = Fruit1::BANANA2;
    std::cout << CONV2NAME(c) << std::endl;
  }

  {
    auto c = Fruit2::BANANA1;
    std::cout << CONV2NAME(c) << std::endl;
  }
  {
    auto c = Fruit2::BANANA2;
    std::cout << CONV2NAME(c) << std::endl;
  }
  {
    auto c = Fruit2::BANANA3;
    std::cout << CONV2NAME(c) << std::endl;
  }

}