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
                if(CWD == oldn){
                    //undo depends on current directory, but the current directory itself has been renamed
                    String signature = getPathSignature(newn);
                    
                    //recreate lock files
                    RNM_LOCK_FILE = RNM_LOCK_FILE_BKP + signature;
                    openLockFile(futil::lock_op::ImmediateLock, true); //closing is done automatically
                    
                    //change original undo file names
                    RNM_FILE_LOG_L = RNM_FILE_LOG_L_BKP + signature;
                    RNM_FILE_LOG_R = RNM_FILE_LOG_R_BKP + signature;
                    
                }
                appendToRFLTMP(oldn,newn);
                current_index+=inc;
                reverse_index-=inc;
                di.directory_index+=inc;
                di.directory_reverse_index-=inc;
                rnc++;
                success=true;
            }
            else {printErrorLog(strerror(errno)+String(": ")+oldn);}
        } else {
            current_index+=inc;
            reverse_index-=inc;
            di.directory_index+=inc;
            di.directory_reverse_index-=inc;
            rnc++;
        }
    }
    return success;
}

void showUndoPaths(){
    std::cout<<"            AVAILABLE UNDO PATHS           \n";
    std::cout<<"                Time | Path                \n";
    std::cout<<"-------------------- | --------------------\n";
    FileArray files;
    getFilesFromDir(files, LOG_DIR_UNDO);
    std::sort(files.begin(),files.end(),[](const File& a, const File& b){return a.mtime<b.mtime;} );
    FileStream f;
    for(size_t i=0;i<files.size();++i){
        size_t pos=0;
        if((pos = (files[i].path).find(RNM_FILE_LOG_L_BKP))!= String::npos && pos==0){
            String fl;
            f.open(files[i].path, std::ios::in);
            if(f.good()) std::getline(f, fl, '\0');
            f.close();
            std::cout<<" "<<formatTime(files[i].mtime, "%d-%m-%Y %I:%M %p")<<" | "<<fl<<"\n\n";
        }
    }
}

void deleteUndoPathsFrom(Int sec){
    FileArray files;
    getFilesFromDir(files, LOG_DIR_UNDO);
    for(size_t i=0;i<files.size();++i){
        if(Int(time(0)-files[i].mtime) > sec){
            std::remove(files[i].path.c_str());
        }
    }
}


bool undoRename(){
    if(!undo_path_show){
        FileStream file_l,file_r;
        String l,r;
        StringArray left,right;
        DirectoryIndex di;
        String rnmf_l, rnmf_r;
        if(!undo_path.empty()){
            String signature = getPathSignature(undo_path);
            rnmf_l = RNM_FILE_LOG_L_BKP + signature;
            rnmf_r = RNM_FILE_LOG_R_BKP + signature;
        } else {
            rnmf_l = RNM_FILE_LOG_L;
            rnmf_r = RNM_FILE_LOG_R;
        }
        file_l.open(rnmf_l,std::ios::binary | std::ios::in);
        file_r.open(rnmf_r,std::ios::binary | std::ios::in);
        if(file_l.good() && file_r.good()){
            while(std::getline(file_l,l,'\0') && std::getline(file_r,r,'\0')){
                left.push_back(l);right.push_back(r);
            }
            file_l.close();file_r.close();
            std::reverse(left.begin(), left.end());
            std::reverse(right.begin(), right.end());
            size_t n = left.size();
            n = n>0?n-1:n;  //last line should not be taken, it's for storing signature path. (actually it was the first line).
            for(size_t i=0;i<n;++i){
                ///do rename and log into rfl
                if(!quiet&&!ALL_YES){std::cout<< NEW_LINE+right[i]+"    ---->    "+left[i]+NEW_LINE;}
                ///do rename
                int confirm;
                if(!all_yes){
                    TIME_COUNT += duration(timeNow() - START_TIME);
                    confirm=selectInput();
                    START_TIME = timeNow();
                    switch(confirm){
                        case 1:
                              Rename(right[i],left[i],di);
                              break;
                        case 2:
                              all_yes=true;
                              Rename(right[i],left[i],di);
                              break;
                        case 3:
                              break;
                        case 4:
                              all_yes=false;
                              Exit(0);
                        default:
                               all_yes=false;
                               break;
                    }
                } else {
                    Rename(right[i],left[i],di);
                }
            }
            return true;
        } else {
            printErrorLog("Undo failed. Required log files not found for undo path: "+ (undo_path.empty()?CWD:undo_path) + "\nRun 'rnm -ups' to see available undo paths");
            return false;
        }
    } else {
        showUndoPaths();
        return true;
    }
}


void updateIndexFlagsFromFlagMaps(){
    IFP=index_int_flag["precision"];
    index_field_length=index_int_flag["length"];
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


void parseIndexFlags(const String& s){
    bool er=true;
    StringArray tokens=split(s,FLAG_DELIM);
    for(int i=0;i<(int)tokens.size();i++){
        er=true;
        if(parseTwoStepIndexFlag(tokens[i])){
            er=false;
            continue;
        }
        
        if(existsInMap(INDEX_FLAGS_M, tokens[i])){
            INDEX_FLAGS |= INDEX_FLAGS_M[tokens[i]];
            er=false;
            continue;
        }
        if(er){printErrorLog("Invalid index flag: "+tokens[i]);Exit(1);}
    }
}

void parseSearchString(String ss, size_t index){
    if(!stringContains(ss,path_delim)){
        ss_search.push_back(ss);
        ss_mod.push_back("");
        jp::Regex re;
        if(!fixed_ss[index]){
            re.setPattern(ss)
              .resetErrors()
              .setJpcre2Option(jpcre2::JIT_COMPILE)
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
                validateModifier(mod, SS_MOD_ALL, "search string in " + ss);
                ss_mod.push_back(mod);
                if(!fixed_ss[index]){
                    re.setModifier(mod)
                      .setPattern(search)
                      .setJpcre2Option(jpcre2::JIT_COMPILE)
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


String processSize(const String& subj, const String& repl, const Double& size, const String& unit_s, const String& errm, char sanitize){
    String res = subj;
    Double unit;
    if(!unit_s.empty())
        unit = getNumberOrExit(errm, unit_s);
    else
        unit = 0;
    return replaceString(res, repl, sanitizeStringForRegex(getSizeByUnit(size, unit), sanitize));
}

///Info Name String Rule.
///This name string rule provides various information about a file: modification time, size, permission etc.
///The general format of this rule is: `/info-propertyname-format/`.
///This name string rule starts with `/info-` and ends with `/`.
///The second part of the string is the property name (access time, modification time, size etc..). Property names are case insensitive.
///The third part of the string is optional and used for pre-formatting the property in a desirable way.
///Property Name | Details | Format | Default format
///------------- | ------- | --------------- | --------
///mtime | Last file modification time (local time) | [Time format](http://man7.org/linux/man-pages/man3/strftime.3.html#DESCRIPTION) | %d-%m-%Y
///atime | Last file access time (local time) | [Time format](http://man7.org/linux/man-pages/man3/strftime.3.html#DESCRIPTION) | %d-%m-%Y
///ctime | Last status change time (local time) | [Time format](http://man7.org/linux/man-pages/man3/strftime.3.html#DESCRIPTION) | %d-%m-%Y
///mtime,GMT | Last file modification time (GMT) | [Time format](http://man7.org/linux/man-pages/man3/strftime.3.html#DESCRIPTION) | %d-%m-%Y
///atime,GMT | Last file access time (GMT) | [Time format](http://man7.org/linux/man-pages/man3/strftime.3.html#DESCRIPTION) | %d-%m-%Y
///ctime,GMT | Last status change time (GMT) | [Time format](http://man7.org/linux/man-pages/man3/strftime.3.html#DESCRIPTION) | %d-%m-%Y
///size | Size of the file | Divide by unit (a positive number, e.g: 1024) | Dynamically selected according to size
///blksize | Block size of filesystem I/O | Divide by unit (a positive number, e.g: 1024) | Dynamically selected according to size
///blocks | Number of 512B blocks | N/A
///perm | Permission of the file | 'oct' for octal permission, 'ls' for human readable permission scheme like the 'ls' command on Linux | oct
///uid | User ID of the owner | N/A
///gid | Group ID of the owner | N/A
///dev | Devic ID of the device containing the file | N/A
///inode | inode number | N/A
///mode | File mode | N/A
///nlink | Number of hard links | N/A
String processInfoNameStringRule(const String& ns, const File& file,const String& delim, const String& delim2,char sanitize){
    String res = ns;
    jp::VecNum v;
    jp::RegexMatch m;
    m.setNumberedSubstringVector(&v).setFindAll().setSubject(ns);
    if(delim == path_delim)
        m.setRegexObject(&multi_re_info1);
    else if(delim == second_delim)
        m.setRegexObject(&multi_re_info2);
    else
        errorExit("Delimiter mismatch during info name stirng parse, please file a bug report, run with -v option to see bug report url");
    m.match();
        
    String tot, prop, op;
    for(size_t i=0;i<v.size();++i){
        //~ std::cout<<"0: "<<v[i][0]<<"\t1: "<<v[i][1]<<"\t2: "<<v[i][2]<<"\n";
        tot = v[i][0];
        prop = toLower0(v[i][1]);
        op = v[i][2];
        
        if(!delim2.empty()){///This tells us to convert the delim based rules to delim2 based rules, no further processing
            res = replaceString(res, tot, replaceStringAll(tot,delim,delim2));
            continue;
        }
        
        if("mtime" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(formatTime(file.mtime, op.empty()?dtf:op), sanitize));
        } else if("mtime,gmt" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(formatTime(file.mtime, op.empty()?dtf:op, 'g'), sanitize));
        } else if("atime" == toLower0(prop)){
            res = replaceString(res, tot, sanitizeStringForRegex(formatTime(file.atime, op.empty()?dtf:op), sanitize));
        } else if("atime,gmt" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(formatTime(file.atime, op.empty()?dtf:op, 'g'), sanitize));
        } else if("ctime" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(formatTime(file.ctime, op.empty()?dtf:op), sanitize));
        } else if("ctime,gmt" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(formatTime(file.ctime, op.empty()?dtf:op, 'g'), sanitize));
        } else if("size" == prop){
            res = processSize(res, tot, file.size, op, "Unit of size", sanitize);
        } else if("blksize" == prop){
            res = processSize(res, tot, file.blksize, op, "Unit of blksize", sanitize);
        } else if("blocks" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(std::to_string(file.blocks), sanitize));
        } else if("perm" == prop){
            if("ls" == op)
                res = replaceString(res, tot, sanitizeStringForRegex(file.perm_ls, sanitize));
            else if("oct" == op)
                res = replaceString(res, tot, sanitizeStringForRegex(file.perm_oct, sanitize));
            else
                res = replaceString(res, tot, sanitizeStringForRegex(file.perm, sanitize));
        } else if("uid" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(file.uid, sanitize));
        } else if("gid" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(file.gid, sanitize));
        } else if("dev" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(file.dev, sanitize));
        } else if("inode" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(file.inode, sanitize));
        } else if("mode" == prop){
            res = replaceString(res, tot, sanitizeStringForRegex(file.mode, sanitize));
        } else if("nlink"){
            res = replaceString(res, tot, sanitizeStringForRegex(file.nlink, sanitize));
        } else {
            errorExit("Invalid info property name: "+prop);
        }
    }
    return res;
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



String processExtendedNameString_d(const String& ns,std::map<String,Double>& ns_rules, Int ifl,
                                   const String& delim, const String& delim2,char sanitize, bool ignore_err = false){
    ///_d stands for double
    String name=ns;
    Int base=NUM_BASE_DEFAULT;
    jp::VecNas vn;
    jp::RegexMatch m;
    m.setSubject(name).setFindAll().setNamedSubstringVector(&vn);
    if(delim == path_delim)
        m.setRegexObject(&multi_nre1).match();
    else if(delim == second_delim)
        m.setRegexObject(&multi_nre2).match();
    else
        errorExit("Delimiter mismatch during extended name string parse, please file a bug report, run with -v option to see bug report url");
        
    for(size_t i=0;i<vn.size();++i){
        String total = vn[i]["total"],rule = vn[i]["rule"], exn = vn[i]["exn"], exv = vn[i]["exv"];
        if(!existsInMap(ns_rules, vn[i]["rule"])) {
            if(!ignore_err){
                printErrorLog("Invalid name string rule '"+vn[i]["rule"]+"' in: "+total);
                Exit(1);
            } else continue;
        }
        if(!delim2.empty()){///This tells us to convert the delim based rules to delim2 based rules, no further processing
            name=replaceString(name,total,replaceStringAll(total,delim,delim2));
            continue;
        }
        if(exn=="b"){///Base conversion
            base=getPositiveIntOrExit("Base ",exv);
            if(base < 2 || base > 36) errorExit("Base must be in the range 2-36, given: "+base.get_str());
            String tmp = toString(ns_rules[rule],base.get_ui(), ifl,IFP,IFF,INDEX_FLAGS);
            tmp=sanitizeStringForRegex(tmp,sanitize);
            name=replaceString(name,total,tmp);
        } else if(exn=="s"){///scientific conversion
            String tmp = toString(ns_rules[rule],base.get_ui(), ifl,IFP,IFF,std::ios::scientific|INDEX_FLAGS);
            tmp=sanitizeStringForRegex(tmp,sanitize);
            name=replaceString(name,total,tmp);
        } else if(exn=="l"){///Latin conversion
            String tmp = toString(ns_rules[rule],base.get_ui(), ifl,IFP,IFF,INDEX_FLAGS,true);
            tmp=sanitizeStringForRegex(tmp,sanitize);
            name=replaceString(name,total,tmp);
        } else if(exn.empty()){
            String tmp = toString(ns_rules[rule],base.get_ui(), ifl,IFP,IFF,INDEX_FLAGS);
            tmp=sanitizeStringForRegex(tmp,sanitize);
            name=replaceString(name,total,tmp);
        }
    }
    return name;
}



String processExtendedPdNameStringRule(const String& ns, const File& file, const String& p_delim, const String& delim2,char sanitize){
    ///file must contain whole path.
    String name=ns;
    ///Let's first find pd_max and all the parent directory names
    StringArray pd_names=split(dirname(file.path),path_delim[0]);
    Int pd_max=pd_names.size()-1;
    String pd_max_s = pd_max.get_str();
    ///Reverse the order of pd_names so that the right most directory name comes in 0th index
    std::reverse(pd_names.begin(),pd_names.end());
    jp::VecNas vn;
    jp::RegexMatch m;
    m.setSubject(name).setFindAll().setNamedSubstringVector(&vn);
    if(p_delim == path_delim)
        m.setRegexObject(&multi_pdre1).match();
    else if(p_delim == second_delim)
        m.setRegexObject(&multi_pdre2).match();
    else
        errorExit("Delimiter mismatch during /pd/ name string parse, please file a bug report, run with -v option to see bug report url");
        
    for(size_t i=0;i<vn.size();++i){
        String pd_name_c;
        String total = vn[i]["total"],rule = vn[i]["rule"], si = vn[i]["si"], ei = vn[i]["ei"], delim=vn[i]["delim"];
        //~ std::cout<<"\ntotal: "<<total<<"\trule: "<<rule<<"\tsi: "<<si<<"\tei: "<<ei<<"\tdelim: "+delim+NEW_LINE;
        if(!delim2.empty()){///This tells us to convert the delim based rules to delim2 based rules, no further processing
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
        jp::Regex re("^"+sanitizeStringForRegex(CWD,'s')+"(/[\\s\\S]*|$)", 0, jpcre2::JIT_COMPILE);
        Int cd_max=split(CWD,path_delim[0]).size()-1;
        if(cd_max<0)cd_max=0;
        if(si=="e"){si=pd_max_s;}
        if(ei=="e"){ei=pd_max_s;}
        if(si == "w" || ei == "w"){
            if(re.match(file.path)){//temp match
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
        pd_name_c = sanitizeStringForRegex(pd_name_c, sanitize);
        name=replaceString(name,total,pd_name_c);
    }
    return name;
}



String parseNameString(const String& ns,const File& file,DirectoryIndex &di, const String& delim,
                         const String& delim2, char sanitize, bool ignore_err = false){
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
        ns_rules["l"]=Double(current_line);
    }
    std::map<String,String>ns_rules_s;
    ns_rules_s["fn"]=fname;
    ns_rules_s["n"]=fnamewe;
    ns_rules_s["e"]=ext;
    ns_rules_s["rn"]=rname;
    ns_rules_s["nsf"]=ns_name;
    //ns_rules_s["pd"]=CPDN;           ///This will be handled with extended function
    ns_rules_s["wd"]=CWDN;
    if(!ns.empty()){
        for(auto const& ent : ns_rules_s){
            ///ent.first is the key, ent.second is the value    
            if(!delim2.empty()){
                name = replaceStringAll(name,delim+ent.first+delim,delim2+ent.first+delim2);
                continue;
            }
            String tmp = ent.second;
            tmp=sanitizeStringForRegex(tmp,sanitize);
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
        
        ///process /info-../ 
        name = processInfoNameStringRule(name,file,delim,delim2,sanitize);
        
        ///for name string rules like /i-b16/, b16 stands for base 16
        ///this should be the last rules to parse
        name=processExtendedNameString_d(name,ns_rules,di.IFL,delim,delim2,sanitize,ignore_err);
    } else if(!ignore_err) {
        printErrorLog("Name String can not be empty");
        Exit(1);
    }
    return name;
}


bool isATotalMatchWithReplaceStringRegex(const String& s, const jp::VecNum& vn){
    String tot;
    for(size_t i=0;i<vn.size();++i) {
        tot += vn[i][0];
        //~ std::cout<<"\ntot: "<<tot<<"\tvn["<<i<<"][0]"<<vn[i][0]<<"\ts: "<<s<<"\n";
    }
    return (tot == s);
}

void parseReplaceString(const StringArray &rs,const File& file,DirectoryIndex &di){
    bool ns_used = false;
    String name;
    jp::VecNum vn;
    jp::RegexMatch rm(&multi_rre);
    rm.setSubject(&name)
      .setNumberedSubstringVector(&vn)
      .setFindAll(true);
    for(size_t i = 0;i < rs.size();i++) {
        name = ltrim(rs[i]);
        rm.match();
        if(name != "" && !isATotalMatchWithReplaceStringRegex(name, vn)){
            ///rs[i] may contain name string rules
            ///We must strip the first slash from it before sending it for name string processing.
            //~ std::cout<<name<<"\n";
            if(name[0] == '/') 
                name = name.substr(1, String::npos);
            else
                errorExit("Invalid replace string format: "+rs[i]);
            //~ std::cout<<name<<"\n";
            ///Now encode second_delim, we need to guard the second delim before we convert path_delim rules to second_delim rules
            name = encodeWithDelim(name, second_delim);
            ///convert path_delim rules to second_delim rule
            name = parseNameString(name, file, di, path_delim, second_delim, 0, true);
            ///Now, for a valid replace string rule, it must match the multi_re, otherwise throw an error.
            ///restore the slash at beginning
            name="/"+name;
            ns_used = true;
            //~ std::cout<<"###: "<<name;
            rm.match(); //perform the match again.
        }
        /// Carefull!!! must not use else if
        if(name!="" && isATotalMatchWithReplaceStringRegex(name, vn)){
            /// Populate rs_search, rs_replace and rs_mod with valid values
            String tot;
            for(size_t j = 0;j<vn.size();++j){
                String se, rep, mod;
                se = vn[j][1];
                rep = vn[j][2];
                mod = vn[j][3];
        //~ std::cout<<"\nse: "<<se<<"\trep: "<<rep<<"\tmod: "<<mod;
                ///do name string parse if ns was detected
                if(ns_used){
                    ///convert second delim to path_delim for ns rules
                    se = parseNameString(se,file,di,second_delim,path_delim, false);
                    rep = parseNameString(rep,file,di,second_delim,path_delim, false);
                    ///decode second delim
                    se=decodeWithDelim(se,second_delim);
                    rep=decodeWithDelim(rep,second_delim);
                    ///do the ns parsing
                    ///ns rules in se must be sanitized
                    se = parseNameString(se,file,di,path_delim,"",'s');
                    ///                                         ^must be empty
                    //~ std::cout<<"###: "<<se;
                    rep = parseNameString(rep,file,di,path_delim,"", 'r');
                    ///                                           ^must be empty
                }
                rs_search.push_back(se);
                rs_replace.push_back(processReplacementString(rep));
                validateModifier(mod, RR_MOD_ALL, "replace string in " + rs[i]);
                rs_mod.push_back(mod);
                
                
            }
        } else 
            errorExit("Invalid replace string format: "+rs[i]);
    }

}


String changeCaseAccordingToSS(String s,const String& search,const String& replace,const String& modifier,int user=0){
    if(replace!="\\c"&&replace!="\\C"){
        printWarningLog("Invalid case definition: "+replace+" Only \\c or \\C is allowed");
        return s;
    }
    
    jp::Regex re(search, modifier+"S");
    jp::RegexReplace rr(&re);
    rr.setSubject(&s)
      .setModifier(modifier);

    if(replace=="\\C"){
        s = rr.nreplace(jp::MatchEvaluator([](jp::NumSub m, void*, void*){ return toUpper(m[0]); }));
    }
    else if(replace=="\\c"){
        s = rr.nreplace(jp::MatchEvaluator([](jp::NumSub m, void*, void*){ return toLower(m[0]); }));
    } else {
        printErrorLog("Invalid replace string regex: "+search);
        Exit(1);
    }
    return s;
}

void processReplaceString(StringArray &rs,const File& file,DirectoryIndex &di){
    ///clear rs_* vectors. These are modified according to each file name and thus previous value can not be retained.
    rs_search.clear();rs_replace.clear();rs_mod.clear();
    parseReplaceString(rs,file,di);
    ///we now have valid rs_search, rs_replace and rs_mod
    rname=basename(file.path);
    for(size_t i=0;i<rs_search.size();i++){
        // check for f, d, l modifier
        if(!file.isValidWithMod(rs_mod[i])){
            // do not apply this regex on this file
            //printOutLog("This regex is not applicable to this file: " + file.path);
            continue;
        }
        if(stringContains(rs_replace[i],"\\c")||stringContains(rs_replace[i],"\\C")){
            rname=changeCaseAccordingToSS(rname,rs_search[i],rs_replace[i],rs_mod[i],1);
        }
        ///Add other specialized replace rules here.
        else {
            jp::Regex re(rs_search[i], rs_mod[i]+"S");
            jp::RegexReplace rr(&re);
            rname=rr.setSubject(rname).setModifier(rs_mod[i]).setReplaceWith(rs_replace[i]).replace();
        }
        ///Now a modified name rname is available
    }
}


void incrementReservedIndexes(DirectoryIndex &di){
        ///increment reserved indexes
        di.directory_index_rd+=inc;
        di.directory_reverse_index_rd-=inc;
        current_index_rd+=inc;
        reverse_index_rd-=inc;
}


//~ String parseTrueFalse(bool a){
    //~ if(a)return "true";
    //~ else return "false";
//~ }


File doRename(const File& file,DirectoryIndex &di){
    bool not_skipped=true;
    File file_to_return = file;
    
    if(isInvalidFile(file)){return file_to_return;}
    
    if(ss_search.size()!=0){
        if(!isComplyingToSearchString(file)){
            return file_to_return;
        }
    }
    String oldn=basename(file.path);
    String dir=dirname(file.path);
    String name;
    
    //process name string file and calculate ns_name
    if(!name_string_file.empty()){
        if(current_line_pos<nsflist.size()){
            current_line = lc_list[current_line_pos];
            ns_name=parseNameString(nsflist[current_line_pos],file,di,path_delim, "",0);
            current_line_pos++;
        } else {
            //file is out of names
            printWarningLog("Name string file ran out of names");
            Exit(0);
        }
    }
    
    if(replace_string.size()!=0){processReplaceString(replace_string,file,di);}
    
    if(!name_string.empty()){ //ns is the first priority
        name=parseNameString(name_string,file,di,path_delim, "",0);
    } else if(replace_string.size()!=0){ //rs is prioritized over ns/f
        name = rname;
    } else if(!name_string_file.empty()){
        name = ns_name;
    } else {
        printErrorLog("One of the options: -ns or -nsf or -rs is mandatory");
        Exit(1);
    }
    
    ///sanitize name by removing invalid name string rules and path delimiter
    name=removeInvalidNameStringRules(name);
    name=stripPathDelimiter(name);
    
    if(!name.empty()){
        if(!quiet&&!ALL_YES){std::cout<< NEW_LINE+file.path+"    ---->    [...]"+path_delim+name+NEW_LINE;}
        ///do rename
        int confirm;
        if(!all_yes){
            if(!single_mode){
                TIME_COUNT += duration(timeNow() - START_TIME);
                confirm=selectInput();
                START_TIME = timeNow();
            }
            else{confirm=1;}
            switch(confirm){
                case 1:
                      not_skipped=Rename(file.path,dir+path_delim+name,di);
                      break;
                case 2:
                      all_yes=true;
                      not_skipped=Rename(file.path,dir+path_delim+name,di);
                      break;
                case 3:
                      break;
                case 4:
                      all_yes=false;
                      Exit(0);
                default:
                       all_yes=false;
                       break;
            }
        }
        else{
            ///do rename finally
            not_skipped=Rename(file.path,dir+path_delim+name,di);
        }
    }
    else{
        printWarningLog("Name can not be empty, skipped. ("+file.path+")");not_skipped=false;
    }
    if(not_skipped) file_to_return=File(dir+path_delim+name);
    return file_to_return;
}




void startInDepthRenamingTaskOnDirectory(const String& dir,String base_dir=base_dir){
    FileArray files;
    getFilesFromDir(files,dir);
    directory_count++;
    DirectoryIndex di;
    
    for(size_t i=0;i<files.size();i++){
        if(di.directory_index>end_index && !infinite_end_index){continue;}
        String file=files[i].path;

        if(childDepth(base_dir,file)>depth && depth >=0){continue;}
        String parent="";
        String src_name="";
        if(!files[i]){printWarningLog("No such file or directory: "+file);continue;}
        src_name=basename(file);
        parent=dirname(file);
        CPDN=getParentDirectoryName(file);
    
        if(files[i].isDir()){
            if(exclude_directory){
                if(count_directory){incrementReservedIndexes(di);}
                continue;
            } else{
                if(!file_only&&!link_only){
                    files[i]=doRename(file,di);
                    incrementReservedIndexes(di);
                } else if(count_directory) incrementReservedIndexes(di);
                file = files[i].path;
                src_name=basename(file);
                parent=dirname(file);
                CPDN=getParentDirectoryName(file);
                if(childDepth(base_dir,file)<=depth || depth<0){
                    startInDepthRenamingTaskOnDirectory(file);
                }
            }
        }
        else if(files[i].isFile()){
            if(exclude_file){
                if(count_file) incrementReservedIndexes(di);
                continue;
            }else if(!directory_only&&!link_only){
                files[i]=doRename(file,di);
                incrementReservedIndexes(di);
            } else if(count_file){
                incrementReservedIndexes(di);
            }
        } else if(files[i].isLink()){
            if(exclude_link){
                if(count_link) incrementReservedIndexes(di);
                continue;
            } else if(!directory_only && !file_only){
                files[i]=doRename(file,di);
                incrementReservedIndexes(di);
            } else if(count_link) {
                incrementReservedIndexes(di);
            }
        } else {
            incrementReservedIndexes(di);
            printWarningLog("Not a valid file or directory "+files[i].path);
            continue;
        }
    }
}



void startTask(FileArray& files){
    
    directory_count++;
    DirectoryIndex di;

    for(size_t i=0;i<files.size();i++){
        String file=files[i].path;
        String parent="";
        String src_name="";
        if(!files[i]){printWarningLog("No such file or directory: "+files[i].path);continue;}
        src_name=basename(file);
        parent=dirname(file);
        CPDN=getParentDirectoryName(file);

        if(files[i].isDir()){
            base_dir=dirname(file);
            if(exclude_directory){
                if(count_directory){incrementReservedIndexes(di);}
                continue;
            } else {
                if(!file_only&&!link_only){
                    files[i]=doRename(file,di);
                    incrementReservedIndexes(di);
                } else if(count_directory) incrementReservedIndexes(di);
                file = files[i].path;
                src_name=basename(file);
                parent=dirname(file);
                CPDN=getParentDirectoryName(file);
                base_dir = file;
                if(childDepth(base_dir,file)<=depth || depth<0){
                    startInDepthRenamingTaskOnDirectory(file);
                }
            }
        } else if(files[i].isFile()){
            if(exclude_file){
                if(count_file) incrementReservedIndexes(di);
                continue;
            } else if(!directory_only&&!link_only){
                files[i]=doRename(file,di);
                incrementReservedIndexes(di);
            } else if(count_file){
                incrementReservedIndexes(di);
            }
        } else if(files[i].isLink()){
            if(exclude_link){
                if(count_link) incrementReservedIndexes(di);
                continue;
            } else if(!directory_only && !file_only){
                files[i]=doRename(file,di);
                incrementReservedIndexes(di);
            } else if(count_link) {
                incrementReservedIndexes(di);
            }
        } else {
            incrementReservedIndexes(di);
            printWarningLog("Not a valid file or directory: "+ files[i].path);
            continue;
        }
        
    }
}


#endif
