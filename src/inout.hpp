/***********************************************************************
 * Bulk rename utility for Unix (rnm)
 * Author: Md. Jahidul Hamid <jahidulhamid@yahoo.com>
 * Copyright (C) 2015 by Md. Jahidul Hamid <jahidulhamid@yahoo.com>
 *   
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * ********************************************************************/
/***********************************************************************
 * Global conventions:
 * 
 * * Always use absolute paths (mind the undo option uses full path).
 * * IFP can't be 0 by default. Make it -1 (disabled).
 * * Try to skip files with warning (not error).
 * * Exit with exit status 1 in case of error.
 * 
 * ********************************************************************/
 

#ifndef __INOUT_HPP
#define __INOUT_HPP


#include "global.hpp"
#include "class.hpp"
#include "strutils.hpp"


void signalHandler( int signum ) { 
    Exit(2);
}

void unsafeExitSignalHandler(int signum){
    unsafeExit(1);
}

struct Except: virtual public std::exception{
    int status;
    explicit Except(int a): status(a)
    {}
    virtual ~Except() throw () {}
};

//~ template<typename T>
//~ void print(const T& s){ std::cout<<NEW_LINE<<s<<NEW_LINE;}


void getLine(String& s){
    std::cin.clear();
    //~ std::cin.ignore(std::numeric_limits<std::streamsize>::max());
    signal(SIGINT, unsafeExitSignalHandler); //agressive exit needed
    std::getline(std::cin, s);
    signal(SIGINT, signalHandler); //restore to safe signal handler
}


String getCurrentDir(void){
    char c[PATH_MAX];
    char * ptr = getcwd(c, sizeof(c)/sizeof(char));
    if(ptr) return String(c);
    else return ".";
}


void prepareLogDir(){
    mkdir(LOG_DIR_PARENT.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(LOG_DIR.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(LOG_DIR_UNDO.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void openLockFile(futil::lock_op ltype = futil::lock_op::ImmediateLock, bool cleanfs_on_exit = false){
    RNM_LOCK_FILE_F.open(RNM_LOCK_FILE, "w+b"); //futil automatically closes a file if opened already
    if(!RNM_LOCK_FILE_F.setLock(ltype)){
        errorExitExtra("Couldn't get lock. Another rnm process is running on current directory. Let it finish first. Abort.", cleanfs_on_exit);
    }
    //~ return strerror(errno);
}

void openLogFiles(){
    if(File(ERROR_LOG).size > 10 * MB)
        ERROR_LOG_F.open(ERROR_LOG.c_str(), std::ios::out);
    else
        ERROR_LOG_F.open(ERROR_LOG.c_str(), std::ios::app | std::ios::out);
    if(File(OUT_LOG).size > 10 * MB)
        OUT_LOG_F.open(OUT_LOG.c_str(), std::ios::out);
    else
        OUT_LOG_F.open(OUT_LOG.c_str(), std::ios::app | std::ios::out);
    //~ return strerror(errno);
}


void closeLockFile(){
    RNM_LOCK_FILE_F.close();
}

void closeLogFiles(){
    ERROR_LOG_F.close();
    OUT_LOG_F.close();
}

void removeLockFile(){
    std::remove(RNM_LOCK_FILE.c_str());
}


void cleanFiles(){
    removeLockFile();
}

void printErrorLog0(String str, const String& fn, size_t line){
    str="E: "+str+" (@rnm/"+fn+"/"+std::to_string(line)+")";
    if(!double_quiet) std::cerr<<str+NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    ERROR_LOG_F<<str+"\t\t\t\t@"+dt;
}


void printWarningLog0(String str, const String& fn, size_t line){
    str="W: "+str+" (@rnm/"+fn+"/"+std::to_string(line)+")";
    if(!quiet){std::cerr<<str+NEW_LINE;}
    time_t now = time(0);
    char* dt = ctime(&now);
    ERROR_LOG_F<<str+"\t\t\t\t@"+dt;
}

    
void printOutLog(const String& str){
    if(!quiet)std::cout<<NEW_LINE+str+NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    OUT_LOG_F<<str+"\t\t        @"+dt;
}


void errorExit0(const String& s, const String& fn, size_t line, bool cleanfs){
    printErrorLog0(s, fn, line);
    Exit(1, cleanfs);
}


//~ String copyFile(const String& src, const String& dest){
    //~ int outfd = open(dest.c_str(),O_RDWR);
    //~ int infd = open(src.c_str(),O_RDWR);          
    //~ struct stat stat_buf ;
    //~ fstat(infd,&stat_buf);
    //~ Uint size = sendfile(outfd,infd,0,stat_buf.st_size);
    //~ return std::to_string(size);
//~ }


//~ void copyFile2(const String& src,const String& dst){
    //~ std::ifstream source(src, std::ios::binary);
    //~ std::ofstream dest(dst, std::ios::binary);
    //~ dest << source.rdbuf();
    //~ source.close();
    //~ dest.close();
    //~ ////~ return strerror(errno);
//~ }


void finalizeRFL(){
    if(rnc > 0 && !simulation){//do this only if files were actually renamed
        //~ copyFile2(RNM_FILE_LOG_L_TMP,RNM_FILE_LOG_L);
        //~ copyFile2(RNM_FILE_LOG_R_TMP,RNM_FILE_LOG_R);
        RNM_FILE_LOG_L_F.open(RNM_FILE_LOG_L, std::ios::binary | std::ios::out);
        RNM_FILE_LOG_L_F<<CWD<<'\0';
        RNM_FILE_LOG_L_F<<RNM_FILE_LOG_L_S;
        
        RNM_FILE_LOG_R_F.open(RNM_FILE_LOG_R, std::ios::binary | std::ios::out);
        RNM_FILE_LOG_R_F<<CWD<<'\0';
        RNM_FILE_LOG_R_F<<RNM_FILE_LOG_R_S;
        
        RNM_FILE_LOG_L_F.close();
        RNM_FILE_LOG_R_F.close();
    }
}


void appendToRFLTMP(const String& str1,const String& str2){
    //~ RNM_FILE_LOG_L_TMP_F<<str1<<'\0';
    //~ RNM_FILE_LOG_R_TMP_F<<str2<<'\0';
    //~ return strerror(errno);
    RNM_FILE_LOG_L_S += str1;
    RNM_FILE_LOG_L_S.push_back('\0');
    RNM_FILE_LOG_R_S += str2;
    RNM_FILE_LOG_R_S.push_back('\0');

}

int selectInput(){
    String msg= "\nSelect action:\n\n\
    1. Confirm for this file only.\n\
    2. Confirm for all.\n\
    3. Skip this file.\n\
    4. Skip all and exit\n\n\
Your choice (#?): \
";

    while(true){
        std::cout<<msg;
        String s;
        getLine(s);
        if(s.length()!=1) s="-"+s;
        switch(s[0]){
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            default: std::cerr<<"\nInvalid choice!\n";break;
        }
    }
}


Int getIntOrExit(const String& name, const String& s){
    try {
        return Int(s);
    } catch(const std::invalid_argument& e){
    }
    printErrorLog(name+ " must be an integer. Invalid value: "+s);
    Exit(1);
    return 0;
}

Int getPositiveIntOrExit(const String& name, const String& s){
    try {
        Int x(s);
        if(x>=0) return x;
    } catch(const std::invalid_argument& e){
    }
    printErrorLog(name+" must be a positive integer. Invalid value: "+s);
    Exit(1);
    return 0;
}

Double getNumberOrExit(const String& name, const String& s){
    try{
        return Double(s);
    } catch(const std::invalid_argument& e){
    }
    printErrorLog(name+" must be a number. Invalid value: "+s);
    Exit(1);
    return 0;
}

Double getPositiveNumberOrExit(const String& name,const String& extra,const String& s){
    try{
        Double x(s);
        if(x>=0) return x;
    } catch(const std::invalid_argument& e){
    }
    printErrorLog(name+" must be a positive number. Invalid value: "+s+"\n"+extra);
    Exit(1);
    return 0;
}


String getSingleCharacterStringOrExit(const String& name, const String& s){
    if(s.length() != 1){
        printErrorLog(name+" must be a single character. Invalid value: "+s);
        Exit(1);
    }
    return s;
}




StringArray getLineFromFileAndReturnVector(const String& filename){
    StringArray list;
    String line;
    char delim='\n';
    FileStream file;
    file.open(filename,std::ios::in);
    if(!file.good()){printErrorLog("Couldn't open file: "+filename);Exit(1);}
    while(std::getline(file,line,delim)){
        if ( line.size() && line[line.size()-1] == '\r' ) {line = line.substr( 0, line.size() - 1 );}
        if(line.empty()){continue;}
        list.push_back(line);
    }
    file.close();
    
    return list;
}

void getNameListFromFile(StringArray& nlist, const String& filename, Uint si, Uint ei, int mod=1){
    String line;
    StringArray list;
    Uint start=si,end=ei;
    Uint local_linc = linc.get_ui();
    char delim='\n';
    if(mod==0){delim='\0';}
    FileStream file;
    if(mod==1){file.open(filename,std::ios::in);}
    else {file.open(filename,std::ios::binary | std::ios::in);}
    if(!file.good()){printErrorLog("Couldn't open file: "+filename);Exit(1);}
    while(std::getline(file,line,delim)){
        if(mod==1){
            if ( line.size() && line[line.size()-1] == '\r' ) {
               line = line.substr( 0, line.size() - 1 );
            }
        }
        list.push_back(line);
    }
    file.close();
    Uint n = list.size();
    if(si==0) start = n>0?n-1:n;
    else start = si - 1;
    if(ei==0) end = n>0?n-1:n;
    else end = ei - 1;
    if(start<=end){
        for(Uint i = start; i<n && i <= end ; i = i + local_linc){
            if(list[i].empty()) continue;
            nlist.push_back(list[i]);
            lc_list.push_back(i);
        }
    } else {
        for(Uint i = start; i<n && i >= end ; i = i - local_linc){
            if(list[i].empty()) continue;
            nlist.push_back(list[i]);
            lc_list.push_back(i);
        }
    }
    
}


void cleanup(bool cleanfs){
    closeLogFiles();
    closeLockFile();
    finalizeRFL();
    if(cleanfs) cleanFiles();
}


void unsafeExit(int a, bool cleanfs){
    cleanup(cleanfs);
    std::exit(a);
}

void Exit(int a, bool cleanfs){
    showResult();
    cleanup(cleanfs);
    throw Except(a);
}


bool isImmediateChild(const File& prevf,const File& newf){
        if(prevf.path==dirname(newf.path))return true;
        else return false; 
}


bool isChild(const File& parent, const File& child){
  std::size_t found = child.path.find(parent.path+path_delim);
  if (found!=String::npos && found==0) return true;
  else return false;
}


bool isChildDir(const File& parent, File child){
    if(child.isDir()) return isChild(parent,child);
    else return isChild(parent, File(dirname(child.path)));
}


Int childDepth(const File& parent, File child){
    if(isChild(parent, child)){
        return split(child.path,'/').size() - split(parent.path, '/').size();
    } else return LOWEST_DEPTH;
}



bool compareNatV(const File& a, const File& b){
    return compareNat(a.path, b.path);
}

bool compareNatG(const File& a, const File& b){
    return (a.path<b.path);
}

    
void sortVector(FileArray& files){
    if(sort_type=="natural") std::sort(files.begin(), files.end(), compareNatV); 
    else if(sort_type=="general")std::sort(files.begin(), files.end(), compareNatG);
    else if(sort_type=="none");
    else if(sort_type == "mtime") std::sort(files.begin(), files.end(), [](const File& a, const File& b){return a.mtime>b.mtime;});
    else if(sort_type == "atime") std::sort(files.begin(), files.end(), [](const File& a, const File& b){return a.atime>b.atime;});
    else if(sort_type == "ctime") std::sort(files.begin(), files.end(), [](const File& a, const File& b){return a.ctime>b.ctime;});
    else if(sort_type == "size")  std::sort(files.begin(), files.end(), [](const File& a, const File& b){return a.size >b.size; });
    else{std::sort(files.begin(), files.end(), compareNatV);}
    
    if(!sort_type2.empty()){
        if(sort_type2 == "directory") std::stable_sort(files.begin(), files.end(), [](const File& a, const File& b){return a.isDir() >b.isDir(); });
        else if(sort_type2 == "file") std::stable_sort(files.begin(), files.end(), [](const File& a, const File& b){return a.isFile()>b.isFile();});
        else if(sort_type2 == "link") std::stable_sort(files.begin(), files.end(), [](const File& a, const File& b){return a.isLink()>b.isLink();});
    }
    
    ///reverse the sort if reverse_sort is true
    if(reverse_sort){std::reverse(files.begin(), files.end());}
}


void getFilesFromDir(FileArray& files, const String& file){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (file.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            
            String filename=file+path_delim+ent->d_name;
            if(basename(filename)=="." || basename(filename)==".."){continue;}
            files.push_back(File(filename));
        }
        closedir (dir);
    } 
    else {
        printErrorLog((String(strerror(errno))+": "+file).c_str());
    }
    sortVector(files);
    //~ return files;
}


bool isInvalidFile(const File& f){
    String file = f.path;
    bool status=false;
    
    if(CWD==f.path && !force){
        status = true;
        printWarningLog("renaming working directory not permitted. If you insist, try applying force.");
    }
    
    StringArray invf={HOME,
        "/bin","/usr","/usr/bin","/tmp",
        "/dev","/etc","/proc","/home",
        "/boot","/root","/lib","/media","/opt",
        "/srv","/sbin","/sys","/var","/include",
        "/local","/libexec","/log","/mail","/spool",
        "/mnt","/share","/unix"};
    for(int i=0;i<(int)invf.size();i++){
        if(file==invf[i]){
            if(!force){
                status=true;
                printWarningLog("rename not permitted: "+file);
            }
        }
    }
    
    //these are not permitted even with force
    if(!super_force){
        if(file==root_filesystem){status=true;printWarningLog("rename not permitted: "+file);}
        if(file==LOG_DIR_PARENT){status=true;printWarningLog("rename not permitted: "+file);}
        if(file==LOG_DIR){status=true;printWarningLog("rename not permitted: "+file);}
        if(file==LOG_DIR_UNDO){status=true;printWarningLog("rename not permitted: "+file);}
    }
    
    
    return status;
}



void showResult(){
    if(undo_path_show) return;
    TIME_COUNT += duration(timeNow() - START_TIME);
    TimeType t, d, h, m, s, sf, b;
    t = TIME_COUNT;
    sf = std::modf(t,&b);
    t = b;
    d = t/(24*60*60);
    t = std::fmod(t,24*60*60);
    h = t/(60*60);
    t = std::fmod(t,60*60);
    m = t/60;
    s = std::fmod(t,60);
    s += sf;
    std::string msg = rnc.get_str() + " file" + (rnc>1?"s":"") + " renamed in";
    if(d>=1) msg += " " + toStringWithFloatingPointDigit(d, '0') + " day" + (std::floor(d)>1?"s":"");
    if(h>=1) msg += " " + toStringWithFloatingPointDigit(h, '0') + " hour" + (std::floor(h)>1?"s":"");
    if(m>=1) msg += " " + toStringWithFloatingPointDigit(m, '0') + " minute" + (std::floor(m)>1?"s":"");
    msg += " " + toStringWithFloatingPointDigit(s, '4') + " second" + (s>1?"s":"");
    printOutLog(msg);
    if(!quiet && simulation) std::cout<< " (simulation)"+NEW_LINE;
    
}


#endif
