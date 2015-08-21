#ifndef __GLOBALS_H
#define __GLOBALS_H

/////includes
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <limits>
#include <cerrno>
#include <cstdlib>
#include <ctime>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <cctype>
#include <regex>
#include <cstddef> 
#include <dirent.h>
#include <map>



/////typedefs
typedef long long Int;
typedef double Double;
typedef std::string String;
typedef std::stringstream Stream;
typedef std::vector<String> StringArray;
typedef std::regex Regex;
typedef std::map<Int,String> NameList;
typedef std::map<Int,Int> IntMap;


/////defs
#define Exit(a) std::exit((a))
#define getLine(s) std::getline(std::cin,(s))
#define getLineFromFile(f,line,delim) std::getline((f),(line),(delim))
#define regexMatch(s,e) std::regex_match((s),(e))
#define getCurrentDir(a) getcwd((a), sizeof((a)))
#define RegexResult std::smatch
#define NON_RECURSIVE_REGEX_REPLACE std::regex_constants::format_first_only
#define DEFAULT_REGEX_REPLLACE std::regex_constants::format_default
#define ECMASCRIPT std::regex_constants::ECMAScript
#define ICASE std::regex_constants::icase

#define print std::cout<<
#define printe std::cerr<<
#define ENDL std::endl
#define input std::cin>>
#define LIMIT_OF_DOUBLE std::numeric_limits<Double>::max();
#define INT_MAX std::numeric_limits<Int>::max();
#define FileStream std::fstream
#define $ios std::ios
#define $append std::ios::app
#define $ate std::ios::ate
#define $binary std::ios::binary
#define $read std::ios::in
#define $write std::ios::out
#define $trunc std::ios::trunc
#define $beg std::ios::beg
#define $cur std::ios::cur
#define $end std::ios::end

#define OK "OK"



/////Bolleans
bool quiet=false;
bool force=false;
bool all_yes=false;
bool directory_only=false;
bool file_only=false;
bool exclude_directory=false;
bool reverse_line=false;
bool undo=false;
bool fixed_ss=false;
bool show_options=false;
bool simulation=true;
bool line_upward=true;
bool ign=false;


/////Doubles
Double start_index=1;
Double current_index=1;
Double current_index_rd=1;
Double reverse_start_index=-1;
Double reverse_index=reverse_start_index;
Double reverse_index_rd=reverse_start_index;
Double end_index=LIMIT_OF_DOUBLE;
Double inc=1;
Double directory_index=1;
Double directory_index_rd=1;
Double directory_reverse_index=-1;
Double directory_reverse_index_rd=-1;
Double DIRECTORY_INDEX=directory_index;
Double DIRECTORY_REVERSE_INDEX=directory_reverse_index;

/////ints

Int linc=1;
Int start_line=1;
Int current_line=start_line;
Int current_abs_line=start_line;
Int end_line=0;
Int directory_count=0;
Int depth=0;
Int LOWEST_DEPTH=0;
int input_field_length=1;
Int rnc=0; //rename count
IntMap abslc_list;

//////// nsf related
NameList nsflist;


//////// IFL related
String IFF="0"; //input field filler
int IFP=2;    //input field precision
String blank_str="";
String NEW_LINE="\n";
/////Strings
String path_delim="/";
////getting Linux Home and defining some paths
String getLinuxHome(){const char *homedir;if ((homedir = getenv("HOME")) == NULL) {homedir = getpwuid(getuid())->pw_dir;}return String(homedir);}
String HOME=getLinuxHome();
String LOG_DIR_PARENT=HOME+path_delim+".neuro";
String LOG_DIR=LOG_DIR_PARENT+path_delim+"rnm";
String ERROR_LOG=LOG_DIR+path_delim+"errors.log";
String OUT_LOG=LOG_DIR+path_delim+"out.log";
String RNM_FILE_LOG_L=LOG_DIR+path_delim+"rfl.l.log";
String RNM_FILE_LOG_R=LOG_DIR+path_delim+"rfl.r.log";
String RNM_FILE_LOG_L_TMP=RNM_FILE_LOG_L+".tmp";
String RNM_FILE_LOG_R_TMP=RNM_FILE_LOG_R+".tmp";


StringArray file_vector;
char self_dir[FILENAME_MAX];
String base_dir="";
String self_path="";
String search_string="";
String replace_string="";
String name_string="";
String name_string_file="";
String rname="";
String rs_search="";
String rs_replace="";
String rs_mod="";
String ss_search="";
String ss_mod="";


String project_name="rnm";
String executable_name="rnm";
String version="3.0.0";
String author_name="Jahidul Hamid";
String author_email="jahidulhamid@yahoo.com";
String bug_report_url="http://github.com/neurobin/"+project_name+"/issues";



String version_info="\n\
Rename Utility "+project_name+"\n\
Version:        "+version+"\n\
Author:         "+author_name+"\n\
Author Email:   "+author_email+"\n\
Bug Report:     "+bug_report_url+"\n\
License:        GNU GPL public license v3\n\
";


String help_message="\n\
************** "+project_name+" "+version+" *******************\n\
\n\
Usage: "+project_name+" Directory/File/Path [options]\n\
\n\
One of the options of [-ns] or [-nsf] is mandatory.\n\
Options are **not** sequential, their position in the argument list\n\
have no siginificance. For example, rnm filepath -ns name is the same\n\
as rnm -ns name filepath. Though passing the directory/file path at the end\n\
of the arguement list is considered to be safe and wise.\n\
\n\
options:\n\
\n\
-h, --help     : Show help menu\n\
\n\
-i,-si         : Starting index\n\
-ei            : End index i.e index to stop renaming from.\n\
                 Note that directory index /id/ will renew in each directory\n\
                 i.e in each directory rename will be performed from start index\n\
                 to end index.\n\
-ifl           : Index field length. not occupied field will be\n\
                 filled with 0's.\n\
\n\
-ns            : Name string\n\
     \n\
-nsf           : Name string file. File containing name string (one per line).\n\
                 [-nsf /hist/] i.e a value passed /hist/ as Name string file, will\n\
                 try to take the file from history.\n\
-l, -sl        : Start Line number in name string file.\n\
-li, -sli      : Same as -l or -sl, except line number will be decremented in each iteration.\n\
-el            : End line number. Line number to stop renaming from.\n\
-eli           : Same as -el, except line number will be decremented in each iteration.\n\
\n\
-ss            : Search string\n\
                 String that will be used to search for files with matching names.\n\
                 Regex is allowed (posix compliant extended regex).\n\
     \n\
\n\
-sF            : Fixed search string (not treated as regex).\n\
\n\
-dp            : Depth of folder. 0 means unlimited depth i.e all files and subdirectories will\n\
                 be included. Other values may be 1 2 3 etc...\n\
                 Default depth is 1.\n\
       \n\
-D             : Directories and files both will be treated in the same way,\n\
                 i.e apply rename on direcotry as well as on files.\n\
                 This is a peculier option and may seem slightly confusing.\n\
                 No subdirectories will be renamed i.e only file and folders on the\n\
                 current directory. If only one directory is passed as the argument,\n\
                 that directory will be renamed not any file or folder inside that\n\
                 directory.\n\
       \n\
-oD            : Apply rename on directory only. No subdirectories.\n\
\n\
-ed            : Apply rename on files only, exclude any and all directory and their contents.\n\
\n\
-y             : Confirm Yes to all\n\
\n\
-f, --force    : Force rename\n\
\n\
-u, -U, --undo : Undo renaming\n\
\n\
-v             : Version info\n\
\n\
-q             : Quiet operation\n\
\n\
Technical Terms:\n\
\n\
Reserved Index    : Index will be incremented even if \n\
                    any file is skipped renaming in order\n\
                    to reserve the index for that skipped file\n\
                \n\
Name String       : A string which is parsed to create name for new files.\n\
                    Name sting is parsed by the following rules:\n\
     /i/ in name string will be replaced with index.\n\
     /ir/ in name string will be replaced with reserved index.\n\
     /id/ in name string will be replaced with directory index (index inside a directory).\n\
     /idr/ in name string will be replaced with reserved directory index\n\
     /n/ in name string will be replaced with filename without extention.\n\
     /fn/ in name string will be replaced with full name of the files.\n\
     /smn/ in name string will be replaced with Sed Modified Name.\n\
     /rn/ in name string will be replaced with Sed Modified Name.\n\
     /l/ in name string will be replaced with line number from list file.\n\
     /dc/ in name string will be replaced with directory count\n\
     /-i/ in name string will be replaced with inverse index.\n\
     /-ir/ in name string will be replaced with inverse reserved index.\n\
  In general - in the above replacement rules (applies to indexes excluding /l/ and /dc/)\n\
  will mean inverse index conforming to their meaning.\n\
  \n\
Inverse Index     : Decrementing index.\n\
     \n\
Search String     : A string that is used to search for files with matching\n\
                    filenames against the search string. By default it is\n\
                    a regex if -sF option is not used.\n\
                 \n\
Index Field Length: An integer value defining the field length of index.\n\
                    empty field will be filled with 0's. For example, if\n\
                    the value is 3, then index will be 001, 002, 003, etc..\n\
                    \n\
Sed Modified Name : The name can be modified at runtime using the default\n\
                    sed tool available in Linux system. You can insert sed\n\
                    script in the form /search_string/replace_string/modifier;\n\
                    \n\
                    For example: '/video/Episode/gi;' will replace every instances\n\
                    of 'video' with 'Episode' in the name of the file found (or the name \n\
                    taken from Name String File).\n\
                    \n\
                    The format must be of :\n\
                    /search_string/replace_string/modifier;\n\
                    \n\
                    Two modifiers are avalilable: g and i.\n\
                    g stands for global. Replaces every instances of match found.\n\
                    i stands case insensitive search.\n\
                    \n\
                    This replaced modified name is available through Name String Rule: \n\
                    /smn/ or /rn/ (stands for replaced name)\n\
                    \n\
                    Note that sed basic regex mode is used to do this job.\n\
";



#endif
