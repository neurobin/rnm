/***********************************************************************
 * futil (File utility)
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


#ifndef __FUTIL_HPP
#define __FUTIL_HPP
 
namespace futil{
    enum lock_t
        {
            ReadLock        = F_RDLCK,
            WriteLock       = F_WRLCK,
            UnLock          = F_UNLCK
        };
    enum lock_op
        {
            ImmediateLock   = F_SETLK,
            WaitLock        = F_SETLKW,
            GetLock         = F_GETLK
        };
    enum seek_t
        {
            beg             = SEEK_SET,
            cur             = SEEK_CUR,
            end             = SEEK_END
        };
    class FileStream{
        int fd;
        struct flock lock;
        seek_t pos;
        lock_op lop;
        lock_t lot;
        int lock_status;
        bool locked;
        int status;
        FILE* f;
        
        void init(){
            fd = 0;
            f = 0;
            pos = seek_t::beg;
            lop = lock_op::ImmediateLock;
            lot = lock_t::WriteLock;
            lock_status = 0;
            locked = false;
        }
        
        public:
        
        FileStream(){init();}
        FileStream(const std::string& s, const std::string& modes){
            init();
            open(s, modes);
        }
        
        virtual ~FileStream(){
            close();
        }
        
        void open(const std::string& s, const std::string& modes){
            close();
            f = ::fopen(s.c_str(), modes.c_str());
            fd = ::fileno(f);
            status = errno;
        }
        
        int getStatus() const{
            return status;
        }
        
        FILE* getFilePointer()const{
            return f;
        }
        
        int getFileDescriptor()const{
            return fd;
        }
        
        bool operator!() const { 
            return (f == 0); 
        }
        explicit operator bool() const {
            return (f != 0);
        }
        bool setLock(lock_op op=lock_op::ImmediateLock, lock_t lck=lock_t::WriteLock, seek_t sk=seek_t::beg){
            lop = op;
            lot = lck;
            pos = sk;
            lock.l_type = lot;
            lock.l_whence = pos;
            int ret = fcntl (fd, lop, &lock);
            lock_status = errno;
            locked = (ret != -1);
            return locked;
        }
        bool isLockedByOther() const {
            //must be called after setLock, otherwise no effect.
            return (lock_status == EACCES || lock_status == EAGAIN);
        }
        bool unLock(){
            if(!locked) return true;
            lock.l_type = lock_t::UnLock;
            int ret = fcntl(fd, lop, &lock);
            lock_status = errno;
            //~ std::cout<<"unlocked\n";
            return (ret != -1);
        }
        
        void close(){
            unLock();
            if(f) ::fclose(f);
            f = 0;
        }
        bool write(const char* s){
            return (fputs(s, f) >= 0);
        }
        bool write(const std::string& s){
            return (fputs(s.c_str(), f) >= 0);
        }
        bool write(const char ch){
            return (fputc(ch, f) != EOF);
        }
        bool flush(){
            if(f) return (::fflush(f) == 0);
            else return false;
        }
        FileStream& operator <<(const char* s){
            write(s);
            return *this;
        }
        FileStream& operator <<(const std::string& s){
            write(s);
            return *this;
        }
        FileStream& operator <<(const char ch){
            write(ch);
            return *this;
        }
    };
    
}

#endif
