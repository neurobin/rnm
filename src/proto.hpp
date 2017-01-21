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
 

#ifndef __PROTO_HPP
#define __PROTO_HPP

//forward decl
class File;

/////typedefs
typedef uint_fast64_t Uint;
typedef uint_fast8_t Ush;
typedef int_fast16_t Short;
typedef mpf_class Double;
typedef mpz_class Int;
typedef char Char;
typedef std::string String;
typedef std::ios_base::fmtflags IOFormatFlag;
typedef std::vector<IOFormatFlag> IOFormatFlagArray;
typedef std::vector<String> StringArray;
typedef std::vector<String> NameList;
typedef std::vector<Int> IntVec;
typedef std::vector<Uint> UintVec;
typedef std::fstream FileStream;
typedef jpcre2::select<char> jp;
typedef std::vector<jp::Regex> RegexArray;
typedef std::vector<bool> BoolArray;
typedef std::vector<File> FileArray;

//function prototypes
void Exit(int, bool cleanfs=true);
void unsafeExit(int, bool cleanfs=true);
void errorExit(const String& s, bool cleanfs=true);


#endif
