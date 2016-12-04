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
 
 
#include "funcs.hpp"



void checkArgAvailability(const StringArray& sa,int i){
    if(i>=(int)sa.size()){
        printErrorLog("One or more argument/s missing");
        Exit(1);
    }
}

int main(int argc, char* argv[]) {getCurrentDir(self_dir);self_path=self_dir+String(path_delim)+executable_name;
    CWD=toString(self_dir);
    CWDN=basename(self_dir);
    prepareLogDir();
    
    

    
    //////////////////////////////// Opt parse////////////////////////////////////////////
    
    StringArray args;
    for(int i = 1; i < argc; i++){args.push_back(argv[i]);}
    
    bool skipcount=false;
    bool noopt=false;
  { //Scope for the following objects
    Options inc_obj("-inc","--increment-value");
    Options linc_obj("-linc","--line-increment-value");
    Options si_obj("-i","-si","--index","--start-index");
    Options ei_obj("-ei","--end-index");
    Options ifl_obj("-ifl","--index-field-length");
    Options ifp_obj("-ifp","--index-field-precision");
    Options iff_obj("-iff","--index-field-filler");
    Options ns_obj("-ns","--name-string");
    Options nsf_obj("-ns/f","--name-string-file");
    Options sl_obj("-l","-sl","--line","--start-line");
    Options el_obj("-el","--end-line");
    Options ss_obj("-ss","--search-string");
    Options ss_file_obj("-ss/f","--search-string-file");
    Options rs_obj("-rs","--replace-string");
    Options rs_file_obj("-rs/f","--replace-string-file");
    Options re_obj("-re","--regex");
    Options dp_obj("-dp","--depth");
    Options if_obj("-if","--index-flags");
    
    for(int i = 0; i < (int)args.size(); i++){
      if(skipcount){skipcount=false;continue;}
        
      String opt=toLower(String(args[i]));
      
     if(!noopt){
      if(opt=="-h"||opt=="--help"){
          printe help_message;
          return 0;
        }
      else if(opt=="-v"||opt=="--version"){
          printe version_info;
          return 0;
        }
        
      else if(opt=="-q"||opt=="--quiet"){
          quiet=true;
          
        }
      
      else if(opt=="-fo"||opt=="--file-only"){
          file_only=true;
          
        }
      
      else if(opt=="-do"||opt=="--directory-only"){
          directory_only=true;
        
        }
        
      else if(opt=="-ed"||opt=="--exclude-directory"){
          exclude_directory=true;
          
        }
        
      else if(opt=="-inc"||opt=="--increment-value"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveNumber("Increment Value","\nNegative increment i.e decrement will be available using name string rule:\n\
"+path_delim+"-i"+path_delim+", "+path_delim+"-ir"+path_delim+", "+path_delim+"-id"+path_delim+" etc..\n",args[i+1]);
          inc=stringTo<decltype(inc)>(args[i+1]);
          skipcount=true;
          
          inc_obj.count++;
          if(inc_obj.count>1){printWarningLog("Increment value overwritten");}
        }
        
      else if(opt=="-linc"||opt=="--line-increment-value"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Line Increment Value",args[i+1]);
          linc=stringTo<decltype(linc)>(args[i+1]);
          skipcount=true;
          
          linc_obj.count++;
          if(linc_obj.count>1){printWarningLog("Line increment value overwritten");}
        }
        
        
      else if(opt=="-i"||opt=="-si"||opt=="--index"||opt=="--start-index"){
          checkArgAvailability(args,i+1);
          mustBeANumber("Start Index",args[i+1]);
          start_index=stringTo<decltype(start_index)>(args[i+1]);
          current_index=start_index;
          current_index_rd=start_index;
          reverse_index=start_index;
          reverse_index_rd=start_index;
          DIRECTORY_INDEX=start_index;
          DIRECTORY_REVERSE_INDEX=start_index;
          skipcount=true;
          
          si_obj.count++;
          if(si_obj.count>1){printWarningLog("Start index overwritten");}
        }
        
      else if(opt=="-ei"||opt=="--end-index"){
          checkArgAvailability(args,i+1);
          mustBeANumber("End Index",args[i+1]);
          end_index=stringTo<decltype(end_index)>(args[i+1]);
          skipcount=true;
          
          ei_obj.count++;
          if(ei_obj.count>1){printWarningLog("End index overwritten");}
        }
        
        
      else if(opt=="-if"||opt=="--index-flags"){
          checkArgAvailability(args,i+1);
          parseIndexFlags(args[i+1]);
          skipcount=true;
          if_obj.count++;
          if(if_obj.count>1){printWarningLog("Override occurred in Index flags");}
        }
        
      else if(opt=="-ifl"||opt=="--index-field-length"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Index field length",args[i+1]);
          index_field_length=stringTo<decltype(index_field_length)>(args[i+1]);
          skipcount=true;
          
          ifl_obj.count++;
          if(ifl_obj.count>1){printWarningLog("Index field length overwritten");}
        }
        
      else if(opt=="-ifp"||opt=="--index-field-precision"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Index field precision",args[i+1]);
          IFP=stringTo<decltype(IFP)>(args[i+1]);
          skipcount=true;
          
          ifp_obj.count++;
          if(ifp_obj.count>1){printWarningLog("Index field precision overwritten");}
        }
        
        
      else if(opt=="-iff"||opt=="--index-field-filler"){
          checkArgAvailability(args,i+1);
          mustBeAValidSingleCharacter("Index field filler",args[i+1]);
          IFF=stringTo<decltype(IFF)>(args[i+1]);
          IFF=replaceStringAll(IFF,"\\","");
          skipcount=true;
          
          iff_obj.count++;
          if(iff_obj.count>1){printWarningLog("Index field filler overwritten");}
        }
      
      else if(opt=="-ns"||opt=="--name-string"){
          checkArgAvailability(args,i+1);
          name_string=args[i+1];
          skipcount=true;
          
          ns_obj.count++;
          if(ns_obj.count>1){printWarningLog("Name string overwritten");}
        }
        
        
      else if(opt=="-ns/f"||opt=="--name-string-file"){
          checkArgAvailability(args,i+1);
          name_string_file=args[i+1];
          skipcount=true;
          
          nsf_obj.count++;
          if(nsf_obj.count>1){printErrorLog("Only one name string file is allowed");Exit(1);}
        }
        
        
      else if(opt=="-ns/fn"||opt=="--name-string-file-null-terminated"){
          checkArgAvailability(args,i+1);
          name_string_file=args[i+1];
          nsf_n=true;
          skipcount=true;
          
          nsf_obj.count++;
          if(nsf_obj.count>1){printErrorLog("Only one name string file is allowed");Exit(1);}
        }
        
      else if(opt=="-l"||opt=="-sl"||opt=="--line"||opt=="--start-line"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Start Line",args[i+1]);
          start_line=stringTo<decltype(start_line)>(args[i+1]);
          current_line=start_line;
          skipcount=true;
          
          sl_obj.count++;
          if(sl_obj.count>1){printWarningLog("Start line overwritten");}
        }
        
      else if(opt=="-lv"||opt=="-slv"||opt=="--line-reverse"||opt=="--start-line-reverse"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("Start Line",args[i+1]);
          start_line=stringTo<decltype(start_line)>(args[i+1]);
          current_line=start_line;
          skipcount=true;
          reverse_line=true;
          
          sl_obj.count++;
          if(sl_obj.count>1){printWarningLog("Start line overwritten");}
        }
        
      else if(opt=="-el"||opt=="--end-line"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("End Line",args[i+1]);
          end_line=stringTo<decltype(end_line)>(args[i+1]);
          skipcount=true;
          
          el_obj.count++;
          if(el_obj.count>1){printWarningLog("End line overwritten");}
        }
      
      else if(opt=="-elv"||opt=="--end-line-reverse"){
          checkArgAvailability(args,i+1);
          mustBeAPositiveInteger("End Line",args[i+1]);
          end_line=stringTo<decltype(end_line)>(args[i+1]);
          skipcount=true;
          reverse_line=true;
          
          el_obj.count++;
          if(el_obj.count>1){printWarningLog("End line overwritten");}
        }
        
      else if(opt=="-ss"||opt=="--search-string"){
          checkArgAvailability(args,i+1);
          search_string.push_back(args[i+1]);
          fixed_ss.push_back(false);
          skipcount=true;
          
          ss_obj.count++;
          ///multple is allowed
        }
        
        
      else if(opt=="-ss/f"||opt=="--search-string-file"){
          checkArgAvailability(args,i+1);
          search_string_file=args[i+1];
          skipcount=true;
          
          if(isFile(search_string_file)){
            StringArray temp=getLineFromFileAndReturnVector(search_string_file);
            ///append temp to search_string
            search_string.insert(search_string.end(),temp.begin(),temp.end());
            ///Populate fixed_ss with same number of boolean value
            for(Int i=0;i<(Int)temp.size();i++){
                fixed_ss.push_back(false);
                }
          }
          else{
            printErrorLog("Search string file not found: "+search_string_file);
            Exit(1);
          }
          ss_file_obj.count++;
          ///multple is allowed
        }
        
         
      else if(opt=="-ssf"||opt=="--search-string-fixed"){
          checkArgAvailability(args,i+1);
          search_string.push_back(args[i+1]);
          fixed_ss.push_back(true);
          skipcount=true;
          
          ss_obj.count++;
          ///multple is allowed
        }
        
          
      else if(opt=="-ssf/f"||opt=="--search-string-fixed-file"){
          checkArgAvailability(args,i+1);
          search_string_file=args[i+1];
          skipcount=true;
          if(isFile(search_string_file)){
            StringArray temp=getLineFromFileAndReturnVector(search_string_file);
            ///append temp to search_string
            search_string.insert(search_string.end(),temp.begin(),temp.end());
            ///Populate fixed_ss with same number of boolean value
            for(Int i=0;i<(Int)temp.size();i++){
                fixed_ss.push_back(true);
                }
          }
          else{
            printErrorLog("Search string file not found: "+search_string_file);
            Exit(1);
          }
          ss_file_obj.count++;
          ///multple is allowed
        }
        
        
      else if(opt=="-rs"||opt=="--replace-string"){
          checkArgAvailability(args,i+1);
          replace_string.push_back(args[i+1]);
          skipcount=true;
          
          rs_obj.count++;
          ///multple is allowed
        }
        
        
      else if(opt=="-rs/f"||opt=="--replace-string-file"){
          checkArgAvailability(args,i+1);
          replace_string_file=args[i+1];
          skipcount=true;
          if(isFile(replace_string_file)){
              StringArray temp=getLineFromFileAndReturnVector(replace_string_file);
            replace_string.insert(replace_string.end(),temp.begin(),temp.end());
          }
          else{
            printErrorLog("Replace string file not found: "+replace_string_file);
            Exit(1);
          }
          rs_file_obj.count++;
          ///multple is allowed
        }
        
      else if(opt=="-re" || opt == "--regex"){
          checkArgAvailability(args,i+1);
          re_type=toLower(args[i+1]);
          skipcount=true;
          
          re_obj.count++;
          if(re_obj.count>1){printWarningLog("Regex mode overwritten");}
        }
        
        else if(opt=="-rel"||opt=="--regex-locale"){
          re_locale=true;
          
        }
        
       
      else if(opt=="-dp"||opt=="--depth"){
          checkArgAvailability(args,i+1);
          mustBeAnInteger("Depth",args[i+1]);
          depth=stringTo<decltype(depth)>(args[i+1]);
          if(depth<0){depth=std::numeric_limits<Int>::max();}
          skipcount=true;
          
          dp_obj.count++;
          if(dp_obj.count>1){printWarningLog("Directory depth overwritten");}
        }
        
        
      else if(opt=="-cd"||opt=="--count-directory"){
          count_directory=true;
          
        }
        
        
      else if(opt=="-cf"||opt=="--count-file"){
          count_file=true;
          
        }
        
        
      else if(opt=="-s"||opt=="--sort"){
          sort=true;
          sort_type="default";
          
        }
        
        
      else if(opt=="-s/g"||opt=="--sort/g"){
          sort=true;
          sort_type="general";
        }
        
        
      else if(opt=="-s/n"||opt=="--sort/n"){
          sort=true;
          sort_type="natural";
        }
        
      else if(opt=="-s/none"||opt=="--sort/none"){
          sort=true;
          sort_type="none";
        }
        
      else if(opt=="-y"||opt=="--yes"){
          all_yes=true;
          
        }
        
      else if(opt=="-fl"||opt=="--follow-link"){
          follow_symlink=true;
          
        }
        
      else if(opt=="-shop"||opt=="--show-options"){
          show_options=true;
          
        }
        
      else if(opt=="-f"||opt=="--force"){
          force=true;
          
        }
        
      else if(opt=="-u"||opt=="--undo"){
          undo=true;
          
        }
        
      else if(opt=="-sim"||opt=="--simulation"){
          simulation=true;
          
        }
        
        
      else if(opt=="--"){
          noopt=true;
          
        }
        
        
      else if(opt[0]=='-'){
          printErrorLog("Invalid Option: "+opt);
          Exit(1);
          
        }
        
        else {
            files.push_back(String(args[i]));
            
            
        }
        
     }
        
     else {
            files.push_back(String(args[i]));
            
            
     }

      
      
    }
  } //objects from Options class are destroyed here
    
    //file_vector=files;
    //////////////////////////////////////// Opt parse ends here/////////////////////////////////

    if(undo){undoRename();showResult();Exit(0);}

    
    
    /////////////////////////////////// Various checks///////////////////////////////////////
    
    if(file_only&&directory_only){printErrorLog("File Only and Directory Only, both can't be true at the same time");Exit(1);}
    if(file_only&&exclude_directory){printWarningLog("File Only mode is prioritized over Exclude Directory mode");}
    if(simulation&&quiet){printWarningLog("Quiet option won't have any effect with simulation mode");}
    
    
    if(files.size()<=0){String filename_from_stdin;std::getline(std::cin,filename_from_stdin,'\0');
        files.push_back(filename_from_stdin);
        //printErrorLog("No file or directory specified");Exit(1);
        }
    if(files.size()==1){if(!file_only || depth<=0){single_mode=true;}}
    if(simulation){quiet=false;}
    
    
    if(name_string=="" && name_string_file=="" && replace_string.size()==0){
        printErrorLog("One of the options: -ns or -nsf or -rs is mandatory");
        Exit(1);
        }
    if(search_string.size()!=0){
        for(Int i=0;i<(Int)search_string.size();i++){
                parseSearchString(search_string[i],i); 
        }
    ///search_string and fixed_ss will no longer be uesed. Better free up some memory
    search_string.clear();fixed_ss.clear();
    }
    
    
    if(name_string_file!=""){
        if(name_string_file==path_delim+"hist"+path_delim){
            if(isFile(NSF_LIST_FILE)){name_string_file=NSF_LIST_FILE;}
            else{printErrorLog("History not found.");Exit(1);}
        }
        else if(!isFile(name_string_file)){
            printErrorLog("Name String File not found: "+name_string_file);
            Exit(1);
        }
        detectLineUpwardOrDownward();
        copyFile2(name_string_file,NSF_LIST_FILE);
        if(!quiet){print "Reading name string file..."+NEW_LINE;}
        if(!nsf_n){nsflist=getNameListFromFile(name_string_file,start_line,end_line);}
        else{nsflist=getNameListFromFile(name_string_file,start_line,end_line,0);}
    
    }


    /// check regex type
    if(re_type!=""){
        
        if(re_type=="default"){REGEX_TYPE=REGEX_DEFAULT;}
        else if(re_type=="awk"){REGEX_TYPE=REGEX_AWK;}
        else if(re_type=="grep"){REGEX_TYPE=REGEX_GREP;}
        else if(re_type=="egrep"){REGEX_TYPE=REGEX_EGREP;}
        else if(re_type=="basic"){REGEX_TYPE=REGEX_BASIC;}
        else if(re_type=="extended"){REGEX_TYPE=REGEX_EXTENDED;}
        else if(re_type=="ecmascript"){REGEX_TYPE=REGEX_ECMASCRIPT;}
        else {printErrorLog("Invalid regex type: "+re_type);Exit(1);}
        
        }
    else {REGEX_TYPE=REGEX_DEFAULT;}

    
    ////////////////////////////////// Various checks end here//////////////////////////////
    
    ///Sort file if sort is true
    if(sort){sortVector(files);}
    
    startTask(files);
    
    showResult();
    

   return 0;
}

