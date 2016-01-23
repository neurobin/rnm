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


#include "globals.hpp"



class DirectoryIndex{
    
    public:
    
    Double directory_index,directory_index_rd,directory_reverse_index,directory_reverse_index_rd;
    
    DirectoryIndex(){
        directory_index=DIRECTORY_INDEX;
        directory_index_rd=DIRECTORY_INDEX;
        directory_reverse_index=DIRECTORY_REVERSE_INDEX;
        directory_reverse_index_rd=DIRECTORY_REVERSE_INDEX;
        }
        
        
    DirectoryIndex(Double di, Double dri){
        directory_index=di;
        directory_index_rd=di;
        directory_reverse_index=dri;
        directory_reverse_index_rd=dri;
        }
        
        
    DirectoryIndex(Double di){
        directory_index=di;
        directory_index_rd=di;
        directory_reverse_index=di;
        directory_reverse_index_rd=di;
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





#endif
