# rnm
Bulk Rename Utility for GNU/Linux


#Usage:

```
rnm Directory/File/Path [options]
```

One of the options of [-ns] or [-nsf] is mandatory. Filename globbing is allwed.

options:

`-h`, `--help`: Show help menu

`-i`  : Starting index
`-ifl` : Index field length. not occupied field will be
       filled with 0's.

`-ns`  : Name string
     
`-nsf` : Name string file. File containing name string (one per line).
`-l`   : Line number in name string file.

`-ss` : Search string
     String that will be used to search for files with matching names.
     Regex is allowed (posix compliant extended regex).
     

`-sF` : Fixed search string (not treated as regex).

`-dp`  : Depth of folder. 0 means unlimited depth i.e all files and subdirectories will
       be included. Other values may be 1 2 3 etc...
       Default depth is 1.
       
`-D`   : Apply rename on direcotry as well as on files.
       This is a peculier option and slightly confusing to users.
       no subdirectories will be renamed i.e only file and folders on the
       current directory. If only one directory is passed as the argument,
       that directory will be renamed not any file or folder inside that
       directory.
       
`-oD`  : Apply rename on directory only. 

`-y`   : Confirm Yes to all

`-v`   : Version info

`-q`   : Quiet operation

Technical Terms:

Reserved Index    : Index will be incremented even if 
                    any file is skipped renaming in order
                    to reserve the index for that skipped file
                
Name String       : A string which is parsed to create name for new files.
                    Name sting is parsed by the following rules:
     `/i/` in name string will be replaced with index.
     `/ir/` in name string will be replaced with reserved index.
     `/id/` in name string will be replaced with directory index (index inside a directory).
     `/idr/` in name string will be replaced with reserved directory index
     `/n/` in name string will be replaced with filename without extention.
     `/fn/` in name string will be replaced with full name of the files.
     `/l/` in name string will be replaced with line number from list file.
     `/dc/` in name string will be replaced with directory count
     
Search String     : A string that is used to search for files with matching
                    filenames against the search string. By default it is
                    a regex if -sF option is not used.
                 
Index Field Length: An integer value defining the field length of index.
                    empty field will be filled with 0's. For example, if
                    the value is 3, then index will be 001, 002, 003, etc..
                    
        
        
Example:

```
rnm ./New*/* -ns /i/.ext  (globbing is allowed)
rnm "./New Folder" -ns /id//dc/.ext
rnm ./New* -ns /id/.ext -ss "regex"
rnm -nsf filepath -ns /n//id/.ext
rnm -nsf filepath
etc...
```


#Things to care:

1. Newline in file or folder name is not supported. Make sure your folder doesn't contain any file/folder with new line.
2. All options should always be separated by space. -vy won't mean two option: `-v` and `-y`, rather it will mean a single option `-vy`.

