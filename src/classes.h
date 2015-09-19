/// Here resides all the available classes.
#ifndef __CLASSES_H
#define __CLASSES_H


#include "globals.h"



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
        String value1,value2,value3,value4;
        
    public:
        short count;
        Options(String v1)                               {value1=v1;value2="";value3="";value4="";count=0;}
        Options(String v1,String v2)                     {value1=v1;value2=v2;value3="";value4="";count=0;}
        Options(String v1,String v2,String v3)           {value1=v1;value2=v2;value3=v3;value4="";count=0;}
        Options(String v1,String v2,String v3,String v4) {value1=v1;value2=v2;value3=v3;value4=v4;count=0;}
        String getValue1(){return value1;}
        String getValue2(){return value2;}
        String getValue3(){return value3;}
        String getValue4(){return value4;}
    
    


};






#endif
