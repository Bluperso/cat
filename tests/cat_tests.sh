#!/bin/bash

function cat_test {
    OPTIONS="$1"
    INPUT="$2"

    EXPECTED_CMD="cat $OPTIONS $INPUT"
    MY_CMD="./cat $OPTIONS $INPUT"

    echo "Running test: $MY_CMD"
    
    if [ "$INPUT" = "stdin" ]; then
        echo "Who is the best programmer? It's me" | cat $OPTIONS > system_cat.txt 2> system_err.txt
        echo "Who is the best programmer? It's me" | ./cat $OPTIONS > my_cat.txt 2> my_err.txt
    else
        eval "$EXPECTED_CMD" > system_cat.txt 2> system_err.txt
        eval "$MY_CMD" > my_cat.txt 2> my_err.txt
    fi

    if diff -q my_cat.txt system_cat.txt > /dev/null && diff -q my_err.txt system_err.txt > /dev/null; then
        echo "PASS"
    else
        echo "FAIL: Outputs differ"
        echo "Stdout diff:"
        diff -u system_cat.txt my_cat.txt || true
        echo "Stderr diff:"
        diff -u system_err.txt my_err.txt || true
        exit 1
    fi
}

FLAGS=("-b" "-n" "-e" "-t" "-s" "--number-nonblank" "-E" "--number" "--squeeze-blank" "-T" "-b -n -e -t" "-n -s -t" "-s -b" "-E -n -e" "-tbe" "-st" "-bn" "-TE" "-vtb")
TESTS=("test1.txt" "test2.txt" "test3.txt" "test4.txt" "test5.txt" "test.txt")

for j in "${TESTS[@]}"
do
    for i in "${FLAGS[@]}"
    do
        cat_test "$i" $j
    done
done

rm -f my_cat.txt system_cat.txt my_err.txt system_err.txt

echo "All integration tests passed!"