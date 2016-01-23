#include <iostream>
#include <regex>
#include <string>
#include <jpcre2.h>

namespace jregex{
    
    #define REGEX_ECMASCRIPT std::regex::ECMAScript
    #define ICASE std::regex::icase
    #define REGEX_BASIC std::regex::basic
    #define REGEX_EXTENDED std::regex::extended
    #define REGEX_AWK std::regex::awk
    #define REGEX_GREP std::regex::grep
    #define REGEX_EGREP std::regex::egrep
    #define REGEX_LOCALE std::regex::collate
    
    ///Default regex should be predefined in compile time
    #ifndef REGEX_DEFAULT
    #define REGEX_DEFAULT REGEX_ECMASCRIPT
    #endif
    
    namespace constants{
        enum regex_type {Default='0',pcre,ECMAScript,awk,egrep,grep,extended,basic};
    }


    class Regex{
        
        private:
        
        std::string pat_str,modifier,mylocale;
        int error_number,error_code;
        std::regex::flag_type compile_opts;                ///icase,sed,awk,grep,egrep,ECMAScript,basic,extended,collate
        std::regex_constants::match_flag_type action_opts; ///format default and and format first only
        std::regex re;
        bool pcre;
        constants::regex_type rtype;
        
        
        void parseCompileOpts(const std::string& mod);
        
        public:
        
        Regex(){}
        Regex(const std::string& pat){compile(pat,"");}
        Regex(const std::string& pat, const std::string& mod){compile(pat,mod);}
        
        std::string getPattern(){return pat_str;}
        std::string getModifier(){return modifier;}
        constants::regex_type getRegexType(){return rtype;}
        std::regex getRegex(){return re;}
        void compile(const std::string& pat,const std::string& mod);
        
        std::string replace( std::string mains, std::string repl,const std::string& mod="");
        
    };
    
    void Regex::parseCompileOpts(const std::string& mod){
        
        ///Initialize to default if no modifier is passed
        pcre=false;rtype=constants::Default;compile_opts=REGEX_DEFAULT;
        
        ///parse pcre options
        for(int i=0;i<(int)mod.length();i++){
            
            constants::regex_type rt = static_cast<constants::regex_type>(mod[i]);
            
            switch (mod[i]){
                
                case constants::Default    : pcre=false;rtype=rt;compile_opts|=REGEX_DEFAULT;break;
                case constants::pcre       : pcre=true; rtype=rt;break;
                case constants::ECMAScript : pcre=false;rtype=rt;compile_opts|=REGEX_ECMASCRIPT;break;
                case constants::awk        : pcre=false;rtype=rt;compile_opts|=REGEX_AWK;break;
                case constants::egrep      : pcre=false;rtype=rt;compile_opts|=REGEX_EGREP;break;
                case constants::grep       : pcre=false;rtype=rt;compile_opts|=REGEX_GREP;break;
                case constants::extended   : pcre=false;rtype=rt;compile_opts|=REGEX_EXTENDED;break;
                
                case 'e': throw(std::string("This option is only available with pcre regex"));break;
                          //compile_opts |= PCRE2_MATCH_UNSET_BACKREF;break;
                case 'i': compile_opts |= ICASE;break;
                          //compile_opts |= PCRE2_CASELESS;break;
                case 'm': throw(std::string("This option is only available with pcre regex"));break;
                          //compile_opts |= PCRE2_MULTILINE;break;
                case 's': throw(std::string("This option is only available with pcre regex"));break;
                          //compile_opts |= PCRE2_DOTALL;break;
                case 'u': throw(std::string("This option is only available with pcre regex"));break;
                          //compile_opts |= PCRE2_UTF;compile_opts |= PCRE2_UCP;break;
                case 'x': throw(std::string("This option is only available with pcre regex"));break;
                          //compile_opts |= PCRE2_EXTENDED;break;
                case 'A': throw(std::string("This option is only available with pcre regex"));break;
                          //compile_opts |= PCRE2_ANCHORED;action_opts |= PCRE2_ANCHORED;break;
                case 'D': throw(std::string("This option is only available with pcre regex"));break;
                          //compile_opts |= PCRE2_DOLLAR_ENDONLY;break;
                case 'J': throw(std::string("This option is only available with pcre regex"));break;
                          //compile_opts |= PCRE2_DUPNAMES;break;
                case 'S': throw(std::string("This option is only available with pcre regex"));break;
                          //jit_opts=true;jit_opts |= PCRE2_JIT_COMPLETE;break; ///Optimization opt
                case 'U': throw(std::string("This option is only available with pcre regex"));break;
                           //compile_opts |= PCRE2_UNGREEDY;break;
                default : std::string s="Invalid modifier: ";s[s.length()]=mod[i];throw(s);break;
            }
        }
    }
    
    
    void Regex::compile(const std::string& pat,const std::string& mod){
            parseCompileOpts(mod);
            ///Now we know what type of regex it is and it's modifiers
            
            if(pcre){///jpcre2 regex
                
            }
            else{///std::regex
                try{re=std::regex(pat,compile_opts);}
                catch(std::regex_error& e){throw(e);}
                pat_str=pat;
                modifier=mod;
            }
        }

    std::string Regex::replace( std::string mains, std::string repl,const std::string& mod){
        std::string res;
        
        return res;
    }
        
        
}///Namespace

int main(){
    jregex::Regex re("\\d\\w","6");
    jpcre2::Regex re1;
    int a,c=0,b=9;
    a=c=b;
std::cout<<a;

	return 0;
}
