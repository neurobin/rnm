#include "func.hpp"




int main(){
    //~ File file("/home/jahid/Git/Github/neurobin/rnm/src/test.txt");
    //~ std::cout<<"size: "<<file.dev;
    //~ std::cout<<processInfoNameStringRule("/info-mtime,gmt-%d-%m-%Y/@/info-atime-%d-%Y/ size: /info-blksize-1/perm: /info-perm-ls/ dev: /info-dev/", file);
    
    futil::FileStream file("/home/jahid/Git/Github/neurobin/rnm/src/test.txt","w+b");
    while(!file.setLock(futil::lock_op::WaitLock)) std::cout<<"waiting...\n";
    sleep(10);
    return 0;
}
