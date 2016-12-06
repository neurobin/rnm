
#include "func.hpp"

jp::Regex multi_rre("\\s*/([^/]*?)/\\s*([ifdl]*)(\\s*;\\s*|$)",0,jpcre2::JIT_COMPILE);

void cleanExit(){
    throw (std::exception());
}

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here  
   // terminate program  

   //exit(signum);  
    cleanExit();
}


int main(){
    try{
    CWD = getCurrentDir();
    //~ std::cout<<"current dir:"<<getCurrentDir()+NEW_LINE;
    File file1("/media/jahid/StorageP3/Games3/NARUTO SHIPPUDEN Ultimate Ninja STORM 4-CODEX/codex-naruto.shippuden.ultimate.ninja.storm.4.iso");
    
    //~ std::cout<<"size: "<<std::scientific<<getSizeByUnit(Double(file.size),0,10,2,0,IFF," ",std::ios::uppercase)<<"\n"<<formatTime(file.atime, "%d.%m.%Y %H:%M:%S %d %z")
    //~ <<NEW_LINE<<getSizeByUnit(Double(file.blocks),GB, 10,4)
    //~ ;
    
    //~ std::cout<<"\n----\n"<<toString(1,10,8,0,IFF,std::ios::showpos|std::ios::scientific);
    
    //~ print (selectInput());
    
    //~ print(countMatchInRegex("fjdklsj3424","\\d"));
    //~ print(getPositiveIntOrExit("somev","-543t4"));
    
    //~ std::cout<<isComplyingToRegex("7846837483", re);
    
    //~ fixed_ss.clear();
    //~ fixed_ss.push_back(false);
    
    //~ parseSearchString("/h/",0);
    
    //~ jp::Regex re, re2;
    //~ re2 = re;
    //~ std::cout<<"re:"<<!!re<<"re2:"<<!!re2;
    
    //~ for(size_t i =0;i<ss_search.size();++i){
        //~ jp::Regex re = ss_search_re[i];
        //~ std::cout<<"\ns: "<<ss_search[i]<<"\tm: "<<ss_mod[i]<<"\t fixed: "<<!!ss_search_re[i];
        //~ }
    
    //~ std::cout<<"\n"<<isComplyingToSearchString(file);
    
    //~ std::cout<<file.isDir();
    //~ std::cout<<"t: "<<childDepth(File("/media/jahid/StorageP3//"), file);
    //~ reverse_line = true;
    //~ NameList nl = getNameListFromFile("/media/jahid/StorageP2/Naruto/Naruto Shippuuden/narulistd",20,15);
    
    //~ for(size_t i = 0;i<nl.size();++i){
        //~ std::cout<<"\n"<<nl[i];
    //~ }
    
    //~ sort_type=="none";
    //~ FileArray files = getFilesFromDir("/home/jahid/test1/");
    
    //~ String mod = "dl";
    
    //~ for(size_t i = 0;i<files.size();++i){
        //~ std::cout<<"\n"<<basename(files[i].path)<<"\t"<<files[i].isFile()<<"\ttype: "<<files[i].type<<"\tvalid: "<<files[i].isValidWithMod(mod);
    //~ }
       // register signal SIGINT and signal handler  
   signal(SIGINT, signalHandler);  

   //~ while(1){
      //~ std::cout << "Going to sleep...." << std::endl;
      //~ sleep(1);
   //~ }
   
    //~ const char* file = "/home/jahid/test1/index.html";
    //~ int fd;
    //~ struct flock lock;

    //~ printf ("opening %s\n", file);
    //~ /* Open a file descriptor to the file. */
    //~ fd = open (file, O_RDWR);
    //~ printf ("locking\n");
    //~ /* Initialize the flock structure. */
    //~ //memset (&lock, 0, sizeof(lock));
    //~ lock.l_type = F_WRLCK;
    //~ lock.l_whence = SEEK_SET;
    //~ /* Place a write lock on the file. */
    //~ if(fcntl (fd, F_SETLKW, &lock) == -1){
        //~ if (errno == EACCES || errno == EAGAIN) {
            //~ printf("Already locked by another process\n");


            //~ /* We cannot get the lock at the moment */


        //~ } else {
            //~ /* Handle unexpected error */;
        //~ }
    //~ }else { /* Lock was granted... */

    //~ printf ("locked; hit Enter to unlock... ");
    //~ /* Wait for the user to hit Enter. */
    //~ getchar ();

    //~ printf ("unlocking\n");
    //~ /* Release the lock. */
    //~ lock.l_type = F_UNLCK;
    //~ fcntl (fd, F_SETLKW, &lock);

    //~ close (fd);
    //~ }

    //~ String s1 = "3jlfsd.fds3[]$ [ffds]er[][sd][[f]sd\\[\\]";
    //~ String s2 = sanitizeRegexString(s1);
    //~ std::cout<<s1+NEW_LINE+s2+NEW_LINE;
    //~ std::cout<< jp::Regex(s2).match(s1,"g");


    DirectoryIndex di;



    std::cout<<parseNameString("/i-s-3342//pd0-2+/",file1,di,"/","", false);

    //~ std::cout<<processExtendedPdNameStringRule("/pd0-w%/", file1,"/","",0);




    return 0;
    } catch(const std::exception& e){
    return 1;
    }
}
