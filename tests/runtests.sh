#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libsmdev/.libs/libsmdev.1.dylib && test -f ./pysmdev/.libs/pysmdev.so;
then
	install_name_tool -change /usr/local/lib/libsmdev.1.dylib ${PWD}/libsmdev/.libs/libsmdev.1.dylib ./pysmdev/.libs/pysmdev.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

