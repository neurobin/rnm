#!/bin/sh

# Get pcre2
wget -c https://github.com/jpcre2/pcre2/archive/release.tar.gz -O pcre2.tar.gz
mkdir -p pcre2
tar -xf pcre2.tar.gz -C pcre2 --strip-components=1 #&& rm pcre2.tar.gz

# Get jpcre2
wget -c https://github.com/jpcre2/jpcre2/archive/release.tar.gz -O jpcre2.tar.gz
mkdir -p jpcre2
tar -xf jpcre2.tar.gz -C jpcre2 --strip-components=1 #&& rm jpcre2.tar.gz

# Get gmplib 
wget -c https://github.com/Amerge/gmpxx/archive/release.tar.gz -O gmpxx.tar.gz
mkdir -p gmpxx
tar -xf gmpxx.tar.gz -C gmpxx --strip-components=1 #&& rm gmpxx.tar.gz
