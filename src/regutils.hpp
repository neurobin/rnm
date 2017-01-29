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
 

#ifndef __REGUTILS_HPP
#define __REGUTILS_HPP


#include "global.hpp"
#include "strutils.hpp"

jp::Regex multi_sre;
jp::Regex sanity_regex;
jp::Regex repl_sanit_re;
jp::Regex repl_sanit_dol_re;
jp::Regex repl_sanit_and_re;
jp::Regex multi_rre ;

//the following must use the path_delim or second_delim var
jp::Regex multi_re_info1;
jp::Regex multi_re_info2;
jp::Regex multi_nre1;
jp::Regex multi_nre2;
jp::Regex multi_pdre1;
jp::Regex multi_pdre2;

void initRegVars(){
    multi_sre.compile("\\s*/([^/]*?)/\\s*([^/;]*)(\\s*;\\s*|$)",0,jpcre2::JIT_COMPILE);
    sanity_regex.compile("[^\\\\[\\]]",0,jpcre2::JIT_COMPILE);
    repl_sanit_re.compile(R"(\\(\d+|\{[^}]+\}))",0,jpcre2::JIT_COMPILE);
    repl_sanit_dol_re.compile(R"(\\\$|\$(?!\d+|\{[^}]+\}))", 0, jpcre2::JIT_COMPILE);
    repl_sanit_and_re.compile(R"((?<!\\)&)", 0, jpcre2::JIT_COMPILE);
    multi_rre.compile(R"(\s*/([^/]*?)/([^/]*?)/\s*([^/;]*)\s*(;\s*|$))",0,jpcre2::JIT_COMPILE);

    //the following must use the path_delim or second_delim var
    multi_re_info1.compile(path_delim+"info-([^"+path_delim+"-]+)-?([^"+path_delim+"]*)"+path_delim,0, jpcre2::JIT_COMPILE);
    multi_re_info2.compile(second_delim+"info-([^"+second_delim+"-]+)-?([^"+second_delim+"]*)"+second_delim,0, jpcre2::JIT_COMPILE);
    multi_nre1.compile("(?<total>"+path_delim+"(?<rule>-?\\w+)(-?|-(?<exn>[bsl])-?(?<exv>\\d+)?)?"+path_delim+")",0,jpcre2::JIT_COMPILE);
    multi_nre2.compile("(?<total>"+second_delim+"(?<rule>-?\\w+)(-?|-(?<exn>[bsl])-?(?<exv>\\d+)?)?"+second_delim+")",0,jpcre2::JIT_COMPILE);
    multi_pdre1.compile("(?<total>"+path_delim+"(?<rule>pd)-?(?<si>\\d+|[ew])?(-(?<ei>\\d+|[ew]))?-?(?<delim>[^"+
                                                                    path_delim+"]*)?"+path_delim+")",0,jpcre2::JIT_COMPILE);
    multi_pdre2.compile("(?<total>"+second_delim+"(?<rule>pd)-?(?<si>\\d+|[ew])?(-(?<ei>\\d+|[ew]))?-?(?<delim>[^"+
                                                                    second_delim+"]*)?"+second_delim+")",0,jpcre2::JIT_COMPILE);
}

size_t countMatchInRegex(const String& s,const String& re){
    jp::Regex re0(re,0,jpcre2::JIT_COMPILE);
    jp::RegexMatch m(&re0);
    return m.setFindAll()
            .setSubject(s)
            .match();
}


bool isComplyingToRegex(const String& s, jp::Regex &re){
    if(s.empty())return false;
    String total="";
    jp::RegexMatch m(&re);
    jp::VecNum vn;
    m.setSubject(s)
     .addJpcre2Option(jpcre2::FIND_ALL)
     .setNumberedSubstringVector(&vn)
     .match();
    for(size_t i=0;i<vn.size();++i){
        total += vn[i][0];
    }
    if(s!=total){return false;}
    else {return true;}
}


String sanitizeStringForRegex(const String& s, char mode){
    String s1 = s;
    if(mode == 's'){
        s1 = replaceStringAll(s1, "\\","\\\\");
        s1 = replaceStringAll(s1, "[","\\[");
        s1 = replaceStringAll(s1, "]","\\]");
        s1 = sanity_regex.replace(s1,"[$0]","g");
        return s1;
    } else if( mode == 'r') {
        s1 = replaceStringAll(s1, "\\$","\\\\$");
        return replaceStringAll(s1, "&","\\&");
    }
    return s1;
}


String processReplacementString(String replace){
    /// \1 \2 etc will be converted to PCRE2 backreference $1 $2
    /// $ (not capture group) will be converted to $$
    /// \$ will be converted to $$
    /// & will be converted to $0 (the entire match)
    /// \& will be &
    jp::RegexReplace rr(&repl_sanit_re);
    ///capture groups \1, \{22}, \{name}
    rr.setSubject(&replace).setPcre2Option(PCRE2_SUBSTITUTE_GLOBAL);
    replace = rr.setReplaceWith("$$$1").replace();
    ///dangling $
    rr.setRegexObject(&repl_sanit_dol_re);
    replace = rr.setReplaceWith("$$$$").replace();
    ///&
    rr.setRegexObject(&repl_sanit_and_re);
    replace = rr.setReplaceWith("$$0").replace();
    //std::cout<<rr.getErrorMessage();
    
    ///Finally strip invalid namestring rules and slashes
    replace=removeInvalidNameStringRules(replace);
    replace=stripPathDelimiter(replace);
    replace = replaceStringAll(replace, "\\&", "&");
    
    return replace;
}

#endif
