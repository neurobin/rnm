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
 
 
#ifndef __GLOBALS_H
#define __GLOBALS_H


/////includes
#define _FILE_OFFSET_BITS 64
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
#include <csignal>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <cctype>
#include <cstddef> 
#include <dirent.h>
#include <map>
#include <cmath>
#include <algorithm>
#include <locale>
#include <exception>
#include <cstdint> //Fixed width integral types: uint_fast64_t, uint_fast8_t
#include <gmpxx.h> //mpf_class, mpz_class
#include "strnatcmp.hpp"
#define JPCRE2_DISABLE_CHAR1632
#include <jpcre2.hpp>
#include "proto.hpp"
#include "futil.hpp"
#include <chrono>
#include <ratio>

//~ #define RNM_ENABLE_ICU

#ifdef RNM_ENABLE_ICU
#include <unicode/unistr.h>//icu::UnicodeString
#include <unicode/locid.h> //icu::Locale
#else
#define toLower toLower0
#define toUpper toUpper0
#endif

//#define RNM_RELEASE //uncomment it when going for production release

#ifndef RNM_RELEASE
#define LOG(a) std::cout<<a<<"\n";
#else
#define LOG(a) 
#endif


thread_local static std::ostringstream oss_buffer;



typedef std::chrono::high_resolution_clock::time_point TimeVar;
typedef long long Long;
typedef double TimeType;
//~ #define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define duration(a) std::chrono::duration<TimeType, std::ratio<1,1>>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
TimeVar START_TIME = timeNow(); //backup time
TimeType TIME_COUNT = 0;


Double KB = 1024;
Double MB = KB*KB;
Double GB = MB*KB;
Double TB = GB*KB;

/////Bolleans
bool quiet=false;
bool force=false;
bool all_yes=false;
bool directory_only=false;
bool file_only=false;
bool exclude_directory=false;
bool reverse_line=false;
bool undo=false;
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
bool uppercase=false;
bool showbase=false;
bool showpoint=false;
bool showpos=false;


/////Doubles
Double start_index=1;
Double current_index=1;
Double current_index_rd=1;
Double reverse_start_index=-1;
Double reverse_index=reverse_start_index;
Double reverse_index_rd=reverse_start_index;
Double end_index = std::numeric_limits<std::size_t>::max();
Double inc=1;
Double DIRECTORY_INDEX=1;
Double DIRECTORY_REVERSE_INDEX=-1;

/////ints
//~ Int INT_MAX = std::numeric_limits<std::size_t>::max();
Int linc=1;
Int start_line=1;
Int current_line=start_line;
Int current_abs_line=start_line;
Int end_line=0;
Int directory_count=0;
Int depth=0;
Int LOWEST_DEPTH=0;
Int index_field_length=1;
Int rnc=0; //rename count
IntVec abslc_list;


//////// nsf related
NameList nsflist;

/////Strings
String blank_str="";
String NEW_LINE="\n";
String path_delim="/";
String second_delim("\x1f"); //unit separator character dec:31 oc \037 hex: \x1f
String root_filesystem="/";
String CPDN="";
String CWDN="";
String CWD="";
////getting Linux Home and defining some paths
String getUnixHome(){const char *homedir;if ((homedir = getenv("HOME")) == NULL) {homedir = getpwuid(getuid())->pw_dir;}return String(homedir);}
String HOME=getUnixHome();
String LOG_DIR_PARENT=HOME+"/.cache/neurobin";
String LOG_DIR=LOG_DIR_PARENT+"/rnm";
String LOG_DIR_UNDO=LOG_DIR+"/unrec";
String ERROR_LOG=LOG_DIR+"/errors.log";
FileStream ERROR_LOG_F;
String OUT_LOG=LOG_DIR+"/out.log";
FileStream OUT_LOG_F;

String RNM_LOCK_FILE = LOG_DIR + ".lock";
String RNM_LOCK_FILE_BKP = RNM_LOCK_FILE;
futil::FileStream RNM_LOCK_FILE_F;

String RNM_FILE_LOG_L=LOG_DIR_UNDO+"/rfl.l";
String RNM_FILE_LOG_L_BKP = RNM_FILE_LOG_L;
FileStream RNM_FILE_LOG_L_F;

String RNM_FILE_LOG_R = LOG_DIR_UNDO+"/rfl.r";
String RNM_FILE_LOG_R_BKP = RNM_FILE_LOG_R;
FileStream RNM_FILE_LOG_R_F;

String RNM_FILE_LOG_L_TMP = RNM_FILE_LOG_L+".tmp";
String RNM_FILE_LOG_L_TMP_BKP = RNM_FILE_LOG_L_TMP;
FileStream RNM_FILE_LOG_L_TMP_F;

String RNM_FILE_LOG_R_TMP = RNM_FILE_LOG_R+".tmp";
String RNM_FILE_LOG_R_TMP_BKP = RNM_FILE_LOG_R_TMP;
FileStream RNM_FILE_LOG_R_TMP_F;

String NSF_LIST_FILE=LOG_DIR+"/nsf.list";

/////Name string related
String PD_DELIM=" ";


//////// Index flag related
Int LATIN_FALLBACK=55555;
Int latin_fall_back_threshold=LATIN_FALLBACK;
char FLAG_DELIM='/';
Ush NUM_BASE_DEFAULT=10;
Ush NUM_BASE_MIN=2;
Ush NUM_BASE_MAX=36;
String IFF="0"; //index field filler
Int IFP=0;    //index field precision
//~ Short PCRECISION = 0;

///////Index flags management

std::map<String,Int> index_int_flag={{"precision",IFP},{"length",index_field_length},{"latin-fallback",latin_fall_back_threshold}};
std::map<String,String> index_string_flag={{"filler",IFF}};

String IFP_s="unset";
std::map<String, IOFormatFlag> INDEX_FLAGS_M = {
                                                    { "internal",   std::ios::internal },
                                                    { "right",      std::ios::right },
                                                    { "left",       std::ios::left },
                                                    { "uppercase",  std::ios::uppercase },
                                                    { "showbase",   std::ios::showbase },
                                                    { "showpoint",  std::ios::showpoint },
                                                    { "showpos",    std::ios::showpos }
                                                };
IOFormatFlag INDEX_FLAGS = (IOFormatFlag)0;


/////Strings


//StringArray file_vector;
String self_dir;
String base_dir;
String self_path;
StringArray search_string;
String search_string_file;
StringArray replace_string;
String replace_string_file;
String name_string;
String name_string_file;
String rname;
///RegexArray rs_search_re; ///Unfortunately this can not be used. replace string is dynamic and depends on each filename.
StringArray rs_search;
StringArray rs_replace;
StringArray rs_mod;

String dtf = "%d-%m-%Y";


RegexArray ss_search_re; ///search strings are static and do not depend on each filename, therefore they can be globalized
StringArray ss_search;
StringArray ss_mod;
BoolArray fixed_ss;
BoolArray ss_fixed;
String SS_MOD_F_ALL = "fdl";
String SS_MOD_ALL = "ifdl!";
String RR_MOD_ALL = "gifdl";


String sort_type="natural";


/// Project Info
String project_name="rnm";
String executable_name="rnm";
String version="4.0.0";
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
