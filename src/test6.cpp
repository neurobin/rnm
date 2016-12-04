#include "func.hpp"
#include <chrono>
#include <gmpxx.h>

typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()

#define timeNow() std::chrono::high_resolution_clock::now()

std::string fun(char a){
    if(a) return std::string(1,a);
    return std::string();
    }

int main(){
    std::string s("SS");
    std::cout<<s;
    s = std::string();
    std::cout<<s;
    
    return 0;
}


