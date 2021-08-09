#!/bin/bash
assert(){
    expected="$1"
    input="$2"
    
    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"
    
    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expcted expected, but got $actual"
        exit 1
    fi
}

assert 0 0
assert 42 42
assert 41 " 12 + 34 - 5 "
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
assert 0 '-9 / 3 + (-1 + 4)'
assert 0 '2 == 3'
assert 1 '3 - 2 != 8 - 8'
assert 1 '2>=9-7'
assert 1 '9-7 <= 5-1 * 2 + 2 - (9-8)'

echo OK
