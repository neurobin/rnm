#include "funcs.hpp"
#include "times.hpp"



#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>


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
    
    Pcre2Regex(const std::string& re,const std::string& mod=""){pat_str=re;modifier=mod;compile();}
    ~Pcre2Regex(){free();}
    void parseOpts(const std::string& mod);
    void compile(void){compile(pat_str,modifier);}
    void compile(const std::string& re,const std::string& mod="");
    std::string replace(const std::string& mains,const std::string& repl,\
    const std::string& mod="",PCRE2_SIZE out_size=std::numeric_limits<int>::max());
    std::string replace(const std::string& mains,const std::string& repl,\
    size_t out_size){return replace(mains,repl,"",out_size);}
    pcre2_code* getPcreCode(){return code;}
    void free(void){pcre2_code_free(code);}
    
    
};

inline void Pcre2Regex::parseOpts(const std::string& mod){
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


inline void Pcre2Regex :: compile(const std::string& re,const std::string& mod){
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
        std::cout<<returnRegexerror(REGEX_COMPILE_ERROR,(int)error_offset,buffer);
        free();
        throw(returnRegexerror(REGEX_COMPILE_ERROR,(int)error_offset,buffer));
    }
    else if(opt_jit_compile){
        ///perform jit compilation:
        if(pcre2_jit_compile(code, jit_opts)!=0);//{throw(JIT_COMPILE_ERROR);};
            
            
    }
    
    std::cout<<"\ncompile success: "<<code;
    
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
    
    if (ret <= 0){
        PCRE2_UCHAR buffer2[256];
        pcre2_get_error_message(ret, buffer2, sizeof(buffer2));
        free();
        throw(returnRegexerror(REGEX_REPLACE_ERROR,ret,buffer2));
    }

    return toString((char*)output_buffer);
}




int main(){
    {
    Pcre2Regex re("\\d+ (?P<first>\\d+)","");
    
    print "\n"<<~(PCRE2_SIZE)0-1<<"\n";
    std::cout<<"\nmain: "<<re.getPcreCode();
    std::cout<<"\nrepl: "<<re.replace("THis digits4324 423423needs to be replaced:1 87678768","h${first2}&","g");
}






	return 0;
}
