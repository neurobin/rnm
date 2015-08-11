# rnm
Bulk Rename Utility for GNU/Linux written in Bash scripting language. Therefore, the use of this script/tool depends on Bash. Files and folders can be passed as command line arguments to rename them in bulk according to some naming scheme (*Name String*). It provides an undo functionality too to move back an unwanted rename operation.



#Features:

1. Both directories and files can be renamed.
2. Undo functionality (`rnm -u`).
3. Multiple directories and files can be passed as command line arguments.
4. File name can be modified at runtime by passing a *Name String* as argument.
5. Selective rename is possible by passing a *Search String* as argument which will take only the files that matches the search string ( *Search String* is a <a href="https://en.wikipedia.org/wiki/Regular_expression">regex</a> by default). To use a fixed string use the `-sF` option.
6. Name can be taken from a file too. And these names (taken from a file) can be modified with *Name String* at runtime.
7. Name can be taken from a file according to starting line number (`-l` or `-sl`) and ending line number (`-el`).



#Install:

1. Give the <span class="quote">install</span> file execution permission and
2. Run it or just drag and dropt it on terminal and hit <kbd>Enter</kbd>.


#Uninstall:
To uninstall, run:
```
sudo rm /usr/bin/rnm
```

#Usage:

```
rnm Directory/File/Path [options]
```

One of the options of `-ns` or `-nsf` is mandatory. Options are **not** sequential, their position in the argument list have no siginificance. For example, `rnm filepath -ns name` is the same as `rnm -ns name filepath`. Though passing the *Directory/File* path at the end of the arguement list is considered to be safe and wise.

##Options:

`-h`, `--help`      : Show help menu.

`-i`,`-si`          : Starting index.

`-ei`               : End index i.e index to stop renaming from.
       Note that directory index `/id/` will renew in each directory
       i.e in each directory rename will be performed from start index
       to end index.
       
`-ifl`              : Index field length. not occupied field will be
       filled with 0's.

`-ns`               : Name string.
     
`-nsf`              : Name string file. File containing name string (one per line).
       `-nsf /hist/` i.e a value passed `/hist/` as Name string file, will
       try to take the file from history.
       
`-l`, `-sl`         : Start Line number in name string file.

`-li`, `-sli`       : Same as `-l` or -`sl`, except line number will be decremented in each iteration.

`-el`               : End line number. Line number to stop renaming from.

`-eli`              : Same as `-el`, except line number will be decremented in each iteration.

`-ss`               : Search string.
     String that will be used to search for files with matching names.
     Regex is allowed (posix compliant extended regex).
     

`-sF`               : Fixed search string (not treated as regex).

`-dp`               : Depth of folder. 0 means unlimited depth i.e all files and subdirectories will
       be included. Other values may be 1 2 3 etc...
       Default depth is 1.
       
`-D`                : Directories and files both will be treated in the same way,
       i.e apply rename on direcotry as well as on files.
       This is a peculier option and may seem slightly confusing.
       No subdirectories will be renamed i.e only file and folders on the
       current directory. If only one directory is passed as the argument,
       that directory will be renamed not any file or folder inside that
       directory.
       
`-oD`                : Apply rename on directory only. No subdirectories.

`-ed`                : Apply rename on files only, exclude any and all directory and their contents.

`-y`                 : Confirm Yes to all.

`-f`, `--force`      : Force rename

`-u`, `-U`, `--undo` : Undo renaming

`-v`                 : Version info.

`-q`                 : Quiet operation.

##Technical Terms:

**Reserved Index    :** Index will be incremented even if 
                    any file is skipped renaming in order
                    to reserve the index for that skipped file
                
**Name String       :** A string, that is parsed to create names for new files. It can be fixed name which then can be modified for different files at runtime.
                    Name sting is parsed by the following rules:
                    
1. `/i/` in name string will be replaced with index.
2. `/ir/` in name string will be replaced with reserved index.
3. `/id/` in name string will be replaced with directory index (index inside a directory).
4. `/idr/` in name string will be replaced with reserved directory index
5. `/n/` in name string will be replaced with filename without extention. If used with `-nsf` option, the filename will be the name taken from the *Name String File*.
6. `/fn/` in name string will be replaced with full name of the files. If used with `-nsf` option, full name will be the name taken from the *Name String File*.
7. `/smn/` in name string will be replaced with Sed Modified Name.
8. `/rn/` in name string will be replaced with Sed Modified Name.
9. `/l/` in name string will be replaced with line number from *Name String File*.
10. `/dc/` in name string will be replaced with directory count
11. `/-i/` in name string will be replaced with inverse index.
12. `/-ir/` in name string will be replaced with inverse reserved index. In general, `-` in the above replacement rules (applies to indexes excluding `/l/` and `/dc/`) will mean inverse index conforming to their meaning.

**Inverse Index     :** Decrementing index.

**Name String File  :** A file which contains a list of name string (one per line, empty line will make files skip) to be applied to the new files.
     
**Search String     :** A string that is used to search for files with matching
                    filenames against the search string. By default it is
                    a regex if `-sF` option is not used.
                 
**Index Field Length:** An integer value defining the field length of index.
                    empty field will be filled with 0's. For example, if
                    the value is `3`, then index will be `001`, `002`, `003`, etc..
                    
**Directory Index   :** Index inside a directory i.e for every new directory
                     index is counted from the start.
                     
**Sed Modified Name :** The name can be modified at runtime using the default
                    sed tool available in Linux system. You can insert sed
                    script in the form `/search_string/replace_string/modifier;`
                    
                    For example: `/video/Episode/gi;` will replace every instances
                    of `video` with `Episode` in the name of the file found (or the name 
                    taken from Name String File).
                    
                    The format must be of :
                    `/search_string/replace_string/modifier;`
                    
                    Two modifiers are avalilable: `g` and `i`.
                    `g` stands for global. Replaces every instances of match found.
                    `i` stands case insensitive search.
                    
                    This replaced modified name is available through Name String Rule: 
                    `/smn/` or `/rn/` (stands for replaced name)
                    
                    Note that sed basic regex mode is used to do this job.
        
        
###Example:

```
rnm file -ns new_file
rnm folder -ns "New Folder" -oD          (-oD forces Directory only mode)
rnm ./New*/* -ns /i/.ext                 (globbing is allowed)
rnm ./New*/* -ns /i/.ext -ed             (-ed forces file only mode)
rnm "./New Folder" -ns /id//dc/.ext
rnm ./New* -ns /id/.ext -ss "regex"
rnm -nsf filepath -ns /n//id/.ext
rnm -nsf filepath
etc...
```


#Things to care:

1. Newline in file or folder name is not supported. Make sure your folders/files don't contain any new line in their name.
2. All options should always be separated by space. For Example: `-vy` won't mean two option `-v` and `-y`, rather it will mean a single option `-vy`.
3. Any non option argument will be treated as file or directory path. For example in <pre><code>rnm file1 file2 -- -ns fd</code></pre>`file1`, `file2` and `--` will be taken as file paths.
4. Be wary of filename globbing. Command like `rnm ./*` will take all files and folders as arguments and thus the files and immediate contents (by default) of the folders will be subject to rename operation. If you don't want to rename any folder contents (when the folder is passed as an argument), use `-ed` option to exclude any and all directory which have mistakenly or deliberately passed as arguments.
5. This is a dangerous tool like `rm`, so use with care. If you make a mistake and do some unwanted rename, run `rnm -u` to undo.
6. Always use at least one forward slash in *Directory/File/Path*. If it's the current directory then `./filename` or `./foldername`, if you want globbing, then `./*`. Never use `filename` or `foldername` without a slash in their path, and never use `*` to glob into the current folder, instead use `./*`. It's very important, so be careful about it.

