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
#include "funcs.hpp"

extern "C" {
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h> 
}
//~ template<typename T>
//~ std::string toString(T a){
    //~ std::stringstream ss;
    //~ ss <<a;
    //~ return ss.str();
//~ }



int main(){
    
    
pcre2_code *re;

int errornumber;


PCRE2_SIZE erroroffset;
//~ int bh[8];
//~ pcre2_config(PCRE2_CONFIG_JIT,bh);

//~ std::cout<<"jit: "<<*bh<<"\n";


std::cout<<"\nreached: ";

/* As pattern and subject are char arguments, they can be straightforwardly
cast to PCRE2_SPTR as we are working in 8-bit code units. */

PCRE2_SPTR pattern = (PCRE2_SPTR)(R"(.*)");
PCRE2_SPTR subject = (PCRE2_SPTR)(R"(I am  7a a  8b simp6ele string)");
PCRE2_SPTR replace = (PCRE2_SPTR)(R"($0fsdgfg)");
PCRE2_SIZE subject_length = strlen((char *)subject);


/*************************************************************************
* Now we are going to compile the regular expression pattern, and handle *
* any errors that are detected.                                          *
*************************************************************************/

re = pcre2_compile(
  pattern,               /* the pattern */
  PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
  PCRE2_MATCH_UNSET_BACKREF,                     /* default options */
  &errornumber,          /* for error number */
  &erroroffset,          /* for error offset */
  NULL);                 /* use default compile context */

/* Compilation failed: print the error message and exit. */

if (re == NULL)
  {
  PCRE2_UCHAR buffer[256];
  pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
  printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset,
    buffer);
  return 1;
  }

print FILENAME_MAX;
PCRE2_SIZE replace_length = strlen((char *)replace);
PCRE2_SIZE outlengthptr=FILENAME_MAX;
PCRE2_UCHAR* output_buffer;
output_buffer=(PCRE2_UCHAR*)malloc(outlengthptr * sizeof *output_buffer);
uint32_t rplopts=PCRE2_SUBSTITUTE_GLOBAL;
int ret=pcre2_substitute(
  re,                    /*Points to the compiled pattern*/
  subject,               /*Points to the subject string*/
  subject_length,        /*Length of the subject string*/
  0,                     /*Offset in the subject at which to start matching*/
  rplopts,               /*Option bits*/
  0,                     /*Points to a match data block, or is NULL*/
  0,                     /*Points to a match context, or is NULL*/
  replace,               /*Points to the replacement string*/
  replace_length,        /*Length of the replacement string*/
  output_buffer,         /*Points to the output buffer*/
  &outlengthptr          /*Points to the length of the output buffer*/
);
std::cout<<"rl: "<<replace_length<<"\n";
std::cout<<"ob: "<<(unsigned char *)output_buffer<<"\n"<<"ret: "<<ret;

if (ret <=0)
  {
  PCRE2_UCHAR buffer2[256];
  pcre2_get_error_message(ret, buffer2, sizeof(buffer2));
  printf("PCRE2 replace failed at offset %d: %s\n", (int)ret,
    buffer2);
  return 1;
  }



	return 0;
}
