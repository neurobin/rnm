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
typedef std::regex::flag_type RegexType;


/////defs
#define Exit(a) std::exit((a))
#define getLine(s) std::getline(std::cin,(s))
#define getLineFromFile(f,line,delim) std::getline((f),(line),(delim))
#define regexMatch(s,e) std::regex_match((s),(e))
#define getCurrentDir(a) getcwd((a), sizeof((a)))
#define RegexResult std::smatch
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
bool simulation=false;
bool line_upward=true;
bool ign=false;
bool nsf_n=false;
bool single_mode=false;
bool re_locale=false;

/////Doubles
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

//////// IFL related
String IFF="0"; //input field filler
int IFP=2;    //input field precision
String blank_str="";
String NEW_LINE="\n";
/////Strings
String path_delim="/";
String root_filesystem="/";
String CPDN="";
String CWDN="";
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

/// Project Info
String project_name="rnm";
String executable_name="rnm";
String version="3.0.4";
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
-h, --help     : Show help menu\n\
\n\
--index, --start-index,\n\
-i,-si         : Starting index\n\
\n\
--end-index,\n\
-ei,           : End index i.e index to stop renaming from. It works on directory index.\n\
                 \n\
--increment-value,\n\
-inc           : Increment value (floating point decimal). The amount, index will be\n\
                incremented or decremented in each iteration. Decremented index is\n\
                 available through name string rule: `"+path_delim+"-i"+path_delim+"`, `"+path_delim+"-id"+path_delim+"` etc..\n\
                 \n\
--line-increment-value,\n\
-linc          : The amount line count will be incremented or decremented in each iteration.\n\
                 This is always a positive integer.\n\
                 \n\
--index-field-length,\n\
-ifl           : Index field length. Non occupied field will be\n\
                 filled with index field fillers (set with -iff). iff is set to the\n\
                 character 0 by default.\n\
\n\
--index-field-filler,\n\
-iff           : Not occupied field in index will be filled with a character\n\
                 which is set by this option.\n\
                 \n\
--index-field-precision,\n\
-ifp           : Index is a floating point decimal value. This sets the precision\n\
                 i.e the number of digits that should be taken after the decimal point.\n\
                 \n\
--name-string,\n\
-ns            : Name string\n\
     \n\
--name-string-file,\n\
-nsf           : Name string file. File containing name string (one per line).\n\
--name-string-file-null-terminated,\n\
-nsfn          : Name String file. This takes a null terminated *Name String* file, i.e\n\
                 filenames are terminated by null character (\\0) instead of new line (\\n).\n\
                 \n\
--line, --start-line,\n\
-l, -sl        : Start Line number in name string file.\n\
\n\
--line-reverse, --start-line-reverse,\n\
-lv, -slv      : Same as -l or -sl, except line number will be decremented in each\n\
                 iteration.\n\
\n\
--end-line,\n\
-el            : End line number. Line number to stop renaming from.\n\
\n\
--end-line-reverse,\n\
-elv           : Same as -el, except line number will be decremented in each iteration.\n\
\n\
--search-string,\n\
-ss            : Search string\n\
                 String that will be used to search for files with matching names.\n\
                 This is generally regex (ECMAScript regex) if not pass with -ssf.\n\
     \n\
--search-string-fixed,\n\
-ssf           : Fixed search string (not treated as regex).\n\
\n\
--replace-string,\n\
-rs            : Replace string. A string in the form /search_string/replace_string/modifier \n\
\n\
--regex,\n\
-re            : regex mode. Available regex modes are basic, extended, grep, awk, egrep, ecmascript.\n\
                 ECMAScript regex is the default mode.\n\
\n\
--regex-locale,\n\
-rel            : If this is passed as argument, regex will follow Locale. that is regex like\n\
                [a-z] will have their meaning according to the system locale.\n\
                \n\
--depth,\n\
-dp            : Depth of folder. -1(any negative number) means unlimited depth i.e all files and subdirectories\n\
                 will be included. Other values may be 0 1 2 3 etc...\n\
                 Default depth is 0, i.e directory contents will be ignored.\n\
       \n\
--file-only,\n\
-fo            : File only mode. Only files are renamed (not directory).\n\
                 Goes to subdirectory/s if depth (`-dp`) is set to `1` or greater.\n\
                 Default depth is set to 0.\n\
                 \n\
--directory-only,\n\
-do            : Apply rename on directory only. No subdirectories.\n\
\n\
--exclude-directory,\n\
-ed            : Apply rename on files only, exclude any and all directory and their\n\
                 contents. Forces file only mode regardless of depth (-dp) value.\n\
\n\
--yes\n\
-y             : Confirm Yes to all\n\
\n\
--undo,\n\
-u             : Undo renaming\n\
\n\
--version,\n\
-v             : Version info\n\
\n\
--quiet,\n\
-q             : Quiet operation\n\
\n\
--force\n\
-f             : Apply force. Enables renaming some non permitted files/directories\n\
                 except / (The root filesystem directory) and rnm itself\n\
\n\
--show-options,\n\
-shop          : This shows an info about the various options passed as arguments\n\
                 and how they are being treated behind the scene.\n\
                 \n\
--simulation,\n\
-sim           : This runs a simulation of rename instead of actual rename operation,\n\
                 and prints all kinds of available outputs.\n\
                 \n\
Terminology       :\n\
\n\
Reserved Index    : Index will be incremented even if \n\
                    any file is skipped renaming in order\n\
                    to reserve the index for that skipped file\n\
                \n\
Reverse Index     : Decrementing index.\n\
\n\
Name String       : A string which is parsed to create name for new files.\n\
                    Name sting must be wrapped around with filepath delimiter "+path_delim+":\n\
                    "+path_delim+"pd"+path_delim+": parent directory name,"+path_delim+"wd"+path_delim+": working directory name.\n\
                    "+path_delim+"i"+path_delim+": index, "+path_delim+"ir"+path_delim+": index reserved,"+path_delim+"id"+path_delim+": directory index,\n\
                    "+path_delim+"idr"+path_delim+": reserved directory index,"+path_delim+"n"+path_delim+": name without extension,\n\
                    "+path_delim+"fn"+path_delim+": full name,"+path_delim+"rn"+path_delim+": replaced name,"+path_delim+"l"+path_delim+": line number,\n\
                    "+path_delim+"la"+path_delim+": actual line number,"+path_delim+"dc"+path_delim+": directory count,\n\
                    "+path_delim+"-i"+path_delim+": inverse index,"+path_delim+"-ir"+path_delim+": inverse reserved index,\n\
                    In general - in the above replacement rules (applies to indexes excluding line index)\n\
                    will mean reverse index conforming to their meaning. See man rnm for more details.\n\
  \n\
     \n\
Name String File  : A file which contains a list of name string (one per line).\n\
                    Empty lines will be ignored and line number won't be counted.\n\
                    Actual line number (which counts the empty lines too) is\n\
                    available through name string rule : "+path_delim+"la"+path_delim+".\n\
                    If passed with -nsfn filename will be expected to be null terminated.\n\
                    \n\
Search String     : A string that is used to search for files with matching\n\
                    filenames against the search string. By default it is\n\
                    a regex if `-ssF` option is not used. It uses the \n\
                    ECMAScript regex syntax by default.\n\
                 \n\
Index Field Length: An integer value defining the field length of index.\n\
                    By default empty field will be filled with 0's. For example, if\n\
                    the value is 3, then index will be 001, 002, 003, etc..\n\
                    Different filler (other than 0) can be provided with the `-iff` option.\n\
                    \n\
Replaced Name     : The name can be modified at runtime using replace string.\n\
                    replace string will be parsed to create a new *Name String* rule: "+path_delim+"rn"+path_delim+"\n\
                    which can be used in *Name String*. If name string is not passed as argument,\n\
                    the new name of the file will be `/rn/`. *Replaced Name* is always \n\
                    generated from the old filename.\n\
                    \n\
Replace String    : *Replace String* is a regex of the form: \n\
                    "+path_delim+"search_part"+path_delim+"replace_part"+path_delim+"modifier\n\
                    where search_part is the regex to search for and\n\
                    replace_part is the string to replace with. Name String rules can be used\n\
                    in search_part and replace_part in Replace String.\n\
                    \n\
                    *Replace String* is always performed on old file name.\n\
                    See details on the manual (man rnm).\n\
                    \n\
    Only invalid characters for a file or directory name is the path delimiter and the null character (\\0).\n\
    See more details on the manual (man rnm)\n\
";



#endif
