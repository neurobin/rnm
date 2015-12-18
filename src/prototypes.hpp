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


#ifndef __PROTOTYPES_HPP
#define __PROTOTYPES_HPP

#include "classes.hpp"

/// Function prototypes
template<typename T>
String toString(T);
template<typename T>
T stringTo(const String& s);
String toLower(String);
String prepareLogDir();
String appendToFile(const String&,const String& str);
String printOutLog(const String& str);
String printErrorLog(String str);
String printWarningLog(String str);
void printOpts(void);
void startTask(StringArray& files);
String replaceStringAll(String, const String& , const String& );
String replaceStringWithDelims(String s,const String& rep, const String& delim1,const String& delim2);
String replaceStringWithDelims(String s,const String& rep, const String& delim);
String replaceStringAllWithDelims(String s,const String& rep, const String& delim1, const String& delim2);
String replaceStringAllWithDelims(String s,const String& rep, const String& delim);
String parseNameString(const String& ns,const String& file,DirectoryIndex &);
String regexReplace(const String& s,const String& search,const String& replace,const String& modifier,int user=0);
void parseReplaceString(StringArray& rs ,const String& file,DirectoryIndex &);
String copyFile(const String& src,const String& dst);
String copyFile2(const String& src,const String& dst);
void finalizeRFL();
bool undoRename();
bool Rename(const String& oldn,const String& newn,DirectoryIndex &);
bool isPathValid(const String& file);
String appendToRFLTMP(const String& str1,const String& str2);
void showResult(void);
void parseSearchString(String ss,Int index);
String basename(const String& file);
String dirname(const String& file);
String fileNameWithoutExtension(const String& filename);
String fileExtension(const String& filename);
void checkArgAvailability(const StringArray& sa,int i);
bool stringContains(String main,const String& sub);
bool isInt(const String& x, int base=10);
bool isPositiveInt(const String& x, int base=10);
void mustBeANumber(const String& name,const String& x);
void mustBeAPositiveNumber(const String& name,const String& extra,const String& x);
void mustBeAnInteger(const String& name,const String& x);
void mustBeAPositiveInteger(const String& name,const String& x);
bool isSingleCharacter(const String& x);
void mustBeAValidSingleCharacter(const String& name,const String& x);
bool isFile(const String& file);
NameList getNameListFromFile(const String& filename,int si,int ei,int mod);
Regex createRegex(const String& s,bool case_sensitive);
void incrementReservedIndexes(DirectoryIndex &di);
bool isComplyingToRegex(String &s,Regex &re);
bool Rename2(const String& oldn,const String& newn);
String reverseString(const String& s);
String trim(String s,const String& delim=" \n\r\t");
String ltrim(String s,const String& delim=" \n\r\t");
String rtrim(String s,const String& delim=" \n\r\t");
String trimFloatingZero(const String& s);
void parseIndexFlags(const String& s);
void printIndexFlags(void);
StringArray split(const String &text, char sep);
bool setIndexFlagInd(String s);
bool setIndexFlagAdjust(String s);
String toStringAccordingToIFL(Double index,int ifl,int number_base=NUM_BASE,IOFormatFlag index_flag_float=INDEX_FLAG_FLOAT,bool latin=false);
String toStringAccordingToMyConvention(int val);
String toStringAccordingToMyConvention(double val);
String processExtendedNameString_d(String ns,std::map<String,Double>& ns_rules,int ifl);
String processExtendedPdNameStringRule(String ns, const String& file);
template<typename T1, typename T2>
bool existsInMap(std::map<T1,T2> mymap, T1 key);
String removeInvalidNameStringRules(const String& ns);
void updateIndexFlagsFromIntFlagMap(void);
bool parseTwoStepIndexFlag(const String& s);
String convertToLatinNumber(Int num);




#endif
