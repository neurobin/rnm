#include <iostream>
#include <regex>
#include <string>

typedef std::regex_iterator<std::string::iterator> RegexIterator;
typedef std::string String;
typedef std::regex Regex;

bool isComplyingToRegex(String& s,Regex &re){
    String total="";
    RegexIterator it(s.begin(), s.end(), re);
    RegexIterator it_end;
    while(it!=it_end){total+=it->str();++it;}
    if(s!=total){return false;}
    else return true;
}


int main(){
    String name="/jk/me/gi";
    String pat = R"(^\s*/([^/]*)/([^/]*)/\s*([gi]{0,2})\s*([;]\s*|$))";
    Regex multi_re (pat);
    std::cout<< isComplyingToRegex(name,multi_re);
}
