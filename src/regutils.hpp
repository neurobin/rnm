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


size_t countMatchInRegex(const String& s,const String& re){
    return jp::Regex(re)
            .initMatch()
            .setJpcre2Option(jpcre2::FIND_ALL)
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


String sanitizeStringForRegex(const String& s){
    String s1 = s;
    s1 = sanity_regex.replace(s1,"[$0]","g");
    return replaceStringAll(s1, "\\","\\\\");
}


#endif
