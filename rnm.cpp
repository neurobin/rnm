
#include "globals.h"



StringArray files;



/////function prototypes
template<typename T>
String toString(T);
String toLower(String);
String prepareLogDir();
template<typename T>
String appendToFile(T);
String printOutLog(String);
String printErrorLog(String);
void printOpts(void);
void startTask(StringArray);
String replaceStringAll(String, String , String );
String parseNameString(String,String);
String regexReplace(String ,String ,String ,String );
String parseNameString(String ,String );
void parseReplaceString(String ,String );
String copyFile(String, String );


////getting Linux Home and defining some paths
String getLinuxHome(){const char *homedir;if ((homedir = getenv("HOME")) == NULL) {homedir = getpwuid(getuid())->pw_dir;}return String(homedir);}
String HOME=getLinuxHome();
String LOG_DIR_PARENT=HOME+"/.neuro";
String LOG_DIR=LOG_DIR_PARENT+"/rnm";
String ERROR_LOG=LOG_DIR+"/errors.log";
String OUT_LOG=LOG_DIR+"/out.log";
String RNM_FILE_LOG=LOG_DIR+"/rfl.log";
String RNM_FILE_LOG_TMP=RNM_FILE_LOG+".tmp";

String prepareLogDir(){
    mkdir(LOG_DIR_PARENT.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(LOG_DIR.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    FileStream file;
    file.open(RNM_FILE_LOG_TMP.c_str(),$write);
    if(!file.good()){printe strerror(errno);}
    file.close();
    return toString(strerror(errno));
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
    printe str NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    return appendToFile(ERROR_LOG,str+"\t\t\t\t@"+dt);
    
}

String printWarningLog(String str){
    str="W: "+str;
    if(!quiet){printe str NEW_LINE;}
    time_t now = time(0);
    char* dt = ctime(&now);
    return appendToFile(ERROR_LOG,str+"\t\t\t\t@"+dt)+appendToFile(OUT_LOG,str+"\t\t\t\t@"+dt);
    
}

    
String printOutLog(String str){
    if(!quiet)print str NEW_LINE;
    time_t now = time(0);
    char* dt = ctime(&now);
    return appendToFile(OUT_LOG,str+"\t\t        @"+dt);

}

String appendToRFLTMP(String str1,String str2){
    const char* name=toString(RNM_FILE_LOG_TMP).c_str();
    FileStream file;
    file.open(name,$binary | $append);
    if(!quiet){print str1<<"    \t---->\t    "<<str2 NEW_LINE;}
    time_t now = time(0);
    char* dt = ctime(&now);
    String str=str1+"\\    \t---->\t    \\"+str2+"\\    \t\t        @"+dt;
    file<<str;file<<'\0';
    copyFile(RNM_FILE_LOG_TMP,RNM_FILE_LOG);
    return strerror(errno);

}

String copyFile(String src, String dest){
    int outfd = open(dest.c_str(),O_RDWR);
    int infd = open(src.c_str(),O_RDWR);          
    struct stat stat_buf ;
    fstat(infd,&stat_buf);
    ssize_t size = sendfile(outfd,infd,0,stat_buf.st_size);
    return toString(size);
}

void finalizeRFL(){
    copyFile(RNM_FILE_LOG_TMP,RNM_FILE_LOG);
    }

bool isInt(String x){
    Regex e ("^-?\\d+");
    if (std::regex_match (x,e)) return true;
    else return false;}

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
    else return false;}

template<typename T>
void mustBeANumber(String name,T x){
    if(!isNumber(x)){
       printErrorLog(name+" must be a valid number");
       Exit(1);
       }
    }
    
template<typename T>
void mustBeAPositiveInteger(String name,T x){
    if(!isPositiveInt(x)){
       printErrorLog(name+" must be a valid positive integer");
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
    
String basename(String file){
    
    std::size_t found = file.find_last_of("/\\");
    return file.substr(found+1);
    
}

std::string fileExtension(std::string file){
    
    std::regex re(".*[^\\.]+\\.([^\\.]+$)");
    std::smatch result;
    if(std::regex_match(file,result,re))return result[1];
    else return "";
    
}

std::string fileNameWithoutExtension(std::string file){
  
    std::regex re("(.*[^\\.]+)\\.[^\\.]+$");
    std::smatch result;
    if(std::regex_match(file,result,re))return result[1];
    else return file;
    
}

String dirname(String file){
    
    std::size_t found = file.find_last_of("/\\");
    return file.substr(0,found);
    
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
    
    
template<typename T>
void mustBeAnInteger(String name,T x){
    if(!isInt(x)){
       printErrorLog(name+" must be a valid integer");
       Exit(1);
       }
    }

    
void checkArgAvailability(StringArray sa,int i){
    if(i>=(int)sa.size()){
        printErrorLog("One or more argument/s missing");
        Exit(1);
    }
    }


int main(int argc, char* argv[]) {getCurrentDir(self_dir);self_path=self_dir+String("/")+executable_name;
    prepareLogDir();
    
    

    
//////////////////////////////// Opt parse////////////////////////////////////////////
    bool skipcount=false;
    StringArray args;
    for(int i = 1; i < argc; i++){args.push_back(argv[i]);}
    
    
    for(int i = 0; i < (int)args.size(); i++){
      if(skipcount){skipcount=false;continue;}
        
      String opt=toLower(String(args[i]));
      
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
          mustBeANumber("Increment Value",args[i+1]);
          inc=stringTo<decltype(inc)>(args[i+1]);
          skipcount=true;
        }
        
        
      else if(opt=="-i"||opt=="-si"||opt=="--index"||opt=="--start-index"){
          checkArgAvailability(args,i+1);
          mustBeANumber("Start Index",args[i+1]);
          start_index=stringTo<decltype(start_index)>(args[i+1]);
          current_index=start_index;
          DIRECTORY_INDEX=start_index;
          directory_index=DIRECTORY_INDEX;
          directory_reverse_index=directory_index;
          DIRECTORY_REVERSE_INDEX=directory_reverse_index;
          skipcount=true;
        }
        
      else if(opt=="-ei"||opt=="--end-index"){
          checkArgAvailability(args,i+1);
          mustBeANumber("End Index",args[i+1]);
          end_index=stringTo<decltype(end_index)>(args[i+1]);
          skipcount=true;
          
        }
        
      else if(opt=="-ifl"||opt=="--index-field-length"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Index field length",args[i+1]);
          input_field_length=stringTo<decltype(input_field_length)>(args[i+1]);
          skipcount=true;
          
        }
        
      else if(opt=="-ifp"||opt=="--index-field-precision"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Index field precision",args[i+1]);
          IFP=stringTo<decltype(IFP)>(args[i+1]);
          skipcount=true;
          
        }
        
      else if(opt=="-iff"||opt=="--index-field-filler"){
          checkArgAvailability(args,i+1);
          mustBeAValidSingleCharacter("Index field filler",args[i+1]);
          IFF=stringTo<decltype(IFF)>(args[i+1]);
          IFF=replaceStringAll(IFF,"\\","");
          skipcount=true;
          
        }
      
      else if(opt=="-ns"||opt=="--name-string"){
          checkArgAvailability(args,i+1);
          name_string=args[i+1];
          skipcount=true;
          
        }
        
      else if(opt=="-nsf"||opt=="--name-string-file"){
          checkArgAvailability(args,i+1);
          name_string_file=args[i+1];
          skipcount=true;
          
        }
        
      else if(opt=="-l"||opt=="-sl"||opt=="--line"||opt=="--start-line"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Start Line",args[i+1]);
          start_line=stringTo<decltype(start_line)>(args[i+1]);
          current_line=start_line;
          skipcount=true;
          
        }
        
      else if(opt=="-lv"||opt=="-slv"||opt=="--line-reverse"||opt=="--start-line-reverse"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Start Line",args[i+1]);
          start_line=stringTo<decltype(start_line)>(args[i+1]);
          current_line=start_line;
          skipcount=true;
          reverse_line=true;
          
        }
        
      else if(opt=="-el"||opt=="--end-line"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("End Line",args[i+1]);
          end_line=stringTo<decltype(end_line)>(args[i+1]);
          skipcount=true;
          
        }
      
      else if(opt=="-elv"||opt=="--end-line-reverse"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("End Line",args[i+1]);
          end_line=stringTo<decltype(end_line)>(args[i+1]);
          skipcount=true;
          reverse_line=true;
          
        }
        
      else if(opt=="-ss"||opt=="--search-string"){
          checkArgAvailability(args,i+1);
          search_string=args[i+1];
          skipcount=true;
          
        }
        
      else if(opt=="-rs"||opt=="--replace-string"){
          checkArgAvailability(args,i+1);
          replace_string=args[i+1];
          skipcount=true;
          
        }
        
        
      else if(opt=="-ssf"||opt=="--search-string-fixed"){
          checkArgAvailability(args,i+1);
          search_string=args[i+1];
          fixed_ss=true;
          skipcount=true;
          
        }
        
      else if(opt=="-dp"||opt=="--depth"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Depth",args[i+1]);
          depth=stringTo<decltype(depth)>(args[i+1]);
          skipcount=true;
          
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
        
        
        else {
            files.push_back(String(args[i]));
            
            
        }

      
      
    }
    file_vector=files;
//////////////////////////////////////// Opt parse ends here/////////////////////////////////

if(show_options){printOpts();}
    
    
    /////////////////////////////////// Various checks///////////////////////////////////////
    
    if(files.size()<=0){printErrorLog("No file or directory specified");Exit(1);}
    
    
    if(name_string=="" && name_string_file==""){
        printErrorLog("One of the options: -ns or -nsf is mandatory");
        Exit(1);
        }
    if(search_string!=""){
        Regex re("^/[^/]*/i?$");
        if(!regexMatch(search_string,re)){
            printErrorLog("Invalid search string");
            Exit(1);
            }
        }
    
    
    if(replace_string!=""){
        Regex re("^/.*/.*/[gi]{0,2}$");
        if(!regexMatch(replace_string,re)){
            printErrorLog("Invalid replace string");
            Exit(1);
            }
        }
    
    
    
    
    
    
    ////////////////////////////////// Various checks ends here//////////////////////////////
    
    
    
    
    
    
startTask(files);
    
    

    if(!quiet){printOutLog(toString(rnc)+" file/s renamed");}
    
    
    
    
    
    
    
    
    
    
    
    
   return 0;
} 


bool isImmediateChild(String prevf,String newf){
    

        if(prevf==dirname(newf)){
            return true;
            }
        else return false;
    
    
}

bool isChild(String parent,String child){
  std::size_t found = child.find(parent+"/");
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


Int childDepth(String parent,String child){
    String childstr=child;
    if(isChildDir( parent, child)){
        if(isDir(child)){
            childstr=replaceString(child,parent+"/",String(""));
        }
        else{
            childstr=replaceString(dirname(child),parent+"/",String(""));
        }
        return countMatchInRegex(childstr,"/?[^/]+/?");
    }
    else return DEPTH_COUNT_INITIAL;
    
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
    
String stripBothSlashes(String s){
    s=replaceStringAll(s,"/","");
    s=replaceStringAll(s,"\\","");
    return s;
    }
    
String processReplacementString(String replace){
    /// \1 \2 etc will be converted to c++ backreference $1 $2
    /// & will be converted to c++ $& (a copy of the entire match)
    /// \p will be converted to c++ $` (prefix)
    /// \s will be converted to c++ $´ (suffix)
    /// $ will be converted to c++ $$
    /// \& will be &
    replace=replaceStringAll(replace,"&","$&");
    replace=replaceStringAll(replace,"\\p","$`");
    replace=replaceStringAll(replace,"\\s","$´");
    replace=replaceStringAll(replace,"$","$$");
    replace=replaceStringAll(replace,"\\&","&");
    replace=regexReplace(replace,"\\\\(\\d{0,2})","$$$1","g");
    
    
    ///Finally strip slashes
    replace=stripBothSlashes(replace);
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


bool doRename(String file){
    bool not_skipped=true;
        
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
    else {printErrorLog("One of the options: -ns or -nsf is mandatory");Exit(1);}
    




    ///sanitize name
    name=stripBothSlashes(name);
    
    
    
    
    if(name!=""){
        ///do rename
        appendToRFLTMP(file,dir+"/"+name);
    }
    else{
        printWarningLog("Name can not be empty");not_skipped=false;
    }
    
    
    if(current_index==end_index){printOutLog("End index reached");Exit(1);}
    if(current_line==end_line){printOutLog("End line reached");Exit(1);}
    
    return not_skipped;
}



void startInDepthFileOnlyTaskOnDirectory(String dir,String base_dir=base_dir){
    StringArray files;
    files=getFilesFromDir(dir);
    
    directory_count++;
    directory_index=DIRECTORY_INDEX;
    directory_reverse_index=DIRECTORY_REVERSE_INDEX;
    for(int i=0;i<(int)files.size();i++){
        
        String file=files[i];
        
        print file NEW_LINE;
        if(childDepth(base_dir,file)>=depth){continue;}
        String parent="";
        String src_name="";
        if(!isPathValid(file)){printWarningLog("No such file or directory: "+file);continue;}
        file=getAbsolutePath(file);
        src_name=basename(file);
        parent=dirname(file);
        //print childDepth(base_dir,file)<<"\t"<<base_dir NEW_LINE;
        //print depth_count<<"\t"<<prev_file.length()<<"\t"<<file.length() NEW_LINE;
        if(isDir(file)){
            
            if(file_only){
                
                if(childDepth(base_dir,file)<depth){
                    

                    startInDepthFileOnlyTaskOnDirectory(file);
                }
                else{
                
                
                }
                
                
            }
            
            else if(exclude_directory){
                continue;
            }
            
            else if(directory_only){
                
                
                doRename(file);
                
                
            }
            
            
        }
        else if(isFile(file)){
            
            
            
            doRename(file);
            
            
            
            
        }
        else {
        printWarningLog("Not a valid file or directory");
            
        }
        
        
    
    }
    
    
}


void startTask(StringArray files){
    
    directory_count++;
    directory_index=DIRECTORY_INDEX;
    directory_reverse_index=DIRECTORY_REVERSE_INDEX;
    for(int i=0;i<(int)files.size();i++){
        
        String file=files[i];
        String parent="";
        String src_name="";
        if(!isPathValid(file)){printWarningLog("No such file or directory: "+file);continue;}
        file=getAbsolutePath(file);
        src_name=basename(file);
        parent=dirname(file);
        print file NEW_LINE;
        //print depth_count<<"\t"<<prev_file.length()<<"\t"<<file.length() NEW_LINE;
        if(isDir(file)){
            base_dir=file;
        
            if(file_only){
                //print childDepth(base_dir,file)<<"\t"<<depth NEW_LINE;
                if(childDepth(base_dir,file)<depth){

                    startInDepthFileOnlyTaskOnDirectory(file);
                    //depth_count=DEPTH_COUNT_INITIAL;
                }
                else{
                
                
                }
                
                
            }
            
            else if(exclude_directory){
                continue;
            }
            
            else if(directory_only){
                
                doRename(file);
                
                
                
            }
            
            
        }
        else if(isFile(file)){
            
            
            doRename(file);
            
            
            
            
            
        }
        else {
        printWarningLog("Not a valid file or directory");
            
        }
        
        
    
    }
    

    }




void printOpts(){
    
    print "Executable: "<<self_path NEW_LINE;
    print "Name String: "<<name_string NEW_LINE;
    print "Name String File: "<<name_string_file NEW_LINE;
    print "Search String: "<<search_string NEW_LINE;
    print "Fixed Search String: "<<fixed_ss NEW_LINE;
    print "Depth: "<<depth NEW_LINE;
    print "Input Field Length: "<<input_field_length NEW_LINE;
    print "Undo: "<<undo NEW_LINE;
    print "Force: "<<force NEW_LINE;
    print "Start Index: "<<start_index NEW_LINE;
    print "End Index: "<<end_index NEW_LINE;
    print "Start Line: "<<start_line NEW_LINE;
    print "End Line: "<<end_line NEW_LINE;
    print "Reverse Line: "<<reverse_line NEW_LINE;
    print "Quiet: "<<quiet NEW_LINE;
    print "File Only: "<<file_only NEW_LINE;
    print "Directory Only: "<<directory_only NEW_LINE;
    print "Exclude Directory: "<<exclude_directory NEW_LINE;
    print "Increment Value: "<<inc NEW_LINE;
    
    }
