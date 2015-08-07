# rnm
Bulk Rename Utility for GNU/Linux


#Usage:

`rnm Directory/File/Path [options]`

options:

1. `-h`, `--help`: Show help menu

2. `-i`   : Starting index
3. `-ri`  : Reserve index. Index will be incremented always,
       i.e index will be reserved for skipped file
4. `-ifl` : Index field length. not occupied field will be
       filled with 0's.


5. `-ns`  : Name string
    1. `/i` in name string will be replaced with index.
    2. `/n` in name string will be replaced with filename without extention.
    3. `/nf` in name string will be replaced with full name of the files.
     
6. `-nsf` : Name string file. File containing name string (one per line).
7. `-l`   : Line number in name string file.

8. `-ss`  : Search string
     String that will be used to search for files with matching names.
     Regex is allowed (posix compliant extended regex).
     

9. `-sF`  : Fixed search string (not treated as regex).

10. `-dp`  : Depth of folder. 0 means unlimited depth i.e all files and subdirectories will
       be included. Other values may be 1 2 3 etc...
       Default depth is 1.

11. `-y`   : Confirm Yes to all

12. `-v`   : Version info

13. `-q`   : Quiet operation


#Things to care:

1. Newline in file or folder name is not supported. Make sure your folder doesn't contain any file/folder with new line.
2. All options should always be separated by space. -vy won't mean two option: `-v` and `-y`, rather it will mean a single option `-vy`.

