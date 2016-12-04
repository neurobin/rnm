
#include "func.hpp"

jp::Regex multi_sre("\\s*/([^/]*?)/\\s*([ifdl!]*)(\\s*;\\s*|$)",0,jpcre2::JIT_COMPILE);
jp::Regex multi_rre("\\s*/([^/]*?)/\\s*([ifdl]*)(\\s*;\\s*|$)",0,jpcre2::JIT_COMPILE);

void parseSearchString(String ss, size_t index){
    if(!stringContains(ss,path_delim)){
        ss_search.push_back(ss);
        ss_mod.push_back("");
        jp::Regex re;
        if(!fixed_ss[index]){
            re.setPattern(ss)
              .resetErrors()
              .compile();
            if(!re){
                printErrorLog("Invalid search string regex: "+ss+ ". "+re.getErrorMessage());
                Exit(1);
            }
        }
        ss_search_re.push_back(re);
    } else {
        String search, mod;
        jp::RegexMatch m(&multi_sre);
        jp::VecNum vn;
        size_t count = m.setSubject(ss)
                        .setNumberedSubstringVector(&vn)
                        .addJpcre2Option(jpcre2::FIND_ALL)
                        .match();
        if(count){
            for(size_t i=0;i<vn.size();++i){
                jp::Regex re;
                search = vn[i][1];
                mod = vn[i][2];
                ss_search.push_back(search);
                
                ss_mod.push_back(mod);
                if(!fixed_ss[index]){
                    std::cout<<"reached";
                    re.setModifier(mod)
                      .setPattern(search)
                      .resetErrors()
                      .compile();
                    if(!re){
                        printErrorLog("Invalid search string regex: "+search+ ". "+re.getErrorMessage());
                        Exit(1);
                    }
                }
                //if fixed ss then the follwoing re will be non-compiled
                ss_search_re.push_back(re);
            }
        } else {
            ///search string is a regex and with no modifier syntax `regex` in place of /regex/ is allowed.
            if(!fixed_ss[index])
            printErrorLog("Invalid search regex: "+ss+"\nNote that "+path_delim+" is not allowed inside regex. \n\
            Format: '"+path_delim+"regex(No"+path_delim+")"+path_delim+"modifier;' or 'regex(No"+path_delim+")'");
            else 
            printErrorLog("Invalid search string: "+ss+"\nNote that "+path_delim+" is not allowed in a string. \n\
            Format: '"+path_delim+"string(No"+path_delim+")"+path_delim+"modifier;' or 'string(No"+path_delim+")'");
            Exit(1);
        }
    }
}



bool isComplyingToSearchString(const File& file){
    String name=basename(file.path);
    for(Int i=0;i<(Int)ss_search.size();i++){
        //~ bool case_sensitive=true;
        //~ if(ss_mod[i]=="i"){case_sensitive=false;}
        if(!ss_fixed[i]){
                if(std::regex_search(name,ss_search_re[i])){return true;}
        }
        else{
            if(ss_mod[i]=="i"){
                if(stringContains(toLower(name),toLower(ss_search[i]))){return true;}
            }
            else{
                if(stringContains(name,ss_search[i])){return true;}
            }
        }
    }
    
    return false;
}



int main(){
    //~ std::cout<<"current dir:"<<getCurrentDir()+NEW_LINE;
    //~ File file("/media/jahid/StorageP3/Games3/NARUTO SHIPPUDEN Ultimate Ninja STORM 4-CODEX/codex-naruto.shippuden.ultimate.ninja.storm.4.iso");
    
    //~ std::cout<<"size: "<<std::scientific<<getSizeByUnit(Double(file.size),0,10,2,0,IFF," ",std::ios::uppercase)<<"\n"<<formatTime(file.atime, "%d.%m.%Y %H:%M:%S %d %z")
    //~ <<NEW_LINE<<getSizeByUnit(Double(file.blocks),GB, 10,4)
    //~ ;
    
    //~ std::cout<<"\n----\n"<<toString(1,10,8,0,IFF,std::ios::showpos|std::ios::scientific);
    
    //~ print (selectInput());
    
    //~ print(countMatchInRegex("fjdklsj3424","\\d"));
    //~ print(getPositiveIntOrExit("somev","-543t4"));
    
    //~ std::cout<<isComplyingToRegex("7846837483", re);
    
    fixed_ss.push_back(true);
    
    parseSearchString("/.*(\\d)/ifl;/\\w/!d",0);
    
    for(size_t i =0;i<ss_search.size();++i){
        jp::Regex re = ss_search_re[i];
        std::cout<<"\ns: "<<ss_search[i]<<"\tm: "<<ss_mod[i]<<"\t fixed: "<<!!re;
        }
    
    
    return 0;
}
