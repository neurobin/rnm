# rnm
Bulk Rename Utility for GNU/Linux written in `C++`. Files and directories can be passed as command line arguments to rename them in bulk according to some naming scheme (*Name String*). It uses `C++` regex (<a href="http://www.cplusplus.com/reference/regex/ECMAScript/">ECMAScript regex</a>) to provide search (and replace) functionality. It provides an undo functionality too to move back an unwanted rename operation. 



#Features:

1. Both directories and files can be renamed.
2. Undo functionality (`rnm -u`).
3. Multiple directories and files can be passed as command line arguments.
4. File name can be modified at runtime by passing a *Name String* as argument.
5. File name can be generated by performing regex replace on old file name with *Replace String* (`-rs`).
6. Selective rename is possible by passing a *Search String* as argument which will take only the files that matches the search string ( *Search String* is a <a href="https://en.wikipedia.org/wiki/Regular_expression">regex</a> by default). To use a fixed string use the `-sF` option.
7. Name can be taken from a file too. And these names (taken from a file) can be modified with *Name String* at runtime.
8. Name can be taken from a file according to starting line number (`-l` or `-sl`) and ending line number (`-el`).
9. Null terminated file can be taken as Name String File.



#Install:

1. Give the <span class="quote">install</span> file execution permission and
2. Run it or just drag and dropt it on terminal and hit <kbd>Enter</kbd> (requires root privilege).


#Uninstall:
To uninstall, run:
```
sudo rm /usr/bin/rnm
```

#Usage:

```
rnm Directory/File/Path [options]
```

One of the options of `-ns` or `-nsf` or `-rs` is mandatory. Options are **not** sequential, their position in the argument list have no siginificance. For example, `rnm filepath -ns name` is the same as `rnm -ns name filepath`. Though passing the *Directory/File* path at the end of the arguement list is considered to be safe and wise.

Options are case insensitive, i.e `-ssF` and `-ssf` are the same.

##Options:

`-h`, `--help`       : Show help menu.

`--index`, `--start-index`,
`-i`,`-si`           : Starting index.

`--end-index`,
`-ei`                : End index i.e index to stop renaming from. It is only for files
                       inside a directory that is being recursively taken due to a depth
                       value greater than 0, i.e it works on directory index.
                       Note that directory index `/id/` will renew in each directory
                       i.e in each directory rename will be performed from start index
                       to end index.
                       
`--increment-value`,
`-inc`               : Increment value (floating point decimal). The amount, index will be
                       incremented or decremented in each iteration. Decremented index is
                       available through name string rule: `/-i/`, `/-id/` etc..

`--line-increment-value`,
`-linc`              : The amount line count will be incremented or decremented in each iteration.
                       This is always a positive integer.

       
`-ifl`               : Index field length. Non occupied field will be
                       filled with index field fillers (set with `-iff`). iff is set to the character `0` by default.
       
`--index-field-filler`,
`-iff`               : Not occupied field in index will be filled with a character
                      which is set by this option.

`--index-field-precision`,
`-ifp`               : Index is a floating point decimal value. This sets the precision
                       i.e the number of digits that should be taken after the decimal point.

`--name-string`,
`-ns`                : Name string.
     
`--name-string-file`,
`-nsf`               : Name string file. File containing name string (one per line).
                       `-nsf /hist/` i.e a value passed `/hist/` as Name string file, will
                       try to take the file from history.
       
`--name-string-file-null-terminated`,
`-nsfn`              : Name String file. This takes a null terminated *Name String* file, i.e
                 filenames are terminated by null character (`\0`) instead of new line (`\n`).

`--line`, `--start-line`,
`-l`, `-sl`          : Start Line number in name string file.

`--line-reverse`, `--start-line-reverse`,
`-lv`, `-slv`        : Same as `-l` or `-sl`, except line number will be decremented in each
                       iteration.

`--end-line`,
`-el`                : End line number. Line number to stop renaming from.

`--end-line-reverse`,
`-elv`               : Same as `-el`, except line number will be decremented in each iteration.

`--search-string`,
`-ss`                : Search string
                       String that will be used to search for files with matching names.
                       This is generally regex  (<a href="http://www.cplusplus.com/reference/regex/ECMAScript/">ECMAScript regex</a>) if not pass with `-ssf` option.
     
`--search-string-fixed`,
`-ssf`               : Fixed search string (not treated as regex).

`--depth`,
`-dp`                : Depth of folder. -1(any negative number) means unlimited depth i.e all files and subdirectories
                       will be included. Other values may be 0 1 2 3 etc...
                       Default depth is `0`, i.e directory contents will be ignored.

       
`--file-only`,
`-fo`                : File only mode. Only files are renamed (not directory).
                       Goes to subdirectory/s if depth (`-dp`) is set to `1` or greater.
                       Default depth is set to 0.
                       

`--directory-only`,
`-do`                : Apply rename on directory only. No subdirectories.

`--exclude-directory`,
`-ed`                : Apply rename on files only, exclude any and all directory and their
                       contents. Forces file only mode regardless of depth (-dp) value.


`-y`                 : Confirm Yes to all.

`-f`, `--force`      : Force rename

`-u`, `-U`, `--undo` : Undo renaming

`-v`                 : Version info.

`-q`                 : Quiet operation.

##Technical Terms:

**Reserved Index    :** Index will be incremented even if 
                    any file is skipped renaming in order
                    to reserve the index for that skipped file
                    
**Reverse Index     :** Decrementing index.
                
**Name String       :** A string, that is parsed to create names for new files. It can be fixed name which then can be modified for different files at runtime.
                    Name sting is parsed by the following rules (must be wrapped around with filepath delimiter `/`):
                    
1. `/i/` in name string will be replaced with index.
2. `/ir/` in name string will be replaced with reserved index.
3. `/id/` in name string will be replaced with directory index (index inside a directory).
4. `/idr/` in name string will be replaced with reserved directory index
5. `/n/` in name string will be replaced with filename without extention. If used with `-nsf` option, the filename will be the name taken from the *Name String File*.
6. `/fn/` in name string will be replaced with full name of the files. If used with `-nsf` option, full name will be the name taken from the *Name String File*.
7. `/rn/` in name string will be replaced with Replaced Name.
8. `/l/` in name string will be replaced with line number from *Name String File*.
9. `/la/` in name string will be replaced wiht actual line number from *Name String File*.
10. `/dc/` in name string will be replaced with directory count
11. `/-i/` in name string will be replaced with inverse index.
12. `/-ir/` in name string will be replaced with inverse reserved index. In general, `-` in the above replacement rules (applies to indexes excluding `/l/`, `/la/` and `/dc/`) will mean inverse index conforming to their meaning.


**Name String File  :** A file which contains a list of name string (one per line).
                    Empty lines will be ignored and line number won't be counted.
                    Actual line number (which counts the empty lines too) is
                    available through name string rule : `/la/`.
     
**Search String     :** A string that is used to search for files with matching
                    filenames against the search string. By default it is
                    a regex if `-ssF` option is not used. It's `C++` regex
                    which uses the <a href="http://www.cplusplus.com/reference/regex/ECMAScript/">ECMAScript regex</a> syntax.
                 
**Index Field Length:** An integer value defining the field length of index.
                    By default empty field will be filled with 0's. For example, if
                    the value is `3`, then index will be `001`, `002`, `003`, etc..
                    Different filler (other than 0) can be provided with the `-iff` option.
                    
**Replaced Name     :** The name can be modified at runtime using replace string.
                    replace string will be parsed to create a new *Name String* rule: `/rname/`
                    which can be used in *Name String*. If name string is not passed as argument,
                    the new name of the file will be `rname`. *Replaced Name* is always 
                    generated from the old filename.
                    
**Replace String    :** *Replace String* is a regex of the form: 
                    `/search_string/replace_string/modifier`
                    where search_string is the regex to search for and
                    replace_string is the string to replace with. For replace string,
                    there are four special cases:
                    
1. `&` will be taken as the entire match found by the regex (search_string).
2. `\1`, `\2` etc.. is the back-references, i.e you can access captured groups with these back-references.
3. `\p` is the prefix (i.e., the part of the target sequence that precedes the match)
4. `\s` is the suffix (i.e., the part of the target sequence that follows the match).

to insert a `&` literally, use `\&` and for `\` use `\\`.Two modifiers are avalilable: g and i.
g stands for global and replaces every instances of match found.
i stands case insensitive search (default is case sensitive).*Replace String* is always performed on old file name.
                    
                    Example: '/video/Episode /i//gi' will replace every instances
                    of 'video' with 'Episode index' i.e you will get new rname as:.
                    Episode 1..., Episode 2..., etc...
                    

                    
**Regex             :** Supported regex follows the ECMAScript regex syntax. Visit:
                    <a href="http://www.cplusplus.com/reference/regex/ECMAScript/">ECMAScript Regex</a>
                    for more info.
                    
Only invalid characters for a file or directory name is the path delimiter and the null character (`\0`).
        
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


1. All options should always be separated by space. For Example: `-vy` won't mean two option `-v` and `-y`, rather it will mean a single option `-vy`.
2. Any non option argument will be treated as file or directory path. For example in <pre><code>rnm file1 file2 -- -ns fd</code></pre>`file1`, `file2` and `--` will be taken as file paths.
3. Be wary of filename globbing. Command like `rnm ./*` will take all files and directories as arguments and thus the files and directories will be subject to rename operation. If you don't want to rename directories, use file only mode (`-fo`). If you want to go inside directories, use depth (`-dp`) greater than `0` with file only mode.
4. If you run `rnm . -ns something` or `rnm ./ -ns something`, your current directory will be renamed (be careful).
5. This is a dangerous tool like `rm`, so use with care. If you make a mistake and do some unwanted rename, run `rnm -u` to undo (before running any more rnm command).

