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
 * * Try to skip files with warning (not error).
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
//~ #include <sys/sendfile.h> //not available on mac (not POSIX either)
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
bool double_quiet=false;
bool force=false;
bool super_force=false;
bool all_yes=false;
bool ALL_YES=false;
bool directory_only=false;
bool file_only=false;
bool link_only=false;
bool exclude_directory=false;
bool exclude_file=false;
bool exclude_link=false;
bool reverse_line=false;
bool undo=false;
bool undo_path_show=false;
bool simulation=false;
bool line_upward=true;
bool ign=false;
bool nsf_n=false;
bool single_mode=false;
bool re_locale=false;
bool count_directory=false;
bool count_file=false;
bool count_link=false;
bool sort=false;
bool follow_symlink=false;
bool number_latin=false;
bool reverse_sort=false;
bool uppercase=false;
bool showbase=false;
bool showpoint=false;
bool showpos=false;
bool infinite_end_index=true;


/////Doubles
Double start_index=1;
Double current_index=1;
Double current_index_rd=1;
Double reverse_start_index=-1;
Double reverse_index=reverse_start_index;
Double reverse_index_rd=reverse_start_index;
Double end_index = 0;
Double inc=1;
Double DIRECTORY_INDEX=1;
Double DIRECTORY_REVERSE_INDEX=-1;

/////ints
//~ Int INT_MAX = std::numeric_limits<std::size_t>::max();
Int linc=1;
Int start_line=1;
Int end_line=0;
Uint current_line=1;
Uint current_line_pos = 0;
//~ Int current_abs_line=start_line;
Int directory_count=0;
Int depth=0;
Int LOWEST_DEPTH=0;
Int index_field_length=1;
Int rnc=0; //rename count
//~ Int DUPT = "2592000"; //seconds in a month
Int dupt = 0; //delete undo path which are older than dupt seconds ago


//////// nsf related
StringArray nsflist;
//~ UintVec abslc_list;
UintVec lc_list;

/////Strings
String undo_path;
String blank_str="";
String NEW_LINE="\n";
String path_delim="/";
String second_delim("\x1f"); //unit separator character dec:31 oc \037 hex: \x1f
String root_filesystem="/";
String CPDN="";
String CWDN="";
String CWD="";


////getting Linux Home, data home and defining some paths
String getUnixHome(){
    char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    return String(homedir);
}
String HOME=getUnixHome();
String getUnixDataHome(){
    char *homedir;
    if ((homedir = getenv("XDG_DATA_HOME")) == NULL) {
        return HOME + "/.local/share";
    }
    return String(homedir);
}
//~ String LOG_DIR_PARENT=HOME+"/.cache/neurobin";
String LOG_DIR_PARENT = getUnixDataHome() + "/neurobin";
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
String RNM_FILE_LOG_L_S;

String RNM_FILE_LOG_R = LOG_DIR_UNDO+"/rfl.r";
String RNM_FILE_LOG_R_BKP = RNM_FILE_LOG_R;
FileStream RNM_FILE_LOG_R_F;
String RNM_FILE_LOG_R_S;


/////Name string related
String PD_DELIM=" ";
String ns_name;


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
String base_dir;
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
String sort_type2;


/// Project Info
String project_name="rnm";
String executable_name="rnm";
String version="4.0.3";
String author_name="Md Jahidul Hamid";
String author_email="jahidulhamid@yahoo.com";
String bug_report_url="https://github.com/neurobin/"+project_name+"/issues";



String version_info="\
Name         : "+project_name+" (Bulk rename utility)\n\
Version      : "+version+"\n\
Author       : "+author_name+"\n\
Author Email : "+author_email+"\n\
Bug Report   : "+bug_report_url+"\n\
";


String help_message="\n\
************** "+project_name+" "+version+" **************\n\
\n\
Usage: "+project_name+" [options] directory/file/path\n\
\n\
Options are case insensitive.\n\
\n\
Options:\n\
\n\
  -ns       value: Name string.\n\
  -ns/f     path : Name string file.\n\
  -ns/fn    path : Null terminated Name String file.\n\
  -l, -sl   value: Start Line number.\n\
  -el       value: End line number\n\
  -linc     value: Line increment value.\n\
  -ss       value: Search string\n\
  -ss/f     path : Search string file.\n\
  -ssf      value: Fixed search string.\n\
  -ssf/f    path : Fixed search string file.\n\
  -rs       value: Replace string.\n\
  -rs/f     path : Replace string file.\n\
  -i,-si    value: Starting index.\n\
  -ei       value: End index.\n\
  -inc      value: Increment value.\n\
  -if       value: Index flags.\n\
  -ifl      value: Index field length.\n\
  -iff      value: Index field filler.\n\
  -ifp      value: Index field precision\n\
  -dp       value: Depth of directory.\n\
  -duh      value: Delete undo history older\n\
                   than given seconds.\n\
  -duhd     value: Delete undo history older\n\
                   than given days.\n\
  -fo            : File only mode.\n\
  -do            : Directory only mode.\n\
  -lo            : Link only mode.\n\
  -xd            : Exclude any and all directories\n\
  -xf            : Exclude any and all files\n\
  -xl            : Exclude any and all links\n\
  -fl            : Follow symlink.\n\
  -nfl           : No-follow symlink.\n\
  -cd            : Count directory in reserved index\n\
                   regardless of other options.\n\
  -cf            : Count file in reserved index\n\
                   regardless of other options.\n\
  -cl            : Count link in reserved index\n\
                   regardless of other options.\n\
  -s             : Sort files (default natural sort).\n\
                   -s/n    : Natural sort\n\
                   -s/g    : General sort\n\
                   -s/mt   : Sort according to mtime\n\
                   -s/at   : Sort according to atime\n\
                   -s/ct   : Sort according to ctime\n\
                   -s/sz   : Sort according to size\n\
                   -s/d    : Prioritize directory\n\
                   -s/f    : Prioritize file\n\
                   -s/l    : Prioritize link\n\
                   -s/r    : Reverse the sort\n\
                   -s/no   : No sort\n\
  -y             : Confirm Yes to all and\n\
                   suppress output for every file.\n\
  -u             : Undo renaming.\n\
  -up            : Undo from a undo path\n\
  -ups           : Show available undo paths\n\
  -q             : Quiet operation (includes -y).\n\
  -qq            : Suprress even errors\n\
  -f             : Apply force.\n\
  -ff            : Super force.\n\
                   Bypass all restrictions.\n\
  --             : End of option.\n\
                   Everything after this will be\n\
                   taken as file paths.\n\
  -h             : Show help menu.\n\
  -v             : Version info.\n\
  -sim           : Run simulation.\n\
\n\
See more details on the manual (man rnm).\n\
\n\
";


#endif
