<div id="description"></div>
Bulk Rename Utility written in `C++`. Files and directories can be passed as command line arguments to rename them in bulk according to some naming scheme (*Name String*). If any file or directory path is not passed as command line argument, it will wait for user to type the path i.e it will take the path from standard input `(>=version 3.0.1)`. It uses `C++` regex (<a href="https://www.cplusplus.com/reference/regex/ECMAScript/">ECMAScript (aka JavaScript) regex</a>) as the default regex to provide search (and replace) functionality, other regex modes are available through (`-re`) option. It provides an undo functionality too to move back an unwanted rename operation. You can also run a simulation instead of actual rename to view the potential outcome as program output on terminal with the `-sim` option.


<div id="features"></div>
#Features:

1. Renames files or directories in bulk.
2. Undo functionality (`rnm -u`).
3. Names can be modified by performing regex replace. It enables generating new names by deleting/appending/modifying part of the old filename. It also enables easy conversion from lowercase to uppercase and vice-versa.
4. Lowercase-Uppercase conversion is selective i.e you can apply case conversion in part of the name at a targeted position.
5. Names can be modified with certain predefined rules. For example, you can insert parent directory name or working directory name or index etc... in filename in arbitrary positions.
6. Latin indexing of files. Index can also be converted to different bases (binary, hex, oct etc...).
6. Names can be taken from a file, and these names can also be modified by applying *Name String* rules on them.
7. Search functionality. It uses the <a href="https://www.cplusplus.com/reference/regex/ECMAScript/">ECMAScript (aka JavaScript) regex</a> by default. Regex mode can be changed (POSIX basic, extended, egrep, grep, awk, ECMAScript). Fixed string search is also possible.
8. Null terminated file support for file names (Null termination is a good way to store filenames in a file).
9. Sort functionality. Available sorting methods are: Natural sort and general alphabetical sort.
10. Multiple search criteria and replacement methods are possible.
11. Search keywords and replacement regex can also be supplied form files (single or multiple).

<div id="dependency"></div>
#Dependency:
* `libstdc++6 (>=4.9.2)`

<div id="install"></div>
#Install:

###Unix (32 or 64 bit):

Make sure `libstdc++6 (>=4.9.2)` or `GCC (g++>=4.9.2)` is available in your system.

1. Give the <span class="quote">install</span> file execution permission (`chmod +x install`) and
2. Run it or just drag and drop it on terminal and hit <kbd>Enter</kbd> (requires root privilege).

The install script depends on <span class="quote">Bash Shell</span>. If you don't have `bash` installed, then just copy the suitable binary file (`rnm`) from *bin/x32* or *bin/x64* directory to */usr/bin* directory, and copy the <span class="quote">rnm.1</span> file to */usr/share/man/man1* directory.

###Ubuntu:

Aside from the above generalized method, you can also install it in Ubuntu from PPA (`ppa:neurobin/ppa`).

```sh
sudo add-apt-repository -y ppa:neurobin/ppa
sudo apt-get update
sudo apt-get install rnm
```

If you are in Ubuntu 14.04 or 12.04 (trusty or precise), you will need to add ubuntu-toolchain repository to make `libstdc++6 (>=4.9.2)` available:

```sh
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo add-apt-repository -y ppa:neurobin/ppa
sudo apt-get update
sudo apt-get install rnm
```
<div id="un-install"></div>
#Uninstall:

1. Give the <span class="quote">uninstall</span> file execution permission and
2. Run it or just drag and dropt it on terminal and hit <kbd>Enter</kbd> (requires root privilege).

<div id="usage"></div>
#Usage:

```sh
rnm directory/file/path -ns new_name [other options]
rnm directory/file/path -rs "/search regex/replace string/gi" [other_options]
rnm directory/file/path -nsf namestring/file/path
```

One of the options of `-ns` or `-nsf` or `-rs` is mandatory. Options are **not** sequential, their position in the argument list have no significance. For example, `rnm filepath -ns name` is the same as `rnm -ns name filepath`. Though passing the *Directory/File* path at the end of the argument list is considered to be safe and wise.

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
`-ns/f`              | Name string file. File containing name string (one per line). `-nsf /hist/` i.e a value passed `/hist/` as Name string file, will try to take the file from history.
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
`-re`                | regex mode. Available regex modes are POSIX compliant basic & extended regex, regex used by grep, awk, egrep and the default regex is ECMAScript regex. For example, to have a grep like regex, pass the option `-re grep`, to use POSIX compliant extended regex, pass `-re extended`.
`-rel`               | If this is passed as argument, regex will follow Locale. that is regex like [a-z] will have their meaning according to the system locale. 
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
`--force`, `-f`      | Force rename. Enables renaming some restricted files except `/` and the program itself.
`-v`                 | Version info.
`-q`                 | Quiet operation.
`--`                 | If this option is passed, anything and everything after it will be taken as file path. Put all options before passing this option.
`-shop`              | This shows an info about the various options passed as arguments and how they are being treated behind the scene.
`-sim`               | This runs a simulation of rename instead of actual rename operation, prints all kinds of available outputs. `-q` option won't have any effect if this option is passed.

<div id="technical-terms"></div>
##Technical Terms:

**Reserved Index    :** Index will be incremented even if 
                    any file is skipped renaming in order
                    to reserve the index for that skipped file
                    
**Reverse Index     :** Decrementing index.

<div id="name-string"></div>
**Name String       :** A string, that is parsed to create names for new files. It can be fixed name which then can be modified for different files at runtime.
                    Name sting is parsed by the following rules (must be wrapped around with filepath delimiter `/`):
                    
1. `/i/` in name string will be replaced with index.
2. `/ir/` in name string will be replaced with reserved index.
3. `/id/` in name string will be replaced with directory index (index inside a directory).
4. `/idr/` in name string will be replaced with reserved directory index
5. `/-i/` in name string will be replaced with inverse index.
6. `/-ir/` in name string will be replaced with inverse reserved index. In general, `-i` in the above replacement rules (applies to indexes excluding line indexes) will mean inverse index conforming to their meaning.
7. `/dc/` in name string will be replaced with directory count
8. `/l/` in name string will be replaced with line number from *Name String File*.
9. `/la/` in name string will be replaced with actual line number from *Name String File*.
10. `/n/` in name string will be replaced with filename without extension. If used with `-nsf` option, the filename will be the name taken from the *Name String File*.
11. `/fn/` in name string will be replaced with full name of the files. If used with `-nsf` option, full name will be the name taken from the *Name String File*.
12. `/rn/` in name string will be replaced with Replaced Name.
13. `/pd/` in name string will be replaced with parent directory  name of the current file or directory. 
14. `/wd/`  in  name  string will be replaced with the current working directory name.

<div id="extended-name-string-rule"></div>
**Extended Name String Rule :** This is an extension of the general purpose name string rules mentioned above.

1. Base conversion: `/i-b2/` converts the index to base 2 i.e binary and `/i-b32/` will convert to base 32. This applies to all numbered name string rules. 
2. Latin conversion: `/i-l/` converts the index to Latin number. This applies to all numbered name string rules. 
3. Scientific conversion: `/i-s/` converts the index to scientific form. This applies to all numbered name string rules. 
4. Extended /pd/ rules:
  * `pd0` is the immediate parent directory, `pd1` is the directory before pd0 and so forth.
  * A range can also be supplied to combine all the directories in the level implied by the range. The general format of this rule is `/pd<digits>-<digits>-delimiter/`
  * For example: `/pd0-4--/` will combine all directories from level 0 to 4 inserting the delimiter (`-` in this case) between them and the rule will become something like `dir0-dir1-dir2-dir3-dir4`.
  * In place of `<digits>` you can also supply `e` which generally means the 'end' i.e the deepest level available.
  * Any unavailable level of directory will be ignored and be replaced with empty string.
  * The range is bidirectional e.g `/pd4-0--/` will do the same in reverse order. 




**Name String File  :** A file which contains a list of name string (one per line).
                    Empty lines will be ignored and line number won't be counted.
                    Actual line number (which counts the empty lines too) is
                    available through name string rule : `/la/`.

<div id="search-string"></div>
**Search String     :** A string that is used to search for files with matching
                    filenames against the search string. By default it is
                    a regex if `-ssF` option is not used.It is generally in the form `/regex/modifier` , 
                    where <u>regex</u> is the regex to search for and available modifier is <u>i</u> which implies  case
                    insensitive  search. If no  modifier is used, the regex format can be
                    reduced to `/regex/` or simply `regex`.
                    
Terminate search strings (`/regex/` format only) with `;` to provide multiple search strings, e.g `'/s1/i;/s2/;/s3/'`. This applies to fixed search strings as well.

Also you can provide multiple search strings with repeated `-ss` and/or `-ssf` options and files with repeated `-ss/f` and/or `-ssf/f` options. These options can be mixed with each other too.

                 
**Index Field Length:** An integer value defining the field length of index.
                    By default empty field will be filled with 0's. For example, if
                    the value is `3`, then index will be `001`, `002`, `003`, etc..
                    Different filler (other than 0) can be provided with the `-iff` option.
                    
**Replaced Name     :** The name can be modified at runtime using replace string.
                    replace string will be parsed to create a new *Name String* rule: `/rn/`
                    which can be used in *Name String*. If name string is not passed as argument,
                    the new name of the file will be `/rn/`. *Replaced Name* is always 
                    generated from the old filename.


<div id="replace-string"></div>
**Replace String    :** *Replace String* is a regex of the form: 
                    `/search_part/replace_part/modifier`
                    where search_part is the regex to search for and
                    replace_part is the string to replace with. Name String rules are
                    avalilable in search_part and replace_part in Replace String.
                    Regarding replace string, there are several special cases:
                    
1. `&` will be taken as the entire match found by the regex (search_part).
2. `\1`, `\2` etc.. is the captured groups. If you want to isolate a captured groups, wrap it around with `{}`. For example, if you want to put a digit (2) after captured group `\1`, you can't use it like `\12`. `\12` will mean `12th` captured group not `\1` appended with a digit (1). In this case isolate the captured group with `{}` i.e `\{1}`. 
3. `\c` will convert the matched string to lowercase, and `\C` will convert it to uppercase. No other character is allowed in replace part if this is used. You can still concatenate different replace strings with `;`.
4. `\p` is the prefix (i.e., the part of the target sequence that precedes the match)
5. `\s` is the suffix (i.e., the part of the target sequence that follows the match).

to insert a `&` literally, use `\&` and for `\` use `\\`.Two modifiers are available: g and i.
g stands for global and replaces every instances of match found.
i stands case insensitive search (default is case sensitive).*Replace String* is always performed on old file name.

Terminate replace strings with `;` to provide multiple replace strings, e.g `'/s1/r1/gi;/s2/r2/i;/s3/r3/'`.

You can provide multiple replace strings with repeated `-rs` option and multiple file with repeated `-rs/f` options. These options can be mixed with each other too.

```
Example: '/video/Episode /i//gi' will replace every instances
of 'video' with 'Episode index' i.e you will get new rname as:.
Episode 1..., Episode 2..., etc...
```

<div id="regex"></div>
**Regex             :** Supported regexes are POSIX compliant <a href="https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap09.html#tag_09_03">basic</a> & <a href="https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap09.html#tag_09_04">extended</a> regex, grep, egrep and <a href="https://pubs.opengroup.org/onlinepubs/9699919799/utilities/awk.html#tag_20_06_13_04">awk</a> type regexes and the default <a href="https://www.cplusplus.com/reference/regex/ECMAScript/">ECMAScript Regex</a>. You can change the regex mode with `-re` or `--regex` option.

<div id="keywords"></div>
**Keywords          :**

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

                    
Only invalid characters for a file or directory name is the path delimiter and the null character (`\0`).

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
rnm -nsf filepath -ns /n//id/.ext
rnm -nsf filepath
etc...
```

<div id="things-to-care"></div>
#Things to care:


1. All options should always be separated by space. For Example: `-vy` won't mean two option `-v` and `-y`, rather it will mean a single option `-vy`.
2. Any non option argument will be treated as file or directory path. For example in <pre><code>rnm file1 file2 -- -ns fd</code></pre>`file1`, `file2` and `--` will be taken as file paths.
3. Be wary of filename globbing. Command like `rnm ./*` will take all files and directories as arguments and thus the files and directories will be subject to rename operation. If you don't want to rename directories, use file only mode (`-fo`). If you want to go inside directories, use depth (`-dp`) greater than `0` with file only mode.
4. If you run `rnm . -ns something` or `rnm ./ -ns something`, your current directory will be renamed (be careful).
5. This is a dangerous tool like `rm`, so use with care. If you make a mistake and do some unwanted rename, run `rnm -u` to undo (before running any more `rnm` command).
6. Pass all regex like strings within quotes even if they don't contain any white space.
7. To pass a filename that resembles an option, use `./`, i.e `./-ns` to pass a file named `-ns` in the current directory. Or you can use the `--` option make it a non-option argument; in that case make sure to pass all "Option" arguments before `--`, because everything after `--` will be taken as file path/s.
8. Pass file or directory path list at the end of the argument sequence. Use `--` to make all the arguments after it as paths (not options).

