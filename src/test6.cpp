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
    //~ try{
    //~ signal(SIGINT, signalHandler);  
    //~ std::cout<< futil::io_t::W;
    
    //~ futil::FileStream file, file1;
    
    //~ file.open("/home/jahid/Git/Github/neurobin/rnm/src/test.txt", "w+b");
    
    //~ if(file) std::cout<<"good\n";
    
    //~ if(file.setLock()){
        //~ std::cout<<"Locked\n";
    //~ } else {
        //~ std::cout<<"Couldn't lock";
    //~ }
    
    //~ while (1) {
        //~ if(file.setLock()){
            //~ std::cout<<"locked\n";
            //~ file<<"locked\n";
            //~ file.flush();
            //~ sleep(5);
        //~ } else {
            //~ std::cout<<"waiting\n";
            //~ sleep(2);
        //~ }
    //~ }
    //~ file1.open("/home/jahid/Git/Github/neurobin/rnm/src/test.txt", futil::io_t::W);
    //~ std::cout<<strerror(file1.getStatus());
    
    //~ if(file1.setLock()){
        //~ std::cout<<"Locked\n";
    //~ } else {
        //~ std::cout<<"Couldn't lock";
    //~ }
    //~ } catch (const Except& e){
        //~ return e.status;
    //~ }
    
    //~ return 0;
}


