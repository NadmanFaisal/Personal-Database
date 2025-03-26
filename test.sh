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

