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
 
 

#include "prototypes.hpp"
#include "strnatcmp.hpp"


StringArray files;

bool undoRename(){
    FileStream file_l,file_r;
    String l,r;
    StringArray left,right;
    DirectoryIndex di;
    file_l.open(RNM_FILE_LOG_L,$binary | $read);
    file_r.open(RNM_FILE_LOG_R,$binary | $read);
    if(file_l.good() && file_r.good()){
    while(getLineFromFile(file_l,l,'\0') && getLineFromFile(file_r,r,'\0')){
        left.push_back(l);right.push_back(r);
        }
    file_l.close();file_r.close();
    for(Int i=(Int)left.size()-1;i>=0;i--){
        ///do rename and log into rfl
        if(!quiet){print NEW_LINE+right[i]+"    ---->    "+left[i]+NEW_LINE;}
        Rename(right[i],left[i],di);
        }
    
    return true;
    }
    
    else {
        printErrorLog("Undo failed. Required log files not found.");
        return false;
    }
    
}


void checkArgAvailability(const StringArray& sa,int i){
    if(i>=(int)sa.size()){
        printErrorLog("One or more argument/s missing");
        Exit(1);
    }
}


bool isInvalidFile(const String& file){
    bool status=false;
    StringArray invf={HOME,\
        path_delim+"bin",path_delim+"usr",path_delim+"usr"+path_delim+"bin",path_delim+"tmp",\
        path_delim+"dev",path_delim+"etc",path_delim+"proc",path_delim+"home",\
        path_delim+"boot",path_delim+"root",path_delim+"lib",path_delim+"media",path_delim+"opt",\
        path_delim+"srv",path_delim+"sbin",path_delim+"sys",path_delim+"var",path_delim+"include",\
        path_delim+"local",path_delim+"libexec",path_delim+"log",path_delim+"mail",path_delim+"spool",
        path_delim+"mnt",path_delim+"share",path_delim+"unix"};
    for(int i=0;i<(int)invf.size();i++){
        if(file==invf[i]){
            if(!force){
                status=true;printWarningLog("rename not permitted: "+file);
            }
        }
    }
    
    if(file==path_delim+"usr"+path_delim+"bin"+path_delim+"rnm"){status=true;printWarningLog("rename not permitted: "+file);}
    if(file==root_filesystem){status=true;printWarningLog("rename not permitted: "+file);}
    if(file==self_path){status=true;printWarningLog("rename not permitted: "+file);}
    if(file==RNM_FILE_LOG_L_TMP){status=true;printWarningLog("rename not permitted: "+file);}
    if(file==RNM_FILE_LOG_R_TMP){status=true;printWarningLog("rename not permitted: "+file);}
    if(file==LOG_DIR_PARENT){status=true;printWarningLog("rename not permitted: "+file);}
    if(file==LOG_DIR){status=true;printWarningLog("rename not permitted: "+file);}
    
    
    return status;
}


int selectInput(){
    String msg= "\nSelect action:\n\n\
    1. Confirm for this file only.\n\
    2. Confirm for all.\n\
    3. Skip this file.\n\
    4. Skip all and exit\n\n\
Your choice (#?): \
";

    int a=0;
    while(true){
        print msg;
        String s;
        getLine(s);
        Regex re("^[1-4]$");
        if(!regexMatch(s,re)){print "\nInvalid choice.\n";}
        else {a=stringTo<int>(s);break;}
    }
    return a;
}


String prepareLogDir(){
    mkdir(LOG_DIR_PARENT.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(LOG_DIR.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    FileStream file;
    file.open(RNM_FILE_LOG_L_TMP.c_str(),$write);
    if(!file.good()){printe strerror(errno);}
    file.close();
    file.open(RNM_FILE_LOG_R_TMP.c_str(),$write);
    if(!file.good()){printe strerror(errno);}
    file.close();
    return toString(strerror(errno));
}


bool Rename(const String& oldn,const String& newn,DirectoryIndex &di){
    bool success=false;
    if(isPathValid(newn)){
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


template<typename T>
String toString(T a){
    Stream ss;
    ss <<a;
    return ss.str();
}


template<typename T>
T stringTo(const String& s){
    T a;
    Stream ss;
    ss <<s;
    ss >>a;
    return a;
}


String reverseString(const String& s){
    String temp="";
    for(int i=s.length()-1;i>=0;i--){temp+=s[i];}
    return temp;}
    

String toLower(String s){
    for(int i=0;i<(int)s.length();i++){s[i]=tolower(s[i]);}
    return s;
}


String toUpper(String s){
    for(int i=0;i<(int)s.length();i++){s[i]=toupper(s[i]);}
    return s;
    }
    

String replaceString(String str,const String& replace,const String& with){
    std::size_t pos = str.find(replace);
    if (pos!=String::npos){
    return str.replace (pos,replace.length(),with);}
    else return str;
    
}


String replaceStringAll(String str, const String& replace, const String& with) {
   if(replace!=""){
   std::size_t pos = str.find(replace);
    while ((pos = str.find(replace, pos)) != String::npos) {
         str=str.replace(pos, replace.length(), with);
         pos += with.length();
    }
}
return str;
}


String replaceStringWithDelims(String s,const String& rep, const String& delim1,const String& delim2){
    String::size_type pos1=s.find_first_of(delim1);
    String::size_type pos2=s.find_first_of(delim2,pos1+1);
    if(pos1!=String::npos&&pos2!=String::npos)
    return s.replace(pos1,pos2-pos1+1,rep);
    else return s;
}
///Make an overload of the above function to set default parameter
String replaceStringWithDelims(String s,const String& rep, const String& delim){
    return replaceStringWithDelims(s,rep,delim,delim);
}


String replaceStringAllWithDelims(String s,const String& rep, const String& delim1, const String& delim2) {
    String::size_type pos1=s.find_first_of(delim1);
    String::size_type pos2=s.find_first_of(delim2,pos1+1);
    while ((pos1 = s.find_first_of(delim1,pos1)) != String::npos && (pos2 = s.find_first_of(delim2,pos1+1)) != String::npos) {
         s=s.replace(pos1,pos2-pos1+1,rep);
         pos1 += rep.length();
    }
return s;
}
///Make an overload of the above function to set default parameter
String replaceStringAllWithDelims(String s,const String& rep, const String& delim){
    return replaceStringAllWithDelims(s,rep,delim,delim);
}


String convertToLatinNumber(Int num){
    String res;
    Int latin_num[9];
    Int latin_fixed_value[9]={1000,500,100,50,10,9,5,4,1};
    String latin_fixed_char[9]={"m","d","c","l","x","ix","v","iv","i"};
    for(int i=0;i<9;i++){
        latin_num[i] = num / latin_fixed_value[i];
        num = num % latin_fixed_value[i];
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<latin_num[i];j++)res+=latin_fixed_char[i];
    }
    if(INDEX_FLAGS_I_B["uppercase"]){res=toUpper(res);}
    return res;
}


String toStringAccordingToMyConvention(int val){
    if(val==-1){return "unset";}
    else return toString(val);
   }


String toStringAccordingToMyConvention(double val){
    if(val==-1.0){return "unset";}
    else return toString(val);
   }


int countMatchInRegex(const String& s,const String& re){
 
    std::regex words_regex(re);
    auto words_begin = 
    std::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end = std::sregex_iterator();
 
    return std::distance(words_begin, words_end);

}


int countCharInString(String s, String delim){
    int count=0;
    String::size_type pos = s.find_first_of(delim);
    while ((pos = s.find_first_of(delim, pos)) != String::npos){
        count++;pos++;
    }
    return count;
}


int countMatchInString(String s, String match){
    int count=0;
    String::size_type pos = s.find(match);
    while ((pos = s.find(match, pos)) != String::npos){
        count++;pos+=match.length();
    }
    return count;
}


int countCharWithAlgorithm(String s, char delim){
    return std::count(s.begin(),s.end(),delim);
}


StringArray split(const String &text, char sep) {
    StringArray tokens;
    int start = 0, end = 0;
    while ((Int)(end = text.find(sep, start)) != (Int)String::npos) {
        String temp=text.substr(start, end - start);
        if(temp!="")tokens.push_back(temp);
        start = end + 1;
    }
    String temp=text.substr(start);
    if(temp!="")tokens.push_back(temp);
    return tokens;
}


String ltrim(String s,const String& delim){return s.erase(0,s.find_first_not_of(delim));}
String rtrim(String s,const String& delim){return s.erase(s.find_last_not_of(delim)+1);}
String trim(String s,const String& delim){return rtrim(ltrim(s,delim),delim);}


String trimZeroFromScientificNumber(const String& s){
    if(!stringContains(s,"."))return s;
    int pos = s.find_first_of("eE");
    if(pos!=(int)String::npos){
        return rtrim(rtrim(s.substr(0,pos),"0"),".")+s.substr(pos,1)+ltrim(s.substr(pos+1,String::npos),"0");
    }
    else return s;
}


String trimFloatingZero(const String& s){
    if(!stringContains(s,"."))return s;
    int pos = s.find_first_of("eE");
    if(pos!=(int)String::npos){
        return rtrim(rtrim(s.substr(0,pos),"0"),".")+s.substr(pos,1)+ltrim(s.substr(pos+1,String::npos),"0");
    }
    else return rtrim(rtrim(s,"0"),".");
}


void updateIndexFlagsFromFlagMaps(){
    IFP=index_int_flag["precision"];
    index_field_length=index_int_flag["width"];
    IFF=index_string_flag["filler"];
    latin_fall_back_threshold=index_int_flag["latin-fallback"];
}


bool parseTwoStepIndexFlag(const String& s){
    String key,val;
    int pos = s.find_first_of("=");
    if(pos!=(int)String::npos){
        key=s.substr(0,pos);
        val=s.substr(pos+1,String::npos);
        if(existsInMap(index_int_flag,key)){
            if(isPositiveInt(val)){
            index_int_flag[key]=stringTo<int>(val);
            updateIndexFlagsFromFlagMaps();
            return true;
            }
        }
        else if(existsInMap(index_string_flag,key)){
            if(isSingleCharacter(val)){
                index_string_flag[key]=val;
                updateIndexFlagsFromFlagMaps();
                return true;
                }
        }
    }
    return false;
}


//~ bool parseTwoStepIndexFlag(const String& s){
    //~ String res=trim(s);
    //~ Regex re ("(^[^=]+)=([^=]+$)");
    //~ RegexResult match;
    //~ if(std::regex_match(s,match,re)){
        //~ if(existsInMap(index_int_flag,String(match[1]))){
            //~ if(isPositiveInt(String(match[2]))){
            //~ index_int_flag[String(match[1])]=stringTo<int>(match[2]);
            //~ updateIndexFlagsFromFlagMaps();
            //~ return true;
            //~ }
        //~ }
        //~ else if(existsInMap(index_string_flag,String(match[1]))){
            //~ if(isSingleCharacter(String(match[2]))){
                //~ index_string_flag[String(match[1])]=String(match[2]);
                //~ updateIndexFlagsFromFlagMaps();
                //~ return true;
                //~ }
        //~ }
    //~ }
    //~ return false;}


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
    print "Width: "<<index_field_length<<NEW_LINE;
    print "Filler: "<<IFF<<NEW_LINE;
    print "Precision: "<<toStringAccordingToMyConvention(IFP)<<NEW_LINE;
    print "Adjust field: "+index_flag_adjust_s+NEW_LINE;
    print "Other flags: ";
    for(int i=0;i<(int)index_flag_ind_s.size();i++){
        print index_flag_ind_s[i];
        if(i<(int)index_flag_ind_s.size()-1){print ", ";}
        }
    print NEW_LINE;
    
    }


String convertBase(Double x,int base){
    if(base<NUM_BASE_MIN||base>NUM_BASE_MAX){base=NUM_BASE;}
    String res="";
    String res_left="",res_right="";
    String val="0123456789abcdefghijklmnopqrstuvwxyz";
    Double intp,fractp;
    int prec;
    if(IFP<0){prec=DoubleLimit::max_digits10+2;}
    else{prec=IFP;}
    fractp=std::modf(x, &intp);
    int i=0;
    do{
        int rem=(Int)intp%(Int)base;
        intp=Int((Int)intp/(Int)base);
        res_left+=val[rem];
        i++;}while(intp!=0);
    i=0;
    Double fractph;
    do{
        if((int)i>=(int)prec){break;}
        Double newp=fractp*base;
        Double newintp;
        fractph=fractp;
        fractp=std::modf(newp,&newintp);
        if(fractph<=fractp*1.01&&fractph>=fractp*0.99){
            if(IFP<0){if(i>3)break;}
            else if(i>IFP){break;}
        }
        res_right+=val[(Int)newintp];
        i++;}
    while(i<50 && fractp!=0);
        
    res_left=reverseString(res_left);
    if(res_right=="0"){res_right="";}
    for(int j=0;j<(int)index_flag_ind.size();j++){
        if(index_flag_ind[j]==std::ios::showbase){
            if(base==16)res_left="0x"+res_left;
            if(base==8)res_left="0"+res_left;
        }
        if(index_flag_ind[j]==std::ios::showpoint){
            for(int i=(int)res_right.length();i<IFP;i++){
                res_right+="0";
            }
        }
        if(index_flag_ind[j]==std::ios::uppercase){
            res_left=toUpper(res_left);res_right=toUpper(res_right);
        }
    }
    
    res=rtrim(res_left+"."+toString(res_right),".");
    return res;
}
    

String doubleToString(Double x, int ifl,int number_base,IOFormatFlag index_flag_float,bool latin){
    Stream buffer;
    String res;
    ///set float format
    
    buffer.setf(index_flag_float);
    
    
    ///set other flags
    for(int i=0;i<(int)index_flag_ind.size();i++){
        if(index_flag_ind[i]==std::ios::showpos){continue;}
        if(index_flag_ind[i]==std::ios::showbase){continue;}
        buffer.setf(index_flag_ind[i]);
        }
    
    ///set precision if given
    if(IFP>=0)buffer.precision(IFP);
    
    ///pass double to buffer if base is dec or pass int otherwise
    if(!latin){
        if(number_base!=10){buffer<<convertBase(x,number_base);}
        else{ buffer << x;}
    }
    else{
        if((Int)x<=latin_fall_back_threshold){
            buffer<<convertToLatinNumber((Int)round(x));
        }
        else{
            printWarningLog("Latin format is too large, falling back to decimal.");
            if(number_base!=10){buffer<<convertBase(x,number_base);}
            else{ buffer << x;}
        }
    }
    res= buffer.str();
    
    ///trim zeros from end 1.2000 and 1.2000e+32 1.2000E+32 etc...
    if(number_base==10&&IFP<0&&!latin&&stringContains(res,".")){
        res=trimFloatingZero(res);
    }
    
    if(!latin){
        for(int i=0;i<(int)index_flag_ind.size();i++){
            if(index_flag_ind[i]==std::ios::showpoint){
                if(!stringContains(res,"."))res+=".0";
                break;
            }
        }
    }
    return res;}


String toStringAccordingToIFL(Double index,int ifl,int number_base,IOFormatFlag index_flag_float,bool latin){
    bool negative_flag=false;
    if(index<0){index=fabs(index);negative_flag=true;}
    String res=doubleToString(index,ifl,number_base,index_flag_float,latin);
    Stream buffer;
    buffer.width(ifl);
    buffer.setf(index_flag_adjust);
    buffer.fill(IFF[0]);
    buffer <<res;
    if(negative_flag){
    res= "-"+buffer.str();}
    else {res= buffer.str();}
    
    for(int i=0;i<(int)index_flag_ind.size();i++){
        if(index_flag_ind[i]==std::ios::showpos&&!negative_flag){res="+"+res;}
    }
            
    return res;}
    
    
String changeCaseAccordingToSS(String s,const String& search,const String& replace,const String& modifier,int user=0){
    if(replace!="\\c"&&replace!="\\C"){printWarningLog("Invalid case definition: "+replace+" Only \\c or \\C is allowed");return s;}
    bool case_sensitive=true,global=false;
    String replaced_string=s;
    if(modifier=="gi" || modifier=="ig"){global=true;case_sensitive=false;}
    else if(modifier=="i"){case_sensitive=false;}
    else if(modifier=="g"){global=true;}
    RegexResult res;
    
    RegexIterator it_end;
    try {
        std::regex re (search);
        if(user==1){re=createRegex(search,case_sensitive);}
        else if(!case_sensitive){re= Regex (search, REGEX_DEFAULT | ICASE);}
        
        if(global){
            RegexIterator it(s.begin(), s.end(), re);
            
            while(it!=it_end){
                String match=it->str();++it;
                if(replace=="\\C"){
                    replaced_string=replaceString(replaced_string,match,toUpper(match));
                }
                else if(replace=="\\c"){
                    replaced_string=replaceString(replaced_string,match,toLower(match));
                }
            }
        }
        else{
            if(std::regex_search(replaced_string,res,re)){
                if(replace=="\\C"){
                    replaced_string=replaceString(replaced_string,res[0],toUpper(res[0]));
                }
                else if(replace=="\\c"){
                    replaced_string=replaceString(replaced_string,res[0],toLower(res[0]));
                }
            }
        }
    } 
    catch (std::regex_error& e) {
        printErrorLog("Invalid replace string regex: "+search);
        Exit(1);
    }
    return replaced_string;
}


String regexReplace(const String& s,const String& search,const String& replace,const String& modifier,int user){
    bool case_sensitive=true,global=false;
    String replaced_string=s;
    if(modifier=="gi" || modifier=="ig"){global=true;case_sensitive=false;}
    else if(modifier=="i"){case_sensitive=false;}
    else if(modifier=="g"){global=true;}
    
    try {
        std::regex re (search);
        if(user==1){re=createRegex(search,case_sensitive);}
        else if(!case_sensitive){re= Regex (search, REGEX_DEFAULT | ICASE);}
        
        if(global){
        replaced_string=std::regex_replace (s,re,replace,std::regex_constants::format_default);
        }
        else{
        replaced_string=std::regex_replace (s,re,replace,NON_RECURSIVE_REGEX_REPLACE);
        }
    } 
    catch (std::regex_error& e) {
      printErrorLog("Invalid replace string regex: "+search);
      Exit(1);
    }
    return replaced_string;
}

    
String stripPathDelimiter(const String& s){
    return replaceStringAll(s,path_delim,"");
}

    
void sortVector(StringArray &files){
    if(sort_type=="natural"){std::sort(files.begin(), files.end(), compareNat);}
    else if(sort_type=="general"){std::sort(files.begin(), files.end());}
    else if(sort_type=="none"){}
    else{std::sort(files.begin(), files.end(), compareNat);}
    
    ///reverse the sort if reverse_sort is true
    if(reverse_sort){std::reverse(files.begin(), files.end());}
}

    
String processReplacementString(String replace){
    /// \1 \2 etc will be converted to c++ backreference $1 $2
    /// & will be converted to c++ $& (a copy of the entire match)
    /// \p will be converted to c++ $` (prefix)
    /// \s will be converted to c++ $´ (suffix)
    /// $ will be converted to c++ $$
    /// \& will be &
    
    replace=replaceStringAll(replace,"$","$$");
    replace=replaceStringAll(replace,"&","$&");
    replace=replaceStringAll(replace,"\\&","&");
    replace=replaceStringAll(replace,"\\p","$`");
    replace=replaceStringAll(replace,"\\s","$'");
    replace=regexReplace(replace,"\\\\(\\d{1,2})","$$$1","g");
    replace=regexReplace(replace,"\\\\\\{(\\d)\\}","$$0$1","g");
    replace=regexReplace(replace,"\\\\\\{(\\d\\d)\\}","$$$1","g");
    
    
    ///Finally strip invalid namestring rules and slashes
    replace=removeInvalidNameStringRules(replace);
    replace=stripPathDelimiter(replace);
    
    return replace;
}
    
    
void processReplaceString(StringArray &rs,const String& file,DirectoryIndex &di){
    ///clear rs_* vectors. These are modified according to each file name and thus previous value can not be retained.
    rs_search.clear();rs_replace.clear();rs_mod.clear();
    parseReplaceString(rs,file,di);
    ///we now have valid rs_search, rs_replace and rs_mod
    rname=basename(file);
    for(Int i=0;i<(Int)rs_search.size();i++){
        if(stringContains(rs_replace[i],"\\c")||stringContains(rs_replace[i],"\\C")){
            rname=changeCaseAccordingToSS(rname,rs_search[i],rs_replace[i],rs_mod[i],1);
        }
        else {
            rname=regexReplace(rname,rs_search[i],rs_replace[i],rs_mod[i],1);
        }
        ///Now a modified name rname is available
    }
}
    

void parseReplaceString(StringArray &rs,const String& file,DirectoryIndex &di){
    String name;
    Regex multi_re ("\\s*"+path_delim+"([^"+path_delim+"]*?)"+path_delim+"([^"+path_delim+"]*?)"+path_delim+"\\s*([gi]{0,2})\\s*(;\\s*|$)");
    int subm[]={1,2,3,4};
    for(Int i=0;i<(Int)rs.size();i++){
        name=rs[i];
        if(!isComplyingToRegex(name,multi_re)){name=parseNameString( rs[i], file,di);}
        /// Carefull!!! must not use else if
        if(name!="" && isComplyingToRegex(name,multi_re)){
            /// Populate rs_search, rs_replace and rs_mod with valid values
            RegexTokenIterator end; ///default constructor=end of sequence
            RegexTokenIterator toit (name.begin(), name.end(), multi_re,subm);
            while (toit != end){
                String se, rep, mod;
                se=*toit++;rep=*toit++;mod=*toit++;toit++;
                rs_search.push_back(se);
                rs_replace.push_back(processReplacementString(rep));
                rs_mod.push_back(mod);
            }
        }
        else{
            printErrorLog("Invalid replace string format: "+rs[i]);
            Exit(1);
        }
    }

}


bool isComplyingToRegex(String& s,Regex &re){
    String total="";
    RegexIterator it(s.begin(), s.end(), re);
    RegexIterator it_end;
    while(it!=it_end){total+=it->str();++it;}
    if(s!=total){return false;}
    else {return true;}
    }


void parseSearchString(String ss,Int index){
    Regex multi_re("\\s*"+path_delim+"([^"+path_delim+"]*?)"+path_delim+"\\s*(i?)(\\s*;\\s*|$)");
    int subm[]={1,2,3};
    if(isComplyingToRegex(ss,multi_re)){
        RegexTokenIterator end; //default constructor=end of sequence
        RegexTokenIterator toit (ss.begin(), ss.end(), multi_re,subm);
        
        while (toit != end){
            ss_search.push_back(*toit++);
            ss_mod.push_back(*toit++);
            ss_fixed.push_back(fixed_ss[index]);
            toit++;
        }
    }
    else{
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


bool stringContains(String s1,const String& s2){
    if (s1.find(s2) != String::npos) {return true;}
    else return false;
}


bool isComplyingToSearchString(const String& file){
    String name=basename(file);
    for(Int i=0;i<(Int)ss_search.size();i++){
        bool case_sensitive=true;
        if(ss_mod[i]=="i"){case_sensitive=false;}
        if(!ss_fixed[i]){
            try {
                std::regex re;
                re=createRegex(ss_search[i], case_sensitive);
                if(std::regex_search(name,re)){return true;}
            } 
            catch (std::regex_error& e) {
                printErrorLog("Invalid search string regex: "+ss_search[i]);
                Exit(1);
            }
        }
        else{
            if(ss_mod[i]=="i"){
                if(stringContains(toLower(name),toLower(ss_search[i]))){return true;}
            }
            else{
                if(stringContains(name,ss_search[i])){return true;}
            }
        }
    }
    
    return false;
}


String removeInvalidNameStringRules(const String& ns){
    return replaceStringAllWithDelims(ns,"",path_delim);
}


template<typename T1, typename T2>
bool existsInMap(std::map<T1,T2> mymap, T1 key){
    if ( mymap.find(key) == mymap.end() ) return false;
    else return true;
}


String processExtendedNameString_d(String ns,std::map<String,Double>& ns_rules,int ifl){///_d stands for double
    String name=ns;
    int subm[]={0,1,2,3,4},base=NUM_BASE;
    Regex multi_re (""+path_delim+"(-?[^"+path_delim+"-]+)-([^"+path_delim+"])(\\d*)("+path_delim+")");
    RegexTokenIterator end; ///default constructor=end of sequence
    RegexTokenIterator toit (ns.begin(), ns.end(), multi_re,subm);
    while (toit != end){
        String tot,rulep, basenp,basep;
        tot=*toit++;rulep=*toit++;basenp=*toit++;basep=*toit++;toit++;
        ///print tot+NEW_LINE;print rulep+NEW_LINE;print basenp+NEW_LINE;print basep+NEW_LINE;
        if(basenp=="b"){///Base conversion
            if(isPositiveInt(basep)){
                base=stringTo<int>(basep);
                if(base>=NUM_BASE_MIN&&base<=NUM_BASE_MAX){
                    if(existsInMap(ns_rules,String(rulep))){
                        name=replaceString(name,tot,toStringAccordingToIFL(ns_rules[rulep],index_field_length,base));
                    }
                }
            }
        }
        else if(basenp=="s"){///scientific conversion
            if(basep==""){
                if(existsInMap(ns_rules,String(rulep))){
                    name=replaceString(name,tot,toStringAccordingToIFL(ns_rules[rulep],index_field_length,10,std::ios::scientific));
                }
            }
        }
        else if(basenp=="l"){///Latin conversion
            if(basep==""){
                if(existsInMap(ns_rules,String(rulep))){
                    name=replaceString(name,tot,toStringAccordingToIFL(ns_rules[rulep],index_field_length,10,std::ios::fixed,true));
                }
            }
        }
    }
return name;
}


String processExtendedPdNameStringRule(String ns, const String& file){///file must contain whole path.
    String name=ns;
    int subm[]={0,1,2,3,4};
    
    ///Let's first find pd_max and all the parent directory names
    StringArray pd_names=split(dirname(file),path_delim[0]);
    Int pd_max=(Int)(pd_names.size()-1);
    
    ///Reverse the order of pd_names so that the right most directory name comes in 0th index
    std::reverse(pd_names.begin(),pd_names.end());
    
    
    Regex multi_re (""+path_delim+"pd(\\d*|e)-?(\\d*|e)-?([^/]*)("+path_delim+")",ICASE);
    RegexTokenIterator end; ///default constructor=end of sequence
    RegexTokenIterator toit (ns.begin(), ns.end(), multi_re,subm);
    
    ///Iterate over the regex iterator to get all substrings
    while (toit != end){
        String tot,si,ei,delim;
        String pd_name_c="";
        
        ///Get the matches
        tot=*toit++;si=*toit++;ei=*toit++;delim=*toit++;toit++;
        
        //~ ///manage default delim
        //~ if(ei=="")delim=PD_DELIM;
        
        ///Trim left zeros from si and ei
        ///If there was only zeros in si or ei then both will be empty, make them 0
        if(si!=""){
            si=ltrim(si,"0");
            if(si=="")si="0";
        }
        
        if(ei!=""){
            ei=ltrim(ei,"0");
            if(ei=="")ei="0";
        }
        
        ///Get the range in integer
        Int si_int=stringTo<Int>(si);
        Int ei_int=stringTo<Int>(ei);
        
        if(toLower(si)=="e")si_int=pd_max;
        if(toLower(ei)=="e")ei_int=pd_max;
        
        ///Set overflowed range to pd_max.
        ///This should not be done unless user wants it to be this way.
        ///By default overflowed range will be replaced with empty string.
        //~ if(si_int>pd_max){si_int=pd_max;}
        //~ if(ei_int>pd_max){ei_int=pd_max;}
        
        
        
        ///Handle empty si and ei, this must be the end of si_int and ei_int modification
        if(si=="")si_int=0;
        if(ei=="")ei_int=si_int;
        
        
        
        //print tot+NEW_LINE;print si<<": "<<si_int<<NEW_LINE;print ei<<": "<<ei_int<<NEW_LINE;print delim+NEW_LINE<<pd_max<<NEW_LINE;
        
    
        ///Create a string combining all parent directory names with delims added
        
        if(si_int<=ei_int){
            for(Int i=si_int;i<=ei_int;i++){
                ///if si ei both empty and delim is not empty, then it is an invalid rule
                if(si==""&&ei==""&&delim!="")continue;    ///must continue
                if(i>pd_max)break;                        ///Overflow, break will suffice 
                if(pd_name_c!="")pd_name_c+=delim+pd_names[i];
                else pd_name_c+=pd_names[i];
            }
        }
        else{
            for(Int i=si_int;i>=ei_int;i--){
                ///if si ei both empty and delim is not empty, then it is an invalid rule
                if(si==""&&ei==""&&delim!="")continue;    ///must continue
                if(i>pd_max)continue;                    ///Overflow, must continue
                if(pd_name_c!="")pd_name_c+=delim+pd_names[i];
                else pd_name_c+=pd_names[i];
            }
        }
        ///Finaly replace the pd rule with the newly created combined name
        name=replaceString(name,tot,pd_name_c);
    }
return name;
}


String parseNameString(const String& ns,const String& file,DirectoryIndex &di){
    String fname=basename(file);
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
        ns_rules["l"]=(Double)current_line;
        ns_rules["la"]=(Double)current_abs_line;
    }
    
    
    
    std::map<String,String>ns_rules_s;
    ns_rules_s["fn"]=fname;
    ns_rules_s["n"]=fnamewe;
    ns_rules_s["e"]=ext;
    ns_rules_s["rn"]=rname;
    //ns_rules_s["pd"]=CPDN;           ///This will be handled with extended function
    ns_rules_s["wd"]=CWDN;
    
    
    if(ns!=""){
        
        for(auto const& ent : ns_rules_s){
            ///ent.first is the key, ent.second is the value    
            name=replaceStringAll(name,path_delim+ent.first+path_delim,ent.second);
        }
        
        
        for(auto const& ent : ns_rules){
            ///ent.first is the key, ent.second is the value    
            name=replaceStringAll(name,path_delim+ent.first+path_delim,toStringAccordingToIFL(ent.second,index_field_length));
        }
        
        ///for name string rules like /i-b16/, b16 stands for base 16
        name=processExtendedNameString_d(name,ns_rules,index_field_length);
        
        ///Process /pd/ along with extended pd name string rules
        name=processExtendedPdNameStringRule(name,file);  ///file must be the full path
        
        }
    else{
    printErrorLog("Name String can not be empty");Exit(1);
    }
    return name;
}


String appendToFile(const String& filename, const String& str){
    const char* name=filename.c_str();
    FileStream file;
    file.open(name,$append);
    file<<str;
    file.close();
    return strerror(errno);
}


String printErrorLog(String str){
    str="E: "+str;
    printe str+NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    return appendToFile(ERROR_LOG,str+"\t\t\t\t@"+dt);
    
}


String printWarningLog(String str){
    str="W: "+str;
    if(!quiet){printe str+NEW_LINE;}
    time_t now = time(0);
    char* dt = ctime(&now);
    return appendToFile(ERROR_LOG,str+"\t\t\t\t@"+dt)+appendToFile(OUT_LOG,str+"\t\t\t\t@"+dt);
    
}

    
String printOutLog(const String& str){
    if(!quiet)print NEW_LINE+str+NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    return appendToFile(OUT_LOG,str+"\t\t        @"+dt);

}


String appendToRFLTMP(const String& str1,const String& str2){
    
    const char* filename_l=RNM_FILE_LOG_L_TMP.c_str();
    const char* filename_r=RNM_FILE_LOG_R_TMP.c_str();
    FileStream file;
    file.open(filename_l,$binary | $append);
    file<<str1;file<<'\0';
    file.close();
    
    file.open(filename_r,$binary | $append);
    file<<str2;file<<'\0';
    file.close();
    finalizeRFL();
    return strerror(errno);

}


void finalizeRFL(){
    copyFile2(RNM_FILE_LOG_L_TMP,RNM_FILE_LOG_L);
    copyFile2(RNM_FILE_LOG_R_TMP,RNM_FILE_LOG_R);
}


String copyFile(const String& src, const String& dest){
    int outfd = open(dest.c_str(),O_RDWR);
    int infd = open(src.c_str(),O_RDWR);          
    struct stat stat_buf ;
    fstat(infd,&stat_buf);
    ssize_t size = sendfile(outfd,infd,0,stat_buf.st_size);
    return toString(size);
}


String copyFile2(const String& src,const String& dst){
    std::ifstream source(src, $binary);
    std::ofstream dest(dst, $binary);
    dest << source.rdbuf();
    source.close();
    dest.close();
    return strerror(errno);
}


bool isDir(const String& file){
    struct stat info;
    if( stat( file.c_str(), &info ) != 0 )return false;
    else if( S_ISDIR(info.st_mode) )return true;
    else return false;  
}


bool isFile(const String& file){
    struct stat info;
    if( stat( file.c_str(), &info ) != 0 )return false;
    else if( S_ISREG(info.st_mode) )return true;
    else return false;
}
    
    
bool isPathValid(const String& file){
    struct stat info;
    if( stat( file.c_str(), &info ) != 0 )return false;
    else if( S_ISREG(info.st_mode) || S_ISDIR(info.st_mode) )return true;
    else return false;
}


char * normalizePath(char* pwd, const char * src, char* res) {
	size_t res_len;
	size_t src_len = strlen(src);

	const char * ptr = src;
	const char * end = &src[src_len];
	const char * next;

	if (src_len == 0 || src[0] != '/') {
		// relative path
		size_t pwd_len;

		pwd_len = strlen(pwd);
		memcpy(res, pwd, pwd_len);
		res_len = pwd_len;
	} else {
		res_len = 0;
	}

	for (ptr = src; ptr < end; ptr=next+1) {
		size_t len;
		next = (char*)memchr(ptr, '/', end-ptr);
		if (next == NULL) {
			next = end;
		}
		len = next-ptr;
		switch(len) {
		case 2:
			if (ptr[0] == '.' && ptr[1] == '.') {
				const char * slash = (char*)memrchr(res, '/', res_len);
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


String getAbsolutePath(const String& x){
    char path[FILENAME_MAX+1];
    if(follow_symlink){
        realpath(x.c_str(),path);
        return toString(path);
    }
    else {
        return toString(normalizePath((char*)CWD.c_str(),(const char*)x.c_str(),path));
    }
}


String dirname(const String& file){
    std::size_t found = file.find_last_of(path_delim);
    return file.substr(0,found);
    
}


String basename(const String& file){
    std::size_t found = file.find_last_of(path_delim);
    return file.substr(found+1);
}


String getParentDirectoryName(const String& file){
    return basename(dirname(file));
}


String fileExtension(const String& file){
    String::size_type pos=file.find_last_of('.');
    if(pos!=String::npos&&pos!=0)return file.substr(pos+1);
    else return "";
}


String fileNameWithoutExtension(const String& file){
    String::size_type pos=file.find_last_of('.');
    if(pos!=String::npos&&pos!=0)return file.substr(0,pos);
    else return file;
}


bool isImmediateChild(const String& prevf,const String& newf){
        if(prevf==dirname(newf))return true;
        else return false; 
}


bool isChild(const String& parent,const String& child){
  std::size_t found = child.find(parent+path_delim);
  if (found!=String::npos && found==0) return true;
  else return false;
}


bool isChildDir(const String& parent,const String& child){
    if(isDir(child))return isChild(parent,child);
    else return isChild(parent,dirname(child));
}


Int childDepth(const String& parent,const String& child){
    String childstr=child;
    if(isChildDir( parent, child)){
        if(isDir(child)){
            childstr=replaceString(child,parent+path_delim,String(""));
        }
        else{
            childstr=replaceString(dirname(child),parent+path_delim,String(""));
        }
        return countMatchInRegex(childstr,path_delim+"?[^"+path_delim+"]+"+path_delim+"?");
    }
    else return LOWEST_DEPTH;
}


bool isInt(const String& s, int base){
   if(s.empty() || std::isspace(s[0])) return false ;
   char * p ;
   strtol(s.c_str(), &p, base) ;
   return (*p == 0) ;
}


bool isPositiveInt(const String& s, int base){
   if(s.empty() || std::isspace(s[0]) || s[0]=='-') return false ;
   char * p ;
   strtol(s.c_str(), &p, base) ;
   return (*p == 0) ;
}


bool isNegativeInt(const String& s, int base){
   if(s.empty() || std::isspace(s[0]) || s[0]!='-') return false ;
   char * p ;
   strtol(s.c_str(), &p, base) ;
   return (*p == 0) ;
}


bool isNumber(const std::string& s){
   if(s.empty() || std::isspace(s[0]) || std::isalpha(s[0])) return false ;
   char * p ;
   strtod(s.c_str(), &p) ;
   return (*p == 0) ;
}


//~ template<typename T>
//~ bool isNumber(T x){
    //~ String s;
    //~ Regex e ("^-?\\d*\\.?\\d+$");
    //~ Stream ss; 
    //~ ss << x;
    //~ ss >>s;
    //~ if (std::regex_match (s,e)) return true;
    //~ else return false;
//~ }
    
    
bool isPositiveNumber(const std::string& s){
   if(s.empty() || std::isspace(s[0]) || std::isalpha(s[0]) || s[0]=='-') return false ;
   char * p ;
   strtod(s.c_str(), &p) ;
   return (*p == 0) ;
}


//~ template<typename T>
//~ bool isPositiveNumber(T x){
    //~ String s;
    //~ Regex e ("^\\d*\\.?\\d+$");
    //~ Stream ss; 
    //~ ss << x;
    //~ ss >>s;
    //~ if (std::regex_match (s,e)) return true;
    //~ else return false;
//~ }



void mustBeANumber(const String& name,const String& x){
    if(!isNumber(x)){
       printErrorLog(name+" must be a valid number");
       Exit(1);
       }
}
    
    

void mustBeAPositiveNumber(const String& name,const String& extra,const String& x){
    if(!isPositiveNumber(x)){
       printErrorLog(name+" must be a valid positive number."+extra);
       Exit(1);
       }
}
    

void mustBeAnInteger(const String& name,const String& x){
    if(!isInt(x)){
       printErrorLog(name+" must be a valid integer");
       Exit(1);
       }
}


void mustBeAPositiveInteger(const String& name,const String& x){
    if(!isPositiveInt(x)){
       printErrorLog(name+" must be a valid positive integer.");
       Exit(1);
       }
}    


bool isSingleCharacter(const String& x){
    String s=toString(x);
    if(s.length()!=1)return false;
    else return true;
}
    
    
void mustBeAValidSingleCharacter(const String& name,const String& x){
    String s=toString(x);
    if(s.length()!=1){
        printErrorLog(name+" must be a valid single character");
        Exit(1);
        }
}
    

StringArray getLineFromFileAndReturnVector(const String& filename){
    StringArray list;
    String line;
    char delim='\n';
    FileStream file;
    file.open(filename,$read);
    if(!file.good()){printErrorLog("Couldn't open name string file: "+filename);Exit(1);}
    while(getLineFromFile(file,line,delim)){
        if ( line.size() && line[line.size()-1] == '\r' ) {line = line.substr( 0, line.size() - 1 );}
        if(line=="" || line == toString('\0')){continue;}
        list.push_back(line);
    }
    file.close();
    
    return list;
}


NameList getNameListFromFile(const String& filename,Int si,Int ei,int mod=1){
    NameList list;
    String line;
    Int lc=0,abslc=0;
    Int start=si,end=ei;
    if(si>ei && ei!=0){start=ei;end=si;}
    else if(ei==0 && !reverse_line ){start=si;end=INT_MAX;}
    else if(ei==0 && reverse_line){start=1;end=si;}
    char delim='\n';
    if(mod==0){delim='\0';}
    FileStream file;
    if(mod==1){file.open(filename,$read);}
    else {file.open(filename,$binary | $read);}
    if(!file.good()){printErrorLog("Couldn't open name string file: "+filename);Exit(1);}
    Int tmplinc=linc;
    while(getLineFromFile(file,line,delim) && lc<end){
        abslc++;
        if(mod==1){
            if ( line.size() && line[line.size()-1] == '\r' ) {
               line = line.substr( 0, line.size() - 1 );
            }
        }
        if(line=="" || line == toString('\0')){
            continue;}
        lc++;
        tmplinc--;
        
        if(lc>=start && lc<=end && tmplinc<=0){
            list[lc]=line;abslc_list[lc]=abslc;
            tmplinc=linc;
            
        }
        
    }
    file.close();
    
    return list;
}


String doRename(const String& file,DirectoryIndex &di){
    bool not_skipped=true;
    
    if(isInvalidFile(file)){return file;}
    
    if(ss_search.size()!=0){
        if(!isComplyingToSearchString(file)){
            return file;
        }
    }
    
    
    String oldn=basename(file);
    String dir=dirname(file);
    String name="";
    if(replace_string.size()!=0){processReplaceString(replace_string,file,di);}
    
    if(name_string!="" && name_string_file==""){
        name=parseNameString(name_string,file,di);
        
        
        }
    else if(name_string=="" && name_string_file!=""){
        
        if(nsflist[current_line]!=""){
            current_abs_line=abslc_list[current_line];
            name=parseNameString(nsflist[current_line],file,di);
            
            
            if(!reverse_line){current_line+=linc;}
            else{current_line-=linc;}
        
        }
    }
    else if(name_string!="" && name_string_file!=""){
        if(nsflist[current_line]!=""){
            current_abs_line=abslc_list[current_line];
            name=parseNameString(name_string,nsflist[current_line],di);
            
        
            if(!reverse_line){current_line+=linc;}
            else{current_line-=linc;}
        
        }
    }
    else if(replace_string.size()!=0){name=rname;}
        
    else {printErrorLog("One of the options: -ns or -nsf or -rs is mandatory");Exit(1);}
    
    
    ///sanitize name by removing invalid name string rules and path delimiter
    name=removeInvalidNameStringRules(name);
    name=stripPathDelimiter(name);
    
    
    if(name!=""){
        if(!quiet){print NEW_LINE+file+"    ---->    "+dir+path_delim+name+NEW_LINE;}
        ///do rename
        int confirm;
        
        if(!all_yes){
            if(!single_mode){confirm=selectInput();}
            else{confirm=1;}
            
            switch(confirm){
                case 1:
                      not_skipped=Rename(file,dir+path_delim+name,di);
                      break;
                case 2:
                      all_yes=true;
                      not_skipped=Rename(file,dir+path_delim+name,di);
                      break;
                case 3:
                      break;
                case 4:
                      all_yes=false;
                      showResult();
                      Exit(0);
                default:
                       all_yes=false;
                       break;
            }
            
        }
        else{
            ///do rename finally
            not_skipped=Rename(file,dir+path_delim+name,di);
            
        }
        
    }
    else{
        printWarningLog("Name can not be empty, skipped.");not_skipped=false;
    }
    

  if(name_string_file!=""){
    if(current_line<=0){showResult();Exit(1);}
      
    if(line_upward && end_line!=0){
        if(current_line>end_line){printOutLog("End line reached.");showResult();Exit(1);}
    }
    else if(end_line==0){
        if(current_index_rd>=nsflist.size()){printWarningLog("Name string file ran out of names.");showResult();Exit(1);}
        
        }
    else{
        if(current_line<end_line){printOutLog("End line reached");
            showResult();
            Exit(1);
        }
    }
  }
    String filename_to_return="";
    if(not_skipped){filename_to_return=dir+path_delim+name;}
    else{filename_to_return=file;}
    
    return filename_to_return;
}


StringArray getFilesFromDir(const String& file){
                StringArray files;
                DIR *dir;
                struct dirent *ent;
                if ((dir = opendir (file.c_str())) != NULL) {
                    while ((ent = readdir (dir)) != NULL) {
                        
                        String filename=file+path_delim+ent->d_name;
                        if(basename(filename)=="." || basename(filename)==".."){continue;}
                        files.push_back(filename);
                    }
                    closedir (dir);
                } 
                else {
                    printErrorLog((String(strerror(errno))+": "+file).c_str());
                }
                sortVector(files);
                return files;
    
}


void startInDepthRenamingTaskOnDirectory(const String& dir,String base_dir=base_dir){
    StringArray files;
    files=getFilesFromDir(dir);
    
    directory_count++;
    DirectoryIndex di;
    
    for(Int i=0;i<(Int)files.size();i++){
        if(fabs(di.directory_index)>fabs(end_index)){continue;}
        String file=files[i];

        if(childDepth(base_dir,file)>depth){continue;}
        String parent="";
        String src_name="";
        if(!isPathValid(file)){printWarningLog("No such file or directory: "+file);continue;}
        file=getAbsolutePath(file);
        src_name=basename(file);
        parent=dirname(file);
        CPDN=getParentDirectoryName(file);
    
        if(isDir(file)){
            
            if(file_only){
                
                if(childDepth(base_dir,file)<=depth){
                    
                    startInDepthRenamingTaskOnDirectory(file);
                }
                else{
                
                }
                if(count_directory){incrementReservedIndexes(di);}
                
            }
            
            else if(exclude_directory){
                if(count_directory){incrementReservedIndexes(di);}
                continue;
            }
            
            else{
                
                files[i]=doRename(file,di);
                incrementReservedIndexes(di);
                file=getAbsolutePath(files[i]);
                src_name=basename(file);
                parent=dirname(file);
                CPDN=getParentDirectoryName(file);
                if(childDepth(base_dir,file)<=depth){
                    
                    startInDepthRenamingTaskOnDirectory(file);
                }
            }
            
        }
        else if(isFile(file)){
            
            if(!directory_only){
                files[i]=doRename(file,di);
                incrementReservedIndexes(di);
            }
            else if(count_file){incrementReservedIndexes(di);}
        }
        else {
            incrementReservedIndexes(di);
            printWarningLog("Not a valid file or directory");
            continue;
        }
    }
}


void incrementReservedIndexes(DirectoryIndex &di){
        ///increment reserved indexes
        di.directory_index_rd+=inc;
        di.directory_reverse_index_rd-=inc;
        current_index_rd+=inc;
        reverse_index_rd-=inc;
}


void startTask(StringArray& files){
    
    directory_count++;
    DirectoryIndex di;

    for(Int i=0;i<(Int)files.size();i++){
        String file=files[i];
        String parent="";
        String src_name="";
        if(!isPathValid(file)){printWarningLog("No such file or directory: "+file);continue;}
        file=getAbsolutePath(file);
        src_name=basename(file);
        parent=dirname(file);
        CPDN=getParentDirectoryName(file);

        if(isDir(file)){
            base_dir=dirname(file);
        
            if(file_only){

                if(childDepth(base_dir,file)<=depth){

                    startInDepthRenamingTaskOnDirectory(file);
                }
                else{
                
                
                }
                if(count_directory){incrementReservedIndexes(di);}
                
            }
            
            else if(exclude_directory){
                if(count_directory){incrementReservedIndexes(di);}
                continue;
            }
            
            else{
                
                files[i]=doRename(file,di);
                incrementReservedIndexes(di);
                file=getAbsolutePath(files[i]);
                src_name=basename(file);
                parent=dirname(file);
                CPDN=getParentDirectoryName(file);
                base_dir=dirname(file);
                if(childDepth(base_dir,file)<=depth){
                    
                    startInDepthRenamingTaskOnDirectory(file);
                }
            }
            
            
        }
        else if(isFile(file)){
            
            if(!directory_only){
                files[i]=doRename(file,di);
                incrementReservedIndexes(di);
            }
            else if(count_file){incrementReservedIndexes(di);}
     
        }
        else {
            incrementReservedIndexes(di);
            printWarningLog("Not a valid file or directory");
            continue;
        }
        
    }
}


String parseTrueFalse(bool a){
    if(a)return "true";
    else return "false";
}


void detectLineUpwardOrDownward(){
    if(start_line<=end_line || end_line==0){line_upward=true;}
    else{line_upward=false;reverse_line=true;}
    if(reverse_line){current_line=(start_line>end_line)?start_line:end_line;}
    }


Regex createRegex(const String& s, bool case_sensitive){
    if(re_locale){
        if(!case_sensitive){
            return Regex(s,REGEX_TYPE | REGEX_LOCALE | ICASE);
            }
        else{
            return Regex(s,REGEX_TYPE | REGEX_LOCALE);
            }
        }
    else {
        if(!case_sensitive){
            return Regex(s,REGEX_TYPE | REGEX_LOCALE | ICASE);
            }
        else{
            return Regex(s,REGEX_TYPE | REGEX_LOCALE);
            }
        }
}


void printOpts(){
    
    print "\n\nInfo about this session:\n\
    Executable: "+self_path+"\n\
    Name String: " +name_string+"\n\
    Name String File: " +name_string_file+"\n\
    Fixed Search String: " +parseTrueFalse(ss_fixed[0])+"\n\
    Replace String (first): " +replace_string[0]+"\n\
    Replace String search part (first): "+rs_search[0]+"\n\
    Replace String replace part (first): "+rs_replace[0]+"\n\
    Replace String modifier part (first): "+rs_mod[0]+"\n\
    Regex Type: "+re_type+"\n\
    Regex Locale: "+parseTrueFalse(re_locale)+"\n\
    Depth: "+toString(depth)+"\n\
    Input Field Length: "+toString(index_field_length)+"\n\
    Undo: "+parseTrueFalse(undo)+"\n\
    Start Index: "+toString(start_index)+"\n\
    End Index: "+toString(end_index)+"\n\
    Start Line: "+toString(start_line)+"\n\
    End Line: "+toString(end_line)+"\n\
    Reverse Line: "+parseTrueFalse(reverse_line)+"\n\
    Quiet: "+parseTrueFalse(quiet)+"\n\
    File Only: "+parseTrueFalse(file_only)+"\n\
    Directory Only: "+parseTrueFalse(directory_only)+"\n\
    Exclude Directory: "+parseTrueFalse(exclude_directory)+"\n\
    Count Directory (force): "+parseTrueFalse(count_directory)+"\n\
    Count File (force): "+parseTrueFalse(count_file)+"\n\
    Increment Value: "+toString(inc)+"\n\
    Line Increment Value: "+toString(linc)+"\n\
    Apply force: "+parseTrueFalse(force)+"\n\
";
    printIndexFlags();
    print "Simulation: "+parseTrueFalse(simulation)+"\n\n";
    
    }


void showResult(){
    if(!quiet){printOutLog(toString(rnc)+" file/s renamed");if(simulation)print " (simulation)"+NEW_LINE;}
    if(show_options){printOpts();}
    
}


