[![Build Status](https://travis-ci.org/neurobin/rnm.svg?branch=release)](https://travis-ci.org/neurobin/rnm)

<div id="description"></div>
Renames files/directories in bulk. Naming scheme (*Name String*) can be applied or regex replace can be performed to modify file names on the fly. It uses PCRE2 (revised version of PCRE) regex to provide search (and replace) functionality.

It provides versatile options to modify names, delete/replace part of it, indexing, case conversion, insert text, insert various file information like modification time, access time, permission etc.., insert parent directory names, working directory name etc.. and many more.

File search functionality is provided with PCRE2 regex. Fixed string search is also possible.

Operations (rename, changing names, search) are selective of file type (directory, file, link).

Files can be sorted by name, modification time, access time, size, file type (directory, file, link) etc..

It provides an undo functionality to move back unwanted rename operations. Different **rnm** operation on different directory remembers their own undo history.

Simulations can be run instead of actual rename to view the potential outcome as program output on terminal with the *-sim* option.

# Features at a glance:

1. Rename files/directories/links en masse with dynamically constructed names.
2. Undo rename.
3. Names (or name strings) can be taken from a file.
4. Inserting file information (mtime, atime, ctime, permission etc..) in arbitrary formats.
5. Indexing and formatting the indexes in different ways.
6. Latin, scientific and base conversion for indexes and counters.
7. Inserting different level of parent directory names.
8. Access to current directory name, file name with and without extension.
9. Regex replace to delete/swap/replace part of the file name.
10. A regex replace operation can be performed on file only, directory only, link only or any combination of them.
11. Uppercase/Lowercase conversion on file name (can be done by selecting a part).
12. Replace strings can be taken from files.
13. Search string to search for files with matching regex/string.
14. Search string can be specified for only file, directory, link or any combination of them.
15. Inverse search.
16. Search strings can be taken from files.
17. Sorting files and indexing.
18. Sorting files according to size, modification time, access time etc..
19. Prioritize file, directory or link during sort.
20. Running simulation.

# New features:

1. Unlimited precision for numbers i.e you can perform base conversion, scientific conversion etc.. on an arbitrarily big number. This functionality is provided by GMP BigNum library (GNU Multi-precession library).
2. File information is available, e.g modification time, access time, status change time (time string can be formatted using format string e.g `%d-%m-%Y`), permission, owner id, group id, size, block, inode number, mode etc...
3. Inverse search is possible.
4. Each regex operation can be associated with either link, file or directory, for example, if a replace string is associated with only file, it will not work on directories or links.
5. New sorting methods: 
  1. Sort according to file modification time (recent first).
  2. Sort according to file access time (recent first).
  3. Sort according to file status change time (recent first).
  4. Sort according to file size (larger first).
  5. Prioritize directory when sorting.
  6. Prioritize file when sorting.
  7. Prioritize link when sorting.
  8. Reverse sort (can be used to reverse the order of a previous sort).

# Install:

## For Ubuntu and derivatives:

You can install it in Ubuntu from neurobin PPA (`ppa:neurobin/ppa`).

```sh
sudo add-apt-repository -y ppa:neurobin/ppa
sudo apt-get update
sudo apt-get install rnm
```

If you are in precise (12.04), you will need to add ubuntu-toolchain repository to make `libstdc++6 (>=4.8.4)` available:

```sh
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo add-apt-repository -y ppa:neurobin/ppa
sudo apt-get update
sudo apt-get install rnm
```

## For Archlinux:

Install from AUR:

```bash
git clone https://aur.archlinux.org/rnm.git
cd rnm
makepkg -i
```

## For MacOS:

Install from `neurobin/formula/rnm` with `brew`:

```bash
brew install neurobin/formula/rnm
```

## From pre-built binaries:

You can download a pre-built binary (64 bit, 32 bit, ppc64el) from the [release page](https://github.com/neurobin/rnm/releases) if available. These pre-built binaries depend on `libstdc++6 >= 4.8.4` if not specified otherwise.

## From source:

Make sure you have a C++ compiler equivalent to GCC-4.8 (`g++-4.8`) or later installed on your system.

#### Download dependencies:

It depends on three different libraries:

1. [pcre2](http://pcre.org/)
2. [jpcre2](https://github.com/jpcre2/jpcre2)
3. [gmplib](https://gmplib.org/)

To install dependencies run the `prepare.sh` file:

```sh
cd rnm # i.e go inside the rnm project directory
#Creating the following directories will prevent prepare.sh from installing the corresponding libraries.
#mkdir -p gmpxx  # uncomment and run this command if libgmp-dev (gmpxx) is already installed.
#mkdir -p pcre2  # uncomment and run this command if libpcre2-dev (pcre2) is already installed.
#mkdir -p jpcre2 # uncomment and run this command if libjpcre2-dev (jpcre2) is already installed.
sh prepare.sh
```

#### Install rnm:
After you have all the dependencies at hand, you can simply do:

```sh
autoreconf -f -i #run this one if your automake version is less than 1.15
./configure --prefix=/usr
make
sudo make install
```

<div id="un-install"></div>

# Uninstall:

1. For Debian, Ubuntu and derivatives, uninstall from software manager.
2. For Archlinux, uninstall with pacman.
3. For MacOS, uninstall with `brew uninstall rnm`
4. If you used a pre-built binary, just delete the binary and the man page.
5. If you compiled from the source, do `./configure --prefix=/usr && make && sudo make uninstall`

# Basic options

* `-ns`: Name string, simply new name for files. String wrapped with `/` are treated as special rules and expanded to their special meaning.
* `-rs`: Replace string. Performs regex replace to construct new name.
* `-ss`: Searches files with matching regex.
* `-ssf`: Searches files with matching fixed string.
* `-s`: Sort files.
* `-fo`: File only mode.
* `-do`: Directory only mode.
* `-lo`: Link only mode.
* `-dp`: Depth of directories (how many subdirs to recurse).
* `-u`: Undo the last rename operation in the current directory.
* `-up`: Undo rename from an arbitrary undo path.
* `-ups`: Show available undo paths.
* `-duh`: Delete undo history older than the given seconds.
* `-duhd`: Delete undo history older than the given days.

For a complete list of options and to get a glimpse of what **rnm** is capable of, get a peek at the doc (`rnm.md` or [rnm.html](https://docs.neurobin.org/man/man1/rnm.html)) or man page (`man rnm`).

<div id="usage"></div>
# Usage:

**Simplest example:**

```bash
rnm -ns newname oldfile
#rename by adding something to oldname:
rnm -ns '/fn/ some text to add' oldfile
```
> `/fn/` is a name string rule that expands to file name.

## Indexing

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

> There are lots of magics you can do with index, like converting to binary, octal, hexadecimal etc.., converting to scientific or latin number, use index flags to manipulate their format etc.. Refer to the doc (rnm.md or [rnm.html](https://docs.neurobin.org/man/man1/rnm.html)) or man page (`man rnm`) for complete details.

## Change/Modify/remove part of a file name

**rnm uses PCRE2 regex** to provide regex replace on old filename. The general format for regex replace:

```bash
'/regex/replace/modifier'
```

where `regex` is the regex to match in the file name, replace is the string to replace the match with and modifiers are : g (global) i (case insensitive), d (directory), f (file), l (link).

**swap parts:**

```bash
rnm -rs '/(.*)_(.*)\.mp3/\2_\1.mp3/' ./*
```

Before | After
------ | -----
test_file.mp3 | file_test.mp3
test_song.mp3 | song_test.mp3

> `-rs` takes a replace string of the form `/regex/replace/modifier`
> `\1` is captured group one, `\2` is captured group 2.

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
test_file_123.mp3 | test_file_123.MP3
test_song_456.mp3 | test_song_456.MP3

> `\C` (capital C) in replace part converts the matched part to uppercase

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

> See the doc (rnm.md or [rnm.html](https://docs.neurobin.org/man/man1/rnm.html)) or man page for complete details regarding regex replace.

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
rnm -ns '/fn/.link' -ss '//l'
```

> `//l` and empty search string matches anything in file name.
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


> If you have lots of search strings, you can put them into files and add those files with `-ss/f` option. This option can be given multiple time to add multiple files. `-ss` option can also be given multiple times to add multiple search string. See the docs (rnm.md or [rnm.html](https://docs.neurobin.org/man/man1/rnm.html)) or man page for more ways and details.

## Sort and rename:

There are several sorting mechanisms available:

1. `-s`    : default sort (natural sort)
2. `-s/g`  : general sort
3. `-s/n`  : natural sort
4. `-s/mt` : sort according to file modification time (recent first)
5. `-s/at` : sort according to file access time (recent first)
6. `-s/ct` : sort according to file status change time (recent first)
7. `-s/sz` : sort according to file size (larger first)
8. `-s/d`  : prioritize directory when sorting
9. `-s/f`  : prioritize file when sorting
10. `-s/l` : prioritize link when sorting
11. `-s/r` : reverse the order sorted by above methods.


The default sort which comes with the option `-s` is natural sort (`-s/n`).

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
dir1 | dir1 drwx--x--x


> `perm` is another property for info name string rule, `op=ls` is for permission resembling to `ls` command, `oct` is for octal permission.

> There are lots of properties available for file info which makes all kinds of file information available. See the docs (rnm.md or [rnm.html](https://docs.neurobin.org/man/man1/rnm.html)) or man page for details.


# Miscellaneous examples

## Add/Insert parent directory names to filename:

```bash
rnm -ns '/pd0/-/fn/' dir1/*.png
```
Before | After
------ | -----
dir1/abc.png | dir1/dir1-abc.png

> `/pd0/` is the immediate parent directory

```bash
rnm -ns '/pd1-0--/-/fn/' dir1/dir0/*.png
```
Before | After
------ | -----
dir1/dir0/abc.png | dir1/dir0/dir1-dir0-abc.png

> `/pd1-0--/` adds the directory names from `/pd1/` to `/pd0/` with a delimiter in-between. `/pd0-1- /` would become `dir0 dir1` (here space is the delimiter). The general format is: `/pd<from>-<to>-<delimiter>/`

## Index files by sorting them according to modification time:

```bash
rnm -ns '/fn/ /ir/' -s/mt ./*
```
> The above will append index to the filenames by sorting the files according to modification time (recent first). `/ir/` is reserved-index (index is reserved for skipped files), `-s/mt` sorts according to mtime.

## Index files by sorting them according to size:

```bash
rnm -ns '/fn/ /ir/' -s/sz ./*
```
> The above will append index to the filenames by sorting the files according to size (larger first).

## Treat files, directories, links differently in a single command to rename files:

```bash
rnm -rs '/_/-/gf' -rs '/\./-/gd' -rs '/$/.link/l' ./*
```
This is what the above command does:

1. Replace all underscores with hyphen for regular files (`/_/-/gf`).
2. Replace all dot with hyphen for directories (`/\./-/gd`).
3. Give all symbolic links a .link extension (`/$/.link/l`).

## Search for files matching pattern1 in their name, directories matching pattern2 and links matching pattern3 and append modification time with the filename:

```bash
rnm -ns '/fn/ /info-mtime-%d-%m-%Y %H:%I %p/' -ss '/pattern1/f' -ss '/pattern2/d' -ss '/pattern3/l' ./*
```

[rnm user manual](https://docs.neurobin.org/man/man1/rnm.html)

[Other examples](https://neurobin.org/docs/unix/rnm/bulk-rename-in-linux/)



