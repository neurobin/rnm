#include "globals.hpp"

template<typename T>
String toString(T a){
    Stream ss;
    ss <<a;
    return ss.str();
}

bool stringContains(String s1,const String& s2){
    if (s1.find(s2) != String::npos) {return true;}
    else return false;
}

String regexReplace(const String& s,std::regex re,const String& replace,const String& modifier){
    bool global=false;
    String replaced_string=s;
    if(stringContains(modifier,"g")){global=true;}
    
    try {
        if(global){
        replaced_string=std::regex_replace (s,re,replace,std::regex_constants::format_default);
        }
        else{
        replaced_string=std::regex_replace (s,re,replace,NON_RECURSIVE_REGEX_REPLACE);
        }
    } 
    catch (std::regex_error& e) {
      print "Invalid replace string regex: ";
      Exit(1);
    }
    return replaced_string;
}


StringArray cpdn;

int main(){
    String f="/media/jahid/softs/iso images/windows/kfljsl.iso";
    String d="/home/jahidul/Downloads/sofs/instant/more-instant";
    String e="/windows/kfljsl.iso";
    }
