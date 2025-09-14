#!/bin/bash

MINISHELL="./minishell"   # Path to your minishell executable
TMP_OUT="minishell_out.txt"
BASH_OUT="bash_out.txt"

# Colors
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
NC="\033[0m" # No Color

# Helper function to run a command in minishell and bash
run_test() {
    local cmd="$1"

    echo -e "${YELLOW}=== TEST: $cmd ===${NC}"

    # Run in Minishell
    echo "$cmd" | $MINISHELL > $TMP_OUT 2>&1
    MINISHELL_EXIT=$?

    # Run in Bash
    bash -c "$cmd" > $BASH_OUT 2>&1
    BASH_EXIT=$?

    # Compare outputs
    diff -u $BASH_OUT $TMP_OUT > diff.txt
    DIFF_STATUS=$?

    # Compare exit codes
    EXIT_STATUS_OK=0
    if [ $MINISHELL_EXIT -ne $BASH_EXIT ]; then
        EXIT_STATUS_OK=1
    fi

    if [ $DIFF_STATUS -eq 0 ] && [ $EXIT_STATUS_OK -eq 0 ]; then
        echo -e "${GREEN}PASS${NC} (exit: $MINISHELL_EXIT)"
    else
        echo -e "${RED}FAIL${NC} (Minishell exit: $MINISHELL_EXIT, Bash exit: $BASH_EXIT)"
        echo -e "${RED}Output difference:${NC}"
        cat diff.txt
    fi

    echo ""
}

# ----------------- TESTS -----------------
# ECHO
run_test "echo Hello World"
run_test "echo -n Hello"
run_test "echo -n -n Hello"
run_test "echo ''"
run_test "echo 'Multiple   spaces'"

# CD
run_test "cd /; pwd"
run_test "cd /tmp; pwd"
run_test "cd ~; pwd"
run_test "cd nonexistent_dir"

# PWD
run_test "pwd"

# EXPORT
run_test "export MYVAR=123; echo \$MYVAR"
run_test "export MYVAR; echo \$MYVAR"
run_test "export 1INVALID=abc"
run_test "export ANOTHER_VAR=456; export ANOTHER_VAR; echo \$ANOTHER_VAR"

# UNSET
run_test "export TEMP_VAR=999; unset TEMP_VAR; echo \$TEMP_VAR"
run_test "unset NONEXISTENT"

# ENV
run_test "env"
run_test "export TESTENV=abc; env"

# EXIT
run_test "exit"
run_test "exit 42"
run_test "exit 300"
run_test "exit -1"
run_test "exit abc"
run_test "exit 1 2"

echo -e "${YELLOW}All tests completed.${NC}"
