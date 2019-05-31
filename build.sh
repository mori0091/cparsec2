#!/bin/sh
# -*- coding: utf-8-unix -*-

# Usage: build.sh [options] [target] ...
# Run 'make -C <dir>' for all directory <dir> where a makefile exists.

for i in $(find * -type f -name "GNUmakefile" -o -name "Makefile")
do
    d=$(dirname "$i")
    echo make $@ -C "$d"
    echo "----------------------------------------------------------------"
    make -C "$d" $@
    echo
done
