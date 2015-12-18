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
#include <cmath>
#include <algorithm>


/////typedefs
typedef long long Int;
typedef unsigned long long Uint;
typedef long double Double;
typedef std::string String;
typedef std::stringstream Stream;
typedef std::vector<String> StringArray;
typedef std::vector<bool> BoolArray;
typedef std::regex Regex;
typedef std::map<Int,String> NameList;
typedef std::map<Int,Int> IntMap;
typedef std::regex::flag_type RegexType;
typedef std::regex_iterator<std::string::iterator> RegexIterator;
typedef std::regex_token_iterator<std::string::iterator> RegexTokenIterator;
typedef std::smatch RegexResult;
typedef std::numeric_limits< Double > DoubleLimit;
typedef std::ios_base::fmtflags IOFormatFlag;
typedef std::vector<IOFormatFlag> IOFormatFlagArray;
//~ typedef std::function<std::ios_base&(std::ios_base&)> StdFlag;
//~ typedef std::vector<StdFlag> FlagArray;
typedef std::map<String,int> StringintMap;


/////defs
#define Exit(a) std::exit((a))
#define getLine(s) std::getline(std::cin,(s))
#define getLineFromFile(f,line,delim) std::getline((f),(line),(delim))
#define regexMatch(s,e) std::regex_match((s),(e))
#define getCurrentDir(a) getcwd((a), sizeof((a)))
#define NON_RECURSIVE_REGEX_REPLACE std::regex_constants::format_first_only
#define DEFAULT_REGEX_REPLLACE std::regex_constants::format_default
#define REGEX_ECMASCRIPT std::regex::ECMAScript
#define ICASE std::regex::icase
#define REGEX_BASIC std::regex::basic
#define REGEX_EXTENDED std::regex::extended
#define REGEX_AWK std::regex::awk
#define REGEX_GREP std::regex::grep
#define REGEX_EGREP std::regex::egrep
#define REGEX_DEFAULT REGEX_ECMASCRIPT
#define REGEX_LOCALE std::regex::collate

#define PCRE2_CODE_UNIT_WIDTH 8
#define print std::cout<<
#define printe std::cerr<<
#define ENDL std::endl
#define input std::cin>>
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
BoolArray fixed_ss;
BoolArray ss_fixed;
bool show_options=false;
bool simulation=false;
bool line_upward=true;
bool ign=false;
bool nsf_n=false;
bool single_mode=false;
bool re_locale=false;
bool count_directory=false;
bool count_file=false;
bool sort=false;
bool follow_symlink=false;
bool number_latin=false;
bool reverse_sort=false;

//experimental
bool uppercase=false;
bool showbase=false;
bool showpoint=false;
bool showpos=false;


/////Doubles

Double LIMIT_OF_DOUBLE =std::numeric_limits<Double>::max();
Double start_index=1;
Double current_index=1;
Double current_index_rd=1;
Double reverse_start_index=-1;
Double reverse_index=reverse_start_index;
Double reverse_index_rd=reverse_start_index;
Double end_index=LIMIT_OF_DOUBLE;
Double inc=1;
Double DIRECTORY_INDEX=1;
Double DIRECTORY_REVERSE_INDEX=-1;


/////ints

Int INT_MAX =std::numeric_limits<Int>::max();
Int linc=1;
Int start_line=1;
Int current_line=start_line;
Int current_abs_line=start_line;
Int end_line=0;
Int directory_count=0;
Int depth=0;
Int LOWEST_DEPTH=0;
int index_field_length=1;
Int rnc=0; //rename count
IntMap abslc_list;

//////// nsf related
NameList nsflist;


//// Regex related
String re_type="";
RegexType REGEX_TYPE=REGEX_DEFAULT;


/////Name string related

String PD_DELIM=" ";


//////// Index flag related
int LATIN_FALLBACK=55555;
int latin_fall_back_threshold=LATIN_FALLBACK;
char FLAG_DELIM='/';
int NUM_BASE=10;
int NUM_BASE_MIN=2;
int NUM_BASE_MAX=36;
String IFF="0"; //index field filler
int IFP=-1;    //index field precision

///////Index flags management

std::map<String,int> index_int_flag={{"precision",IFP},{"width",index_field_length},{"latin-fallback",latin_fall_back_threshold}};
std::map<String,String> index_string_flag={{"filler",IFF}};

String IFP_s="unset";
IOFormatFlag INDEX_FLAG_FLOAT=std::ios::fixed;
IOFormatFlag index_flag_adjust=std::ios::right;
String index_flag_adjust_s="right";
IOFormatFlagArray index_flag_ind {};
StringArray index_flag_ind_s {};
///The following array couples must be in sync, don't change the sequence, they are important
StringArray INDEX_FLAGS_AV_A={"internal","right","left"};
IOFormatFlagArray INDEX_FLAGS_A={std::ios::internal,std::ios::right,std::ios::left};
StringArray INDEX_FLAGS_AV_I={"uppercase","showbase","showpoint","showpos"};
std::map<String,bool> INDEX_FLAGS_I_B={{"uppercase",false},{"showbase",false},{"showpoint",false},{"showpos",false}};
IOFormatFlagArray INDEX_FLAGS_I={std::ios::uppercase,std::ios::showbase,std::ios::showpoint,std::ios::showpos};
///The above array couples must be in sync, don't change the sequence, they are important






String blank_str="";
String NEW_LINE="\n";



/////Strings
String path_delim="/";
String root_filesystem="/";
String CPDN="";
String CWDN="";
String CWD="";
////getting Linux Home and defining some paths
String getUnixHome(){const char *homedir;if ((homedir = getenv("HOME")) == NULL) {homedir = getpwuid(getuid())->pw_dir;}return String(homedir);}
String HOME=getUnixHome();
String LOG_DIR_PARENT=HOME+path_delim+".neurobin";
String LOG_DIR=LOG_DIR_PARENT+path_delim+"rnm";
String ERROR_LOG=LOG_DIR+path_delim+"errors.log";
String OUT_LOG=LOG_DIR+path_delim+"out.log";
String RNM_FILE_LOG_L=LOG_DIR+path_delim+"rfl.l.log";
String RNM_FILE_LOG_R=LOG_DIR+path_delim+"rfl.r.log";
String RNM_FILE_LOG_L_TMP=RNM_FILE_LOG_L+".tmp";
String RNM_FILE_LOG_R_TMP=RNM_FILE_LOG_R+".tmp";
String NSF_LIST_FILE=LOG_DIR+path_delim+"nsf.list";


//StringArray file_vector;
char self_dir[FILENAME_MAX];
String base_dir="";
String self_path="";
StringArray search_string;
String search_string_file="";
StringArray replace_string;
String replace_string_file="";
String name_string="";
String name_string_file="";
String rname="";
StringArray rs_search;
StringArray rs_replace;
StringArray rs_mod;
StringArray ss_search;
StringArray ss_mod;
String sort_type="natural";


/// Project Info
String project_name="rnm";
String executable_name="rnm";
String version="3.3.0";
String author_name="Jahidul Hamid";
String author_email="jahidulhamid@yahoo.com";
String bug_report_url="http://github.com/neurobin/"+project_name+"/issues";



String version_info="\n\
Rename Utility "+project_name+"\n\
Version:        "+version+"\n\
Author:         "+author_name+"\n\
Author Email:   "+author_email+"\n\
Bug Report:     "+bug_report_url+"\n\
";


String help_message="\n\
************** "+project_name+" "+version+" *******************\n\
\n\
Usage: "+project_name+" Directory/File/Path [options]\n\
\n\
One of the options of [-ns] or [-nsf] or [-rs] is mandatory.\n\
Options are **not** sequential, their position in the argument list\n\
have no significance. For example, `rnm filepath -ns name` is the same\n\
as `rnm -ns name filepath`. Though passing the directory/file path at the end\n\
of the argument list is considered to be safe and wise.\n\
\n\
Options are case insensitive, i.e `-ssF` and `-ssf` are the same.\n\
\n\
options:\n\
\n\
-h             : Show help menu.\n\
\n\
-i,-si   value : Starting index.\n\
\n\
-ei    value   : End index i.e index to stop renaming from. It works on directory index.\n\
                 \n\
-inc   value   : Increment value (floating point decimal). The amount, index will be\n\
                 incremented or decremented in each iteration. Decremented index is\n\
                 available through name string rule: `"+path_delim+"-i"+path_delim+"`, `"+path_delim+"-id"+path_delim+"` etc..\n\
                 \n\
-linc  value   : The amount line count will be incremented or decremented in each iteration.\n\
                 This is always a positive integer.\n\
                 \n\
                 This can also be set with the -if option.\n\
-if    value   : This sets Index flags. This is a "+path_delim+" separated list of flags\n\
                 that will be used to render the index within it's text field.\n\
\n\
-ifl   value   : Index field length. Non occupied field will be\n\
                 filled with index field fillers (set with -iff). iff is set to the\n\
                 character 0 by default.\n\
                 This can also be set with the -if option.\n\
\n\
-iff   value   : Not occupied field in index will be filled with a character\n\
                 which is set by this option.\n\
                 This can also be set with the -if option.\n\
                 \n\
-ifp   value   : Index is a floating point decimal value. This sets the precision\n\
                 i.e the number of digits that would be taken after the decimal point.\n\
                 This can also be set with the -if option.\n\
                 \n\
-ns    value   : Name string.\n\
     \n\
-ns/f  value   : Name string file. File containing name string (one per line).\n\
\n\
-ns/fn value   : Name String file. This takes a null terminated *Name String* file, i.e\n\
                 filenames are terminated by null character (\\0) instead of new line (\\n).\n\
                 \n\
-l, -sl value  : Start Line number in name string file.\n\
\n\
-lv, -slv value: Same as -l or -sl, except line number will be decremented in each\n\
                 iteration.\n\
\n\
-el    value   : End line number. Line number to stop renaming from.\n\
\n\
-elv   value   : Same as -el, except line number will be decremented in each iteration.\n\
\n\
-ss    value   : Search string\n\
                 String that will be used to search for files with matching names.\n\
                 This is generally regex (ECMAScript regex) if not pass with -ssf.\n\
                 \n\
-ss/f  value   : Search string file. Contains search strings per line.\n\
     \n\
-ssf   value   : Fixed search string (not treated as regex).\n\
\n\
-ssf/f value   : Fixed search string file. Contains fixed search string (per line).\n\
\n\
-rs    value   : Replace string. A string in the form /search_string/replace_string/modifier \n\
\n\
-rs/f  value   : Replace string file. Contains replace string (per line).\n\
\n\
-re    value   : regex mode. Available regex modes are basic, extended, grep, awk, egrep, ecmascript.\n\
                 ECMAScript regex is the default mode.\n\
\n\
-rel           : If this is passed as argument, regex will follow Locale. that is regex like\n\
                [a-z] will have their meaning according to the system locale.\n\
                \n\
-dp    value   : Depth of folder. -1(any negative number) means unlimited depth i.e all files and subdirectories\n\
                 will be included. Other values may be 0 1 2 3 etc...\n\
                 Default depth is 0, i.e directory contents will be ignored.\n\
       \n\
-fo            : File only mode. Only files are renamed (not directory).\n\
                 Goes to subdirectory/s if depth (`-dp`) is set to `1` or greater.\n\
                 Default depth is set to 0.\n\
                 \n\
-do            : Apply rename on directory only.\n\
\n\
-ed            : Apply rename on files only, exclude any and all directories and their\n\
                 contents. equivalent to: '-fo -dp 0'\n\
\n\
-cd            : Count directory in reserved index, regardless of other options.\n\
\n\
-cf            : Count file in reserved index, regardless of other options.\n\
\n\
-s             : Sort files. Default is natural sort. -s/g for general sort.\n\
\n\
-y             : Confirm Yes to all.\n\
\n\
-fl            : follow symlink.\n\
\n\
-u             : Undo renaming.\n\
\n\
-v             : Version info.\n\
\n\
-q             : Quiet operation.\n\
\n\
-f             : Apply force. Enables renaming some non permitted files/directories\n\
                 except / (The root filesystem directory) and rnm itself.\n\
\n\
--             : Everything after this will be taken as file paths.\n\
\n\
-shop          : This shows an info about the various options passed as arguments\n\
                 and how they are being treated behind the scene.\n\
                 \n\
-sim           : This runs a simulation of rename instead of actual rename operation,\n\
                 and prints all kinds of available outputs.\n\
                 \n\
Only invalid characters for a file or directory name is the path delimiter and the null character (\\0).\n\
See more details on the manual (man rnm).\n\
";



#endif
