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

void parseSearchString(String ss, size_t index){
    if(!stringContains(ss,path_delim)){
        ss_search.push_back(ss);
        ss_mod.push_back(SS_MOD_ALL);
        jp::Regex re;
        if(!fixed_ss[index]){
            re.setPattern(ss)
              .resetErrors()
              .compile();
            if(!re){
                printErrorLog("Invalid search string regex: "+ss+ ". "+re.getErrorMessage());
                Exit(1);
            }
        }
        ss_search_re.push_back(re);
    } else {
        String search, mod;
        jp::RegexMatch m(&multi_sre);
        jp::VecNum vn;
        size_t count = m.setSubject(ss)
                        .setNumberedSubstringVector(&vn)
                        .addJpcre2Option(jpcre2::FIND_ALL)
                        .match();
        if(count){
            for(size_t i=0;i<vn.size();++i){
                jp::Regex re;
                search = vn[i][1];
                mod = vn[i][2];
                ss_search.push_back(search);
                
                ss_mod.push_back(mod);
                if(!fixed_ss[index]){
                    re.setModifier(mod)
                      .setPattern(search)
                      .resetErrors()
                      .compile();
                    if(!re){
                        printErrorLog("Invalid search string regex: "+search+ ". "+re.getErrorMessage());
                        Exit(1);
                    }
                }
                //if fixed ss then the follwoing re will be non-compiled
                ss_search_re.push_back(re);
            }
        } else {
            ///search string is a regex and with no modifier syntax `regex` in place of /regex/ is allowed.
            if(!fixed_ss[index])
            printErrorLog("Invalid search regex: "+ss+"\nNote that "+path_delim+" is not allowed inside regex. \n\
            Format: '"+path_delim+"regex(No"+path_delim+")"+path_delim+"modifier;' or 'regex(No"+path_delim+")'");
            else 
            printErrorLog("Invalid search string: "+ss+"\nNote that "+path_delim+" is not allowed in a string. \n\
            Format: '"+path_delim+"string(No"+path_delim+")"+path_delim+"modifier;' or 'string(No"+path_delim+")'");
            Exit(1);
        }
    }
}


bool isInvMatch(const String& mod){
    if(stringContains(mod, "!")) return true;
    else return false;
}

bool isComplyingToSearchString(const File& file){
    String name=basename(file.path);
    jp::RegexMatch m;
    m.setSubject(name);
    for(size_t i=0;i<ss_search.size();i++){
        bool inv = isInvMatch(ss_mod[i]);
        if(stringContains(ss_mod[i],toString(file.type)) ||
            (
            !stringContains(ss_mod[i],"f") &&
            !stringContains(ss_mod[i],"d") &&
            !stringContains(ss_mod[i],"l")
            )){
        } else continue;
        if(ss_search_re[i]){
            m.setRegexObject(&ss_search_re[i])
             .setModifier(ss_mod[i]);
            if(m.match()){
                 if(inv) continue;
            } else if(!inv) continue;
        } else {
            if(stringContains(ss_mod[i],"i")) {
                if(stringContains(toLower(name),toLower(ss_search[i]))){
                    if(inv) continue;
                } else if(!inv) continue;
            }
            else{
                if(stringContains(name,ss_search[i])){
                    if(inv) continue;
                } else if(!inv) continue;
            }
        }
        return true;
    }
    
    return false;
}



#endif
