[![Build Status](https://travis-ci.org/neurobin/rnm.svg?branch=release)](https://travis-ci.org/neurobin/rnm)

**The current version is an alpha release, beta testers are welcome.**

<div id="description"></div>
Renames files/directories in bulk. Naming scheme (*Name String*) can be applied or regex replace can be performed to modify names dynamically. It uses PCRE2 (revised version of PCRE) regex to provide search (and replace) functionality.

It provides versatile options to modify names, delete/replace part of it, indexing, case conversion, insert text, insert various file information like modification time, access time, permission etc.., insert parent directory names and many more.

File search functionality is provided with PCRE2 regex. Fixed string search is also possible.

Operations (rename, changing names, search) are selective of file type (directory, file, link).

Files can be sorted by name, modification time, access time, size, file type (directory, file, link) etc..

It provides an undo functionality to move back unwanted rename operations. Different **rnm** operations on different directory remember their own undo history.

Simulations can be run instead of actual rename to view the potential outcome as program output on terminal with the *-sim* option.


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

1. Unlimited precision for numbers i.e you can perform base conversion, scientific conversion etc.. on an arbitrarily big number. This functionality is provided by GMP BigNum library (GNU Multi-precession library).
2. File information is available, e.g modification time, access time, status change time (time string can be formatted using format string e.g `%d-%m-%Y`), permission, owner id, group id, size, block, inode number, mode etc...
3. Inverse search is possible.
4. Each regex operation can be associated with either link, file or directory, for example, if a replace string is associated with only file, it will not work on directories or links.

#Features that were dropped:

* Multiple regex mode in favor of PCRE2 regex.
* All the previous regex modes; from now only PCRE2 regex will be used.


#Install:

###Install from source:

Make sure you have a C++ compiler equivalent to GCC-5.0 (`g++-5`) or later installed on your system.

####Download dependencies:
To install dependencies run the `prepare.sh` file:

```sh
cd rnm # i.e go inside the rnm project directory
sh prepare.sh
```

It will download three different projects:

1. [pcre2](http://pcre.org/)
2. [jpcre2](https://docs.neurobin.org/jpcre2)
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

# Basic options

1. `-ns`: Name string, simply new name for files. String wrapped with `/` are treated as special rules and expanded to their special meaning.
2. `-rs`: Replace string. Performs regex replace to construct new name.
3. `-ss`: Searches files with matching regex.
4. `-ssf`: Searches files with matching fixed string.
5. `-s`: Sort files.
6. `-fo`: File only mode.
7. `-do`: Directory only mode.

<div id="usage"></div>
#Usage:

**Simplest example:**

```bash
rnm -ns newname oldfile
#rename by adding something to oldname:
rnm -ns '/fn/ some text to add' oldfile
```
> `/fn/` is a name string rule that expands to file name.

##Indexing

**Rename all by appending index to file names:**

```bash
rnm -ns '/n/ /i/./e/' ./* -dp -1
```

Before | After
------ | -----
file.mp3 | file 1.mp3
song.mp3 | song 2.mp3
video/song.mp4 | video/song 3.mp4


> `/n/` is a name string rule which expands to file name without extension of the current renaming file.
> `/e/` is a name string rule which expands to file extension.
> `/i/` is another name string rule which expands to index.
> `-dp -1` means unlimited depth i.e go to all subdirs.


**Rename all by appending directory wise index to file names:**

```bash
rnm -ns '/n/ /id/./e/' ./* -dp -1
```

Before | After
------ | -----
dir1/file.mp3 | dir1/file 1.mp3
dir1/song.mp3 | dir1/song 2.mp3
dir2/file.mp3 | dir2/file 1.mp3
dir2/song.mp3 | dir2/song 2.mp3

> `/id/` is a name string rule which expands to directory index (index inside directory).

**Index with leading zeros ( file 001, 002 etc..):**

```bash
rnm -ns '/n/ /id/./e/' -ifl 4 ./*
```

Before | After
------ | -----
file.mp3 | file 0001.mp3
song.mp3 | song 0002.mp3
... | ...
video.mp4 | video 0010.mp4

> `-ifl 4` sets index field length to 4 and thus remaining space is filled with 0 (by default).

**Use custom increment value:**

```bash
rnm -ns '/n/ /id/./e/' -ifl 2 -inc 4 ./*
```

Before | After
------ | -----
file.mp3 | file 01.mp3
song.mp3 | song 05.mp3

> `-inc 4` sets the index increment value to 4.

**Decreasing index:**

```bash
rnm -ns '/n/ /-id/./e/' -ifl 3 -inc 4 ./*
```

Before | After
------ | -----
file.mp3 | file -001.mp3
song.mp3 | song -005.mp3

> `-inc` is always positive, but decremented index can be accessed by appending `-` in (at beginning) them.

**Set starting index:**

```bash
rnm -ns '/n/ /id/./e/' -ifl 3 -inc 4 -si 4 ./*
```

Before | After
------ | -----
file.mp3 | file 004.mp3
song.mp3 | song 008.mp3

> There are lots of magics you can do with index, like converting to binary, octal, hexadecimal etc.., converting to scientific or latin number, use index flags to manipulate their format etc.. Refer to the doc (rnm.md or rnm.html) or man page (`man rnm`) for complete details.

## Change/Modify/remove part of a file name

**rnm uses PCRE2 regex**

**swap parts:**

```bash
rnm -rs '/(.*)_(.*)\.mp3/\2_\1.mp3/' ./*
```

Before | After
------ | -----
test_file.mp3 | file_test.mp3
test_song.mp3 | song_test.mp3

> `-rs` takes a replace string of the form `/regex/replace/modifier`

**Replace all _ (underscore) to space:**

```bash
rnm -rs '/_/ /g' ./*
```

Before | After
------ | -----
test_file_123.mp3 | test file 123.mp3
test_song_456.mp3 | test song 456.mp3

**Change all .mp3 to .MP3 (uppercase):**

```bash
rnm -rs '/\.mp3/\C/g' ./*
```

Before | After
------ | -----
test_file_123.mp3 | test file 123.MP3
test_song_456.mp3 | test song 456.MP3

> `\C` (capital C) in replace part converts the selected part to uppercase

**Change all before `_` to (lowercase) and all after `.` to (uppercase):**

```bash
rnm -rs '/.*_/\c/;/.*\./\C/g' ./*
```

Before | After
------ | -----
TEST_file_123.mp3 | test_file_123.MP3
TEST_song_456.mp3 | test_song_456.MP3


> `\c` (small c) in replace part converts to lowercase.


**Insert .link at the end for files that are symbolic link:**

```bash
rnm -rs '/$/.link/l' ./*
```

> the *l* modifier tells it to perform the replacement only to symbolic links, other modifiers *f* and *d* are available for files and directories respectively.


> rnm comes with the power of PCRE2 regex and thus you can perform almost all kinds of file name modifications using regex replace. Name string rules ( `/fn/`, `/i/` etc..) are applicable inside regex or replace part.
> If you have lots of regex replace to perform, you can put them into a file and give the file path with `-rs/f` option. This option can be given multiple times to add multiple files. `-rs` option can also be given multiple times to add multiple replace string. Also replace string can be terminated with `;` to add another replace string after it.

> See the doc (rnm.md or rnm.html) or man page for complete details regarding regex replace.

## Search for files and rename:

### Search with regex:

**Rename .mp3 files to .dummy:**

```bash
rnm -ns '/fn/.dummy' -ss '\.mp3$' ./*
```

Before | After
------ | -----
file1.mp3 | file1.mp3.dummy
file2.mp3 | file2.mp3.dummy
file3.mp4 | file3.mp4

**Rename all symbolic links to .link:**

```bash
rnm -ns '/fn/.link' -ss '/.*/l'
```

> the *l* modifier tells it to match link, other modifiers *f* and *d* are available for files and directories respectively.


### Search with fixed string:


**Rename .mp3 files to .dummy:**

```bash
rnm -ns '/fn/.dummy' -ssf '.mp3' ./*
```

Before | After
------ | -----
file1.mp3 | file1.mp3.dummy
file2.mp3 | file2.mp3.dummy
file3.mp4 | file3.mp4

> In this case .mp3 can match at the middle or start too.

### Inverse search:

**Rename all files except .mp3 to .dummy:**

```bash
rnm -ns '/fn/.dummy' -ss '/\.mp3$/!' ./*
#or with fixed string:
rnm -ns '/fn/.dummy' -ssf '/.mp3/!' ./*
#for fixed string search, .mp3 will match at any place in the name
```

Before | After
------ | -----
file1.mp3 | file1.mp3
file2.mp3 | file2.mp3
file3.mp4 | file3.mp4.dummy


> If you have lots of search strings, you can put them into files and add those files with `-ss/f` option. This option can be given multiple time to add multiple files. `-ss` option can also be given multiple times to add multiple search string. See the docs (rnm.md or rnm.html) or man page for more ways and details.

## Sort and rename:

Two sorting mechanisms are available:

1. General sort: Alphabetic sort
2. Natural sort: Human perceivable sort.

The default sort which comes with the option `-s` is natural sort (`-s/n`), other options are `-s/g` and `-s/none`

```bash
rnm -ns '/fn/ /id/' -s ./*
```

## Insert file information

**Add file modification time to file name:**

```bash
rnm -ns '/fn/ @ /info-mtime-%d-%m-%Y %H:%I %p/' ./*
```

Before | After
------ | -----
file1 | file1 @ 27-01-2017 12:35 AM
file2 | file2 @ 17-01-2017 10:34 PM

> `/info-prop-op/` is another name string rule, `prop` is the property name (mtime, atime, perm, uid etc..) and `op` is the format (optional).

**Add file permission to file name:**

```bash
rnm -ns '/fn/ /info-perm-ls/' ./*
```
Before | After
------ | -----
dir1 | dir1 drwxrwxr-x


> `perm` is another property for info name string rule, `op=ls` is for permission resembling to `ls` command, `oct` is for octal permission.

> There are lots of properties available for file info which makes all kinds of file information available. See the docs (rnm.md or rnm.html) or man page for details.





