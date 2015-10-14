#include "classes.hpp"

bool isComplyingToRegex(String &s,Regex &re){
    String total="";
    RegexIterator it(s.begin(), s.end(), re);
    RegexIterator it_end;
    while(it!=it_end){total+=it->str();++it;}
    if(s!=total){return false;}
    else {return true;}
    }

int main(){
    Regex multi_re("\\s*"+path_delim+"([^"+path_delim+"]*?)"+path_delim+"\\s*(i?)(\\s*;\\s*|$)");
    int subm[]={1,2,3};
    String s="/12/i";
    ///Check the regex format for multi search keyword
    if(!isComplyingToRegex(s,multi_re)){print "Error";}
    RegexTokenIterator end; //default constructor=end of sequence
    RegexTokenIterator toit (s.begin(), s.end(), multi_re,subm);
    
    while (toit != end){
    String search_term=*toit++;
    String mod_term=*toit++;
    toit++;
    std::cout << "["<<search_term <<"]["<<mod_term<<"]"<< std::endl;
    }
    return 0;}
