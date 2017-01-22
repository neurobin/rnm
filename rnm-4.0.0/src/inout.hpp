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


void signalHandler( int signum ) { 
    Exit(1);
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

template<typename T>
void print(const T& s){ std::cout<<NEW_LINE<<s<<NEW_LINE;}


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


String prepareLogDir(){
    mkdir(LOG_DIR_PARENT.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(LOG_DIR.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(LOG_DIR_UNDO.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    FileStream file;
    file.open(RNM_FILE_LOG_L_TMP.c_str(),std::ios::out);
    if(!file.good()){std::cerr<<strerror(errno);}
    file.close();
    file.open(RNM_FILE_LOG_R_TMP.c_str(),std::ios::out);
    if(!file.good()){std::cerr<<strerror(errno);}
    file.close();
    return String(strerror(errno));
}

String openLockFile(futil::lock_op ltype = futil::lock_op::ImmediateLock, bool cleanfs_on_exit = false){
    RNM_LOCK_FILE_F.open(RNM_LOCK_FILE, "w+b"); //futil automatically closes a file if opened already
    if(!RNM_LOCK_FILE_F.setLock(ltype)){
        errorExit("Couldn't get lock. Another rnm process is running on current directory. Let it finish first. Abort.", cleanfs_on_exit);
    }
    return strerror(errno);
}

String openTmpFiles(){
    RNM_FILE_LOG_L_TMP_F.open(RNM_FILE_LOG_L_TMP.c_str(), std::ios::binary | std::ios::out);
    RNM_FILE_LOG_R_TMP_F.open(RNM_FILE_LOG_R_TMP.c_str(), std::ios::binary | std::ios::out);
    return strerror(errno);
}

String openTmpFilesForAppend(){
    RNM_FILE_LOG_L_TMP_F.open(RNM_FILE_LOG_L_TMP.c_str(), std::ios::binary | std::ios::app);
    RNM_FILE_LOG_R_TMP_F.open(RNM_FILE_LOG_R_TMP.c_str(), std::ios::binary | std::ios::app);
    return strerror(errno);
}

String openLogFiles(){
    ERROR_LOG_F.open(ERROR_LOG.c_str(), std::ios::app);
    OUT_LOG_F.open(OUT_LOG.c_str(), std::ios::app);
    return strerror(errno);
}

void closeTmpFiles(){
    RNM_FILE_LOG_L_TMP_F.close();
    RNM_FILE_LOG_R_TMP_F.close();
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

void removeTempFiles(){
    std::remove(RNM_FILE_LOG_L_TMP.c_str());
    std::remove(RNM_FILE_LOG_R_TMP.c_str());
}

void cleanFiles(){
    removeTempFiles();
    removeLockFile();
}

void printErrorLog0(String str, const String& fn, size_t line){
    str="E: "+str+" (@rnm/"+fn+"/"+std::to_string(line)+")";
    std::cerr<<str+NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    ERROR_LOG_F<<str+"\t\t\t\t@"+dt;
}

#define printErrorLog(a) printErrorLog0(a, __FILE__, __LINE__)

void printWarningLog0(String str, const String& fn, size_t line){
    str="W: "+str+" (@rnm/"+fn+"/"+std::to_string(line)+")";
    if(!quiet){std::cerr<<str+NEW_LINE;}
    time_t now = time(0);
    char* dt = ctime(&now);
    ERROR_LOG_F<<str+"\t\t\t\t@"+dt;
    OUT_LOG_F<<str+"\t\t\t\t@"+dt;
}

#define printWarningLog(a) printWarningLog0(a, __FILE__, __LINE__)
    
void printOutLog(const String& str){
    if(!quiet)std::cout<<NEW_LINE+str+NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    OUT_LOG_F<<str+"\t\t        @"+dt;
}


void errorExit(const String& s, bool cleanfs){
    printErrorLog(s);
    Exit(1, cleanfs);
}


String copyFile(const String& src, const String& dest){
    int outfd = open(dest.c_str(),O_RDWR);
    int infd = open(src.c_str(),O_RDWR);          
    struct stat stat_buf ;
    fstat(infd,&stat_buf);
    Uint size = sendfile(outfd,infd,0,stat_buf.st_size);
    return std::to_string(size);
}


String copyFile2(const String& src,const String& dst){
    std::ifstream source(src, std::ios::binary);
    std::ofstream dest(dst, std::ios::binary);
    dest << source.rdbuf();
    source.close();
    dest.close();
    return strerror(errno);
}


void finalizeRFL(){
    if(rnc > 0 && !simulation){//do this only if files were actually renamed
        copyFile2(RNM_FILE_LOG_L_TMP,RNM_FILE_LOG_L);
        copyFile2(RNM_FILE_LOG_R_TMP,RNM_FILE_LOG_R);
    }
}


String appendToRFLTMP(const String& str1,const String& str2){
    RNM_FILE_LOG_L_TMP_F<<str1<<'\0';
    RNM_FILE_LOG_R_TMP_F<<str2<<'\0';
    return strerror(errno);

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


NameList getNameListFromFile(const String& filename, Uint si, Uint ei,
                                int mod=1, bool reverse = reverse_line){
    NameList list;
    String line;
    Uint lc=0,abslc=0;
    Uint start=si,end=ei;
    if(ei!=0 && si>ei ){start=ei;end=si; reverse ^= reverse;}
    char delim='\n';
    if(mod==0){delim='\0';}
    FileStream file;
    if(mod==1){file.open(filename,std::ios::in);}
    else {file.open(filename,std::ios::binary | std::ios::in);}
    if(!file.good()){printErrorLog("Couldn't open file: "+filename);Exit(1);}
    while(std::getline(file,line,delim) && (lc<end || end == 0)){
        abslc++;
        if(mod==1){
            if ( line.size() && line[line.size()-1] == '\r' ) {
               line = line.substr( 0, line.size() - 1 );
            }
        }
        if(line.empty()) continue;
        lc++;
        if(lc>=start && (lc <= end || end == 0) ){
            list.push_back(line);abslc_list.push_back(abslc);
        }
    }
    file.close();
    if(reverse) std::reverse(list.begin(), list.end());
    return list;
}



void cleanup(bool cleanfs){
    closeLogFiles();
    closeTmpFiles();
    closeLockFile();
    finalizeRFL();
    if(cleanfs) cleanFiles();
}


void unsafeExit(int a, bool cleanfs){
    cleanup(cleanfs);
    std::exit(a);
}

void Exit(int a, bool cleanfs){
    cleanup(cleanfs);
    throw Except(a);
}


void recreateTmpFiles(const String& signature){
    closeTmpFiles();
    //recreate tmp files
    String tmp_log_l = RNM_FILE_LOG_L_TMP;
    String tmp_log_r = RNM_FILE_LOG_R_TMP;
    RNM_FILE_LOG_L_TMP = RNM_FILE_LOG_L_TMP_BKP + signature;
    RNM_FILE_LOG_R_TMP = RNM_FILE_LOG_R_TMP_BKP + signature;
    if(rename(tmp_log_l.c_str(), RNM_FILE_LOG_L_TMP.c_str()) == 0 && rename(tmp_log_r.c_str(), RNM_FILE_LOG_R_TMP.c_str()) == 0){
        //success
    } else {
        printWarningLog("Problem creating log files. undo may not work for this rnm operation.");
    }
    //open tmp files for append
    openTmpFilesForAppend();
}


#endif
