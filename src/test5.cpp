#include "funcs.hpp"
#include "times.hpp"



#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

typedef std::vector<std::map<std::string,std::string>> MatchVector;
typedef std::map<std::string,std::string> MatchMap;

//~ template<typename T>
//~ std::string toString(T a){
    //~ std::stringstream ss;
    //~ ss <<a;
    //~ return ss.str();
//~ }

std::string returnRegexerror(const std::string& errorstring,int erroroffset, PCRE2_UCHAR* buffer){
    return errorstring+std::to_string(erroroffset)+": "+std::string((char*)buffer);
 }

class Pcre2Regex{
    private:
    std::string pat_str;
    std::string modifier;
    PCRE2_SPTR pattern;
    pcre2_code *code;
    int error_number;
    PCRE2_SIZE error_offset;
    uint32_t compile_opts,action_opts,jit_opts;
    
    
    ///other opts
    bool opt_jit_compile;
    
    ///Exceptions
    std::string REGEX_COMPILE_ERROR = "E: Regex compilation failed at offset ";
    std::string JIT_COMPILE_ERROR   = "E: JIT compilation failed.";
    std::string REGEX_REPLACE_ERROR = "E: Regex replace failed at offset ";
    
    public:
    
    //~ Pcre2Regex(const std::string& re,const std::string& mod=""){pat_str=re;modifier=mod;}
    ~Pcre2Regex(){free();}
    void parseOpts(const std::string& mod);
    void compile(void){compile(pat_str,modifier);}
    void compile(const std::string& re,const std::string& mod="");
    std::string replace(const std::string& mains,const std::string& repl,\
    const std::string& mod="",PCRE2_SIZE out_size=std::numeric_limits<int>::max());
    std::string replace(const std::string& mains,const std::string& repl,\
    size_t out_size){return replace(mains,repl,"",out_size);}
    pcre2_code* getPcreCode(){return code;}
    void free(void){pcre2_code_free(code);print "freed";}
    bool match(const std::string& subject, MatchVector& vec,bool find_all=false);
    
    
};

void Pcre2Regex::parseOpts(const std::string& mod){
    compile_opts=0;
    action_opts=0;
    opt_jit_compile=false;
    jit_opts=PCRE2_JIT_COMPLETE;
    
    ///default options
    compile_opts |= PCRE2_MATCH_UNSET_BACKREF    //unset back-ref will match to empty string 
                 |  PCRE2_ALT_BSUX;              //\u \U \x will act as javascript standard
    
    ///parse pcre options
    for(int i=0;i<(int)mod.length();i++){
        switch (mod[i]){
            case 'g': action_opts  |= PCRE2_SUBSTITUTE_GLOBAL;break;
            case 'i': compile_opts |= PCRE2_CASELESS;break;
            case 'm': compile_opts |= PCRE2_MULTILINE;break;
            case 's': compile_opts |= PCRE2_DOTALL;break;
            case 'u': compile_opts |= PCRE2_UTF;compile_opts |= PCRE2_UCP;break;
            case 'x': compile_opts |= PCRE2_EXTENDED;break;
            case 'A': compile_opts |= PCRE2_ANCHORED;action_opts |= PCRE2_ANCHORED;break;
            case 'D': compile_opts |= PCRE2_DOLLAR_ENDONLY;break;
            case 'J': compile_opts |= PCRE2_DUPNAMES;break;
            case 'S': opt_jit_compile=true;jit_opts |= PCRE2_JIT_COMPLETE;break;//optimization opt this should be default
            case 'U': compile_opts |= PCRE2_UNGREEDY;break;
            default : break;
        }
    }
}


void Pcre2Regex :: compile(const std::string& re,const std::string& mod){
    pattern=(PCRE2_SPTR)re.c_str();
    
    ///populate class compile_opts actions opts etc...
    parseOpts(mod);
    
    ///populate some class vars
    pat_str=re;
    modifier=mod;

/*************************************************************************
* Now we are going to compile the regular expression pattern, and handle *
* any errors that are detected.                                          *
*************************************************************************/

    code = pcre2_compile(
        pattern,                    /* the pattern */
        PCRE2_ZERO_TERMINATED,      /* indicates pattern is zero-terminated */
        compile_opts,               /* default options */
        &error_number,              /* for error number */
        &error_offset,              /* for error offset */
        NULL);                      /* use default compile context */

    /* Compilation failed: print the error message and exit. */

    if (code == NULL){
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(error_number, buffer, sizeof(buffer));
        //~ printf("PCRE2 compilation failed at offset %d: %s\n", (int)error_offset,
          //~ buffer);
        free();
        std::cout<<"\n"<<returnRegexerror(REGEX_COMPILE_ERROR,(int)error_offset,buffer)<<NEW_LINE;
        throw(returnRegexerror(REGEX_COMPILE_ERROR,(int)error_offset,buffer));
    }
    else if(opt_jit_compile){
        ///perform jit compilation:
        if(pcre2_jit_compile(code, jit_opts)!=0);//{throw(JIT_COMPILE_ERROR);};
            
            
    }
    
    
}


std::string Pcre2Regex :: replace(const std::string& mains,const std::string& repl,const std::string& mod,PCRE2_SIZE out_size){
    parseOpts(mod);
    PCRE2_SPTR subject = (PCRE2_SPTR)mains.c_str();
    PCRE2_SIZE subject_length = strlen((char *)subject);
    PCRE2_SPTR replace = (PCRE2_SPTR)repl.c_str();
    PCRE2_SIZE replace_length = strlen((char *)replace);
    PCRE2_SIZE outlengthptr=(PCRE2_SIZE)out_size;
    
    PCRE2_UCHAR* output_buffer;
    output_buffer=(PCRE2_UCHAR*)malloc(outlengthptr);
    int ret=pcre2_substitute(
        code,                              /*Points to the compiled pattern*/
        subject,                           /*Points to the subject string*/
        subject_length,                    /*Length of the subject string*/
        0,                                 /*Offset in the subject at which to start matching*/
        action_opts,                       /*Option bits*/
        0,                                 /*Points to a match data block, or is NULL*/
        0,                                 /*Points to a match context, or is NULL*/
        replace,                           /*Points to the replacement string*/
        replace_length,                    /*Length of the replacement string*/
        output_buffer,                     /*Points to the output buffer*/
        &outlengthptr                      /*Points to the length of the output buffer*/
    );
    
    if (ret < 0){
        PCRE2_UCHAR buffer2[256];
        pcre2_get_error_message(ret, buffer2, sizeof(buffer2));
        free();
        print "\n"<<returnRegexerror(REGEX_REPLACE_ERROR,ret,buffer2)<<NEW_LINE;
        throw(returnRegexerror(REGEX_REPLACE_ERROR,ret,buffer2));
    }

    return toString((char*)output_buffer);
}


bool Pcre2Regex::match(const std::string& s,MatchVector& vec,bool find_all){
    PCRE2_SPTR subject=(PCRE2_SPTR)s.c_str();
    std::map<std::string,std::string> match_map;
    
    uint32_t option_bits;
    uint32_t newline;
    int namecount;
    int name_entry_size;
    PCRE2_SPTR name_table;
    int crlf_is_newline;
    int utf8;
    

    
    
    pcre2_match_data * match_data = pcre2_match_data_create_from_pattern(code, NULL);
    int rc = pcre2_match(
        code,                   /* the compiled pattern */
        subject,              /* the subject string */
        strlen((char *)subject),       /* the length of the subject */
        0,                    /* start at offset 0 in the subject */
        0,                    /* default options */
        match_data,           /* block for storing the result */
        NULL);                /* use default match context */

        /* Matching failed: handle error cases */

    if (rc < 0){
        switch(rc){
            case PCRE2_ERROR_NOMATCH: printf("No match\n"); break;
            /*
            Handle other special cases if you like
            */
            default: printf("Matching error %d\n", rc); break;
        }
    pcre2_match_data_free(match_data);   /* Release memory used for the match */
    //pcre2_code_free(code);                 /* data and the compiled pattern. */
    //~ return 1;
    return false;
    }
    
    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
    printf("\nMatch succeeded at offset %d\n", (int)ovector[0]);
    
    /* The output vector wasn't big enough. This should not happen, because we used
    pcre2_match_data_create_from_pattern() above. */

    if (rc == 0){
        printf("ovector was not big enough for all the captured substrings\n");
        return false;
        }
    
    
    /* Show substrings stored in the output vector by number. Obviously, in a real
    application you might want to do things other than print them. */

    for (int i = 0; i < rc; i++){
        PCRE2_SPTR substring_start = subject + ovector[2*i];
        size_t substring_length = ovector[2*i+1] - ovector[2*i];
        print "\n/"<<(char *)(substring_start)<<"/\n";
        std::string tmps=toString((char *)(substring_start));
        print tmps.substr(0,substring_length)<<NEW_LINE;
        //~ printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);
        match_map[std::to_string(i)]=tmps.substr(0,substring_length);
    }


/**************************************************************************
* That concludes the basic part of this demonstration program. We have    *
* compiled a pattern, and performed a single match. The code that follows *
* shows first how to access named substrings, and then how to code for    *
* repeated matches on the same subject.                                   *
**************************************************************************/

    /* See if there are any named substrings, and if so, show them by name. First
    we have to extract the count of named parentheses from the pattern. */

    (void)pcre2_pattern_info(
    code,                   /* the compiled pattern */
    PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
    &namecount);          /* where to put the answer */

    if (namecount <= 0) printf("No named substrings\n"); else{
        PCRE2_SPTR tabptr;
        printf("Named substrings\n");

        /* Before we can access the substrings, we must extract the table for
        translating names to numbers, and the size of each entry in the table. */

        (void)pcre2_pattern_info(
        code,                       /* the compiled pattern */
        PCRE2_INFO_NAMETABLE,     /* address of the table */
        &name_table);             /* where to put the answer */

        (void)pcre2_pattern_info(
        code,                       /* the compiled pattern */
        PCRE2_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
        &name_entry_size);        /* where to put the answer */

        /* Now we can scan the table and, for each entry, print the number, the name,
        and the substring itself. In the 8-bit library the number is held in two
        bytes, most significant first. */

        tabptr = name_table;
        print name_table;
        for (int i = 0; i < namecount; i++){
            int n = (tabptr[0] << 8) | tabptr[1];
            std::string tmps=toString((char*)(tabptr+2));
            std::string tmps2=toString((char*)(subject + ovector[2*n]));
            print NEW_LINE<<tmps.substr(0,name_entry_size - 3)<<": "<<tmps2.substr(0,(ovector[2*n+1] - ovector[2*n]))<<NEW_LINE;
            match_map[tmps.substr(0,name_entry_size - 3)]=tmps2.substr(0,(ovector[2*n+1] - ovector[2*n]));
            printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
            (int)(ovector[2*n+1] - ovector[2*n]), subject + ovector[2*n]);
            tabptr += name_entry_size;
        }
    }
    
    vec.push_back(match_map);

    ///

    if (!find_all){
        pcre2_match_data_free(match_data);  /* Release the memory that was used */
        //pcre2_code_free(re);                /* for the match data and the pattern. */
        return true;                           /* Exit the program. */
    }

    (void)pcre2_pattern_info(code, PCRE2_INFO_ALLOPTIONS, &option_bits);
    utf8 = (option_bits & PCRE2_UTF) != 0;

    /* Now find the newline convention and see whether CRLF is a valid newline
    sequence. */

    (void)pcre2_pattern_info(code, PCRE2_INFO_NEWLINE, &newline);
    crlf_is_newline = newline == PCRE2_NEWLINE_ANY ||
                  newline == PCRE2_NEWLINE_CRLF ||
                  newline == PCRE2_NEWLINE_ANYCRLF;

/* Loop for second and subsequent matches */

for (;;)
  {
  uint32_t options = 0;                    /* Normally no options */
  PCRE2_SIZE start_offset = ovector[1];  /* Start at end of previous match */

  /* If the previous match was for an empty string, we are finished if we are
  at the end of the subject. Otherwise, arrange to run another match at the
  same point to see if a non-empty match can be found. */

  if (ovector[0] == ovector[1])
    {
    if (ovector[0] == strlen((char*)subject)) break;
    options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
    }

  /* Run the next matching operation */

  rc = pcre2_match(
    code,                   /* the compiled pattern */
    subject,              /* the subject string */
    strlen((char*)subject),       /* the length of the subject */
    start_offset,         /* starting offset in the subject */
    options,              /* options */
    match_data,           /* block for storing the result */
    NULL);                /* use default match context */

  /* This time, a result of NOMATCH isn't an error. If the value in "options"
  is zero, it just means we have found all possible matches, so the loop ends.
  Otherwise, it means we have failed to find a non-empty-string match at a
  point where there was a previous empty-string match. In this case, we do what
  Perl does: advance the matching position by one character, and continue. We
  do this by setting the "end of previous match" offset, because that is picked
  up at the top of the loop as the point at which to start again.

  There are two complications: (a) When CRLF is a valid newline sequence, and
  the current position is just before it, advance by an extra byte. (b)
  Otherwise we must ensure that we skip an entire UTF character if we are in
  UTF mode. */

  if (rc == PCRE2_ERROR_NOMATCH)
    {
    if (options == 0) break;                    /* All matches found */
    ovector[1] = start_offset + 1;              /* Advance one code unit */
    if (crlf_is_newline &&                      /* If CRLF is newline & */
        start_offset < strlen((char*)subject) - 1 &&    /* we are at CRLF, */
        subject[start_offset] == '\r' &&
        subject[start_offset + 1] == '\n')
      ovector[1] += 1;                          /* Advance by one more. */
    else if (utf8)                              /* Otherwise, ensure we */
      {                                         /* advance a whole UTF-8 */
      while (ovector[1] < strlen((char*)subject))       /* character. */
        {
        if ((subject[ovector[1]] & 0xc0) != 0x80) break;
        ovector[1] += 1;
        }
      }
    continue;    /* Go round the loop again */
    }

  /* Other matching errors are not recoverable. */

  if (rc < 0)
    {
    printf("Matching error %d\n", rc);
    pcre2_match_data_free(match_data);
    pcre2_code_free(code);
    return 1;
    }

  /* Match succeded */

  printf("\nMatch succeeded again at offset %d\n", (int)ovector[0]);

  /* The match succeeded, but the output vector wasn't big enough. This
  should not happen. */

  if (rc == 0)
    printf("ovector was not big enough for all the captured substrings\n");

  /* As before, show substrings stored in the output vector by number, and then
  also any named substrings. */

  for (int i = 0; i < rc; i++)
    {
    PCRE2_SPTR substring_start = subject + ovector[2*i];
    size_t substring_length = ovector[2*i+1] - ovector[2*i];
    printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);
    }

  if (namecount <= 0) printf("No named substrings\n"); else
    {
    PCRE2_SPTR tabptr = name_table;
    printf("Named substrings\n");
    for (int i = 0; i < namecount; i++)
      {
      int n = (tabptr[0] << 8) | tabptr[1];
      printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
        (int)(ovector[2*n+1] - ovector[2*n]), subject + ovector[2*n]);
      tabptr += name_entry_size;
      }
    }
  }      /* End of loop to find second and subsequent matches */

printf("\n");
pcre2_match_data_free(match_data);
pcre2_code_free(code);

    
    
    
    return true;
}




int main(){
    {
    MatchVector vec;
    MatchMap map1;
    Pcre2Regex re=Pcre2Regex();
    re.compile("(?P<name>\\d+d)\\s*(\\d+d)");
    std::cout<<"\nrepl: "<<re.replace("THis digits4324 423423needs to be replaced:1 87678768","h\\p","g");
    re.match("I am a digit 67d 89d",vec,true);
    
    for(int i=0;i<(int)vec.size();i++){
        for(auto const& ent : vec[i]){
            print NEW_LINE+ent.first+": "+ent.second+NEW_LINE;
        
        }
        
    }
    
}






	return 0;
}
