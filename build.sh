#!/bin/sh
# -*- coding: utf-8-unix -*-

# Usage: build.sh [options] [target] ...
# Run 'make -C <dir>' for all directory <dir> where a makefile exists.

deco () {
    if [ "${use_color}" != yes ] ; then
        printf '%s' "$1"
        return
    fi
    str=$1; shift
    esc=;
    while [ $# -gt 0 ] ; do
        case "$1" in
            black)     esc="${esc};30" ;;
            red)       esc="${esc};31" ;;
            green)     esc="${esc};32" ;;
            yellow)    esc="${esc};33" ;;
            blue)      esc="${esc};34" ;;
            magenta)   esc="${esc};35" ;;
            cyan)      esc="${esc};36" ;;
            white)     esc="${esc};37" ;;
            bold)      esc="${esc};1"  ;;
            underline) esc="${esc};4"  ;;
        esac
        shift
    done
    if [ "${esc}" ] ; then
        printf '%s' "\033[${esc#;}m${str}\033[0m"
    else
        printf '%s' "${str}"
    fi
}

if [ -t 1 ] ; then
    is_stdout_tty=yes
fi

if [ -x /usr/bin/tput ] ; then
    colors=$(/usr/bin/tput colors)
else
    colors=0
fi

if [ "${is_stdout_tty}" = yes ] ; then
    if [ "${colors}" -ge 8 ] ; then
        use_color=yes
    fi
fi

readonly PASS_MARK=$(deco 'PASS' bold green)
readonly FAIL_MARK=$(deco 'FAIL' bold red)
readonly NEWLINE='
'

summary=$(deco "$0 $*" bold underline)${NEWLINE}
pass=0
fail=0

for i in $(find * -type f -name "GNUmakefile" -o -name "Makefile")
do
    d=$(dirname "$i")
    task="make $@ -C \"$d\""
    echo ${task}
    echo "----------------------------------------------------------------"
    if eval ${task} ; then
        summary="${summary}[${PASS_MARK}] ${task}${NEWLINE}"
        pass=$((pass + 1))
    else
        summary="${summary}[${FAIL_MARK}] ${task}${NEWLINE}"
        fail=$((fail + 1))
    fi
    echo
done

printf "${summary}\n"
echo "$((pass + fail)) projects, $pass passed, $fail failed"
echo
test $fail -eq 0
