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



bool Rename(const String& oldn,const String& newn,DirectoryIndex &di){
    bool success=false;
    if(File(newn)){
        printWarningLog("A file or directory with new name exists: "+newn);
    }
    else {
        if(!simulation){
            
            if(rename(oldn.c_str(),newn.c_str()) == 0){
                appendToRFLTMP(oldn,newn);
                current_index+=inc;
                reverse_index-=inc;
                di.directory_index+=inc;
                di.directory_reverse_index-=inc;
                rnc++;
                success=true;
            }
            else {printErrorLog(strerror(errno));}
        }
        else {
            
                current_index+=inc;
                reverse_index-=inc;
                di.directory_index+=inc;
                di.directory_reverse_index-=inc;
                rnc++;
        }
    }
    return success;
}


bool undoRename(){
    FileStream file_l,file_r;
    String l,r;
    StringArray left,right;
    DirectoryIndex di;
    file_l.open(RNM_FILE_LOG_L,std::ios::binary | std::ios::in);
    file_r.open(RNM_FILE_LOG_R,std::ios::binary | std::ios::in);
    if(file_l.good() && file_r.good()){
    while(std::getline(file_l,l,'\0') && std::getline(file_r,r,'\0')){
        left.push_back(l);right.push_back(r);
        }
    file_l.close();file_r.close();
    for(size_t i=left.size()-1;i>=0;i--){
        ///do rename and log into rfl
        if(!quiet){std::cout<< NEW_LINE+right[i]+"    ---->    "+left[i]+NEW_LINE;}
        Rename(right[i],left[i],di);
        }
    
    return true;
    }
    
    else {
        printErrorLog("Undo failed. Required log files not found.");
        return false;
    }
    
}


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
        //logic:
        //we will let it pass if file is valid
        //otherwise we will skip the loop.
        if(!file.isValidWithMod(ss_mod[i])) continue;
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
        //file matches one of the search string, return true.
        return true;
    }
    //no search string matched
    return false;
}



String processExtendedNameString_d(const String& ns,std::map<String,Double>& ns_rules,Int ifl, const String& delim, const String& delim2,bool sanitize){
    ///_d stands for double
    String name=ns;
    int base=NUM_BASE_DEFAULT;
    jp::Regex multi_nre ("(?<total>"+delim+"(?<rule>-?\\w+)(-?|-(?<exn>[bsl])-?(?<exv>\\d+)?)?"+delim+")",0,jpcre2::JIT_COMPILE);
    jp::RegexMatch m(&multi_nre);
    jp::VecNas vn;
    size_t c = m.setSubject(name).setJpcre2Option(jpcre2::FIND_ALL).setNamedSubstringVector(&vn).match();
    if(c){
        for(size_t i=0;i<vn.size();++i){
            String total = vn[i]["total"],rule = vn[i]["rule"], exn = vn[i]["exn"], exv = vn[i]["exv"];
            if(!existsInMap(ns_rules, vn[i]["rule"])) {
                printErrorLog("Invalid name string rule '"+vn[i]["rule"]+"' in: "+total);
                Exit(1);
            }
            if(delim2!=""){///This tells us to convert the delim based rules to delim2 based rules, no further processing
                name=replaceString(name,total,replaceStringAll(total,delim,delim2));
                continue;
            }
            if(exn=="b"){///Base conversion
                base=std::stoi(exv);
                String tmp = toString(ns_rules[rule],base, index_field_length,0,IFF,INDEX_FLAGS);
                if(sanitize){tmp=sanitizeStringForRegex(tmp);}
                name=replaceString(name,total,tmp);
            } else if(exn=="s"){///scientific conversion
                String tmp = toString(ns_rules[rule],base, index_field_length,0,IFF,std::ios::scientific|INDEX_FLAGS);
                if(sanitize){tmp=sanitizeStringForRegex(tmp);}
                name=replaceString(name,total,tmp);
            } else if(exn=="l"){///Latin conversion
                String tmp = toString(ns_rules[rule],base, index_field_length,0,IFF,INDEX_FLAGS,true);
                if(sanitize){tmp=sanitizeStringForRegex(tmp);}
                name=replaceString(name,total,tmp);
            }
        }
    } else {
        printErrorLog("Invalid name string rule: "+ns);
        Exit(1);
    }
    return name;
}



String processExtendedPdNameStringRule(const String& ns, const File& file, const String& p_delim, const String& delim2,bool sanitize){
    ///file must contain whole path.
    String name=ns;
    ///Let's first find pd_max and all the parent directory names
    StringArray pd_names=split(dirname(file.path),path_delim[0]);
    Int pd_max=pd_names.size()-1;
    String pd_max_s = pd_max.get_str();
    ///Reverse the order of pd_names so that the right most directory name comes in 0th index
    std::reverse(pd_names.begin(),pd_names.end());
    jp::Regex multi_pdre("(?<total>"+p_delim+"(?<rule>pd)-?(?<si>\\d+|[ew])?(-(?<ei>\\d+|[ew]))?-?(?<delim>[^"+
                                                                p_delim+"]*)?"+p_delim+")",0,jpcre2::JIT_COMPILE);
    jp::RegexMatch m(&multi_pdre);
    jp::VecNas vn;
    size_t c = m.setSubject(name).setJpcre2Option(jpcre2::FIND_ALL).setNamedSubstringVector(&vn).match();
    if(c){
        for(size_t i=0;i<vn.size();++i){
            String pd_name_c;
            String total = vn[i]["total"],rule = vn[i]["rule"], si = vn[i]["si"], ei = vn[i]["ei"], delim=vn[i]["delim"];
            //~ std::cout<<"\ntotal: "<<total<<"\trule: "<<rule<<"\tsi: "<<si<<"\tei: "<<ei<<"\tdelim: "+delim+NEW_LINE;
            if(delim2!=""){///This tells us to convert the delim based rules to delim2 based rules, no further processing
                name=replaceString(name,total,replaceStringAll(total,p_delim,delim2));
                continue;
            }
            ///Trim left zeros from si and ei
            ///If there was only zeros in si or ei then both will be empty, make them 0
            if(!si.empty()){
                si = ltrim(si,"0");
                if(si.empty()) si="0"; //this should only be done if si was not empty
            }
            if(!ei.empty()){
                ei = ltrim(ei,"0");
                if(ei.empty()) ei="0"; //this should only be done if ei was not empty
            }
            jp::Regex re("^"+sanitizeStringForRegex(CWD)+"(/[\\s\\S]*|$)");
            Int cd_max=split(CWD,path_delim[0]).size()-1;
            if(cd_max<0)cd_max=0;
            if(si=="e"){si=pd_max_s;}
            if(ei=="e"){ei=pd_max_s;}
            if(si == "w" || ei == "w"){
                if(re.match(file.path)){
                    Int x = pd_max-cd_max;
                    String s = x.get_str();
                    if(si=="w"){si=s;}
                    if(ei=="w"){ei=s;}
                } else si=ei="-1";
            }
            ///Set overflowed range to pd_max.
            ///This should not be done unless user wants it to be this way.
            ///By default overflowed range will be replaced with empty string.
            //~ //if(si_int>pd_max){si_int=pd_max;}
            //~ //if(ei_int>pd_max){ei_int=pd_max;}
            ///Handle empty si and ei, this must be the end of si_int and ei_int modification
            if(si.empty())si="0";
            if(ei.empty())ei=si;
            
            //~ std::cout<<si<<ei;
            ///Get the range in integer
            Int siint(si);
            Int eiint(ei);
            //~ std::cout<< total+NEW_LINE<< si<<": "<<siint<<NEW_LINE<< ei<<": "<<eiint<<NEW_LINE<< delim+NEW_LINE<<pd_max<<NEW_LINE;
            ///Create a string combining all parent directory names with delims added
            if(siint<=eiint){
                for(Int i=siint;i<=eiint;i++){
                    ///if si ei both empty and delim is not empty, then it is an invalid rule
                    //~ if(si==""&&ei==""&&delim!="")continue;    ///must continue
                    if(i>pd_max || i<0)break;                 ///Overflow, break will suffice 
                    if(!pd_name_c.empty())pd_name_c+=delim+pd_names[i.get_ui()];
                    else pd_name_c+=pd_names[i.get_ui()];
                }
            }
            else{
                for(Int i=siint;i>=eiint;i--){
                    ///if si ei both empty and delim is not empty, then it is an invalid rule
                    //~ if(si==""&&ei==""&&delim!="")continue;    ///must continue
                    if(i>pd_max || i<0)continue;              ///Overflow, must continue
                    if(pd_name_c!="")pd_name_c+=delim+pd_names[i.get_ui()];
                    else pd_name_c+=pd_names[i.get_ui()];
                }
            }
            ///Finaly replace the pd rule with the newly created combined name
            if(sanitize){pd_name_c=sanitizeStringForRegex(pd_name_c);}
            name=replaceString(name,total,pd_name_c);
        }
    } else {
        printErrorLog("Invalid name string rule: "+ns);
        Exit(1);
    }
    return name;
}



String parseNameString(const String& ns,const File& file,DirectoryIndex &di, const String& delim,
                         const String& delim2, bool sanitize){
    String fname=basename(file.path);
    if(replace_string.size()==0){rname=fname;}
    String name=ns;
    String fnamewe=fileNameWithoutExtension(fname);
    String ext=fileExtension(fname);
    std::map<String,Double> ns_rules;
    ns_rules["i"]=current_index;
    ns_rules["-i"]=reverse_index;
    ns_rules["ir"]=current_index_rd;
    ns_rules["-ir"]=reverse_index_rd;
    ns_rules["id"]=di.directory_index;
    ns_rules["idr"]=di.directory_index_rd;
    ns_rules["-id"]=di.directory_reverse_index;
    ns_rules["-idr"]=di.directory_reverse_index_rd;
    ns_rules["dc"]=directory_count;
    if(name_string_file!=""){
        ns_rules["l"]=Double(current_line.get_ui());
        ns_rules["la"]=Double(current_abs_line.get_ui());
    }
    std::map<String,String>ns_rules_s;
    ns_rules_s["fn"]=fname;
    ns_rules_s["n"]=fnamewe;
    ns_rules_s["e"]=ext;
    ns_rules_s["rn"]=rname;
    //ns_rules_s["pd"]=CPDN;           ///This will be handled with extended function
    ns_rules_s["wd"]=CWDN;
    if(!ns.empty()){
        for(auto const& ent : ns_rules_s){
            ///ent.first is the key, ent.second is the value    
            if(delim2!=""){name=replaceStringAll(name,delim+ent.first+delim,delim2+ent.first+delim2);continue;}
            String tmp = ent.second;
            if(sanitize){tmp=sanitizeStringForRegex(tmp);}
            name=replaceStringAll(name,delim+ent.first+delim,tmp);
        }
        //~ for(auto const& ent : ns_rules){
            //~ ///ent.first is the key, ent.second is the value    
            //~ if(delim2!=""){name=replaceStringAll(name,delim+ent.first+delim,delim2+ent.first+delim2);continue;}
            //~ String tmp = toString(ent.second,index_field_length);
            //~ if(sanitize){tmp=sanitizeRegexString(tmp);}
            //~ name=replaceStringAll(name,delim+ent.first+delim,tmp);
        //~ }
        ///Process /pd/ along with extended pd name string rules
        name=processExtendedPdNameStringRule(name,file,delim,delim2,sanitize);  ///file must be the full path
        ///for name string rules like /i-b16/, b16 stands for base 16
        ///this should be the last rules to parse
        name=processExtendedNameString_d(name,ns_rules,index_field_length,delim,delim2,sanitize);
    } else {
        printErrorLog("Name String can not be empty");Exit(1);
    }
    return name;
}


#endif
