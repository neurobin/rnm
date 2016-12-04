/***********************************************************************
 * Bulk rename utility for Unix (rnm)
 * Author: Md. Jahidul Hamid <jahidulhamid@yahoo.com>
 * Copyright (C) 2015-2017 by Md. Jahidul Hamid <jahidulhamid@yahoo.com>
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
 
#ifndef FUNC_HPP
#define FUNC_HPP

#include "strnatcmp.hpp"
#include "inout.hpp"
#include "class.hpp"
#include "strutils.hpp"
#include "regutils.hpp"

//~ template<class T>
//~ String toString(const T& x) {
    //~ oss_buffer.str("");
    //~ oss_buffer.clear();
    //~ oss_buffer << x;
    //~ return oss_buffer.str();
//~ }


void updateIndexFlagsFromFlagMaps(){
    IFP=index_int_flag["precision"];
    index_field_length=index_int_flag["width"];
    IFF=index_string_flag["filler"];
    latin_fall_back_threshold=index_int_flag["latin-fallback"];
}


template<typename T1, typename T2>
bool existsInMap(std::map<T1,T2> mymap, T1 key){
    if ( mymap.find(key) == mymap.end() ) return false;
    else return true;
}

bool parseTwoStepIndexFlag(const String& s){
    String key,val;
    size_t pos = s.find_first_of("=");
    if(pos!=String::npos){
        key=s.substr(0,pos);
        val=s.substr(pos+1,String::npos);
        if(existsInMap(index_int_flag,key)){
            index_int_flag[key]=getPositiveIntOrExit(key,val);
            updateIndexFlagsFromFlagMaps();
            return true;
        }
        else if(existsInMap(index_string_flag,key)){
            index_string_flag[key]=getSingleCharacterStringOrExit(key,val);
            updateIndexFlagsFromFlagMaps();
            return true;
        }
    }
    return false;
}

bool setIndexFlagAdjust(String s){
    s=trim(s);
    for(int i=0;i<(int)INDEX_FLAGS_AV_A.size();i++){
        if(s==INDEX_FLAGS_AV_A[i]){
            index_flag_adjust=INDEX_FLAGS_A[i];
            index_flag_adjust_s=INDEX_FLAGS_AV_A[i];
            return true;
        }
    }
    return false;
}


bool setIndexFlagInd(String s){
    s=trim(s);
    for(int j=0;j<(int)INDEX_FLAGS_AV_I.size();j++){
        if(s==INDEX_FLAGS_AV_I[j]){
            INDEX_FLAGS_I_B[INDEX_FLAGS_AV_I[j]]=true;
            index_flag_ind.push_back(INDEX_FLAGS_I[j]);
            index_flag_ind_s.push_back(INDEX_FLAGS_AV_I[j]);
            std::sort( index_flag_ind.begin(), index_flag_ind.end() );
            index_flag_ind.erase( unique( index_flag_ind.begin(), index_flag_ind.end() ), index_flag_ind.end() );
            
            std::sort( index_flag_ind_s.begin(), index_flag_ind_s.end() );
            index_flag_ind_s.erase( unique( index_flag_ind_s.begin(), index_flag_ind_s.end() ), index_flag_ind_s.end() );
            return true;
        }
    }
    return false;
}


void parseIndexFlags(const String& s){
    bool er=true;
    StringArray tokens=split(s,FLAG_DELIM);
    for(int i=0;i<(int)tokens.size();i++){
        er=true;
        if(parseTwoStepIndexFlag(tokens[i])){
            er=false;
            continue;
        }
        
        if(setIndexFlagAdjust(tokens[i])){
            er=false;
            continue;
        }
            
        if(setIndexFlagInd(tokens[i])){
            er=false;
            continue;
        }
        if(er){printErrorLog("Invalid index flag: "+tokens[i]);Exit(1);}
    
    }
}


void printIndexFlags(){
    std::cout<< "Width: "<<index_field_length<<NEW_LINE;
    std::cout<< "Filler: "<<IFF<<NEW_LINE;
    std::cout<< "Precision: "<<IFP.get_str()<<NEW_LINE;
    std::cout<< "Adjust field: "+index_flag_adjust_s+NEW_LINE;
    std::cout<< "Other flags: ";
    for(int i=0;i<(int)index_flag_ind_s.size();i++){
        std::cout<< index_flag_ind_s[i];
        if(i<(int)index_flag_ind_s.size()-1){std::cout<< ", ";}
        }
    std::cout<< NEW_LINE;
    
    }




#endif
