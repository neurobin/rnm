#include "func.hpp"


#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <atomic>
#include <cmath>



int main(){
    File file("/home/jahid/Git/Github/neurobin/rnm/src/test.txt");
    std::cout<<"size: "<<file.dev;
    std::cout<<processInfoNameStringRule("/info-mtime/@/info-atime-%d-%Y/ size: /info-blksize-1/perm: /info-perm-ls/ dev: /info-dev/", file,"/","",0);
    
    //~ futil::FileStream file("/home/jahid/Git/Github/neurobin/rnm/src/test.txt","w+b");
    //~ while(!file.setLock(futil::lock_op::WaitLock)) std::cout<<"waiting...\n";
    //~ sleep(10);
    
    //~ double t, d, h, m, s, sf, b;
    //~ t = 3666.6;
    //~ sf = std::modf(t,&b);
    //~ t = b;
    //~ d = t/(24*60*60);
    //~ t = std::fmod(t,24*60*60);
    //~ h = t/(60*60);
    //~ t = std::fmod(t,60*60);
    //~ m = t/60;
    //~ s = std::fmod(t,60);
    //~ s += sf;
    //~ std::string msg = " file/s renamed in";
    //~ if(d>=1) msg += " " + toStringWithFloatingPointDigit(d, '0') + " day/s";
    //~ if(h>=1) msg += " " + toStringWithFloatingPointDigit(h, '0') + " hour/s";
    //~ if(m>=1) msg += " " + toStringWithFloatingPointDigit(m, '0') + " minute/s";
    //~ msg += " " + toStringWithFloatingPointDigit(s, '4') + " second/s";
    
    //~ std::cout<<msg;
    
    return 0;
}
