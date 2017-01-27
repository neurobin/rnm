#include "func.hpp"
//~ typedef std::chrono::high_resolution_clock::time_point TimeVar;

//~ #define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()

//~ #define timeNow() std::chrono::high_resolution_clock::now()

//~ std::string fun(char a){
    //~ if(a) return std::string(1,a);
    //~ return std::string();
    //~ }
    


//~ void signalHandler( int signum ) {
   //~ Exit(1);
//~ }




int main(){
    NameList names;
    linc=4;
    names = getNameListFromFile("list",1 ,0,1);
    
    for(size_t i=0;i<names.size();++i){
        std::cout<<names[i]<<"\n";
    }
    
    return 0;
}


