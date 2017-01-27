% rnm(1) rnm user manual
% Md Jahidul Hamid <https://github.com/neurobin>
% January 26, 2017

# NAME
rnm - Bulk rename utility

# SYNOPSIS
**rnm** [*options*] *file/paths*

# DESCRIPTION
Renames files/directories in bulk. Naming scheme (*Name String*) can be applied or regex replace can be performed to modify names dynamically. It uses PCRE2 (revised version of PCRE) regex to provide search (and replace) functionality.

It provides versatile options to modify names, delete/replace part of it, indexing, case conversion, insert text, insert various file information like modification time, access time, permission etc.., insert parent directory names and many more.

File search functionality is provided with PCRE2 regex. Fixed string search is also possible.

Operations (rename, changing names, search) are selective of file type (directory, file, link).

Files can be sorted by name, modification time, access time, size, file type (directory, file, link) etc..

It provides an undo functionality to move back unwanted rename operations. Different **rnm** operations on different directory remember their own undo history.

Simulations can be run instead of actual rename to view the potential outcome as program output on terminal with the *-sim* option.


#USAGE

```bash
rnm -ns new_name [other-opts] file/path
rnm -rs "/search/replace/mod" [other-opts] file/path
rnm -ns/f namestring/file/path file/path
```

Options are **not** sequential, their position in the argument list holds no significance except for *-fl* and *-nfl*. For example, the two commands below are the same:

```bash
rnm filepath -ns name
rnm -ns name filepath
```

But the behavior of *-fl* (*--follow-link*) and *-nfl* (*no-follow-link*) option depends on their position. For example:

```bash
rnm -ns '/fn/ /id/' link1 -fl link2 \
    -nfl link3 link4 -fl  link5 link6
```
will rename the symbolic links as:

1. *link1* will itself be renamed, it won't be followed
2. *link2* will be followed and its target will be renamed because *--follow-link* flag was **set** before it.
3. *link3* and *link4* won't be followed because *--follow-link* flag was **unset** by *-nfl* before these links.
4. *link5* and *link6* will be followed because *--follow-link* flag was **set** again before these links.

Options are case insensitive, i.e *-ssF* and *-ssf* are the same.


# OPTIONS

-ns *name-string*
: Name string. This is generally the new name for the file. Any part in this name string wrapped around with the path delimiter (/) is parsed as a name string rule and the new name is formed accordingly. See <a href="#name-string-rule">NAME STRING RULE</a> for details.

-ns/f *file-path*
: Name string file. File containing name strings per line. If this option is passed, any other name string option can not be given (replace strings are allowed). Name strings will directly be extracted from the file and each of these name strings will be applied to each file (one for one). All name string rules are available with the name strings defined in this file, with an additional rule */l/* for accessing line number. To access the result from replace strings, use the name string rule */rn/* in the name string rules defined in the name string file.

-ns/fn *file-path*
: Name String file. This takes a null terminated Name String file, i.e a file where filenames/name strings are terminated by null character (*\\0*) instead of new line (*\\n*).

-sl *positive-integer*
: Start Line number in name string file. If start line is 0, it is changed to the highest line number possible in the file. For example, if you want to go from last line to first line, you can pass start line as 0 and end line as 1. Empty lines are always ignored but line numbers are counted. Start line can be greater than end line, for example, start line 25 and end line 12 would mean: go from line 25 to 12. Default start line is 1.

-l *positive-integer*
: Same as *-sl*

-el *positive-integer*
: End line number in name string file to stop renaming from. If end line is 0, it is changed to the highest line number possible in the file. For example, if you want to go from first line to last line in the file, pass start line as 1 and end line as 0. Empty lines are always ignored but line numbers are counted. End line can be smaller than start line. Default end line is the highest line number in the file.

-linc *positive-integer*
: The amount line count will be incremented in each iteration for name string file.

-ss *search-regex*
: Search string. String that will be used to search for files with matching names. This is generally a regex  if not passed with *-ssf* option. See <a href="#search-string">SEARCH STRING</a> for details. This option can be given multiple times to add search strings.

-ss/f *file-path*
: Search string file. A file containing search string per line. This option can be given multiple times to add search strings.

-ssf *search-term*
: Fixed search string (not treated as regex). See <a href="#search-string">SEARCH STRING</a> for details. This option can be given multiple times to add search strings.

-ssf/f *file-path*
: Search string file. Contains fixed search string per line. This option can be given multiple times to add search strings.

-rs *replace-string*
: A string in the form:

        /regex/replace/modifier
 It performs regex replace on old filename to construct new name. It uses PCRE2 regex. After performing all the regex replace operations, the resultant string is stored in the name string rule */rn/*. It can be used inside a name string or if name string is not given, this will by default become the new name for the current file. See <a href="#replace-string">REPLACE STRING</a> for details. This option can be given multiple times to add replace strings.

-rs/f *file-path*
: Replace string file. A file containing replace string per line. This option can be given multiple times to add replace strings.

-si *number*
: Start index.

-i *number*, 
: Same as *-si*

-ei *number*
: End index. It works on directory index only. When rename is occurring inside a directory for a depth value greater than 0 or negative, directory index is limited to this end index. When incrementing directory index hits this limit, all remaining files/directories/links inside that directory are skipped. All directories and their subdirectories will be subject to this limiting value.

-inc *number*
: Increment value (floating point decimal). The amount, index will be incremented or decremented in each iteration. Decremented index is available through name string rule */-i/*, */-id/* etc..

-if *INDEX-FLAGS*
: This sets Index flags. This is a '/' separated list of flags that will be used to render the index within it's text field. The general format is:
```bash
    '/flag1/flag2/flag3=value/...'
```
 Valued flags are set with *flag=value* format. Ex: 
```bash
    '/uppercase/filler=*/'
```
 See <a href="#index-flags">INDEX FLAGS</a> for details.

-ifl *positive-integer*
: Index field length. Non occupied field will be filled with index field fillers (set with *-iff*). *iff* is set to the character *0* by default.

-iff *character*
: Non-occupied field in index will be filled with a character set by this option.

-ifp *positive-integer*
: Index is a floating point decimal (by default) value. This sets the precision.

-dp *integer*
: Depth of folder. -1(any negative number) means unlimited depth i.e all files and subdirectories will be included. Other values may be 0 1 2 3 etc... Default depth is *0*, i.e directory contents will be ignored.

-fo
: File only mode. Only files are renamed (no directory or link). Goes to subdirectory/s if depth (*-dp*) is greater than 0 or negative.

-do
: Apply rename on directory only. Goes to subdirectories if depth is greater than 0 or negative.

-lo
: Link only mode. Goes to subdirectories if depth is greater than 0 or negative.

-xd
: Exclude directory. Exclude any and all directories and their contents. The depth value *-dp* will have no effect if this option is given. This is by default equivalent to file+link only mode if not overridden by other options. (You can read this option as cross-d which probably makes more sense)

-xf
: Exclude file. Depth value *-dp* is respected and goes to subdirectories if depth is greater than 0 or negative. This is by default equivalent to directory+link only mode if not overridden by other options. (You can read this option as cross-f which probably makes more sense)

-xl
: Exclude link. Depth value *-dp* is respected and goes to subdirectories if depth is greater than 0 or negative. This is by default equivalent to directory+file only mode if not overridden by other options. (You can read this option as cross-l which probably makes more sense)

-fl
: Set follow link flag. After passing this option, any file that is a link will be followed to their original target. If there are multiple links associated, only the end target (the original) will be renamed. This option must be passed before the file path that needs to be followed. Passing it after the file path will have no effect on the previous files.

-nfl
: Unset follow link flag. After this option is passed, previously set *--follow-link* flag will be unset and symbolic links after this point will not be followed unless it gets overridden by another *-fl* option.

-cd
: Count directory in reserved index, regardless of other options. Reserves indexes for directories even if it is file only or link only mode.

-cf
: Count file in reserved index, regardless of other options. Reserves indexes for files even if it is directory only or link only mode.

-cl
: Count link in reserved index, regardless of other options. Reserves indexes for links even if it is directory only or file only mode.

-s
: Sort files in natural order (Human perceivable order). This option can be modified to use other sorting methods. For example: *-s/g* or *--sort/general* will sort the file in general (alphabetic) order. Available sorting methods:
```bash
-s    : default sort (natural sort)
-s/g  : general sort
-s/n  : natural sort
-s/mt : sort according to file modification time (recent first)
-s/at : sort according to file access time (recent first)
-s/ct : sort according to file status change time (recent first)
-s/sz : sort according to file size (larger first)
-s/d  : prioritize directory when sorting
-s/f  : prioritize file when sorting
-s/l  : prioritize link when sorting
-s/r  : reverse the order sorted by above methods.
```

-y
: Confirm Yes to all and suppress printing output for each file.

-u
: Undo rename. Undo depends on working directory. If an **rnm** command is run from ~/somedir, to undo this operation one must run **rnm** from the same directory again or provide the path with *-up* option.

-up
: Undo rename from a given path. After every successful rename (or undo), the undo path is set to the working directory where **rnm** is run from.

-ups
: Show available undo paths.

-q
: Quiet operation (speedy operation).

-qq
: Suppress even error messages.

 -f
: Force rename. Enables renaming some restricted files except */*.

\--
: If this option is passed, anything and everything after it will be taken as file path. Put all options before it.

-h
: Show help menu.

-v
: Show version info.

-sim
: This runs a simulation of rename instead of actual rename operation.


#FULLY SPECIFIED NAMES FOR OPTIONS

All of the shorthand option names have equivalent full names. The use of either of them is same, i.e all option or sub-option should be separated with space. The general rule --someopt=value won't apply, instead **rnm** uses the syntax **--someopts value**. The following table shows the full names corresponding to the shorthand names:

Opt name | Full name
-------- | ---------
-ns | --name-string
-ns/f | --name-string-file
-ns/fn | --name-string-file-null-terminated
-l | --line
-sl | --start-line
-el | --end-line
-linc | --line-increment
-ss | --search-string
-ss/f | --search-string-file
-ssf | --search-string-fixed
-ssf/f | --search-string-fixed-file
-rs | --replace-string
-rs/f | --replace-string-file
-i | --index
-si | --start-index
-ei | --end-index
-inc | --increment
-if | --index-flags
-ifp | --index-field-precision
-ifl | --index-field-length
-iff | --index-field-filler
-dp | --depth
-fo | --file-only
-do | --directory-only
-lo | --link-only
-xd | --exclude-directory
-xf | --exclude-file
-xl | --exclude-link
-fl | --follow-link
-nfl | --no-follow-link
-cd | --count-directory
-cf | --count-file
-cl | --count-link
-s | --sort
-s/g | --sort/general
-s/n | --sort/natural
-s/none | --sort/none
-s/mt | --sort/mtime
-s/at | --sort/atime
-s/ct | --sort/ctime
-s/d | --sort/directory
-s/f | --sort/file
-s/l | --sort/link
-s/sz | --sort/size
-s/r | --sort/reverse
-y | --yes
-u | --undo
-up | --undo-path
-ups | --undo-path-show
-q | --quiet
-qq | --quiet-quiet
-f | --force
-h | --help
-v | --version
-sim | --simulation

#TERMINOLOGY
These are the technical terms that will be thrown around a bit for convenience.

##RESERVED INDEX
Index will be incremented even if  any file is skipped renaming in order to reserve the index for that skipped file. These are constructed by appending the `r` character with the index identifier, e.g */ir/*, */-idr/* etc..


##REVERSE INDEX
Decrementing index. These are constructed by inserting a *-* before the index identifier e.g */-i/*, */-id/* etc..


##NAME STRING
A string, that is parsed to create names for new files. It can be fixed name which then can be modified for different files at runtime. Name sting is parsed by some rules (Name String Rule). (must be wrapped around with filepath delimiter */*).


##NAME STRING RULE
A name string rule starts and ends with a */* character, These special forms are parsed and expanded to their special meaning. For example */i/* would expand to file index.

###INDEX RULES

1. */i/* : Index.
2. */ir/* : Reserved index.
3. */id/* : Directory index (index inside a directory).
4. */idr/* : Reserved directory index
5. */-i/* : Inverse index.
6. */-ir/* : Inverse reserved index.

In general, *-i* in the above name string rules will mean inverse index conforming to their meaning.

### COUNTERS

1. */dc/* : Directory count
2. */l/* : Line number from *Name String File*.

Base conversion, scientific conversion and Latin conversions are applicable on these counters. See <a href="#extended-index-rules">EXTENDED INDEX RULES</a>.

#### EXTENDED INDEX RULES

**Base conversion:**

```bash
/<rule>-b<base>/
```
For example, */i-b8/* will convert the index to octal. \<base\> can be 2 to 36.


**Scientific conversion:**

```bash
/<rule>-s/
```
For example, */i-s/* will convert the index to scientific form (n.fE+-p)

**Latin conversion:**

```bash
/<rule>-l/
```
For example, */i-l/* will convert the index to Latin form.


**Examples:**

```bash
#indexing:
rnm -ns '/fn/ /i/' ./*
#indexing by directory:
rnm -ns '/fn/ /id/' ./*
#indexing with binary number
rnm -ns '/fn/ /id-b2/' ./*
#indexing with scientific number
rnm -ns '/fn/ /id-s/' ./*
#indexing with latin number
rnm -ns '/fn/ /id-l/' ./*
```

### FILENAME
1. */fn/* : Full name of the file.
2. */n/*  : File name without extension.
3. */e/*  : File extension.
2. */rn/* : Replaced Name, generated by replace strings.
3. */pd/* : Parent directory  name of the current file or directory. 
4. */wd/* : Current working directory name.

#### EXTENDED PD RULES
Its general format is

```bash
/pd<digits>-<digits>-delimiter/
```

It specifies a bidirectional range of parent directories.

1. */pd0/* is the immediate parent directory name, *pd1* is the directory before *pd0* and so forth.
2. */pd0-2-+/* will expand by concatenating pd0 to pd2 and with the *delimiter* in-between (e.g *dir0+dir1+dir2*).
3. */pd2-0-+/* will do the same as above but in reverse order (*dir2+dir1+dir0*).
4. In place of *\<digits\>* you can supply *e* which generally means the 'end' i.e the deepest level available.
5. In place of *\<digits\>* you can also supply *w* which means the level of working directory.
6. Any unavailable level of directory will be ignored and be replaced with empty string.

**Examples:**

```bash
#Appending parent directory names
#with space in-between
rnm -ns '/pd0-e- /' ./*
rnm -ns '/pdw-0- /' ./*
```

###INFO-NAME STRING RULE
This name string rule provides basic information about a file, directory or link. The general format of this rule is:

```bash
/info-prop-op/
```
where *info-* is the identifier for this rule, *prop* is the property name and *op* is an optional entry which is used for additional formatting.

**File time:**

*mtime*
: File modification time. **op**: Time format (default *%d-%m-%Y*)

*mtime,GMT*
: File modification time in GMT. **op**: Time format (default *%d-%m-%Y*)

*atime*
: File access time. **op**: Time format (default *%d-%m-%Y*)

*atime,GMT*
: File access time in GMT. **op**: Time format (default *%d-%m-%Y*)

*ctime*
: File status change time. **op**: Time format (default *%d-%m-%Y*)

*ctime,GMT*
: File status change time in GMT. **op**: Time format (default *%d-%m-%Y*)

**Time Formats:**

Time format string is used to specify an arbitrary date-time format. For example, *%d-%m%-Y* would produce something like *22-01-2017*. This format string is exactly the same as the format string taken by `strftime` (3). Some frequently used character sequences:

* %a, %A: week day name short and full respectively
* %b, %B: month name short and full
* %d: month day number
* %H: Hour in 24hr format
* %I: Hour in 12hr format
* %j: Day of the year (001-366)
* %m: month number
* %M: Minute
* %p: AM/PM
* %P: am/pm (lowercase)
* %S: second in a minute
* %t: tab character
* %u: week day number (1-7)
* %%: A literal % character.

You can find a lot more of these character sequences described in details [here](http://man7.org/linux/man-pages/man3/strftime.3.html#DESCRIPTION).

**Example:**

```bash
rnm -ns '/fn/ - /info-mtime-%d-%m-%Y %I:%M:%S %p/' ./*
#The above appends modification time to the file names.
```

**File size:**

*size*
: Size of file. **op**: Unit (default: chosen according to size)

*blksize*
: Size of system I/O block. **op**: Unit (default: chosen according to size)

*blocks*
: Number of 512B blocks. **op**: N/A

**Other info:**

*perm*
: File permission. **op**: Either *ls* or *oct*.

*uid*
: Owner ID. **op**: N/A

*gid*
: Group ID. **op**: N/A

*dev*
: Device ID. **op**: N/A

*inode*
: Inode number. **op**: N/A

*mode*
: File mode. **op**: N/A

*nlink*
: Number of hard links. **op**: N/A

**Examples:**

```bash
#Appending different file info to their names
rnm -ns '/fn/ size: /info-size/' ./*
rnm -ns '/fn/ size: /info-size-1024/' ./* #size in KB
rnm -ns '/fn/ octperm: /info-perm-oct/ lsperm: /info-perm-ls/' ./*
rnm -ns '/fn/ uid: /info-uid/' ./*
```

##NAME STRING FILE
A file which contains a list of name string (one per line). Empty lines will be ignored but line number will be counted.

Each name string taken from this file is applied to each file, thus if there's 100 name strings in this file, their will be 100 rename only. All these name strings are parsed the same way as regular name strings given with *-ns* option with an additional rule */l/* (line number).

A null terminated name string file is that one where name strings (i.e filenames) are terminated with null character instead of newline (*\\n*). These are generally binary files and can be generated with other tools.


##SEARCH STRING
A string that is used to search for files with matching filenames against the search string. By default it is a regex if *-ssf* option is not used. It is generally in the form 

```bash
/regex/modifier
```
where *regex* is the regex to search for and available modifiers are *i* (case insensitive), *f* (file), *d* (directory), *l* (link), *!* (inverse search). If no  modifier is used, the regex format can be reduced to */regex/* or simply *regex*. 

Terminate search strings (*/regex/* format only) with *;* to provide multiple search strings, e.g *'/s1/i;/s2/;/s3/'*. This applies to fixed search strings as well. 

Also you can provide multiple search strings with repeated *-ss* and/or *-ssf* options and files with repeated *-ss/f* and/or *-ssf/f* options. These options can be mixed with each other too.

**Examples:**

```bash
#rename only mp3 and mp4 files
rnm -ns '/fn/ /i/' -ss '/.*\.mp(3|4)$/' ./*
#rename everything excluding mp3 and mp4 files
rnm -ns '/fn/ /i/' -ss '/.*\.mp(3|4)$/!' ./*
#rename file with .video in their names
# (fixed string search)
rnm -ns '/fn/ /i/' -ssf '.video' ./*
```


##INDEX FIELD LENGTH
An integer value defining the field length of index. By default empty field will be filled with 0's. For example, if the value is *3*, then indexes will be *001*, *002*, *003*, etc.. Different filler (other than 0) can be provided with *-iff* option.

##REPLACED NAME
The name can be modified at runtime using replace string. Replace string will be parsed to create a new *Name String* rule: */rn/* which can be used in *Name String*. If name string is not passed as argument, the new name of the file will be */rn/*. *Replaced Name* is always generated from the old filename (even when name string file is given).


##REPLACE STRING
*Replace String* is of the form: 

```bash
/regex/replace/modifier
```
where *regex* is the regex to search for and *replace* is the string to replace with. *Name String* rules are available in *regex* and *replace* part in Replace String.

Special character sequences for *replace* part:

1. *&* will be taken as the entire match found by the *regex*.
2. *\\1*, *\\2* etc.. are the captured groups. If you want to isolate a captured group, wrap it around with *{}*. For example, if you want to put a digit (2) after captured group *\\1*, you can't use it like *\\12*. *\\12* will mean *12th* captured group not *\\1* appended with a digit (1). In this case isolate the captured group with *{}* i.e *\\{1}2*.
3. Named captured group can be accessed as *\\{name}*.
4.  *\\c* will convert the matched string to lowercase, and *\\C* will convert it to uppercase. No other character is allowed in replace part if this is used. You can still concatenate different replace strings with *;* (semicolon).
5. to insert a *&* literally, use *\\&* and for *\\* use *\\\\*.
6. Modifiers are *i* (case insensitive), *g* (global), *f* (file), *d* (directory), *l* (link).

*Replace String* is always performed on old file name (even when name string file is given).

You can provide multiple replace strings with repeated *-rs* option and multiple file with repeated *-rs/f* options. These options can be mixed with each other too.

Multiple replace strings can also be given by terminating each individual replace strings with *;* (semicolon):

```bash
rnm -rs '/.*_/\C/gi;/-/ /g;/#/@/g' ./*
```

**Examples:**

```bash
#convert to uppercase
rnm -rs '/.*/\C/' ./*
#convert to lowercase
rnm -rs '/.*/\c/' ./*
#replace all @ with - in the names
rnm -rs '/@/-/g'
#Treat files and directories differently
rnm -rs '/@/-/gf;/#/@/gd;'
#the above will replace all @ in file (not directory)
#names to - and all # in directory names to @
```

##INDEX FLAGS

* *uppercase* : means uppercase.
* *showpoint* : show point regardless if it's an integer or floating point value.
* *showbase* : means show base (Hex or Oct).
* *showpos* : show + sign for positive numbers.
* *right* : adjust right.
* *left* : adjust left.
* *internal* : adjust internal.
* *precision* : set precision. Used as `precision=value`
* *length* : sets length. Used as `length=value`
* *filler* : sets filler. Used as `filler=value` 

##MODIFIERS

1. *i*: case insensitive match or replace.
2. *g*: global replace
3. *f*: True if it's a file, otherwise false.
4. *d*: True if it's a directory, otherwise false.
5. *l*: True if it's a link, otherwise false.
6. *!*: inverse search.

The *f*, *d* and *l* modifiers are ORed when combined.

###EXAMPLES:

```bash
rnm file -ns new_file
rnm file -rs "/f/F/"
#-do forces Directory only mode
rnm folder -ns "New Folder" -do
rnm ./New*/* -ns /i/.ext
#This will go inside the New Folder directory
rnm "./New Folder" -ns /id//dc/.ext -dp 1 -fo
# -dp -1 (infinite depth)
rnm "./New Folder" -ns /id//dc/.ext -dp -1 -fo
rnm ./New* -ns /id/.ext -ss "regex"
rnm -ns/f filepath -ns /n//id/.ext
rnm -ns/f filepath
```

**Only invalid characters for a file or directory name is the path delimiter (/) and the null character (*\\0*).**

#THINGS TO CARE

1. All options should always be separated by space. For Example: *-vy* won't mean two option *-v* and *-y*, rather it will mean a single option *-vy*.
2. Any dangling argument with *-* at beginning will be treated as an option and produce error if invalid.
3. If filename/path argument may include *-* at the beginning, use *--* to make all the arguments after this option as paths/names (not options).
6. If you run `rnm . -ns something` or `rnm ./ -ns something`, your current directory will be renamed (be careful).
6. This is a dangerous tool like `rm`, so use with care. If you make a mistake and do some unwanted rename, run `rnm -u` to undo (before running any more **rnm** command).
7. Pass all regex like strings within quotes even if they don't contain any white space.
8. To pass a filename that resembles an option, use `./`, i.e `./-ns` to pass a file named `-ns` in the current directory. Or you can use the `--` option to make it a non-option argument; in that case make sure to pass all "Option" arguments before `--`, because everything after `--` will be taken as file path/s.
9. A common mistake is to pass files like this: `rnm -ns 'something' *`, it will work as long as no file names contain *-* at the beginning, a safer approach is `rnm -ns 'something' ./*`

#LIMITS

**Maximum length of file name :** FILENAME_MAX

**Default  latin fallback :** 55555. After this value Latin conversion will fall back to decimal. You can override this with *-if* option by setting */latin-fallback=6666/* (or some other value). Big Latin number may produce much larger name which in turn may give you error due to limit on length of file names.

#LOG DIR
~/.cache/neurobin/rnm

#EXIT STATUS

0  On success

1 On failure

2 On interrupt

#BUG REPORT

<https://github.com/neurobin/rnm/issues>

