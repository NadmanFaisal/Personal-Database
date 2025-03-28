#!/bin/bash

# Compile the program into executable file
gcc main.c InputBuffer.c MetaCommand.c Row.c Statement.c Table.c -o mydb


# Test 1: Test whether insertion and retrieval is working as expected
test1=$(./mydb <<EOF
insert 1 wowza people@shabang.com
select
.exit
EOF
)

# Expected output
test1_expected_output="db > Executed!
db > (1, wowza, people@shabang.com)
Executed
db > "

# Compare actual output with expected
if [ "$test1" == "$test1_expected_output" ]; then
  echo "Test 1 PASSED"
else
  echo "Test 1 FAILED"
  echo "Expected:"
  echo $'$test1_expected_output\n'
  echo "Actual:"
  echo "$test1"
  exit 1
fi

# Test 2: Table full error
script=""
for i in $(seq 1 1401); do
  script+="insert $i user$i person$i@example.com"$'\n'
done
script+=".exit"

test2=$(echo "$script" | ./mydb)

# Get the second last response from the system
last_message=$(echo "$test2" | tail -n 2 | head -n 1)

if [ "$last_message" == "db > Error: Table is full." ]; then
  echo "Test 2 PASSED"
else
  echo "Test 2 FAILED"
  echo $'Expected: db > Error: Table is full.\n'
  echo "Actual: $last_message"
  exit 1
fi

# Test 3: Test edge case for max string length of email and username (username: 32 chars, email: 255 chars)
long_username=$(printf 'a%.0s' {1..32})
long_email=$(printf 'a%.0s' {1..255})

test3=$(./mydb <<EOF
insert 1 $long_username $long_email
select
.exit
EOF
)

test3_expected_output="db > Executed!
db > (1, $long_username, $long_email)
Executed!
db > "

if [ "$test3" == "$test3_expected_output" ]; then
  echo "Test 3 PASSED"
else
  echo "Test 3 FAILED"
   echo "Expected:"
   echo $'$test3_expected_output\n'
  echo "Actual:"
  echo "$test3"
  exit 1
fi

#Test 4: Not allow insertion for fields greater than max chars allowed
long_username=$(printf 'a%.0s' {1..33})   # 33 chars, 1 over the limit
long_email=$(printf 'a%.0s' {1..256})     # 256 chars, 1 over the limit

test4=$(./mydb <<EOF
insert 1 $long_username $long_email
select
.exit
EOF
)

test4_expected_output="db > String is too long.
db > Executed!
db > "

if [ "$test4" == "$test4_expected_output" ]; then
  echo "Test 4 PASSED"
else
  echo "Test 4 FAILED"
  echo "Expected:"
  echo $'$test4_expected_output\n'
  echo "Actual:"
  echo "$test4"
  exit 1
fi

#Test 5: Handle negative ID values
test5=$(./mydb <<EOF
insert -1 foo bar@gmail.com
select
.exit
EOF
)

test5_expected_output="db > ID must be a positive integer.
db > Executed!
db > "

if [ "$test5" == "$test5_expected_output" ]; then
  echo "Test 5 PASSED"
else
  echo "Test 5 FAILED"
  echo "Expected:"
  echo "$test5_expected_output"
  echo "Actual:"
  echo "$test5"
  exit 1
fi