#!/bin/bash
# Info tool testing script
#
# Version: 20200705

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

PROFILES=("smdevinfo");
OPTIONS_PER_PROFILE=("");
OPTION_SETS="";

INPUT_GLOB="*";

if test -n "${SKIP_TOOLS_TESTS}" || test -n "${SKIP_TOOLS_END_TO_END_TESTS}";
then
	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="../smdevtools/smdevinfo";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="../smdevtools/smdevinfo.exe";
fi

if ! test -x "${TEST_EXECUTABLE}";
then
	echo "Missing test executable: ${TEST_EXECUTABLE}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

INPUT_FILE="/dev/sda";

if ! test -b ${INPUT_FILE} || ! test -r ${INPUT_FILE};
then
	INPUT_FILE="/dev/vda";
fi

if ! test -b ${INPUT_FILE} || ! test -r ${INPUT_FILE};
then
	INPUT_FILE="/dev/null";
fi

run_test_on_input_file "smdevinfo" "smdevinfo" "with_stdout_reference" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_FILE}" "${OPTIONS}";
RESULT=$?;

exit ${RESULT};

