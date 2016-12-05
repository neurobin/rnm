
#include "func.hpp"

jp::Regex multi_rre("\\s*/([^/]*?)/\\s*([ifdl]*)(\\s*;\\s*|$)",0,jpcre2::JIT_COMPILE);



int main(){
    CWD = getCurrentDir();
    //~ std::cout<<"current dir:"<<getCurrentDir()+NEW_LINE;
    File file("/media/jahid/StorageP3/Games3/NARUTO SHIPPUDEN Ultimate Ninja STORM 4-CODEX/codex-naruto.shippuden.ultimate.ninja.storm.4.iso");
    
    //~ std::cout<<"size: "<<std::scientific<<getSizeByUnit(Double(file.size),0,10,2,0,IFF," ",std::ios::uppercase)<<"\n"<<formatTime(file.atime, "%d.%m.%Y %H:%M:%S %d %z")
    //~ <<NEW_LINE<<getSizeByUnit(Double(file.blocks),GB, 10,4)
    //~ ;
    
    //~ std::cout<<"\n----\n"<<toString(1,10,8,0,IFF,std::ios::showpos|std::ios::scientific);
    
    //~ print (selectInput());
    
    //~ print(countMatchInRegex("fjdklsj3424","\\d"));
    //~ print(getPositiveIntOrExit("somev","-543t4"));
    
    //~ std::cout<<isComplyingToRegex("7846837483", re);
    
    //~ fixed_ss.push_back(true);
    
    //~ parseSearchString("/h/",0);
    
    //~ for(size_t i =0;i<ss_search.size();++i){
        //~ jp::Regex re = ss_search_re[i];
        //~ std::cout<<"\ns: "<<ss_search[i]<<"\tm: "<<ss_mod[i]<<"\t fixed: "<<!!re;
        //~ }
    
    //~ std::cout<<"\n"<<isComplyingToSearchString(file);
    
    //~ std::cout<<file.isDir();
    //~ std::cout<<"t: "<<childDepth(File("/media/jahid/StorageP3//"), file);
    //~ reverse_line = true;
    //~ NameList nl = getNameListFromFile("/media/jahid/StorageP2/Naruto/Naruto Shippuuden/narulistd",20,15);
    
    //~ for(size_t i = 0;i<nl.size();++i){
        //~ std::cout<<"\n"<<nl[i];
    //~ }
    
    sort_type=="none";
    FileArray files = getFilesFromDir("/home/jahid/test1/");
    
    for(size_t i = 0;i<files.size();++i){
        std::cout<<"\n"<<files[i].path;
    }
    
    return 0;
}
