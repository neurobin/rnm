#include "classes.hpp"

bool isComplyingToRegex(String &s,Regex &re){
    String total="";
    RegexIterator it(s.begin(), s.end(), re);
    RegexIterator it_end;
    while(it!=it_end){total+=it->str();++it;}
    if(s!=total){return false;}
    else {return true;}
    }
    
StringArray getLineFromFileAndPushToArray(String filename){
    StringArray list;
    String line;
    char delim='\n';
    FileStream file;
    file.open(filename,$read);
    if(!file.good()){printErrorLog("Couldn't open name string file: "+filename);Exit(1);}
    while(getLineFromFile(file,line,delim)){
        if ( line.size() && line[line.size()-1] == '\r' ) {line = line.substr( 0, line.size() - 1 );}
        if(line=="" || line == toString('\0')){continue;}
        list.push_back(line);
    }
    file.close();
    
    return list;
}

    

int main(){
    Regex multi_re ("\\s*"+path_delim+"([^"+path_delim+"]*?)"+path_delim+"([^"+path_delim+"]*?)"+path_delim+"\\s*([gi]{0,2})\\s*(;\\s*|$)");
    int subm[]={1,2,3,4};
    String s="/search/i/ gi;";
    getLine(s);
    ///Check the regex format for multi search keyword
    if(!isComplyingToRegex(s,multi_re)){print "Error";}
    RegexTokenIterator end; //default constructor=end of sequence
    RegexTokenIterator toit (s.begin(), s.end(), multi_re,subm);
    
    while (toit != end){
    String search_term=*toit++;
    String replace_term=*toit++;
    String mod_term=*toit++;
    toit++;
    std::cout << "["<<search_term <<"]["<<replace_term<<"]["<<mod_term<<"]"<< std::endl;
    }
    main();
    return 0;}
