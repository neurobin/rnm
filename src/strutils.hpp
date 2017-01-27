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
 

#ifndef __STRUTILS_HPP
#define __STRUTILS_HPP


#include "global.hpp"
//~ #include "class.hpp"



String toString(Char a){
    if (a) return String(1, a);
    else return String();
}

#ifdef RNM_ENABLE_ICU
String toUpper(const String& s, const char* loc_s = 0){
    icu::Locale loc(loc_s);
    icu::UnicodeString us(s.c_str());
    us.toUpper(loc);
    String res;
    return us.toUTF8String(res);
}

String toLower(const String& s, const char* loc_s = 0){
    icu::Locale loc(loc_s);
    icu::UnicodeString us(s.c_str());
    us.toLower(loc);
    String res;
    return us.toUTF8String(res);
}
#endif

String toLower0(String s){
    for(size_t i=0;i<s.length();i++){s[i]=::tolower(s[i]);}
    return s;
}

String toUpper0(String s){
    for(size_t i=0;i<s.length();i++){s[i]=::toupper(s[i]);}
    return s;
}


String toStringAccordingToMyConvention(int val){
    if(val==-1){return "unset";}
    else return std::to_string(val);
}


String toStringAccordingToMyConvention(double val){
    if(val==-1.0){return "unset";}
    else return std::to_string(val);
}


bool stringContains(String subject,const String& search){
    if (subject.find(search) != String::npos) {return true;}
    else return false;
}

bool stringContains(String subject,const char search){
    if (subject.find(search) != String::npos) {return true;}
    else return false;
}

String ltrim(String s,const String& delim=" \n\r\t"){return s.erase(0,s.find_first_not_of(delim));}
String rtrim(String s,const String& delim=" \n\r\t"){return s.erase(s.find_last_not_of(delim)+1);}
String trim(String s,const String& delim=" \n\r\t"){return rtrim(ltrim(s,delim),delim);}


//~ String trimZeroFromScientificNumber(const String& s){
    //~ if(!stringContains(s,"."))return s;
    //~ int pos = s.find_first_of("eE");
    //~ if(pos!=(int)String::npos){
        //~ return rtrim(rtrim(s.substr(0,pos),"0"),".")+s.substr(pos,1)+ltrim(s.substr(pos+1,String::npos),"0");
    //~ }
    //~ else return s;
//~ }


//~ String trimFloatingZero(const String& s){
    //~ if(!stringContains(s,"."))return s;
    //~ int pos = s.find_first_of("eE");
    //~ if(pos!=(int)String::npos){
        //~ return rtrim(rtrim(s.substr(0,pos),"0"),".")+s.substr(pos,1)+ltrim(s.substr(pos+1,String::npos),"0");
    //~ }
    //~ else return rtrim(rtrim(s,"0"),".");
//~ }

String replaceString(String str,const String& replace,const String& with){
    std::size_t pos = str.find(replace);
    if (pos!=String::npos){
    return str.replace (pos,replace.length(),with);}
    else return str;
    
}


String replaceStringAll(String str, const String& old, const String& new_s) {
    if(!old.empty()){
        size_t pos = str.find(old);
        while ((pos = str.find(old, pos)) != String::npos) {
             str=str.replace(pos, old.length(), new_s);
             pos += new_s.length();
        }
    }
    return str;
}

String encodeWithDelim(const String& s, const String& delim){
    return replaceStringAll(s,delim,"["+delim+"]");
}

String decodeWithDelim(const String& s, const String& delim){
    return replaceStringAll(s,"["+delim+"]",delim);
}

String replaceStringWithDelims(String s,const String& rep, const String& delim1,const String& delim2){
    String::size_type pos1=s.find_first_of(delim1);
    String::size_type pos2=s.find_first_of(delim2,pos1+1);
    if(pos1!=String::npos&&pos2!=String::npos)
    return s.replace(pos1,pos2-pos1+1,rep);
    else return s;
}
///Make an overload of the above function to set default parameter
String replaceStringWithDelims(String s,const String& rep, const String& delim){
    return replaceStringWithDelims(s,rep,delim,delim);
}


String replaceStringAllWithDelims(String s,const String& rep, const String& delim1, const String& delim2) {
    String::size_type pos1=s.find_first_of(delim1);
    String::size_type pos2=s.find_first_of(delim2,pos1+1);
    while ((pos1 = s.find_first_of(delim1,pos1)) != String::npos && (pos2 = s.find_first_of(delim2,pos1+1)) != String::npos) {
         s=s.replace(pos1,pos2-pos1+1,rep);
         pos1 += rep.length();
    }
return s;
}
///Make an overload of the above function to set default parameter
String replaceStringAllWithDelims(String s,const String& rep, const String& delim){
    return replaceStringAllWithDelims(s,rep,delim,delim);
}


    
String stripPathDelimiter(const String& s){
    return replaceStringAll(s,path_delim,"");
}


String removeInvalidNameStringRules(const String& ns){
    return replaceStringAllWithDelims(ns,"",path_delim);
}


size_t countCharInString(String s, char delim){
    return std::count(s.begin(),s.end(),delim);
}


size_t countMatchInString(String s, String match){
    size_t count=0;
    String::size_type pos = s.find(match);
    while ((pos = s.find(match, pos)) != String::npos){
        count++;pos+=match.length();
    }
    return count;
}


StringArray split(const String &text, char sep) {
    StringArray tokens;
    size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != String::npos) {
        String temp=text.substr(start, end - start);
        if(temp!="")tokens.push_back(temp);
        start = end + 1;
    }
    String temp=text.substr(start);
    if(temp!="")tokens.push_back(temp);
    return tokens;
}


String formatTime(time_t t,const String& form, char type = 0){
    struct tm tb;
    if(type == 'g') { gmtime_r(&t, &tb); }
    else { localtime_r(&t, &tb); } //default is local time
    char timbuf[100];
    strftime(timbuf, sizeof(timbuf), form.c_str(), &tb);
    return String(timbuf);
}


String fixPrec(const Double& x, String s, const Int& prec, size_t ind){
    if(prec >= ind && prec > 0 ){
        if(x>0){
            if(prec < s.length() - 1) s = s.substr(0, prec.get_ui()+1);
        } else {
            if(prec < s.length() - 2) s = s.substr(0, prec.get_ui()+2);
        }
    }
    //insert extra 0s after radix point
    if(x>=0) while(prec > s.length()-1) s+="0";
    else while(prec > s.length()-2) s+="0";
    return s;
}

String fixIFL(const String& s, const Int& ifl, const String& iff, IOFormatFlag flags){
    if(ifl <= s.length()) return s;
    oss_buffer.str("");
    oss_buffer.clear();
    oss_buffer.width(ifl.get_ui());
    oss_buffer.setf(flags);
    oss_buffer.fill(iff[0]);
    if(s[0] == '-') {
        oss_buffer<<s.substr(1,String::npos);
        return "-"+oss_buffer.str(); 
    } else {
        oss_buffer<<s;
        return oss_buffer.str();
    }
}

String convertBase( const Double& x, Ush base, const Int& ifl,
                    Int prec, const String& iff,
                    IOFormatFlag flags = (IOFormatFlag)0){
    if(base < 2 || base > 36) base = 10;
    String s;
    mp_exp_t exp;
    s = x.get_str(exp, (int)base, 0);
    //if prec is <=0, estimate a suitable prec
    if(prec <= 0 && ceil(x) != x) prec = abs(2*exp);
    if(s.empty()) s = "0";
    while(exp > Int(x>=0?s.length():s.length()-1)) s+="0"; //Add missing zeros at right
    //insert radix point
    size_t ind = 0;
    if(x<0) ind++; //for negative sign
    if(exp > 0) {
        ind += exp;
        s.insert(ind,".");
    } else if(exp < 0) {
        //for negative insert 0's along with radix point
        while(exp++!=0) s.insert(ind,"0");
        s.insert(ind,"0.");
    } else {
        if(x!=0 || (flags & std::ios::showpoint)!=0 || prec >= 2)
            s.insert(ind,"0.");
    }
    if((flags & std::ios::showpoint)!=0 && s[s.length()-1] == '.' &&
                (prec > (x>0?s.length()-1:s.length()-2) || prec == 0)
                ) s+="0";
    if(stringContains(s,".")) s = fixPrec(x,s,prec,ind);
    s = fixIFL(rtrim(s,"."), ifl, iff, flags);
    if((flags & std::ios::showbase)!=0){
        switch(base){
            case 8: if(x>=0) s = "0" + s; else s.insert(1,"0");break;
            case 16: if(x>=0) s = "0x" + s; else s.insert(1, "0x");break;
            default:break;
        }
    }
    if(x >= 0 &&(flags & std::ios::showpos)!=0) s="+"+s;
    return s;
}

String convertToScientific( const Double& x, const Int& ifl,
                            Int prec, const String& iff,
                            IOFormatFlag flags){
    String s, exp_s, exp_sgn;
    exp_s = "e";
    mp_exp_t exp;
    s = x.get_str(exp, 10, 0); //only 10 base is allowed
    //if prec is <=0, estimate a suitable prec
    if(prec <= 0 && ceil(x) != x) prec = abs(2*exp);
    if(s.empty()) s = "0";
    while(exp > Int(x>=0?s.length():s.length()-1)) s+="0"; //Add missing zeros at right
    if(x!=0) exp--; //we will move the radix point once towards right i.e 0.32 will be 3.2
    if(exp>=0) exp_sgn = "+";
    //handle precision
    if(x!=0) s = x>0?s.insert(1,"."):s.insert(2,".");
    else if((flags & std::ios::showpoint)!=0 || prec >= 2) s = s.insert(0,"0.");
    if((flags & std::ios::showpoint)!=0 && s[s.length()-1] == '.') s+="0";
    if(stringContains(s,".")) s = fixPrec(x,s,prec,1);
    s = fixIFL(s, ifl, iff, flags);
    s = rtrim(s,".") + exp_s + exp_sgn + std::to_string(exp);
    if(x >= 0 &&(flags & std::ios::showpos)!=0) s="+"+s;
    return s;
}

String convertToLatin(Int num, IOFormatFlag flags = (IOFormatFlag)0){
    //num must be positive
    bool neg = false;
    if(num<0){
        neg = true;
        num = abs(num);
    }
    struct latindata_t { Int num; char const* c; };
    const struct latindata_t latindata[] =
    {
        {1000, "m"}, {900, "cm"},
        {500, "d"}, {400, "cd"},
        {100, "c"}, { 90, "xc"},
        { 50, "l"}, { 40, "xl"},
        { 10, "x"}, { 9, "ix"},
        { 5, "v"}, { 4, "iv"},
        { 1, "i"},
        { 0, 0} // end marker
    };
    String res;
    for (const latindata_t* current = latindata; current->num > 0; ++current){
        while (num >= current->num){
            res += current->c;
            num -= current->num;
        }
    }
    if(neg) res = "-" + res;
    else if((flags & std::ios::showpos)!=0) res = "+" + res;
    return res;
}

Double round(const Double& x){
    if((ceil(x)-x)<=0.5) return ceil(x);
    else return floor(x);
}


String toString(const Double& x, Ush base, Int ifl, const Int& prec, const String& iff, IOFormatFlag flags, bool latin = false){
    String s;
    if(latin){
        if(x<=latin_fall_back_threshold){
            s = convertToLatin(Int(convertBase(round(x),10,0,0,"")), flags);
        } else{
            printWarningLog("Latin format is too large, falling back to decimal.");
            s = convertBase(x,10,ifl,prec,iff,flags);
        }
    } else if((flags & std::ios::scientific)!=0){
        s = convertToScientific(x,ifl,prec,iff,flags); 
    } else{
        s = convertBase(x,base,ifl,prec,iff,flags);
    }
    if((flags & std::ios::uppercase)!=0) s = toUpper0(s);
    return s;
}


String getSizeByUnit(   const Double& x, 
                        Double unit, //if unit is 0, we will find a suitable unit
                        bool add_unit_s = true,
                        Ush base = NUM_BASE_DEFAULT,
                        const Int& ifl = 0,
                        const Int& prec = 0,
                        const String& iff = "",
                        const String& delim = "",
                        IOFormatFlag flags = (IOFormatFlag)0,
                        bool latin = 0){//FLAG_FLOAT as oflags will have no effect
    String unit_s;
    if(unit <= 0){
        //choose a suitable unit
        Double ax = abs(x);
        if(ax >= TB)       unit = TB;
        else if(ax >= GB)  unit = GB;
        else if(ax >= MB)  unit = MB;
        else if(ax >= KB)  unit = KB;
        else               unit = 1;
    }
    if(add_unit_s){
        if(unit == TB)      unit_s = "TB";
        else if(unit == GB) unit_s = "GB";
        else if(unit == MB) unit_s = "MB";
        else if(unit == KB) unit_s = "KB";
        else if(unit == 1)  unit_s = "B";
    }
    return toString(x/unit,base,ifl, prec, iff, flags, latin) +delim+ unit_s;
}


String dirname(const String& file){
    std::size_t found = file.find_last_of(path_delim);
    return file.substr(0,found);
    
}


String basename(const String& file){
    std::size_t found = file.find_last_of(path_delim);
    return file.substr(found+1);
}


String getParentDirectoryName(const String& file){
    return basename(dirname(file));
}


String fileExtension(const String& file){
    String::size_type pos=file.find_last_of('.');
    if(pos!=String::npos&&pos!=0)return file.substr(pos+1);
    else return "";
}


String fileNameWithoutExtension(const String& file){
    String::size_type pos=file.find_last_of('.');
    if(pos!=String::npos&&pos!=0)return file.substr(0,pos);
    else return file;
}

void validateModifier(const String& mod, const String& valid_mods, const String& err_m_for){
    for(size_t i=0;i<mod.length();++i)
        if(!stringContains(valid_mods, mod[i]))
            errorExit("Invalid modifier '"+toString(mod[i])+"' for "+err_m_for);
}

String getPathSignature(const String& path){
    return replaceStringAll(path, "/",".");
}



std::string toStringWithFloatingPointDigit(double x, char f){
    char c[] = {'%','.',f,'f','\0'};
    size_t length = snprintf(0, 0, c, x) + 1;
    char* s = new char[length];
    snprintf(s, length, c, x);
    std::string res = std::string(s);
    delete[] s;
    return res;
}



#endif
