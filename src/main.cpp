﻿
#include "classes.hpp"

/// Function prototypes
template<typename T>
String toString(T);
template<typename T>
T stringTo(String s);
String toLower(String);
String prepareLogDir();
template<typename T>
String appendToFile(T);
String printOutLog(String);
String printErrorLog(String);
void printOpts(void);
void startTask(StringArray);
String replaceStringAll(String, String , String );
String parseNameString(String,String,DirectoryIndex &);
String regexReplace(String ,String ,String ,String,int );
void parseReplaceString(StringArray ,String,DirectoryIndex &);
String copyFile(String, String );
String copyFile2(String src,String dst);
void finalizeRFL();
bool undoRename();
bool Rename(String,String,DirectoryIndex &);
String printWarningLog(String str);
bool isPathValid(String file);
String appendToRFLTMP(String str1,String str2);
void showResult(void);
void parseSearchString(String ss,Int index);
String basename(String file);
String dirname(String file);
String fileNameWithoutExtension(String filename);
String fileExtension(String filename);
void checkArgAvailability(StringArray sa,int i);
bool stringContains(String main,String sub);
template<typename T>
void mustBeAPositiveNumber(String name,String extra,T x);
template<typename T>
void mustBeAnInteger(String name,T x);
template<typename T>
void mustBeAPositiveInteger(String name,T x);
template<typename T>
void mustBeAValidSingleCharacter(String name,T x);
template<typename T>
void mustBeANumber(String name,T x);
bool isFile(String file);
NameList getNameListFromFile(String filename,int si,int ei,int mod);
Regex createRegex(String s,bool case_sensitive);
void incrementReservedIndexes(DirectoryIndex &di);
bool isComplyingToRegex(String &s,Regex &re);
bool Rename2(String oldn,String newn);


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


void checkArgAvailability(StringArray sa,int i){
    if(i>=(int)sa.size()){
        printErrorLog("One or more argument/s missing");
        Exit(1);
    }
}

bool isInvalidFile(String file){
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
Your choice: \
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


bool Rename(String oldn,String newn,DirectoryIndex &di){
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
T stringTo(String s){
    T a;
    Stream ss;
    ss <<s;
    ss >>a;
    return a;
}



String toLower(String s){
    for(int i=0;i<(int)s.length();i++){s[i]=tolower(s[i]);}
    return s;
}

String toUpper(String s){
    for(int i=0;i<(int)s.length();i++){s[i]=toupper(s[i]);}
    return s;
    }

String replaceString(String str,String replace,String with){
    std::size_t pos = str.find(replace);
    if (pos!=std::string::npos){
    return str.replace (pos,replace.length(),with);}
    else return str;
    
}

String replaceStringAll(String str, String replace, String with) {
   if(replace!=""){
   std::size_t pos = str.find(replace);
    while ((pos = str.find(replace, pos)) != std::string::npos) {
         str.replace(pos, replace.length(), with);
         pos += with.length();
    }
}
return str;
}

int countMatchInRegex(std::string s,std::string re){
 
    std::regex words_regex(re);
    auto words_begin = 
    std::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end = std::sregex_iterator();
 
    return std::distance(words_begin, words_end);

}


String toStringAccordingToIFL(Double index,int ifl){
    bool negative_flag=false;
    if(index<0){index=fabs(index);negative_flag=true;}
    Stream buffer;
    buffer.width(ifl);
    buffer.fill(IFF[0]);
    buffer.precision(IFP);
    buffer <<index;
    if(negative_flag){
    return "-"+buffer.str();}
    else return buffer.str();}
    
    
String changeCaseAccordingToSS(String s,String search,String replace,String modifier,int user=0){
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
                    replaced_string=replaceStringAll(replaced_string,match,toUpper(match));
                }
                else if(replace=="\\c"){
                    replaced_string=replaceStringAll(replaced_string,match,toLower(match));
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


String regexReplace(String s,String search,String replace,String modifier,int user=0){
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
    
String stripPathDelimiter(String s){
    s=replaceStringAll(s,path_delim,"");
    
    return s;
}
    
bool compareNat(const std::string& a, const std::string& b){
    if (a.empty())
        return true;
    if (b.empty())
        return false;
    if (std::isdigit(a[0]) && !std::isdigit(b[0]))
        return true;
    if (!std::isdigit(a[0]) && std::isdigit(b[0]))
        return false;
    if (!std::isdigit(a[0]) && !std::isdigit(b[0]))
    {
        if (a[0] == b[0])
            return compareNat(a.substr(1), b.substr(1));
        return (toUpper(a) < toUpper(b));
        //toUpper() is a function to convert a std::string to uppercase.
    }

    // Both strings begin with digit --> parse both numbers
    std::istringstream issa(a);
    std::istringstream issb(b);
    int ia, ib;
    issa >> ia;
    issb >> ib;
    if (ia != ib)
        return ia < ib;

    // Numbers are the same --> remove numbers and recurse
    std::string anew, bnew;
    std::getline(issa, anew);
    std::getline(issb, bnew);
    return (compareNat(anew, bnew));
}
    
void sortVector(StringArray &files){
    if(sort_type=="natural"){std::sort(files.begin(), files.end(), compareNat);}
    else if(sort_type=="general"){std::sort(files.begin(), files.end());}
    else if(sort_type=="none"){}
    else{std::sort(files.begin(), files.end(), compareNat);}
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
    
    
    ///Finally strip slashes
    replace=stripPathDelimiter(replace);
    return replace;
}
    
    
void processReplaceString(StringArray rs,String file,DirectoryIndex &di){
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
    

void parseReplaceString(StringArray rs,String file,DirectoryIndex &di){
    String name;
    Regex multi_re ("\\s*"+path_delim+"([^"+path_delim+"]*?)"+path_delim+"([^"+path_delim+"]*?)"+path_delim+"\\s*([gi]{0,2})\\s*(;\\s*|$)");
    int subm[]={1,2,3,4};
    for(Int i=0;i<(Int)rs.size();i++){
        name=rs[i];
        if(!isComplyingToRegex(name,multi_re)){name=parseNameString( rs[i], file,di);}
        /// Carefull!!! must not use else if
        if(name!="" && isComplyingToRegex(name,multi_re)){
            /*re ="^"+path_delim+"([^"+path_delim+"]*)"+path_delim+"[^"+path_delim+"]*"+path_delim+"[gi]{0,2}$";
            if(std::regex_match(name,result,re)){rs_search=result[1];}
            re ="^"+path_delim+"[^"+path_delim+"]*"+path_delim+"([^"+path_delim+"]*)"+path_delim+"[gi]{0,2}$";
            if(std::regex_match(name,result,re)){rs_replace=result[1];rs_replace=processReplacementString(rs_replace);}
            re ="^"+path_delim+"[^"+path_delim+"]*"+path_delim+"[^"+path_delim+"]*"+path_delim+"([gi]{0,2})$";
            if(std::regex_match(name,result,re)){rs_mod=result[1];}
            */
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

bool isComplyingToRegex(String &s,Regex &re){
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
        /*re="^"+path_delim+"([^"+path_delim+"]*)"+path_delim+"i?$";
        if(std::regex_match(ss,result,re)){ss_search=result[1];}
        re="^"+path_delim+"[^"+path_delim+"]*"+path_delim+"(i?)$";
        if(std::regex_match(ss,result,re)){ss_mod=result[1];}
        * */
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

bool stringContains(String s1,String s2){
    if (s1.find(s2) != String::npos) {return true;}
    else return false;
}

bool isComplyingToSearchString(String file){
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




String parseNameString(String ns,String file,DirectoryIndex &di){
    String fname=basename(file);
    if(replace_string.size()==0){rname=fname;}
    String name=ns;
    String fnamewe=fileNameWithoutExtension(fname);
    String ext=fileExtension(fname);
    if(ns!=""){
        name=replaceStringAll(name,path_delim+"fn"+path_delim,fname);
        name=replaceStringAll(name,path_delim+"n"+path_delim,fnamewe);
        name=replaceStringAll(name,path_delim+"e"+path_delim,ext);
        name=replaceStringAll(name,path_delim+"rn"+path_delim,rname);
        name=replaceStringAll(name,path_delim+"i"+path_delim,toStringAccordingToIFL(current_index,index_field_length));
        name=replaceStringAll(name,path_delim+"-i"+path_delim,toStringAccordingToIFL(reverse_index,index_field_length));
        name=replaceStringAll(name,path_delim+"ir"+path_delim,toStringAccordingToIFL(current_index_rd,index_field_length));
        name=replaceStringAll(name,path_delim+"-ir"+path_delim,toStringAccordingToIFL(reverse_index_rd,index_field_length));
        name=replaceStringAll(name,path_delim+"id"+path_delim,toStringAccordingToIFL(di.directory_index,index_field_length));
        name=replaceStringAll(name,path_delim+"idr"+path_delim,toStringAccordingToIFL(di.directory_index_rd,index_field_length));
        name=replaceStringAll(name,path_delim+"-id"+path_delim,toStringAccordingToIFL(di.directory_reverse_index,index_field_length));
        name=replaceStringAll(name,path_delim+"-idr"+path_delim,toStringAccordingToIFL(di.directory_reverse_index_rd,index_field_length));
        name=replaceStringAll(name,path_delim+"dc"+path_delim,toString(directory_count));
        name=replaceStringAll(name,path_delim+"pd"+path_delim,CPDN);
        name=replaceStringAll(name,path_delim+"wd"+path_delim,CWDN);
        
        
        if(name_string_file!=""){
            name=replaceStringAll(name,path_delim+"l"+path_delim,toStringAccordingToIFL((Double)current_line,index_field_length));
            name=replaceStringAll(name,path_delim+"la"+path_delim,toStringAccordingToIFL((Double)current_abs_line,index_field_length));
        }
        
        
        }
    else{
    printErrorLog("Name String can not be empty");Exit(1);
    }
    return name;
}



template<typename T>
String appendToFile(T filename, String str){
    const char* name=toString(filename).c_str();
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

    
String printOutLog(String str){
    if(!quiet)print NEW_LINE+str+NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    return appendToFile(OUT_LOG,str+"\t\t        @"+dt);

}

String appendToRFLTMP(String str1,String str2){
    
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


String copyFile(String src, String dest){
    int outfd = open(dest.c_str(),O_RDWR);
    int infd = open(src.c_str(),O_RDWR);          
    struct stat stat_buf ;
    fstat(infd,&stat_buf);
    ssize_t size = sendfile(outfd,infd,0,stat_buf.st_size);
    return toString(size);
}

String copyFile2(String src,String dst){
    std::ifstream source(src, $binary);
    std::ofstream dest(dst, $binary);
    dest << source.rdbuf();
    source.close();
    dest.close();
    return strerror(errno);
}


bool isDir(String file){
    struct stat info;

    if( stat( file.c_str(), &info ) != 0 ){
        return false;
    }
    
    else if( S_ISDIR(info.st_mode) ) {
            return true;
        
    
    }
    else{
        
        return false;
        }
    
    
}
    
bool isFile(String file){
    struct stat info;

    if( stat( file.c_str(), &info ) != 0 ){
        return false;
    }
    else if( S_ISREG(info.st_mode) ) {
        return true;
    }
    else{
        
        return false;
        }
    
    
}
    
bool isPathValid(String file){
    struct stat info;

    if( stat( file.c_str(), &info ) != 0 ){
        return false;
    }
    else if( S_ISREG(info.st_mode) || S_ISDIR(info.st_mode) ) {
        return true;
    }
    else{
        return false;
        }
    
    
}

String getAbsolutePath(String x){
    char path[FILENAME_MAX+1];
    realpath(x.c_str(),path);
    return toString(path);
    
}

String dirname(String file){
    
    std::size_t found = file.find_last_of(path_delim);
    return file.substr(0,found);
    
}

String basename(String file){
    
    std::size_t found = file.find_last_of(path_delim);
    return file.substr(found+1);
    
}

String getParentDirectoryName(String file){
    return basename(dirname(file));
}

String fileExtension(std::string file){
    
    Regex re(".*[^\\.]+\\.([^\\.]+$)");
    RegexResult result;
    if(std::regex_match(file,result,re))return result[1];
    else return "";
    
}

String fileNameWithoutExtension(std::string file){
  
    Regex re("(.*[^\\.]+)\\.[^\\.]+$");
    RegexResult result;
    if(std::regex_match(file,result,re))return result[1];
    else return file;
    
}


bool isImmediateChild(String prevf,String newf){
    

        if(prevf==dirname(newf)){
            return true;
            }
        else return false;
    
    
}

bool isChild(String parent,String child){
  std::size_t found = child.find(parent+path_delim);
  if (found!=std::string::npos && found==0) return true;
  else return false;


}

bool isChildDir(String parent,String child){
    if(isDir(child)){
        return isChild(parent,child);
    }
    else{
        return isChild(parent,dirname(child));
    }
    
}

Int childDepth(String parent,String child){
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


bool isInt(String x){
    Regex e ("^-?\\d+");
    if (std::regex_match (x,e)) return true;
    else return false;
}

bool isPositiveInt(String x){
    Regex e ("^\\d+");
    if (std::regex_match (x,e)) return true;
    else return false;}

template<typename T>
bool isNumber(T x){
    String s;
    Regex e ("^-?\\d*\\.?\\d+");
    Stream ss; 
    ss << x;
    ss >>s;
    if (std::regex_match (s,e)) return true;
    else return false;
}
    
template<typename T>
bool isPositiveNumber(T x){
    String s;
    Regex e ("^\\d*\\.?\\d+");
    Stream ss; 
    ss << x;
    ss >>s;
    if (std::regex_match (s,e)) return true;
    else return false;
}

template<typename T>
void mustBeANumber(String name,T x){
    if(!isNumber(x)){
       printErrorLog(name+" must be a valid number");
       Exit(1);
       }
}
    
template<typename T>
void mustBeAPositiveNumber(String name,String extra,T x){
    if(!isPositiveNumber(x)){
       printErrorLog(name+" must be a valid positive number."+extra);
       Exit(1);
       }
}
    
    
template<typename T>
void mustBeAPositiveInteger(String name,T x){
    if(!isPositiveInt(x)){
       printErrorLog(name+" must be a valid positive integer.");
       Exit(1);
       }
}    
    
    
template<typename T>
void mustBeAValidSingleCharacter(String name,T x){
    String s=toString(x);
    if(s.length()!=1){
        printErrorLog(name+" must be a valid single character");
        Exit(1);
        }
}
    
template<typename T>
void mustBeAnInteger(String name,T x){
    if(!isInt(x)){
       printErrorLog(name+" must be a valid integer");
       Exit(1);
       }
}

StringArray getLineFromFileAndReturnVector(String filename){
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


NameList getNameListFromFile(String filename,Int si,Int ei,int mod=1){
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



String doRename(String file,DirectoryIndex &di){
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
    
    
    
    
    ///sanitize name
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


StringArray getFilesFromDir(String file){
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


void startInDepthRenamingTaskOnDirectory(String dir,String base_dir=base_dir){
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

        ///increment reserved indexes
        /*di.directory_index_rd+=inc;
        di.directory_reverse_index_rd-=inc;
        current_index_rd+=inc;
        reverse_index_rd-=inc;
        */
    
    }
    
    
}

void incrementReservedIndexes(DirectoryIndex &di){
        ///increment reserved indexes
        di.directory_index_rd+=inc;
        di.directory_reverse_index_rd-=inc;
        current_index_rd+=inc;
        reverse_index_rd-=inc;
    
}


void startTask(StringArray files){
    
    directory_count++;
    DirectoryIndex di;
    
    if((Int)files.size()>0){CWDN=getParentDirectoryName(getAbsolutePath(files[0]));}
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
        
        ///increment reserved indexes
        /*di.directory_index_rd+=inc;
        di.directory_reverse_index_rd-=inc;
        current_index_rd+=inc;
        reverse_index_rd-=inc;*/
        
    
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


Regex createRegex(String s, bool case_sensitive){
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
    Simulation: "+parseTrueFalse(simulation)+"\n\n\
";
    
    }



void showResult(){
    if(!quiet){printOutLog(toString(rnc)+" file/s renamed");if(simulation)print " (simulation)"+NEW_LINE;}
    if(show_options){printOpts();}
    
}




int main(int argc, char* argv[]) {getCurrentDir(self_dir);self_path=self_dir+String(path_delim)+executable_name;
    prepareLogDir();
    
    

    
    //////////////////////////////// Opt parse////////////////////////////////////////////
    
    StringArray args;
    for(int i = 1; i < argc; i++){args.push_back(argv[i]);}
    
    bool skipcount=false;
    bool noopt=false;
  { //Scope for the following objects
    Options inc_obj("-inc","--increment-value");
    Options linc_obj("-linc","--line-increment-value");
    Options si_obj("-i","-si","--index","--start-index");
    Options ei_obj("-ei","--end-index");
    Options ifl_obj("-ifl","--index-field-length");
    Options ifp_obj("-ifp","--index-field-precision");
    Options iff_obj("-iff","--index-field-filler");
    Options ns_obj("-ns","--name-string");
    Options nsf_obj("-ns/f","--name-string-file");
    Options sl_obj("-l","-sl","--line","--start-line");
    Options el_obj("-el","--end-line");
    Options ss_obj("-ss","--search-string");
    Options ss_file_obj("-ss/f","--search-string-file");
    Options rs_obj("-rs","--replace-string");
    Options rs_file_obj("-rs/f","--replace-string-file");
    Options re_obj("-re","--regex");
    Options dp_obj("-dp","--depth");
    
    for(int i = 0; i < (int)args.size(); i++){
      if(skipcount){skipcount=false;continue;}
        
      String opt=toLower(String(args[i]));
      
     if(!noopt){
      if(opt=="-h"||opt=="--help"){
          printe help_message;
          return 0;
        }
      else if(opt=="-v"||opt=="--version"){
          printe version_info;
          return 0;
        }
        
      else if(opt=="-q"||opt=="--quiet"){
          quiet=true;
          
        }
      
      else if(opt=="-fo"||opt=="--file-only"){
          file_only=true;
          
        }
      
      else if(opt=="-do"||opt=="--directory-only"){
          directory_only=true;
        
        }
        
      else if(opt=="-ed"||opt=="--exclude-directory"){
          exclude_directory=true;
          
        }
        
      else if(opt=="-inc"||opt=="--increment-value"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveNumber("Increment Value","\nNegative increment i.e decrement will be available using name string rule:\n\
"+path_delim+"-i"+path_delim+", "+path_delim+"-ir"+path_delim+", "+path_delim+"-id"+path_delim+" etc..\n",args[i+1]);
          inc=stringTo<decltype(inc)>(args[i+1]);
          skipcount=true;
          
          inc_obj.count++;
          if(inc_obj.count>1){printWarningLog("Increment value overwritten");}
        }
        
      else if(opt=="-linc"||opt=="--line-increment-value"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Line Increment Value",args[i+1]);
          linc=stringTo<decltype(linc)>(args[i+1]);
          skipcount=true;
          
          linc_obj.count++;
          if(linc_obj.count>1){printWarningLog("Line increment value overwritten");}
        }
        
        
      else if(opt=="-i"||opt=="-si"||opt=="--index"||opt=="--start-index"){
          checkArgAvailability(args,i+1);
          mustBeANumber("Start Index",args[i+1]);
          start_index=stringTo<decltype(start_index)>(args[i+1]);
          current_index=start_index;
          current_index_rd=start_index;
          reverse_index=start_index;
          reverse_index_rd=start_index;
          DIRECTORY_INDEX=start_index;
          DIRECTORY_REVERSE_INDEX=start_index;
          skipcount=true;
          
          si_obj.count++;
          if(si_obj.count>1){printWarningLog("Start index overwritten");}
        }
        
      else if(opt=="-ei"||opt=="--end-index"){
          checkArgAvailability(args,i+1);
          mustBeANumber("End Index",args[i+1]);
          end_index=stringTo<decltype(end_index)>(args[i+1]);
          skipcount=true;
          
          ei_obj.count++;
          if(ei_obj.count>1){printWarningLog("End index overwritten");}
        }
        
      else if(opt=="-ifl"||opt=="--index-field-length"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Index field length",args[i+1]);
          index_field_length=stringTo<decltype(index_field_length)>(args[i+1]);
          skipcount=true;
          
          ifl_obj.count++;
          if(ifl_obj.count>1){printWarningLog("Index field length overwritten");}
        }
        
      else if(opt=="-ifp"||opt=="--index-field-precision"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Index field precision",args[i+1]);
          IFP=stringTo<decltype(IFP)>(args[i+1]);
          skipcount=true;
          
          ifp_obj.count++;
          if(ifp_obj.count>1){printWarningLog("Index field precision overwritten");}
        }
        
      else if(opt=="-iff"||opt=="--index-field-filler"){
          checkArgAvailability(args,i+1);
          mustBeAValidSingleCharacter("Index field filler",args[i+1]);
          IFF=stringTo<decltype(IFF)>(args[i+1]);
          IFF=replaceStringAll(IFF,"\\","");
          skipcount=true;
          
          iff_obj.count++;
          if(iff_obj.count>1){printWarningLog("Index field filler overwritten");}
        }
      
      else if(opt=="-ns"||opt=="--name-string"){
          checkArgAvailability(args,i+1);
          name_string=args[i+1];
          skipcount=true;
          
          ns_obj.count++;
          if(ns_obj.count>1){printWarningLog("Name string overwritten");}
        }
        
        
      else if(opt=="-ns/f"||opt=="--name-string-file"){
          checkArgAvailability(args,i+1);
          name_string_file=args[i+1];
          skipcount=true;
          
          nsf_obj.count++;
          if(nsf_obj.count>1){printErrorLog("Only one name string file is allowed");Exit(1);}
        }
        
        
      else if(opt=="-ns/fn"||opt=="--name-string-file-null-terminated"){
          checkArgAvailability(args,i+1);
          name_string_file=args[i+1];
          nsf_n=true;
          skipcount=true;
          
          nsf_obj.count++;
          if(nsf_obj.count>1){printErrorLog("Only one name string file is allowed");Exit(1);}
        }
        
      else if(opt=="-l"||opt=="-sl"||opt=="--line"||opt=="--start-line"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Start Line",args[i+1]);
          start_line=stringTo<decltype(start_line)>(args[i+1]);
          current_line=start_line;
          skipcount=true;
          
          sl_obj.count++;
          if(sl_obj.count>1){printWarningLog("Start line overwritten");}
        }
        
      else if(opt=="-lv"||opt=="-slv"||opt=="--line-reverse"||opt=="--start-line-reverse"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Start Line",args[i+1]);
          start_line=stringTo<decltype(start_line)>(args[i+1]);
          current_line=start_line;
          skipcount=true;
          reverse_line=true;
          
          sl_obj.count++;
          if(sl_obj.count>1){printWarningLog("Start line overwritten");}
        }
        
      else if(opt=="-el"||opt=="--end-line"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("End Line",args[i+1]);
          end_line=stringTo<decltype(end_line)>(args[i+1]);
          skipcount=true;
          
          el_obj.count++;
          if(el_obj.count>1){printWarningLog("End line overwritten");}
        }
      
      else if(opt=="-elv"||opt=="--end-line-reverse"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("End Line",args[i+1]);
          end_line=stringTo<decltype(end_line)>(args[i+1]);
          skipcount=true;
          reverse_line=true;
          
          el_obj.count++;
          if(el_obj.count>1){printWarningLog("End line overwritten");}
        }
        
      else if(opt=="-ss"||opt=="--search-string"){
          checkArgAvailability(args,i+1);
          search_string.push_back(args[i+1]);
          fixed_ss.push_back(false);
          skipcount=true;
          
          ss_obj.count++;
          ///if(ss_obj.count>1){printWarningLog("Search string overwritten");}
        }
        
        
      else if(opt=="-ss/f"||opt=="--search-string-file"){
          checkArgAvailability(args,i+1);
          search_string_file=args[i+1];
          skipcount=true;
          
          if(isFile(search_string_file)){
            StringArray temp=getLineFromFileAndReturnVector(search_string_file);
            ///append temp to search_string
            search_string.insert(search_string.end(),temp.begin(),temp.end());
            ///Populate fixed_ss with same number of boolean value
            for(Int i=0;i<(Int)temp.size();i++){
                fixed_ss.push_back(false);
                }
          }
          else{
            printErrorLog("Search string file not found: "+search_string_file);
            Exit(1);
          }
          ss_file_obj.count++;
          //if(ss_file_obj.count>1){printWarningLog("Search string file option changed");}
        }
        
         
      else if(opt=="-ssf"||opt=="--search-string-fixed"){
          checkArgAvailability(args,i+1);
          search_string.push_back(args[i+1]);
          fixed_ss.push_back(true);
          skipcount=true;
          
          ss_obj.count++;
          if(ss_obj.count>1){printWarningLog("Search string overwritten");}
        }
        
          
      else if(opt=="-ssf/f"||opt=="--search-string-fixed-file"){
          checkArgAvailability(args,i+1);
          search_string_file=args[i+1];
          skipcount=true;
          if(isFile(search_string_file)){
            StringArray temp=getLineFromFileAndReturnVector(search_string_file);
            ///append temp to search_string
            search_string.insert(search_string.end(),temp.begin(),temp.end());
            ///Populate fixed_ss with same number of boolean value
            for(Int i=0;i<(Int)temp.size();i++){
                fixed_ss.push_back(true);
                }
          }
          else{
            printErrorLog("Search string file not found: "+search_string_file);
            Exit(1);
          }
          ss_file_obj.count++;
          //if(ss_file_obj.count>1){printWarningLog("Search string file option changed");}
        }
        
        
      else if(opt=="-rs"||opt=="--replace-string"){
          checkArgAvailability(args,i+1);
          replace_string.push_back(args[i+1]);
          skipcount=true;
          
          rs_obj.count++;
          //if(rs_obj.count>1){printWarningLog("Replace string overwritten");} //this is invalid from version 3.1.0
           
        }
        
        
      else if(opt=="-rs/f"||opt=="--replace-string-file"){
          checkArgAvailability(args,i+1);
          replace_string_file=args[i+1];
          skipcount=true;
          if(isFile(replace_string_file)){
              StringArray temp=getLineFromFileAndReturnVector(replace_string_file);
            replace_string.insert(replace_string.end(),temp.begin(),temp.end());
          }
          else{
            printErrorLog("Replace string file not found: "+replace_string_file);
            Exit(1);
          }
          rs_file_obj.count++;
          //if(rs_file_obj.count>1){printWarningLog("Replace string file option changed");}
        }
        
      else if(opt=="-re" || opt == "--regex"){
          checkArgAvailability(args,i+1);
          re_type=toLower(args[i+1]);
          skipcount=true;
          
          re_obj.count++;
          if(re_obj.count>1){printWarningLog("Regex mode overwritten");}
        }
        
        else if(opt=="-rel"||opt=="--regex-locale"){
          re_locale=true;
          
        }
        
       
      else if(opt=="-dp"||opt=="--depth"){
          checkArgAvailability(args,i+1);
          mustBeAnInteger("Depth",args[i+1]);
          depth=stringTo<decltype(depth)>(args[i+1]);
          if(depth<0){depth=std::numeric_limits<Int>::max();}
          skipcount=true;
          
          dp_obj.count++;
          if(dp_obj.count>1){printWarningLog("Directory depth overwritten");}
        }
        
        
      else if(opt=="-cd"||opt=="--count-directory"){
          count_directory=true;
          
        }
        
        
      else if(opt=="-cf"||opt=="--count-file"){
          count_file=true;
          
        }
        
        
      else if(opt=="-s"||opt=="--sort"){
          sort=true;
          sort_type="default";
          
        }
        
        
      else if(opt=="-s/g"||opt=="--sort/g"){
          sort=true;
          sort_type="general";
        }
        
        
      else if(opt=="-s/n"||opt=="--sort/n"){
          sort=true;
          sort_type="natural";
        }
        
      else if(opt=="-s/none"||opt=="--sort/none"){
          sort=true;
          sort_type="none";
        }
        
      else if(opt=="-y"||opt=="--yes"){
          all_yes=true;
          
        }
        
        
      else if(opt=="-shop"||opt=="--show-options"){
          show_options=true;
          
        }
        
      else if(opt=="-f"||opt=="--force"){
          force=true;
          
        }
        
      else if(opt=="-u"||opt=="--undo"){
          undo=true;
          
        }
        
      else if(opt=="-sim"||opt=="--simulation"){
          simulation=true;
          
        }
        
        
      else if(opt=="--"){
          noopt=true;
          
        }
        
        
      else if(opt[0]=='-'){
          printErrorLog("Invalid Option: "+opt);
          Exit(1);
          
        }
        
        else {
            files.push_back(String(args[i]));
            
            
        }
        
     }
        
     else {
            files.push_back(String(args[i]));
            
            
     }

      
      
    }
  } //objects from Options class are destroyed here
    
    //file_vector=files;
    //////////////////////////////////////// Opt parse ends here/////////////////////////////////

    if(undo){undoRename();showResult();Exit(0);}

    
    
    /////////////////////////////////// Various checks///////////////////////////////////////
    
    if(file_only&&directory_only){printErrorLog("File Only and Directory Only, both can't be true at the same time");Exit(1);}
    if(file_only&&exclude_directory){printWarningLog("File Only mode is prioritized over Exclude Directory mode");}
    if(simulation&&quiet){printWarningLog("Quiet option won't have any effect with simulation mode");}
    
    
    if(files.size()<=0){String filename_from_stdin;std::getline(std::cin,filename_from_stdin,'\0');
        files.push_back(filename_from_stdin);
        //printErrorLog("No file or directory specified");Exit(1);
        }
    if(files.size()==1){if(!file_only || depth<=0){single_mode=true;}}
    if(simulation){quiet=false;}
    
    
    if(name_string=="" && name_string_file=="" && replace_string.size()==0){
        printErrorLog("One of the options: -ns or -nsf or -rs is mandatory");
        Exit(1);
        }
    if(search_string.size()!=0){
        for(Int i=0;i<(Int)search_string.size();i++){
            if(stringContains(search_string[i],path_delim)){
                parseSearchString(search_string[i],i); 
            }
            else {
                ss_search.push_back(search_string[i]);
                ss_mod.push_back("");
                ss_fixed.push_back(fixed_ss[i]);
            
            }    
        }
    ///search_string and fixed_ss will no longer be uesed. Better free up some memory
    search_string.clear();fixed_ss.clear();
    }
    
    
    if(name_string_file!=""){
        if(name_string_file==path_delim+"hist"+path_delim){
            if(isFile(NSF_LIST_FILE)){name_string_file=NSF_LIST_FILE;}
            else{printErrorLog("History not found.");Exit(1);}
        }
        else if(!isFile(name_string_file)){
            printErrorLog("Name String File not found: "+name_string_file);
            Exit(1);
        }
        detectLineUpwardOrDownward();
        copyFile2(name_string_file,NSF_LIST_FILE);
        if(!quiet){print "Reading name string file..."+NEW_LINE;}
        if(!nsf_n){nsflist=getNameListFromFile(name_string_file,start_line,end_line);}
        else{nsflist=getNameListFromFile(name_string_file,start_line,end_line,0);}
    
    }


    /// check regex type
    if(re_type!=""){
        
        if(re_type=="default"){REGEX_TYPE=REGEX_DEFAULT;}
        else if(re_type=="awk"){REGEX_TYPE=REGEX_AWK;}
        else if(re_type=="grep"){REGEX_TYPE=REGEX_GREP;}
        else if(re_type=="egrep"){REGEX_TYPE=REGEX_EGREP;}
        else if(re_type=="basic"){REGEX_TYPE=REGEX_BASIC;}
        else if(re_type=="extended"){REGEX_TYPE=REGEX_EXTENDED;}
        else if(re_type=="ecmascript"){REGEX_TYPE=REGEX_ECMASCRIPT;}
        else {printErrorLog("Invalid regex type: "+re_type);Exit(1);}
        
        }
    else {REGEX_TYPE=REGEX_DEFAULT;}

    
    ////////////////////////////////// Various checks end here//////////////////////////////
    
    ///Sort file if sort is true
    
    if(sort){sortVector(files);}
 
    
    startTask(files);
    
    showResult();
    

   return 0;
} 

