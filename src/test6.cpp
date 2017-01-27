#include "func.hpp"
#include "times.hpp"
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
    //~ NameList names;
    //~ linc=4;
    //~ getNameListFromFile(names,"list",98 ,1,1);
    
    //~ for(size_t i=0;i<names.size();++i){
        //~ std::cout<<names[i]<<"\n";
    //~ }
    
    //~ std::string encrypted = encryptDecrypt("kylewbanks.com");
    //~ std::cout << "Encrypted:" << encrypted << "\n";
    
    //~ std::string decrypted = encryptDecrypt(encrypted);
    //~ std::cout << "Decrypted:" << decrypted << "\n";
    std::string s1 = "fjdskl fjklsdjfl;kjads;lfjl;dkjfl;asjdfl;kjdslkfj sdfl;gkjlsdkgj";
    std::string s2 = "ds";
    std::string s3 = "fjlsdj flje l;fkjdsl fj";
    
    std::cout<<"en: "<<encryptDecrypt( s1)<<"\n\n";
    std::cout<<"de: "<<encryptDecrypt( encryptDecrypt( s1))<<"\n\n";
    
    sleep(1);
    main();
    
    return 0;
}


