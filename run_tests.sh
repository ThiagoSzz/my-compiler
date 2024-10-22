#!/bin/bash

TOTAL_TESTS=0
ERR_COUNT=0
SUCCESS_COUNT=0
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0;38;5;250m'
NC_DARKER='\033[0;38;5;242m'
START_BOLD='\033[1m'
RESET_BOLD='\033[0m'

# Build
make > /dev/null

# Iterate over all test files
for filename in tests/*; do
  TOTAL_TESTS=$(($TOTAL_TESTS + 1))

  # Check if the test is expected to fail or pass
  if grep -q '//FAIL' "$filename";
  then
    expected="FAIL"
  else
    expected="PASS"
  fi

  # Run the test
  output=$(./etapa2 < "$filename" || true)

  # Check if the output contains the line of the error
  if echo $output | grep -q "line";
  then
    actual="FAIL"
  else
    actual="PASS"
  fi

  # Trim the filename
  trimmed_filename="${filename#tests/}"
  trimmed_filename="${trimmed_filename%.txt}"

  # Check if expected equals actual
  if 
    { [ "$expected" = "PASS" ] && [ "$actual" = "PASS" ]; } || 
    { [ "$expected" = "FAIL" ] && [ "$actual" = "FAIL" ]; }; 
  then
    echo "${GREEN}${START_BOLD}PASS${RESET_BOLD} ${NC}$trimmed_filename   ${NC_DARKER}(expected: $expected, actual: $actual)"
    SUCCESS_COUNT=$(($SUCCESS_COUNT + 1))
  else
    echo "${RED}${START_BOLD}FAIL${RESET_BOLD} ${NC}$trimmed_filename   ${NC_DARKER}(expected: $expected, actual: $actual)"
    ERR_COUNT=$(($ERR_COUNT + 1))
  fi
done

# Print results
printf "\n"
echo "${NC}Tests: $TOTAL_TESTS total ${NC_DARKER}→ ${GREEN}$SUCCESS_COUNT passed${NC}, ${RED}$ERR_COUNT failed${NC}"

# Exit with error code if there are any failed tests
if [ "$ERR_COUNT" -gt 0 ];
then
  exit 1
fi

# Clean up
make clean > /dev/null