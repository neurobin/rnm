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

std::string encryptDecrypt(std::string s) {
    char key[] = " \n\r\f\v\t/"; //Any chars will work
    std::string out = s;
    for (size_t i = 0; i < s.size(); i++)
        out[i] = s[i] ^ key[i % (sizeof(key) / sizeof(char))];
    return out;
}

std::string rhash(std::string s){
    for(size_t i=0;i<s.size();++i){
        s[i] = s[i] * 387420489 % 4000000000;
    }
    return s;
}

std::string un_hash(std::string s){
    for(size_t i=0;i<s.size();++i){
        s[i] = s[i] * 3513180409 % 4000000000;
    }
    return s;
}

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
    std::string s1 = "I am a stri/ng\n of multiline///\n\r";
    std::string s2 = "ds";
    std::string s3 = "fjlsdj flje l;fkjdsl fj";
    
    std::cout<<"en: "<<std::hash<String>{}(s1)<<"\n\n";
    std::cout<<"en: "<<std::hash<String>{}(s1)<<"\n\n";
    //~ std::cout<<"de: "<<un_hash( rhash( s1))<<"\n\n";
    
    //~ sleep(1);
    
    //~ for(size_t i=0;i<512;++i) std::cout<<char(i^='/');
    
    
    //~ main();
    
    return 0;
}


