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
 

#ifndef __CLASSES_H
#define __CLASSES_H


#include "func.hpp"



//~ class DirectoryIndex{
    
    //~ public:
    
    //~ Double directory_index,directory_index_rd,directory_reverse_index,directory_reverse_index_rd;
    
    //~ DirectoryIndex(){
        //~ directory_index=DIRECTORY_INDEX;
        //~ directory_index_rd=DIRECTORY_INDEX;
        //~ directory_reverse_index=DIRECTORY_REVERSE_INDEX;
        //~ directory_reverse_index_rd=DIRECTORY_REVERSE_INDEX;
        //~ }
        
        
    //~ DirectoryIndex(Double di, Double dri){
        //~ directory_index=di;
        //~ directory_index_rd=di;
        //~ directory_reverse_index=dri;
        //~ directory_reverse_index_rd=dri;
        //~ }
        
        
    //~ DirectoryIndex(Double di){
        //~ directory_index=di;
        //~ directory_index_rd=di;
        //~ directory_reverse_index=di;
        //~ directory_reverse_index_rd=di;
        //~ }
    
    
    
//~ };


//~ class Options{
    //~ private:
        //~ std::string value1,value2,value3,value4;
        
    //~ public:
        //~ short count;
        //~ Options(std::string v1)                               {value1=v1;value2="";value3="";value4="";count=0;}
        //~ Options(std::string v1,std::string v2)                     {value1=v1;value2=v2;value3="";value4="";count=0;}
        //~ Options(std::string v1,std::string v2,std::string v3)           {value1=v1;value2=v2;value3=v3;value4="";count=0;}
        //~ Options(std::string v1,std::string v2,std::string v3,std::string v4) {value1=v1;value2=v2;value3=v3;value4=v4;count=0;}
        //~ std::string getValue1(){return value1;}
        //~ std::string getValue2(){return value2;}
        //~ std::string getValue3(){return value3;}
        //~ std::string getValue4(){return value4;}
    
    


//~ };


//~ struct File {
    //~ char type;      //'f': file, 'd': directory, 'l': link, 0: non-existent
    //~ time_t atime;   //file access time (st_atime)
    //~ time_t mtime;   //file modification time (st_mtime)
    //~ time_t ctime;   //file status change time (st_ctime)
    //~ String inode;   //inode number (st_ino)
    //~ String mode;    //file mode (st_mode)
    //~ String nlink;   //number of hard links (st_nlink)
    //~ String uid;     //user ID of owner (st_uid)
    //~ String gid;     //group ID of owner (st_gid)
    //~ String dev;     //ID of device containing file (st_dev)
    //~ size_t size;    //total size, in bytes (st_size)
    //~ size_t blksize; //blocksize for file system I/O (st_blksize)
    //~ size_t blocks;  //number of 512B blocks allocated (st_blocks)
    //~ String path;    //Absolute path (Absolute path is essential for the undo operation)
    
    //~ bool is_file;   //not a dir, can be a file or a link. should be used to check if it's a file or directory
    
    //~ //permission
    //~ String perm_oct;
    //~ String perm_ls;
    //~ String perm;
    
    //~ void init() {
        //~ type = 0;
        //~ is_file = false;
        //~ atime = 0;
        //~ mtime = 0;
        //~ ctime = 0;
        //~ size = 0;
        //~ blksize = 0;
        //~ blocks = 0;
        //~ }
    
    //~ File(const String& file) {
        //~ init(); //initialize vars
        //~ char abspath[FILENAME_MAX+1];
        //~ if(follow_symlink){
            //~ realpath(file.c_str(),abspath);
            //~ path = String(abspath);
        //~ }
        //~ else {
            //~ path = String(normalizePath((char*)(CWD.c_str()),(const char*)(file.c_str()),abspath));
        //~ }
        //~ struct stat finfo;
        //~ if(stat(path.c_str(), &finfo ) != 0 ) {
            //~ //file doesn't exist
            //~ type = 0;
        //~ } else{
            //~ //file exists
            //~ if(S_ISREG(finfo.st_mode)) { type = 'f'; is_file = true; }
            //~ if(S_ISDIR(finfo.st_mode)) { type = 'd'; is_file = false;}
            //~ if(S_ISLNK(finfo.st_mode)) { type = 'l'; is_file = true; }
            
            //~ atime = finfo.st_atime;
            //~ mtime = finfo.st_mtime;
            //~ ctime = finfo.st_ctime;
            //~ inode = toString(finfo.st_ino);
            //~ mode = toString(finfo.st_mode);
            //~ nlink = toString(finfo.st_nlink);
            //~ uid = toString(finfo.st_uid);
            //~ gid = toString(finfo.st_gid);
            //~ dev = toString(finfo.st_dev);
            //~ size = finfo.st_size;
            //~ blksize = finfo.st_blksize;
            //~ blocks = finfo.st_blocks;
            
            //~ //get permission
            //~ char mybuff[50];
            //~ sprintf(mybuff,"%#o",finfo.st_mode);
            //~ perm_oct = String(mybuff);
            //~ perm_ls = lsperms(finfo.st_mode);
            //~ //perm is defaulted to octal perm
            //~ perm = perm_oct;
        //~ }
    //~ }
    
//~ };


class Number{
    String value;
    bool is_integer = false;
    
    public:
    
    Number(){value = "";}
    template<class T>
    Number(T x){value = std::to_string(x);}
    Number(const char* x) {value = String(x);}
    Number(String const& x) {value = x;}
    
    explicit operator bool const () { return (value != "" && value != "nan");}
    bool operator!(){return (value == "" || value == "nan");}
    
    bool isInt(){return is_integer;}
    bool isFloat() {return !is_integer;}
    
    Number operator+(Number& var){
        Number out;
        String expr = value + " + " + var.value;
        return out;
        }
    Number operator-(Number& var){
        Number out;
        
        return out;
        }
    Number operator*(Number& var){
        Number out;
        
        return out;
        }
    Number operator/(Number& var){
        Number out;
        
        return out;
        }
    Number operator%(Number& var){
        Number out;
        
        return out;
        }
    Number& operator++(){
        
        return *this;
        }
    Number& operator--(){
        
        return *this;
        }
    
    };


#endif
