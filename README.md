[![Build Status](https://travis-ci.org/neurobin/rnm.svg?branch=release)](https://travis-ci.org/neurobin/rnm)

**The current version is an alpha release, beta testers are welcome.**

<div id="description"></div>
Bulk Rename Utility written in `C++`. Files and directories can be passed as command line arguments to rename them in bulk according to some naming scheme (*Name String*). If any file or directory path is not passed as command line argument, it will wait for user to type the path i.e it will take the path from standard input `(>=version 3.0.1)`. It uses PCRE2 (revised version of PCRE) regex to provide search (and replace) functionality. It provides an undo functionality too to move back an unwanted rename operation. You can also run a simulation instead of actual rename to view the potential outcome as program output on terminal with the `-sim` option.


<div id="features"></div>
#Features:

1. Renames files or directories in bulk.
2. Undo functionality (`rnm -u`).
3. Names can be modified by performing regex replace. It enables generating new names by deleting/appending/modifying part of the old filename. It also enables easy conversion from lowercase to uppercase and vice-versa.
4. Lowercase-Uppercase conversion is selective i.e you can apply case conversion in part of the name at a targeted position.
5. Names can be modified with certain predefined rules. For example, you can insert parent directory name or working directory name or index etc... in filename in arbitrary positions.
6. Latin indexing of files. Index can also be converted to different bases (binary, hex, oct etc...).
6. Names can be taken from a file, and these names can also be modified by applying *Name String* rules on them.
7. Search functionality. It uses PCRE2 regex. Fixed string search is also possible.
8. Null terminated file support for file names (Null termination is a good way to store filenames in a file).
9. Sort functionality. Available sorting methods are: Natural sort and general alphabetical sort.
10. Multiple search criteria and replacement methods are possible.
11. Search keywords and replacement regex can also be supplied form files (single or multiple).

#New features:

1. Unlimited precission for numbers i.e you can perform base conversion, scientific conversion etc.. on an arbitrarily big number. This functionality is provided by GMP BigNum library (GNU Multi-precesion library).
2. File information is available, e.g modification time, access time, status change time (time string can be formatted using format string e.g %d-%m-%Y), permission, owner id, group id, size, block, inode number, mode etc...
3. Inverse search is possible.
4. Each regex operation can be associated with either link, file or directory, for example, if a replace string is associated with only file, it will not work on directories or links.

#Features that were dropped:

* Multiple regex mode in favor of PCRE2 regex.
* All the previous regex modes; from now only PCRE2 regex will be used.


#Install:

###Install from source:

Make sure you have a C++ compiler equivalent to GCC-5.0 (g++-5.0) or later installed on your system.

####Download dependencies:
To install dependencies run the `prepare.sh` file:

```sh
cd rnm # i.e go inside the rnm project directory
sh prepare.sh
```

It will download three different projects:

1. [pcre2](http://pcre.org/)
2. [jpcre2](https::/docs.neurobin.org/jpcre2)
3. [gmplib](https://gmplib.org/)

####Install rnm:
After you have all the dependencies at hand, you can simply do:

```sh
./configure --enable-static --disable-shared --enable-cxx --enable-jit
#--enable-cxx is mandatory, --enable-jit will improve performance during patter matching
make
sudo make install-strip #binary size will be minimum comapared to `sudo make install`
```
Hold tight until it finishes compiling and building **rnm**.

###Install from pre-built binary:

####From launchpad PPA:
**(currently not available because this version is still in testing stage)**

Aside from the above generalized method, you can also install it in Ubuntu from PPA (`ppa:neurobin/ppa`).

```sh
sudo add-apt-repository -y ppa:neurobin/ppa
sudo apt-get update
sudo apt-get install rnm
```

If you are in Ubuntu 14.04 or 12.04 (trusty or precise), you will need to add ubuntu-toolchain repository to make `libstdc++6 (>=5.0.0)` available:

```sh
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo add-apt-repository -y ppa:neurobin/ppa
sudo apt-get update
sudo apt-get install rnm
```

####For others:
**(currently not available because this version is still in testing stage)**

You can download a pre-built binary (64 and/or 32 bit) from the [release page](https://github.com/neurobin/rnm/releases) if available.

<div id="un-install"></div>
#Uninstall:

1. For ubuntu uninstall from software manager.
2. If you used a pre-built binary, just delete the binary and the man page.
3. If you compiled from the source, do `./configure --enable-static --disable-shared --enable-cxx --enable-jit && make && sudo make uninstall`

<div id="usage"></div>
#Usage:

```sh
rnm directory/file/path -ns new_name [other options]
rnm directory/file/path -rs "/search regex/replace string/gi" [other_options]
rnm directory/file/path -ns/f namestring/file/path
```

One of the options of `-ns` or `-ns/f` or `-rs` is mandatory. Options are **not** sequential, their position in the argument list holds no significance. For example, `rnm filepath -ns name` is the same as `rnm -ns name filepath`. Though passing the *directory/file* path at the end of the argument list is considered to be safe.

Options are case insensitive, i.e `-ssF` and `-ssf` are the same.

<div id="options"></div>
##Options:

Options | Description
------- | -----------
`-h`, `--help`       | Show help menu.
`-i`,`-si`           | Starting index.
`-ei`                | End index i.e index to stop renaming from. It is only for files inside a directory that is being recursively taken due to a depth value greater than 0, i.e it works on directory index. Note that directory index `/id/` will renew in each directory i.e in each directory rename will be performed from start index to end index.
`-inc`               | Increment value (floating point decimal). The amount, index will be incremented or decremented in each iteration. Decremented index is available through name string rule `/-i/`, `/-id/` etc..
`-linc`              | The amount line count will be incremented or decremented in each iteration. This is always a positive integer.
`-if`                | This sets Index flags. This is a '/' separated list of flags that will be used to render the index within it's text field. The general format is `'/keyword1/keyword2/...'`. Valued flags are set with *keyword=value* format. Ex: `-if '/uppercase/showpoint/showpos/showbase/left/length=4/precision=3/filler=*/latin-fallback=687678/'`. See <a href="#keywords">keywords section</a> for details.
`-ifl`               | Index field length. Non occupied field will be filled with index field fillers (set with `-iff`). iff is set to the character `0` by default.
`-iff`               | Not occupied field in index will be filled with a character which is set by this option.
`-ifp`               | Index is a floating point decimal value. This sets the precision.
`-ns`                | Name string. This is generally the new name for the file. Any part in this name string wrapped around with the path delimiter (/) is parsed as a name string rule and the new name is formed accordingly. See <a href="#name-string">Name String Rule</a> for more details.
`-ns/f`              | Name string file. File containing name string (one per line). `-ns/f /hist/` i.e a value passed `/hist/` as Name string file, will try to take the file from history.
`-ns/fn`             | Name String file. This takes a null terminated *Name String* file, i.e filenames are terminated by null character (`\0`) instead of new line (`\n`).
`-l`, `-sl`          | Start Line number in name string file.
`-lv`, `-slv`        | Same as `-l` or `-sl`, except line number will be decremented in each iteration. 
`-el`                | End line number. Line number to stop renaming from. 
`-elv`               | Same as `-el`, except line number will be decremented in each iteration. 
`-ss`                | Search string. String that will be used to search for files with matching names. This is generally a regex  if not passed with `-ssf` option. 
`-ss/f`              | Search string file. A file containing search string per line. 
`-ssf`               | Fixed search string (not treated as regex). 
`-ssf/f`             | Search string file. Contains fixed search string per line. 
`-rs`                | Replace string. A string in the form `/search_string/replace_string/modifier`. See **Replace String** in <a href="#technical-terms">Terminology</a> for details. 
`-rs/f`              | Replace string file. A file containing replace string per line. 
`-dp`                | Depth of folder. -1(any negative number) means unlimited depth i.e all files and subdirectories will be included. Other values may be 0 1 2 3 etc... Default depth is `0`, i.e directory contents will be ignored.
`-fo`                | File only mode. Only files are renamed (not directory). Goes to subdirectory/s if depth (`-dp`) is set to `1` or greater. depth is set to 0. 
`-do`                | Apply rename on directory only. 
`-ed`                | Apply rename on files only, exclude any and all directory and their contents. This option is equivalent to file only mode with a zero (`0`) depth value. 
`-cd`                | Count directory in reserved index, regardless of other options. Reserves indexes for directories even if it is File Only mode. 
`-cf`                | Count file in reserved index, regardless of other options. Reserves indexes for files even if it is Directory Only mode. 
`-s`                 | Sort files in natural order (Human perceivable order). This option can be modified to use other sorting methods. For example: `-s/g` or `--sort/g` will sort the file in general (alphabetical) order. Other options are `-s/n` (Natural sort), `-s/none` (No sort).
`-y`                 | Confirm Yes to all.
`-fl`                | Follow symbolic link.
`-u`                 | Undo renaming
`--force`, `-f`      | Force rename. Enables renaming some restricted files except `/`.
`-v`                 | Version info.
`-q`                 | Quiet operation.
`--`                 | If this option is passed, anything and everything after it will be taken as file path. Put all options before passing this option.
`-shop`              | This shows an info about the various options passed as arguments and how they are being treated behind the scene.
`-sim`               | This runs a simulation of rename instead of actual rename operation, prints all kinds of available outputs. `-q` option won't have any effect if this option is passed.

<div id="technical-terms"></div>
#Technical Terms:
These are the technical terms that will be thrown around a bit for convenience.

##Reserved Index
Index will be incremented even if  any file is skipped renaming in order to reserve the index for that skipped file. These are constructed by appending the `r` character with the index identifier, e.g `/ir/`, `/-idr/` etc..


##Reverse Index
Decrementing index. These are constructed by inserting a `-` before the index indentifier e.g `/-i/`, `/-id/` etc..


##Name String
A string, that is parsed to create names for new files. It can be fixed name which then can be modified for different files at runtime. Name sting is parsed by some rules (Name String Rule). (must be wrapped around with filepath delimiter `/`).


##Name String Rule
A name string rule starts and end with a `/` character, These special forms are parsed and expanded to their meaning. For example `/i/` would expand to file index.

<div id="name-string-rules"></div>
###General Name String Rules:

1. `/i/` will be expanded to index.
2. `/ir/` will be expanded to reserved index.
3. `/id/` will be expanded to directory index (index inside a directory).
4. `/idr/` will be expanded to reserved directory index
5. `/-i/` will be expanded to inverse index.
6. `/-ir/` will be expanded to inverse reserved index. In general, `-i` in the above replacement rules (applies to indexes excluding line indexes) will mean inverse index conforming to their meaning.
7. `/dc/` will be expanded to directory count
8. `/l/` will be expanded to line number from *Name String File*.
9. `/la/` will be expanded to actual line number from *Name String File*.
10. `/n/` will be expanded to filename without extension. If used with `-ns/f` option, the filename will be the name taken from the *Name String File*.
11. `/fn/` will be expanded to full name of the files. If used with `-ns/f` option, full name will be the name taken from the *Name String File*.
12. `/rn/` will be expanded to Replaced Name.
13. `/pd/` will be expanded to parent directory  name of the current file or directory. 
14. `/wd/` will be expanded to the current working directory name.

<div id="extended-name-string-rule"></div>
###Extended Name String Rules:
This is an extension of the general purpose name string rules mentioned above.

1. Base conversion: `/i-b2/` converts the index to base 2 i.e binary and `/i-b32/` will convert to base 32. This applies to all numbered name string rules. 
2. Latin conversion: `/i-l/` converts the index to Latin number. This applies to all numbered name string rules. 
3. Scientific conversion: `/i-s/` converts the index to scientific form. This applies to all numbered name string rules. 
4. Extended `/pd/` rules:
  * `pd0` is the immediate parent directory, `pd1` is the directory before `pd0` and so forth.
  * A range can also be supplied to combine all the directories in the level implied by the range. The general format of this rule is `/pd<digits>-<digits>-delimiter/`
  * For example: `/pd0-4--/` will combine all directories from level 0 to 4 inserting the delimiter (`-` in this case) between them and the rule will become something like `dir0-dir1-dir2-dir3-dir4`.
  * In place of `<digits>` you can also supply `e` which generally means the 'end' i.e the deepest level available.
  * In place of `<digits>` you can also supply `w` which means the level of working directory.
  * Any unavailable level of directory will be ignored and be replaced with empty string.
  * The range is bidirectional e.g `/pd4-0--/` will do the same in reverse order.

<div id="info-name-string-rule"></div>
###Info-Name String Rules
This name string rule provides basic information about a file, directory or link. The general format of this rule is: `/info-prop-op/`, where `info-` is the identifier for this rule, `prop` is the property name and `op` is an optional entry which is used for additional formatting.

Property | Details | Formatting operation
-------- | ------- | -------------------
`mtime` | File modification time | A format string (see <a href="#time-format-string">Time Format String</a> below)
`mtime,GMT` |  File modification time in GMT | A format string (see <a href="#time-format-string">Time Format String</a> below)
`atime` | File access time | A format string (see <a href="#time-format-string">Time Format String</a> below)
`atime,GMT` | File access time in GMT | A format string (see <a href="#time-format-string">Time Format String</a> below)
`ctime` | File status change time | A format string (see <a href="#time-format-string">Time Format String</a> below)
`ctime,GMT` | File status change time in GMT | A format string (see <a href="#time-format-string">Time Format String</a> below)
`size` | Size of file  | Unit, if not given a suitable unit will be chosen.
`blksize` | Size of system I/O block | Unit
`blocks` | Number of 512B blocks | N/A
`perm` | File permission | Either `ls` or `oct`
`uid` | Owner ID | N/A
`gid` | Group ID | N/A
`dev` | Device ID | N/A
`inode`| inode number | N/A
`mode` | File mode | N/A
`nlink` | Number of hard links | N/A

<div id="time-format-string"></div>
####Time Format String
This format string is used to specify an arbitrary date-time format. For example, `%d-%m%-Y` would produce something like `22-01-2017`. This format string is exactly the same as the format string taken by strftime(3) function. Some frequently used character sequences:

* `%a, %A`: week day name short and full respectively
* `%b, %B`: month name short and full
* `%d`: month day number
* `%H`: Hour in 24hr format
* `%I`: Hour in 12hr format
* `%j`: Day of the year (001-366)
* `%m`: month number
* `%M`: Minute
* `%p`: AM/PM
* `%P`: am/pm (lowercase)
* `%S`: second in a minute
* `%t`: tab character
* `%u`: week day number (1-7)
* `%%`: A literal `%` character.

You can find a lot more of these character sequences described in details [here](http://man7.org/linux/man-pages/man3/strftime.3.html#DESCRIPTION).

##Name String File
A file which contains a list of name string (one per line). Empty lines will be ignored and line number won't be counted. Actual line number (which counts the empty lines too) is available through name string rule : `/la/`.


##Search String
A string that is used to search for files with matching filenames against the search string. By default it is a regex if `-ssF` option is not used. It is generally in the form `/regex/modifier` , where <u>regex</u> is the regex to search for and available modifiers are <u>i (case insensitive), f (file), d (directory), l (link), ! (inverse search)</u>. If no  modifier is used, the regex format can be reduced to `/regex/` or simply `regex`. 

Terminate search strings (`/regex/` format only) with `;` to provide multiple search strings, e.g `'/s1/i;/s2/;/s3/'`. This applies to fixed search strings as well. 

Also you can provide multiple search strings with repeated `-ss` and/or `-ssf` options and files with repeated `-ss/f` and/or `-ssf/f` options. These options can be mixed with each other too.


##Index Field Length
An integer value defining the field length of index. By default empty field will be filled with 0's. For example, if the value is `3`, then index will be `001`, `002`, `003`, etc.. Different filler (other than 0) can be provided with the `-iff` option.


##Replaced Name
The name can be modified at runtime using replace string. Replace string will be parsed to create a new *Name String* rule: `/rn/` which can be used in *Name String*. If name string is not passed as argument, the new name of the file will be `/rn/`. *Replaced Name* is always generated from the old filename.


##Replace String
*Replace String* is of the form:  `/search_part/replace_part/modifier` where search\_part is the regex to search for and replace\_part is the string to replace with. Name String rules are available in search\_part and replace\_part in Replace String.

There are several special cases for replace\_part:

1. `&` will be taken as the entire match found by the regex (search_part).
2. `\1`, `\2` etc.. is the captured groups. If you want to isolate a captured groups, wrap it around with `{}`. For example, if you want to put a digit (2) after captured group `\1`, you can't use it like `\12`. `\12` will mean `12th` captured group not `\1` appended with a digit (1). In this case isolate the captured group with `{}` i.e `\{1}`.
3.  `\c` will convert the matched string to lowercase, and `\C` will convert it to uppercase. No other character is allowed in replace part if this is used. You can still concatenate different replace strings with `;`.
4. to insert a `&` literally, use `\&` and for `\` use `\\`.
5. Modifiers are i (case insensitive), g (global), f (file), d (directory), l(link).
6. *Replace String* is always performed on old file name.
7. You can provide multiple replace strings with repeated `-rs` option and multiple file with repeated `-rs/f` options. These options can be mixed with each other too.

<div id="keywords"></div>
##Keywords at a glance

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

##Modifiers at a glance

* *i*: case insensitive match or replace.
* *g*: global replace
* *f*: perform search or replace if it's a file
* *d*: perform search or replace if it's a directory
* *l*: perform search or replace if it's a link
* *!*: inverse search.

<div id="examples"></div>
###Example:

```sh
rnm file -ns new_file
rnm file -rs "/f/F/"                             ( will replace f with F in the name, i.e new name will be: File)
rnm folder -ns "New Folder" -do                  (-do forces Directory only mode)
rnm ./New*/* -ns /i/.ext                         (globbing is allowed)
rnm ./New*/* -ns /i/.ext -ed                     (-ed forces file only mode)
rnm "./New Folder" -ns /id//dc/.ext -dp 1 -fo    (This will go inside the New Folder directory)
rnm ./New* -ns /id/.ext -ss "regex"
rnm -ns/f filepath -ns /n//id/.ext
rnm -ns/f filepath
etc...
```
**Only invalid characters for a file or directory name is the path delimiter and the null character (`\0`).**

<div id="things-to-care"></div>
#Things to care:

1. All options should always be separated by space. For Example: `-vy` won't mean two option `-v` and `-y`, rather it will mean a single option `-vy`.
2. Any non option argument will be treated as file or directory path. For example in <pre><code>rnm file1 file2 -dummy -ns fd</code></pre>`file1`, `file2` and `-dummy` will be taken as file paths.
3. If filename/path argument may include `-` at the beginning, use `--` to make all the arguments after this option as paths/names (not options).
4. Be wary of filename globbing. Command like `rnm ./*` will take all files and directories as arguments and thus the files and directories will be subject to rename operation. If you don't want to rename directories, use file only mode (`-fo`). If you want to go inside directories, use depth (`-dp`) greater than `0` with file only mode.
6. If you run `rnm . -ns something` or `rnm ./ -ns something`, your current directory will be renamed (be careful).
6. This is a dangerous tool like `rm`, so use with care. If you make a mistake and do some unwanted rename, run `rnm -u` to undo (before running any more `rnm` command).
7. Pass all regex like strings within quotes even if they don't contain any white space.
8. To pass a filename that resembles an option, use `./`, i.e `./-ns` to pass a file named `-ns` in the current directory. Or you can use the `--` option to make it a non-option argument; in that case make sure to pass all "Option" arguments before `--`, because everything after `--` will be taken as file path/s.


