#!/bin/sh
# -*- coding: utf-8-unix -*-

usage () {
    echo "usage: $0 {*.o|*.a}..."
} 1>&2

usageExit () {
    usage
    exit $1
}

[ $# -gt 0 ] || usageExit 1

NM='nm -n --defined-only --extern-only'
TESTS=$(${NM} "$@" | sed -nE -e 's/^.+ D _?(TestIt_test_.*)/\1/p' | sort)
[ $? -eq 0 ] || usageExit 1

if [ "${TESTS}" ] ; then
    TESTS_DECL=$(printf 'extern TestSt %s;\n' ${TESTS})
    TESTS_REFS=$(printf '    &%s,\n' ${TESTS})
fi

cat <<END
#include "testit.h"

${TESTS_DECL}

static Test tests[] = {
${TESTS_REFS}
    0,
};
Test* testit_tests_ = &(tests[0]);
int testit_tests_total_ = (sizeof(tests)/sizeof(tests[0])) - 1;
END
