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


#ifndef __CLASS_HPP
#define __CLASS_HPP

#include "global.hpp"
//~ #include "strutils.hpp"

const void * my_memrchr(const void *s, int c, size_t n){
    /*
     * Copyright (c) 2007 Todd C. Miller <Todd.Miller@courtesan.com>
     *
     * Permission to use, copy, modify, and distribute this function for any
     * purpose with or without fee is hereby granted, provided that the above
     * copyright notice and this permission notice appear in all copies.
     */

    const unsigned char *cp;

    if (n != 0) {
        cp = (unsigned char *)s + n;
        do {
            if (*(--cp) == (unsigned char)c)
                return((void *)cp);
        } while (--n != 0);
    }
    return(NULL);
}


char * normalizePath(const char* pwd, const char * src, char* res) {
	Uint res_len;
	Uint src_len = strlen(src);

	const char * ptr = src;
	const char * end = &src[src_len];
	const char * next;

	if (src_len == 0 || src[0] != '/') {
		// relative path
		Uint pwd_len;

		pwd_len = strlen(pwd);
		memcpy(res, pwd, pwd_len);
		res_len = pwd_len;
	} else {
		res_len = 0;
	}

	for (ptr = src; ptr < end; ptr=next+1) {
		Uint len;
		next = (char*)memchr(ptr, '/', end-ptr);
		if (next == NULL) {
			next = end;
		}
		len = next-ptr;
		switch(len) {
		case 2:
			if (ptr[0] == '.' && ptr[1] == '.') {
				const char * slash = (const char*)my_memrchr(res, '/', res_len);
				if (slash != NULL) {
					res_len = slash - res;
				}
				continue;
			}
			break;
		case 1:
			if (ptr[0] == '.') {
				continue;
			}
			break;
		case 0:
			continue;
		}

		if (res_len != 1)
			res[res_len++] = '/';

		memcpy(&res[res_len], ptr, len);
		res_len += len;
	}

	if (res_len == 0) {
		res[res_len++] = '/';
	}
	res[res_len] = '\0';
	return res;
}


char filetypeletter(int mode){
    char    c;
    if (S_ISREG(mode))
        c = '-';
    else if (S_ISDIR(mode))
        c = 'd';
    else if (S_ISBLK(mode))
        c = 'b';
    else if (S_ISCHR(mode))
        c = 'c';
#ifdef S_ISFIFO
    else if (S_ISFIFO(mode))
        c = 'p';
#endif  /* S_ISFIFO */
#ifdef S_ISLNK
    else if (S_ISLNK(mode))
        c = 'l';
#endif  /* S_ISLNK */
#ifdef S_ISSOCK
    else if (S_ISSOCK(mode))
        c = 's';
#endif  /* S_ISSOCK */
#ifdef S_ISDOOR
    /* Solaris 2.6, etc. */
    else if (S_ISDOOR(mode))
        c = 'D';
#endif  /* S_ISDOOR */
    else{
        /* Unknown type -- possibly a regular file? */
        c = '?';
    }
    return(c);
}

String lsperms(int mode)
{
    const char *rwx[] = {"---", "--x", "-w-", "-wx",
    "r--", "r-x", "rw-", "rwx"};
    char bits[11];

    bits[0] = filetypeletter(mode);
    strcpy(&bits[1], rwx[(mode >> 6)& 7]);
    strcpy(&bits[4], rwx[(mode >> 3)& 7]);
    strcpy(&bits[7], rwx[(mode & 7)]);
    if (mode & S_ISUID)
        bits[3] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID)
        bits[6] = (mode & S_IXGRP) ? 's' : 'l';
    if (mode & S_ISVTX)
        bits[9] = (mode & S_IXUSR) ? 't' : 'T';
    bits[10] = '\0';
    return(String(bits));
}

struct DirectoryIndex{
    Double directory_index,directory_index_rd,directory_reverse_index,directory_reverse_index_rd;
    Int IFL;

    DirectoryIndex(){
        init();
        directory_index=DIRECTORY_INDEX;
        directory_index_rd=DIRECTORY_INDEX;
        directory_reverse_index=DIRECTORY_REVERSE_INDEX;
        directory_reverse_index_rd=DIRECTORY_REVERSE_INDEX;
        }


    DirectoryIndex(const Double& di, const Double& dri){
        init();
        directory_index=di;
        directory_index_rd=di;
        directory_reverse_index=dri;
        directory_reverse_index_rd=dri;
        }


    DirectoryIndex(const Double& di){
        init();
        directory_index=di;
        directory_index_rd=di;
        directory_reverse_index=di;
        directory_reverse_index_rd=di;
        }


    private:
    void init(){
        IFL = index_field_length;
    }

};




class Options{
    private:
        std::string value1,value2,value3,value4;

    public:
        short count;
        Options(std::string v1)                               {value1=v1;value2="";value3="";value4="";count=0;}
        Options(std::string v1,std::string v2)                     {value1=v1;value2=v2;value3="";value4="";count=0;}
        Options(std::string v1,std::string v2,std::string v3)           {value1=v1;value2=v2;value3=v3;value4="";count=0;}
        Options(std::string v1,std::string v2,std::string v3,std::string v4) {value1=v1;value2=v2;value3=v3;value4=v4;count=0;}
        std::string getValue1(){return value1;}
        std::string getValue2(){return value2;}
        std::string getValue3(){return value3;}
        std::string getValue4(){return value4;}




};


struct File {
    private:
    //~ bool isfile;
    //~ bool isdir;

    public:
    char type;      //'f': file, 'd': directory, 'l': link, 0 (NULL): non-existent
    time_t atime;   //file access time (st_atime)
    time_t mtime;   //file modification time (st_mtime)
    time_t ctime;   //file status change time (st_ctime)
    String inode;   //inode number (st_ino)
    String mode;    //file mode (st_mode)
    String nlink;   //number of hard links (st_nlink)
    String uid;     //user ID of owner (st_uid)
    String gid;     //group ID of owner (st_gid)
    String dev;     //ID of device containing file (st_dev)
    Uint size;      //total size, in bytes (st_size)
    Uint blksize;   //blocksize for file system I/O (st_blksize)
    Uint blocks;    //number of 512B blocks allocated (st_blocks)
    String path;    //Absolute path (Absolute path is essential for the undo operation)

    //permission
    String perm_oct;
    String perm_ls;
    String perm;

    void init() {
        type = 0;
        atime = 0;
        mtime = 0;
        ctime = 0;
        size = 0;
        blksize = 0;
        blocks = 0;
        //~ isfile = false;
        //~ isdir = false;
        }

    File(const String& file) {
        init(); //initialize vars
        char abspath[FILENAME_MAX+1];
        if(follow_symlink){
            char* res = realpath(file.c_str(),abspath);
            if(res) path = String(abspath);
            else path = file;
        }
        else {
            //~ char* pwd = new char[CWD.size()+1];
            path = String(normalizePath(CWD.c_str(),file.c_str(),abspath));
        }
        struct stat finfo;
        if((follow_symlink?stat(path.c_str(), &finfo ):lstat(path.c_str(), &finfo )) != 0 ) {
            //file doesn't exist
            type = 0;
        } else{
            //file exists
            if(S_ISREG(finfo.st_mode)) { type = 'f'; }
            else if(S_ISDIR(finfo.st_mode)) { type = 'd'; }
            else if(S_ISLNK(finfo.st_mode)) { type = 'l'; }
            //no else needed

            atime = finfo.st_atime;
            mtime = finfo.st_mtime;
            ctime = finfo.st_ctime;
            inode = std::to_string(finfo.st_ino);
            mode = std::to_string(finfo.st_mode);
            nlink = std::to_string(finfo.st_nlink);
            uid = std::to_string(finfo.st_uid);
            gid = std::to_string(finfo.st_gid);
            dev = std::to_string(finfo.st_dev);
            size = finfo.st_size;
            blksize = finfo.st_blksize;
            blocks = finfo.st_blocks;

            //get permission
            char mybuff[50];
            sprintf(mybuff,"%#o",finfo.st_mode);
            perm_oct = String(mybuff);
            perm_ls = lsperms(finfo.st_mode);
            //perm is defaulted to octal perm
            perm = perm_oct;
        }
    }

    bool operator!() const {
        return (type == 0);
    }

    explicit operator bool() const {
        return (type != 0);
    }

    bool isFile() const {
        return (type == 'f');
    }

    bool isDir() const{
        return (type == 'd');
    }

    bool isLink() const{
        return (type == 'l');
    }

    bool isValidWithMod(const String& mod) const{
        if(type == 0)  return false;
        size_t len = mod.length();
        size_t flag = 0;
        for(size_t i = 0;i<len;++i){
            switch(mod[i]){
                case 'f': if(isFile()) return true; else break;
                case 'd': if(isDir())  return true; else break;
                case 'l': if(isLink()) return true; else break;
                default: ++flag; break;
            }
        }
        //if f, d, l neither was present, it will pass the test by default.
        if(flag == len) return true;
        else return false;
    }
};



#endif
