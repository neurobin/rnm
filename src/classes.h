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



#endif
