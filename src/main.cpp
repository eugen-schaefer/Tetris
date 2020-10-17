#include <iostream>
#include "my_function.h"

int main(){

    std::cout << "Hello world !" << std::endl;

    int val{5};

    std::cout << "The double value of " << val << " is " << DoubleTheValue(val) << std::endl;
}