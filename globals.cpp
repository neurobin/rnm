#include "globals.h"


bool undoRename(){
    FileStream file_l,file_r;
    String l,r;
    StringArray left,right;
    file_l.open(RNM_FILE_LOG_L,$binary | $read);
    file_r.open(RNM_FILE_LOG_R,$binary | $read);
    if(file_l.good() && file_r.good()){
    while(getLineFromFile(file_l,l,'\0') && getLineFromFile(file_r,r,'\0')){
        left.push_back(l);right.push_back(r);
        }
    file_l.close();file_r.close();
    for(int i=0;i<(int)left.size();i++){
        ///do rename and log into rfl
        
        }
    
    return true;
    }
    
    else {
        printErrorLog("Undo failed. Required log files not found");
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
    StringArray invf={HOME,"/",LOG_DIR_PARENT,LOG_DIR,RNM_FILE_LOG_L,RNM_FILE_LOG_R,RNM_FILE_LOG_L_TMP,RNM_FILE_LOG_R_TMP,\
        "/bin","/usr","/usr/bin","/tmp","/dev","/etc","/proc","/sys","/home"};
    for(int i=0;i<(int)invf.size();i++){
        if(file==invf[i]){status=true;printWarningLog("rename not permitted: "+invf[i]);}
    }
    
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


String Rename(String oldn,String newn){
    
    if(isPathValid(newn)){
        if(!quiet){
            printErrorLog("File/Directory exists: "+newn);
            //Exit(1);
        }
    }
    else {
        if(!simulation){
            
            if(rename(oldn.c_str(),newn.c_str()) == 0){
                appendToRFLTMP(oldn,newn);
                current_index+=inc;
                reverse_index-=inc;
                directory_index+=inc;
                directory_reverse_index-=inc;
                if(name_string_file!=""){current_line+=linc;}
                rnc++;
            }
            else {printErrorLog(strerror(errno));}
        }
        else {
            
                current_index+=inc;
                reverse_index-=inc;
                directory_index+=inc;
                directory_reverse_index-=inc;
                if(name_string_file!=""){current_line+=linc;}
                rnc++;
        }
        
    }
    return strerror(errno);
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
    Regex re("-[^-]*");
    std::smatch result;
    if(regex_match(toString(index),result,re)){index=fabs(index);negative_flag=true;}
    Stream buffer;
    buffer.width(ifl);
    buffer.fill(IFF[0]);
    buffer.precision(IFP+1);
    buffer <<index;
    if(negative_flag){
    return "-"+buffer.str();}
    else return buffer.str();}

String regexReplace(String s,String search,String replace,String modifier){
    bool case_sensitive=true,global=false;
    String replaced_string=s;
    if(modifier=="gi" || modifier=="ig"){global=true;case_sensitive=false;}
    else if(modifier=="i"){case_sensitive=false;}
    else if(modifier=="g"){global=true;}
    
    try {
        std::regex re (search);
        if(!case_sensitive){re= Regex (search, ECMASCRIPT | ICASE);}
        
        if(global){
        replaced_string=std::regex_replace (s,re,replace,std::regex_constants::format_default);
        }
        else{
        replaced_string=std::regex_replace (s,re,replace,NON_RECURSIVE_REGEX_REPLACE);
        }
    } 
    catch (std::regex_error& e) {
      printErrorLog("Invalid replace string");
      Exit(1);
    }
    return replaced_string;
}
    
String stripPathDelimiter(String s){
    s=replaceStringAll(s,path_delim,"");
    //s=replaceStringAll(s,"\\","\\\\");
    return s;
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
    replace=replaceStringAll(replace,"\\s","$´");
    replace=regexReplace(replace,"\\\\(\\d{0,2})","$$$1","g");
    
    
    ///Finally strip slashes
    replace=stripPathDelimiter(replace);
    return replace;
}
    
    
void processReplaceString(String rs,String file){
    parseReplaceString(rs,file);
    ///we now have valid rs_search, rs_replace and rs_mod
    rname=basename(file);
    rname=regexReplace(rname,rs_search,rs_replace,rs_mod);
    ///Now a modified name rname is available
}
    

void parseReplaceString(String rs,String file){
    String name=parseNameString( rs, file);
    ///bool re_g=false,re_i=false;
    ///std::regex_constants::format_first_only
    Regex re ("^/[^/]*/[^/]*/[gi]{0,2}$");
    RegexResult result;
    if(name!="" && regexMatch(name,re)){
        re ="^/([^/]*)/[^/]*/[gi]{0,2}$";
        if(std::regex_match(name,result,re)){rs_search=result[1];}
        re ="^/[^/]*/([^/]*)/[gi]{0,2}$";
        if(std::regex_match(name,result,re)){rs_replace=result[1];rs_replace=processReplacementString(rs_replace);}
        re ="^/[^/]*/[^/]*/([gi]{0,2})$";
        if(std::regex_match(name,result,re)){rs_mod=result[1];}
        
        
    }
    else{
        printErrorLog("Invalid replace string");
        Exit(1);
    }
    

}

void parseSearchString(String ss){
  if(!fixed_ss){
    Regex re("^/[^"+path_delim+"]*/i?$");
    RegexResult result;
    if(regexMatch(ss,re)){
        re="^/([^"+path_delim+"]*)/i?$";
        if(std::regex_match(ss,result,re)){ss_search=result[1];}
        re="^/[^"+path_delim+"]*/(i?)$";
        if(std::regex_match(ss,result,re)){ss_mod=result[1];}
    }
    else{
        printErrorLog("Invalid search string");
        Exit(1);
    } 
  }
  else{
    ///search string is a fixed string, not to be modified.
  }
}

bool stringContains(String s1,String s2){
    if (s1.find(s2) != String::npos) {return true;}
    else return false;
}

bool isComplyingToSearchString(String file){
    String name=basename(file);
    bool case_sensitive=true;
    if(ss_mod=="i"){case_sensitive=false;}
  if(!fixed_ss){
    try {
        std::regex re (ss_search);
        if(!case_sensitive){re= Regex (ss_search, ECMASCRIPT | ICASE);}
        if(std::regex_search(name,re)){return true;}
        else{return false;}
    } 
    catch (std::regex_error& e) {
      printErrorLog("Invalid search string");
      Exit(1);
    }
  }
  else{
      if(stringContains(name,search_string)){return true;}
      else return false;
      
  }
    
    
}




String parseNameString(String ns,String file){
    String fname=basename(file);
    if(replace_string==""){rname=fname;}
    String name=ns;
    String fnamewe=fileNameWithoutExtension(fname);
    String ext=fileExtension(fname);
    if(ns!=""){
        name=replaceStringAll(name,"/fn/",fname);
        name=replaceStringAll(name,"/n/",fnamewe);
        name=replaceStringAll(name,"/e/",ext);
        name=replaceStringAll(name,"/rn/",rname);
        name=replaceStringAll(name,"/i/",toStringAccordingToIFL(current_index,input_field_length));
        name=replaceStringAll(name,"/-i/",toStringAccordingToIFL(reverse_index,input_field_length));
        name=replaceStringAll(name,"/ir/",toStringAccordingToIFL(current_index_rd,input_field_length));
        name=replaceStringAll(name,"/-ir/",toStringAccordingToIFL(reverse_index_rd,input_field_length));
        name=replaceStringAll(name,"/id/",toStringAccordingToIFL(directory_index,input_field_length));
        name=replaceStringAll(name,"/idr/",toStringAccordingToIFL(directory_reverse_index,input_field_length));
        name=replaceStringAll(name,"/-id/",toStringAccordingToIFL(directory_index_rd,input_field_length));
        name=replaceStringAll(name,"/-idr/",toStringAccordingToIFL(directory_reverse_index_rd,input_field_length));
        name=replaceStringAll(name,"/dc/",toString(directory_count));
        
        
        if(name_string_file!=""){name=replaceStringAll(name,"/l/",toStringAccordingToIFL((Double)current_line,input_field_length));}
        
        
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
            childstr=replaceString(dirname(child),parent+"/",String(""));
        }
        return countMatchInRegex(childstr,"/?[^/]+/?");
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



bool doRename(String file){
    bool not_skipped=true;
    
    if(isInvalidFile(file)){return false;}
    
    if(search_string!=""){
        if(!isComplyingToSearchString(file)){
            return false;
        }
    }
    
    
    String oldn=basename(file);
    String dir=dirname(file);
    String name="";
    if(replace_string!=""){processReplaceString(replace_string,file);}
    
    if(name_string!="" && name_string_file==""){
        name=parseNameString(name_string,file);
        
        
        }
    else if(name_string=="" && name_string_file!=""){
        
        
        }
    else if(name_string!="" && name_string_file!=""){
        
        
        }
    else if(replace_string!=""){name=rname;}
        
    else {printErrorLog("One of the options: -ns or -nsf or -rs is mandatory");Exit(1);}
    




    ///sanitize name
    name=stripPathDelimiter(name);
    
    
    
    
    if(name!=""){
        if(!quiet){
            //String out=file+"    ---->    "+dir+path_delim+name;
            
            print file+"    ---->    "+dir+path_delim+name+NEW_LINE;
            
        }
        ///do rename
        int confirm;
        
        if(!all_yes){
            confirm=selectInput();
            
            switch(confirm){
                case 1:
                      Rename(file,dir+path_delim+name);
                      break;
                case 2:
                      all_yes=true;
                      Rename(file,dir+path_delim+name);
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
            Rename(file,dir+path_delim+name);
            
        }
        
        
    }
    else{
        printWarningLog("Name can not be empty, skipped.");not_skipped=false;
    }
    
    
    if(current_index==end_index){printOutLog("End index reached");Exit(1);}
    if(current_line==end_line){printOutLog("End line reached");Exit(1);}
    
    return not_skipped;
}


StringArray getFilesFromDir(String file){
                StringArray files;
                DIR *dir;
                struct dirent *ent;
                if ((dir = opendir (file.c_str())) != NULL) {
                    while ((ent = readdir (dir)) != NULL) {
                        
                        String filename=file+"/"+ent->d_name;
                        if(basename(filename)=="." || basename(filename)==".."){continue;}
                        files.push_back(filename);
                    }
                    closedir (dir);
                } 
                else {
                    perror ("");
                    print"Could not open directory";
                }
    return files;
    
}


void startInDepthFileOnlyTaskOnDirectory(String dir,String base_dir=base_dir){
    StringArray files;
    files=getFilesFromDir(dir);
    
    directory_count++;
    directory_index=DIRECTORY_INDEX;
    directory_index_rd=DIRECTORY_INDEX;
    directory_reverse_index=DIRECTORY_REVERSE_INDEX;
    directory_reverse_index_rd=DIRECTORY_REVERSE_INDEX;
    for(int i=0;i<(int)files.size();i++){
        
        String file=files[i];
        
        //print file+NEW_LINE;
        if(childDepth(base_dir,file)>depth){continue;}
        String parent="";
        String src_name="";
        if(!isPathValid(file)){printWarningLog("No such file or directory: "+file);continue;}
        file=getAbsolutePath(file);
        src_name=basename(file);
        parent=dirname(file);
        
        
        //print childDepth(base_dir,file)<<"\t"<<base_dir+NEW_LINE;
        //print depth_count<<"\t"<<prev_file.length()<<"\t"<<file.length()+NEW_LINE;
        if(isDir(file)){
            
            if(file_only){
                
                if(childDepth(base_dir,file)<=depth){
                    

                    startInDepthFileOnlyTaskOnDirectory(file);
                }
                else{
                
                
                }
                
                
            }
            
            else if(exclude_directory){
                continue;
            }
            
            else{
                
                
                doRename(file);
                
                
            }
            
            
        }
        else if(isFile(file)){
            
            
            if(!directory_only){
                doRename(file);
            }
            
            
            
            
        }
        else {
        printWarningLog("Not a valid file or directory");
        continue;
            
        }

        ///increment reserved indexes
        directory_index_rd+=inc;
        directory_reverse_index_rd-=inc;
        current_index_rd+=inc;
        reverse_index_rd-=inc;
        
    
    }
    
    
}


void startTask(StringArray files){
    
    directory_count++;
    directory_index=DIRECTORY_INDEX;
    directory_index_rd=DIRECTORY_INDEX;
    directory_reverse_index=DIRECTORY_REVERSE_INDEX;
    directory_reverse_index_rd=DIRECTORY_REVERSE_INDEX;
    for(int i=0;i<(int)files.size();i++){
        
        String file=files[i];
        String parent="";
        String src_name="";
        if(!isPathValid(file)){printWarningLog("No such file or directory: "+file);continue;}
        file=getAbsolutePath(file);
        src_name=basename(file);
        parent=dirname(file);
        
        //print depth_count<<"\t"<<prev_file.length()<<"\t"<<file.length()+NEW_LINE;
        if(isDir(file)){
            base_dir=dirname(file);
        
            if(file_only){
                //print childDepth(base_dir,file)<<"\t"<<depth+NEW_LINE;
                if(childDepth(base_dir,file)<=depth){

                    startInDepthFileOnlyTaskOnDirectory(file);
                    //depth_count=DEPTH_COUNT_INITIAL;
                }
                else{
                
                
                }
                
                
            }
            
            else if(exclude_directory){
                continue;
            }
            
            else{
                //print file;
                doRename(file);
                
                
                
            }
            
            
        }
        else if(isFile(file)){
            
            if(!directory_only){
                doRename(file);
            }
            
            
            
            
            
        }
        else {
        printWarningLog("Not a valid file or directory");
        continue;
        }
        
        ///increment reserved indexes
        directory_index_rd+=inc;
        directory_reverse_index_rd-=inc;
        current_index_rd+=inc;
        reverse_index_rd-=inc;
        
    
    }
    

}

String parseTrueFalse(bool a){
    if(a)return "true";
    else return "false";
}


void printOpts(){
    
    print "\n\nInfo about this session:\n\
    Executable: "+self_path+"\n\
    Name String: " +name_string+"\n\
    Name String File: " +name_string_file+"\n\
    Search String: " +search_string+"\n\
    Fixed Search String: " +parseTrueFalse(fixed_ss)+"\n\
    Replace String: " +replace_string+"\n\
    Replace String search part (last): "+rs_search+"\n\
    Replace String replace part (last): "+rs_replace+"\n\
    Replace String modifier part (last): "+rs_mod+"\n\
    Depth: "+toString(depth)+"\n\
    Input Field Length: "+toString(input_field_length)+"\n\
    Undo: "+parseTrueFalse(undo)+"\n\
    Force: "+parseTrueFalse(force)+"\n\
    Start Index: "+toString(start_index)+"\n\
    End Index: "+toString(end_index)+"\n\
    Start Line: "+toString(start_line)+"\n\
    End Line: "+toString(end_line)+"\n\
    Reverse Line: "+parseTrueFalse(reverse_line)+"\n\
    Quiet: "+parseTrueFalse(quiet)+"\n\
    File Only: "+parseTrueFalse(file_only)+"\n\
    Directory Only: "+parseTrueFalse(directory_only)+"\n\
    Exclude Directory: "+parseTrueFalse(exclude_directory)+"\n\
    Increment Value: "+toString(inc)+"\n\
    Simulation: "+parseTrueFalse(simulation)+"\n\n\
";
    
    }



void showResult(){
    if(!quiet){printOutLog(toString(rnc)+" file/s renamed");if(simulation)print " (simulation)"+NEW_LINE;}
    if(show_options || simulation){printOpts();}
    
}
