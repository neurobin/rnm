#include <iostream>
#include <jpcre2.hpp>
#include <string>

typedef std::string String;
typedef jpcre2::select<char> jp;

bool isComplyingToRegex(String& s, jp::Regex &re){
    String total="";
    jp::VecNum v;
    re.initMatch().setNumberedSubstringVector(&v).setFindAll();
    re.match(s);
    for(size_t i = 0; i < v.size(); ++i) {
        std::cout<<"\n\t"<<v[i][0]<<"\t\t"<<total;
        total += v[i][0];
    }
    return (s == total);
}


int main(){
    String name="/jk/me/gi;/fds/fsd/g;/hj/hj";
    String pat = R"(\s*/([^/]*)/([^/]*)/\s*([gi]{0,2})\s*([;]\s*|$))";
    jp::Regex multi_re (pat);
    std::cout<< isComplyingToRegex(name,multi_re);
}
