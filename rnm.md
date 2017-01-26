% rnm(1) rnm user manual
% Md Jahidul Hamid <https://github.com/neurobin>
% January 26, 2017

# NAME
rnm - Bulk rename utility

# SYNOPSIS
**rnm** [*options*] *file/paths*

# DESCRIPTION
Renames files/directories in bulk. Naming scheme (*Name String*) can be applied or rgex replace can be performed to modify names dynamically. It uses PCRE2 (revised version of PCRE) regex to provide search (and replace) functionality. It provides an undo functionality too to move back unwanted rename operations. You can also run a simulation instead of actual rename to view the potential outcome as program output on terminal with the *-sim* option.


#USAGE

```sh
rnm -ns new_name [other-opts] file/path
rnm -rs "/search/replace/mod" [other-opts] file/path
rnm -ns/f namestring/file/path file/path
```

Options are **not** sequential, their position in the argument list holds no significance. For example, **rnm filepath -ns name** is the same as **rnm -ns name filepath**. Though passing the file path at the end of the argument list is considered to be safe.

Options are case insensitive, i.e *-ssF* and *-ssf* are the same.


# OPTIONS

-h, --help
: Show help menu.

-ns *string*
: Name string. This is generally the new name for the file. Any part in this name string wrapped around with the path delimiter (/) is parsed as a name string rule and the new name is formed accordingly. See <a href="#name-string-rule">NAME STTRING RULE</a> for details.

-ns/f *file-path*
: Name string file. File containing name string (one per line).

-ns/fn *file-path*
: Name String file. This takes a null terminated *Name String* file, i.e a file where filenames are terminated by null character (*\\0*) instead of new line (*\\n*).

-ss *search-regex*
: Search string. String that will be used to search for files with matching names. This is generally a regex  if not passed with *-ssf* option. See <a href="search-string">SEARCH STRING</a> for details.

-ss/f *file-path*
: Search string file. A file containing search string per line. 

-ssf *search-term*
: Fixed search string (not treated as regex). See <a href="search-string">SEARCH STRING</a> for details.

-ssf/f *file-path*
: Search string file. Contains fixed search string per line.

-rs *replace-string*
: A string in the form */search_regex/replace_part/modifier*. See <a href="#replace-string">REPLACE STRING</a> for details.

-rs/f *file-path*
: Replace string file. A file containing replace string per line.

-i *number*, -si *number*
: Starting index.

-ei *number*
: End index i.e index to stop renaming from. It is only for files inside a directory that is being recursively taken due to a depth value greater than 0, i.e it works on directory index. Note that directory index */id/* will renew in each directory i.e in each directory rename will be performed from start index to end index.

-inc *positive-integer*
: Increment value (floating point decimal). The amount, index will be incremented or decremented in each iteration. Decremented index is available through name string rule */-i/*, */-id/* etc..

-linc *positive-integer*
: The amount line count will be incremented or decremented in each iteration. This is always a positive integer.

-if *INDEX-FLAGS*
: This sets Index flags. This is a '/' separated list of flags that will be used to render the index within it's text field. The general format is *'/keyword1/keyword2/...'*. Valued flags are set with *keyword=value* format. Ex: *'/uppercase/filler=\*/'*. See <a href="#index-flags">INDEX FLAGS</a> for details.

-ifl *positive-integer*
: Index field length. Non occupied field will be filled with index field fillers (set with *-iff*). *iff* is set to the character *0* by default.

-iff *character*
: Not occupied field in index will be filled with a character which is set by this option.

-ifp *positive-integer*
: Index is a floating point decimal value. This sets the precision.

-l *positive-integer*, -sl *positive-integer*
: Start Line number in name string file. If it's smaller the end line number, the line number is incremented in each iteration and if it's greater than end line number, line number will be decremented in each iteration till it reaches end line number

-lv *positive-integer*, -slv *positive-integer*
: Same as *-l* or *-sl*, except it reverses their meaning.
 
-el *positive-integer*
: End line number. Line number to stop renaming from. 
-elv *positive-integer*
: Same as *-el*, except  it reverses their meaning.

-dp *integer*
: Depth of folder. -1(any negative number) means unlimited depth i.e all files and subdirectories will be included. Other values may be 0 1 2 3 etc... Default depth is *0*, i.e directory contents will be ignored.

-fo
: File only mode. Only files are renamed (not directory). Goes to subdirectory/s if depth (*-dp*) is set to *1* or greater.

-do
: Apply rename on directory only.

-ed
: Apply rename on files only, exclude any and all directory and their contents. This option is equivalent to file only mode with a zero (*0*) depth value.

-cd
: Count directory in reserved index, regardless of other options. Reserves indexes for directories even if it is File Only mode.

-cf
: Count file in reserved index, regardless of other options. Reserves indexes for files even if it is Directory Only mode.

-s
: Sort files in natural order (Human perceivable order). This option can be modified to use other sorting methods. For example: *-s/g* or *--sort/g* will sort the file in general (alphabetical) order. Other options are *-s/n* (Natural sort), *-s/none* (No sort).

-y
: Confirm Yes to all.

-fl
: Follow symbolic link.

-u
: Undo renaming

 -f, --force
: Force rename. Enables renaming some restricted files except */*.

-v
: Show version info.

-q
: Quiet operation (this increases the speed).

\--
: If this option is passed, anything and everything after it will be taken as file path. Put all options before passing this option.

-sim
: This runs a simulation of rename instead of actual rename operation.

#TERMINOLOGY
These are the technical terms that will be thrown around a bit for convenience.

##RESERVED INDEX
Index will be incremented even if  any file is skipped renaming in order to reserve the index for that skipped file. These are constructed by appending the `r` character with the index identifier, e.g */ir/*, */-idr/* etc..


##REVERSE INDEX
Decrementing index. These are constructed by inserting a *-* before the index identifier e.g */-i/*, */-id/* etc..


##NAME STRING
A string, that is parsed to create names for new files. It can be fixed name which then can be modified for different files at runtime. Name sting is parsed by some rules (Name String Rule). (must be wrapped around with filepath delimiter */*).


##NAME STRING RULE
A name string rule starts and ends with a */* character, These special forms are parsed and expanded to their meaning. For example */i/* would expand to file index.

###INDEX RULES

1. */i/* : Index.
2. */ir/* : Reserved index.
3. */id/* : Directory index (index inside a directory).
4. */idr/* : Reserved directory index
5. */-i/* : Inverse index.
6. */-ir/* : Inverse reserved index.

In general, *-i* in the above name string rules will mean inverse index conforming to their meaning.


#### EXTENDED INDEX RULES

1. **Base conversion: Format**: */\<rule\>-b\<base\>/*, e.g */i-b8/* will convert the index to octal. \<base\> can be 2 to 36.
2. **Scientific conversion**: Format: */\<rule\>-s/*, e.g */i-s/* will convert the index to scientific form (n.fE+-p)
3. **Latin conversion**: Format: */\<rule\>-l/*, e.g */i-l/* will convert the index to latin form.


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

### COUNTERS

1. */dc/* : directory count
2. */l/* : line number from *Name String File*.
3. */la/* : actual line number from *Name String File*.

### FILENAME
1. */fn/* : Full name of the files. If used with *-ns/f* option, full name will be taken from the *Name String File*.
2. */n/* : Name without extension. If used with *-ns/f* option, name will be taken from the *Name String File*.
3. */e/* : File extension.
2. */rn/* : Replaced Name, generated by replace strings.
3. */pd/* : Parent directory  name of the current file or directory. 
4. */wd/* : Current working directory name.

#### EXTENDED PD RULES
Its general format is */pd\<digits\>-\<digits\>-delimiter/*. It specifies a bidirectional range of parent directories.

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
This name string rule provides basic information about a file, directory or link. The general format of this rule is: */info-prop-op/*, where *info-* is the identifier for this rule, *prop* is the property name and *op* is an optional entry which is used for additional formatting.

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
A file which contains a list of name string (one per line). Empty lines will be ignored and line number won't be counted. Actual line number (which counts the empty lines too) is available through the name string rule : */la/*.


##SEARCH STRING
A string that is used to search for files with matching filenames against the search string. By default it is a regex if *-ssf* option is not used. It is generally in the form */regex/modifier* , where *regex* is the regex to search for and available modifiers are *i* (case insensitive), *f* (file), *d* (directory), *l* (link), *!* (inverse search). If no  modifier is used, the regex format can be reduced to */regex/* or simply *regex*. 

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
An integer value defining the field length of index. By default empty field will be filled with 0's. For example, if the value is *3*, then index will be *001*, *002*, *003*, etc.. Different filler (other than 0) can be provided with the *-iff* option.

##REPLACED NAME
The name can be modified at runtime using replace string. Replace string will be parsed to create a new *Name String* rule: */rn/* which can be used in *Name String*. If name string is not passed as argument, the new name of the file will be */rn/*. *Replaced Name* is always generated from the old filename (even when name string file is given).


##REPLACE STRING
*Replace String* is of the form:  */search_part/replace_part/modifier* where search\_part is the regex to search for and replace\_part is the string to replace with. *Name String* rules are available in search\_part and replace\_part in Replace String.

There are several special cases for replace\_part:

1. *&* will be taken as the entire match found by the regex (search\_part).
2. *\\1*, *\\2* etc.. is the captured groups. If you want to isolate a captured group, wrap it around with *{}*. For example, if you want to put a digit (2) after captured group *\\1*, you can't use it like *\\12*. *\\12* will mean *12th* captured group not *\1* appended with a digit (1). In this case isolate the captured group with *{}* i.e *\\{1}2*.
3.  *\\c* will convert the matched string to lowercase, and *\\C* will convert it to uppercase. No other character is allowed in replace part if this is used. You can still concatenate different replace strings with *;*.
4. to insert a *&* literally, use *\\&* and for *\\* use *\\\\*.
5. Modifiers are *i* (case insensitive), *g* (global), *f* (file), *d* (directory), *l* (link).
6. *Replace String* is always performed on old file name (even when name string file is given).
7. You can provide multiple replace strings with repeated *-rs* option and multiple file with repeated *-rs/f* options. These options can be mixed with each other too.

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
* *precision* : set precision. Used like precision=value
* *length* : sets length. Used like length=value
* *filler* : sets filler. Used like filler=value 

##MODIFIERS

1. *i*: case insensitive match or replace.
2. *g*: global replace
3. *f*: True if it's a file, otherwise false.
4. *d*: True if it's a directory, otherwise false.
5. *l*: True if it's a link, otherwise false.
6. *!*: inverse search.

The *f*, *d* and *l* modifiers are ORed when combined.

###EXAMPLE:

```sh
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
2. Any argument with *-* at beginning will be treated as an option and produce error if invalid.
3. If filename/path argument may include *-* at the beginning, use *--* to make all the arguments after this option as paths/names (not options).
6. If you run `rnm . -ns something` or `rnm ./ -ns something`, your current directory will be renamed (be careful).
6. This is a dangerous tool like `rm`, so use with care. If you make a mistake and do some unwanted rename, run `rnm -u` to undo (before running any more **rnm** command).
7. Pass all regex like strings within quotes even if they don't contain any white space.
8. To pass a filename that resembles an option, use `./`, i.e `./-ns` to pass a file named `-ns` in the current directory. Or you can use the `--` option to make it a non-option argument; in that case make sure to pass all "Option" arguments before `--`, because everything after `--` will be taken as file path/s.
9. A common mistake is to pass files like this: `rnm -ns 'something' *`, it will work as long as no file names contain *-* at the beginning, a safer approach is `rnm -ns 'something' ./*`

