#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libsmdev/.libs/libsmdev.1.dylib" ] && [ -f ./pysmdev/.libs/pysmdev.so ]
then
    install_name_tool -change /usr/local/lib/libsmdev.1.dylib "${PWD}/libsmdev/.libs/libsmdev.1.dylib" ./pysmdev/.libs/pysmdev.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

