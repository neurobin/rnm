#include "funcs.hpp"


int main () {
    Double f = 3887.14159;
    std::cout.setf(std::ios::fixed,std::ios::floatfield);
    std::cout.precision(1);
    std::cout << f << std::endl;
    return 0;
}
