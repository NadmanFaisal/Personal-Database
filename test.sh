#!/bin/bash

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
Executed!
db > "

# Compare actual output with expected
if [ "$test1" == "$test1_expected_output" ]; then
  echo "Test 1 PASSED"
else
  echo "Test 1 FAILED"
  echo "Expected:"
  echo "$test1_expected_output"
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
  echo "Expected: db > Error: Table is full."
  echo "Actual: $last_message"
  exit 1
fi