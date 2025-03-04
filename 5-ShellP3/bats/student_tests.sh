#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check invalid command fails" {
    run ./dsh <<EOF                
invalidCommand
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="forkerror:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "check exit" {
    run ./dsh <<EOF                
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>cmdloopreturned-7"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "check cat runs without errors" {
    run ./dsh <<EOF                
cat dragon.c
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check uname runs without errors" {
    run ./dsh <<EOF                
uname
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check dragon runs without errors" {
    run ./dsh <<EOF                
dragon
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "cd with an absolute path works" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
cd /tmp
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/tmpdsh3>dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "check command with spaces between quotes works" {
    run "./dsh" <<EOF                
echo "    hello           world   "       
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="    hello           world   dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "pipe with 2 commands" {
    run ./dsh <<EOF
echo "hello world" | wc -w
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output (should count words)
    expected_output="2dsh3>dsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipes with 3 commands" {
    run ./dsh <<EOF
echo -e "hello\nworld" | grep "hello" | wc -l
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="1dsh3>dsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipes with 4 commands" {
    run ./dsh <<EOF
ls -1 | grep ".c" | sort | wc -l
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="3dsh3>dsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipes with the max of 8 commands" {
    run ./dsh <<EOF
echo "test1 test2 test3" | cat | sort | uniq | wc -l | cat | cat | cat
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="1dsh3>dsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipes with 9 commands..." {
    run ./dsh <<EOF
command1 | command2 | command3 | command4 | command5 | command6 | command7 | command8 | command9
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>error:pipinglimitedto8commandsdsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipes with commands that require args" {
    run ./dsh <<EOF
ls -l | grep ".c" | wc -l
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>3dsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$status" -eq 0 ]
}

@test "pipe with an invalid command" {
    run ./dsh <<EOF
invalidCommand | wc -l
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="execvpfailed:Nosuchfileordirectory0dsh3>dsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipe with leading spaces" {
    run ./dsh <<EOF
        echo "hello world" |         wc -w
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output (should count words)
    expected_output="2dsh3>dsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipe with trailing spaces" {
    run ./dsh <<EOF
echo "hello world"     | wc -w          
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output (should count words)
    expected_output="2dsh3>dsh3>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}





