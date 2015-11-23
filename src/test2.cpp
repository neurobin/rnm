#include <iostream>
#include <vector>
#include <algorithm>
#include "strnatcmp.hpp"

int main(){
    std::vector<std::string> files;
    
    files.push_back("20.txt");
    files.push_back("10.txt");
    files.push_back("1.txt");
    files.push_back("z2.txt");
    files.push_back("z10.txt");
    files.push_back("z100.txt");
    files.push_back("1_t.txt ");
    files.push_back("ABc");
    files.push_back("aBCd");
    files.push_back("aBc");
    files.push_back("aaa");
    files.push_back("aBcd");
    files.push_back("aaA");
    
    std::sort(files.begin(),files.end(),compareNat);
    
    for(int i=0;i<(int)files.size();i++)std::cout<< files[i]+"\n";
    
	return 0;
}
