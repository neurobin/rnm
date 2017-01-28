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
 
 
#include "func.hpp"


void checkArgAvailability(const StringArray& sa,int i){
    if(i>=(int)sa.size()){
        printErrorLog("One or more argument/s missing");
        Exit(1);
    }
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);      //interactive attention signal
    signal(SIGABRT, signalHandler);     //Abort , abnormal termination
    signal(SIGFPE, signalHandler);      //Erroneous Arithmetic operation
    signal(SIGILL, signalHandler);      //Illigal instruction
    signal(SIGSEGV, signalHandler);     //Invalid access of storage
    signal(SIGTERM, signalHandler);     //A termination request
    std::ios::sync_with_stdio(false);
    try{
        ///The following must be the first line
        CWD = getCurrentDir();
        CWDN=basename(CWD);
        //change names of rnm file log files to reflect the current directory
        String signature = getPathSignature(CWD);
        RNM_LOCK_FILE += signature;
        RNM_FILE_LOG_L += signature;
        RNM_FILE_LOG_R += signature;
        //~ RNM_FILE_LOG_L_TMP += signature;
        //~ RNM_FILE_LOG_R_TMP += signature;
        
        //prepare logs
        prepareLogDir();
        openLogFiles();
        //~ openTmpFiles();
        openLockFile();
        
        ///FileArray
        FileArray files;

        
        //////////////////////////////// Opt parse////////////////////////////////////////////
        
        StringArray args;
        for(int i = 1; i < argc; i++){args.push_back(argv[i]);}
        
        bool skipcount=false;
        bool noopt=false;
        { //Scope for the following objects
        Options inc_obj("-inc","--increment");
        Options linc_obj("-linc","--line-increment");
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
        Options dp_obj("-dp","--depth");
        Options if_obj("-if","--index-flags");
        
        for(int i = 0; i < (int)args.size(); i++){
            if(skipcount){skipcount=false;continue;}
            String opt=toLower(String(args[i]));
            if(!noopt){
                if(opt=="-h"||opt=="--help"){
                  std::cout<< help_message;
                  Exit(0);
                } else if(opt=="-v"||opt=="--version"){
                  std::cout<< version_info;
                  Exit(0);
                } else if(opt=="-q"||opt=="--quiet"){
                  quiet=true;
                } else if(opt=="-qq"||opt=="--quiet-quiet"){
                  quiet=true;
                  double_quiet=true;
                } else if(opt=="-fo"||opt=="--file-only"){
                  file_only=true;
                } else if(opt=="-do"||opt=="--directory-only"){
                  directory_only=true;
                }else if(opt=="-lo"||opt=="--link-only"){
                  link_only=true;
                } else if(opt=="-xd"||opt=="--exclude-directory"){
                  exclude_directory=true; 
                } else if(opt=="-xf"||opt=="--exclude-file"){
                  exclude_file=true; 
                } else if(opt=="-xl"||opt=="--exclude-link"){
                  exclude_link=true; 
                } else if(opt=="-inc"||opt=="--increment"){
                  checkArgAvailability(args,i+1);
                  inc = getPositiveNumberOrExit("Increment Value","\nNegative increment i.e decrement will be available using name string rule:\n\
                /-i/, /-ir/, /-id/ etc..\n",args[i+1]);
                  skipcount=true; 
                  inc_obj.count++;
                  if(inc_obj.count>1){printWarningLog("Increment value overwritten");}
                } else if(opt=="-linc"||opt=="--line-increment"){
                  checkArgAvailability(args,i+1);
                  linc = getPositiveIntOrExit("Line Increment Value",args[i+1]);
                  if(linc<1) errorExit("Line increment can not be less than 1");
                  skipcount=true; 
                  linc_obj.count++;
                  if(linc_obj.count>1){printWarningLog("Line increment value overwritten");}
                } else if(opt=="-i"||opt=="-si"||opt=="--index"||opt=="--start-index"){
                  checkArgAvailability(args,i+1);
                  start_index=getNumberOrExit("Start Index",args[i+1]);;
                  current_index=start_index;
                  current_index_rd=start_index;
                  reverse_index=start_index;
                  reverse_index_rd=start_index;
                  DIRECTORY_INDEX=start_index;
                  DIRECTORY_REVERSE_INDEX=start_index;
                  skipcount=true; 
                  si_obj.count++;
                  if(si_obj.count>1){printWarningLog("Start index overwritten");}
                } else if(opt=="-ei"||opt=="--end-index"){
                  checkArgAvailability(args,i+1);
                  end_index=getNumberOrExit("End Index",args[i+1]);
                  skipcount=true;
                  infinite_end_index=false;
                  ei_obj.count++;
                  if(ei_obj.count>1){printWarningLog("End index overwritten");}
                } else if(opt=="-if"||opt=="--index-flags"){
                  checkArgAvailability(args,i+1);
                  parseIndexFlags(args[i+1]);
                  skipcount=true;
                  if_obj.count++;
                  if(if_obj.count>1){printWarningLog("Override occurred in Index flags");}
                } else if(opt=="-ifl"||opt=="--index-field-length"){
                  checkArgAvailability(args,i+1);
                  index_field_length=getPositiveIntOrExit("Index field length",args[i+1]);
                  skipcount=true; 
                  ifl_obj.count++;
                  if(ifl_obj.count>1){printWarningLog("Index field length overwritten");}
                } else if(opt=="-ifp"||opt=="--index-field-precision"){
                  checkArgAvailability(args,i+1);
                  IFP=getPositiveIntOrExit("Index field precision",args[i+1]);
                  skipcount=true; 
                  ifp_obj.count++;
                  if(ifp_obj.count>1){printWarningLog("Index field precision overwritten");}
                } else if(opt=="-iff"||opt=="--index-field-filler"){
                  checkArgAvailability(args,i+1);
                  IFF=getSingleCharacterStringOrExit("Index field filler",args[i+1]);
                  IFF=replaceStringAll(IFF,"\\","");
                  skipcount=true; 
                  iff_obj.count++;
                  if(iff_obj.count>1){printWarningLog("Index field filler overwritten");}
                } else if(opt=="-ns"||opt=="--name-string"){
                  checkArgAvailability(args,i+1);
                  name_string=args[i+1];
                  skipcount=true; 
                  ns_obj.count++;
                  if(ns_obj.count>1){printWarningLog("Name string overwritten");}
                } else if(opt=="-ns/f"||opt=="--name-string-file"){
                  checkArgAvailability(args,i+1);
                  name_string_file=args[i+1];
                  skipcount=true; 
                  nsf_obj.count++;
                  if(nsf_obj.count>1){printErrorLog("Only one name string file is allowed");Exit(1);}
                } else if(opt=="-ns/fn"||opt=="--name-string-file-null-terminated"){
                  checkArgAvailability(args,i+1);
                  name_string_file=args[i+1];
                  nsf_n=true;
                  skipcount=true; 
                  nsf_obj.count++;
                  if(nsf_obj.count>1){printErrorLog("Only one name string file is allowed");Exit(1);}
                } else if(opt=="-l"||opt=="-sl"||opt=="--line"||opt=="--start-line"){
                  checkArgAvailability(args,i+1); 
                  start_line=getPositiveIntOrExit("Start Line",args[i+1]);
                  skipcount=true; 
                  sl_obj.count++;
                  if(sl_obj.count>1){printWarningLog("Start line overwritten");}
                } else if(opt=="-el"||opt=="--end-line"){
                  checkArgAvailability(args,i+1); 
                  end_line=getPositiveIntOrExit("End Line",args[i+1]);
                  skipcount=true; 
                  el_obj.count++;
                  if(el_obj.count>1){printWarningLog("End line overwritten");}
                } else if(opt=="-ss"||opt=="--search-string"){
                  checkArgAvailability(args,i+1);
                  search_string.push_back(args[i+1]);
                  fixed_ss.push_back(false);
                  skipcount=true; 
                  ss_obj.count++;
                  ///multple is allowed
                } else if(opt=="-ss/f"||opt=="--search-string-file"){
                  checkArgAvailability(args,i+1);
                  search_string_file=args[i+1];
                  skipcount=true; 
                  if(File(search_string_file).isFile()){
                    StringArray temp=getLineFromFileAndReturnVector(search_string_file);
                    ///append temp to search_string
                    search_string.insert(search_string.end(),temp.begin(),temp.end());
                    ///Populate fixed_ss with same number of boolean value
                    for(Int i=0;i<(Int)temp.size();i++){
                        fixed_ss.push_back(false);
                    }
                  } else{
                    printErrorLog("Search string file not found: "+search_string_file);
                    Exit(1);
                  }
                  ss_file_obj.count++;
                  ///multple is allowed
                } else if(opt=="-ssf"||opt=="--search-string-fixed"){
                  checkArgAvailability(args,i+1);
                  search_string.push_back(args[i+1]);
                  fixed_ss.push_back(true);
                  skipcount=true; 
                  ss_obj.count++;
                  ///multple is allowed
                } else if(opt=="-ssf/f"||opt=="--search-string-fixed-file"){
                  checkArgAvailability(args,i+1);
                  search_string_file=args[i+1];
                  skipcount=true;
                  if(File(search_string_file).isFile()){
                    StringArray temp=getLineFromFileAndReturnVector(search_string_file);
                    ///append temp to search_string
                    search_string.insert(search_string.end(),temp.begin(),temp.end());
                    ///Populate fixed_ss with same number of boolean value
                    for(Int i=0;i<(Int)temp.size();i++){
                        fixed_ss.push_back(true);
                        }
                  } else {
                    printErrorLog("Search string file not found: "+search_string_file);
                    Exit(1);
                  }
                  ss_file_obj.count++;
                  ///multple is allowed
                } else if(opt=="-rs"||opt=="--replace-string"){
                  checkArgAvailability(args,i+1);
                  replace_string.push_back(args[i+1]);
                  skipcount=true; 
                  rs_obj.count++;
                  ///multple is allowed
                } else if(opt=="-rs/f"||opt=="--replace-string-file"){
                  checkArgAvailability(args,i+1);
                  replace_string_file=args[i+1];
                  skipcount=true;
                  if(File(replace_string_file).isFile()){
                      StringArray temp=getLineFromFileAndReturnVector(replace_string_file);
                    replace_string.insert(replace_string.end(),temp.begin(),temp.end());
                  }
                  else{
                    printErrorLog("Replace string file not found: "+replace_string_file);
                    Exit(1);
                  }
                  rs_file_obj.count++;
                  ///multple is allowed
                } else if(opt=="-dp"||opt=="--depth"){
                  checkArgAvailability(args,i+1); 
                  depth=getIntOrExit("Depth",args[i+1]);
                  if(depth<0){depth=std::numeric_limits<size_t>::max();}
                  skipcount=true; 
                  dp_obj.count++;
                  if(dp_obj.count>1){printWarningLog("Directory depth overwritten");}
                } else if(opt=="-cd"||opt=="--count-directory"){
                  count_directory=true; 
                } else if(opt=="-cf"||opt=="--count-file"){
                  count_file=true; 
                } else if(opt=="-cl"||opt=="--count-link"){
                  count_link=true; 
                } else if(opt=="-s"||opt=="--sort"){
                  sort=true;
                  sort_type="default"; 
                } else if(opt=="-s/g"||opt=="--sort/general"){
                  sort=true;
                  sort_type="general";
                } else if(opt=="-s/n"||opt=="--sort/natural"){
                  sort=true;
                  sort_type="natural";
                } else if(opt=="-s/none"||opt=="--sort/none"){
                  sort=true;
                  sort_type="none";
                } else if(opt=="-s/mt"||opt=="--sort/mtime"){
                  sort=true;
                  sort_type="mtime";
                } else if(opt=="-s/at"||opt=="--sort/atime"){
                  sort=true;
                  sort_type="atime";
                } else if(opt=="-s/ct"||opt=="--sort/ctime"){
                  sort=true;
                  sort_type="ctime";
                } else if(opt=="-s/sz"||opt=="--sort/size"){
                  sort=true;
                  sort_type="size";
                } else if(opt=="-s/d"||opt=="--sort/directory"){
                  sort=true;
                  sort_type2="directory";
                } else if(opt=="-s/f"||opt=="--sort/file"){
                  sort=true;
                  sort_type2="file";
                } else if(opt=="-s/l"||opt=="--sort/link"){
                  sort=true;
                  sort_type2="link";
                } else if(opt=="-s/r"||opt=="--sort/reverse"){
                    reverse_sort=true;
                } else if(opt=="-y"||opt=="--yes"){
                  all_yes=true;
                  ALL_YES=true;
                } else if(opt=="-fl"||opt=="--follow-link"){
                  follow_symlink=true; 
                } else if(opt=="-nfl"||opt=="--no-follow-link"){
                  follow_symlink=false; 
                } else if(opt=="-f"||opt=="--force"){
                  force=true; 
                } else if(opt=="-ff"||opt=="--force-force"){
                  super_force=true;
                  force=true;
                } else if(opt=="-u"||opt=="--undo"){
                  undo=true; 
                } else if(opt=="-up"||opt=="--undo-path"){
                  undo=true; 
                  checkArgAvailability(args,i+1);
                  undo_path = args[i+1];
                } else if(opt=="-ups"||opt=="--undo-path-show"){
                  undo=true;
                  undo_path_show=true;
                } else if(opt=="-sim"||opt=="--simulation"){
                  simulation=true; 
                } else if(opt=="--"){
                  noopt=true; 
                } else if(opt[0]=='-'){
                  printErrorLog("Invalid Option: "+opt);
                  Exit(1); 
                } else {
                    files.push_back(File(args[i])); 
                }

            } else {
                files.push_back(File(args[i])); 
            }
        }
      } //objects from Options class are destroyed here
        //file_vector=files;
        //////////////////////////////////////// Opt parse ends here/////////////////////////////////

        if(undo){
            START_TIME = timeNow();
            undoRename();
            Exit(0);
        }

        
        
        /////////////////////////////////// Various checks///////////////////////////////////////
        
        if(file_only&&directory_only){printErrorLog("File Only and Directory Only, both can't be true at the same time");Exit(1);}
        if(file_only&&link_only){printErrorLog("File Only and Link Only, both can't be true at the same time");Exit(1);}
        if(link_only&&directory_only){printErrorLog("Link Only and Directory Only, both can't be true at the same time");Exit(1);}
        //~ if(file_only&&exclude_directory){printWarningLog("File Only mode is prioritized over Exclude Directory mode");}
        //~ if(simulation&&quiet){printWarningLog("Quiet option won't have any effect with simulation mode");}
        
        
        if(files.size()<=0){
            String filename_from_stdin;
            signal(SIGINT, unsafeExitSignalHandler); //agressive exit needed
            std::getline(std::cin,filename_from_stdin,'\0');
            signal(SIGINT, signalHandler); //restore to safe signal handler
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
            for(size_t i=0;i<search_string.size();i++){
                    parseSearchString(search_string[i],i); 
            }
        ///search_string and fixed_ss will no longer be uesed. Better free up some memory
        search_string.clear();fixed_ss.clear();
        }
        
        
        if(!name_string_file.empty()){
            if(!name_string.empty()){
                errorExit("-ns or equivalent options can not be given with -ns/f or -ns/fn");
            }
            if(!quiet){std::cout<< "Reading name string file..."+NEW_LINE;}
            if(!nsf_n){
                getNameListFromFile(nsflist,name_string_file,start_line.get_ui(),end_line.get_ui());
            } else{
                getNameListFromFile(nsflist,name_string_file,start_line.get_ui(),end_line.get_ui(),0);
            }
        
        }

        
        ////////////////////////////////// Various checks end here//////////////////////////////
        
        ///Sort file if sort is true
        if(sort){sortVector(files);}
        START_TIME = timeNow();
        startTask(files);
        
        showResult();
        
        cleanup(true);
    } catch (const Except& e){
        //result is shown by exit
        return e.status;
    }
   return 0;
}

