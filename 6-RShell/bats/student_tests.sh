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
    expected_output="forkerror:Nosuchfileordirectorylocalmodedsh4>localmodedsh4>dsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "check invalid command and valid command" {
    run ./dsh <<EOF                
invalidCommand
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="forkerror:Nosuchfileordirectorylocalmodedsh4>localmodedsh4>dsh4>cmdloopreturned-7"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "check exit runs without errors" {
    run ./dsh <<EOF                
exit
EOF

    # Assertions
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

@test "classic echo hello world" {
    run ./dsh <<EOF
echo "Hello world"
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="Helloworldlocalmodedsh4>dsh4>cmdloopreturned0"

    echo "Output: $output"
    echo "Exit Status: $status"
    echo "$stripped_output -> $expected_output"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "cd with an absolute path (tmp) works" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
cd /tmp
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/tmplocalmodedsh4>dsh4>dsh4>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "cd to an invalid directory fails" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
cd invalidDir
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="cdfailed:Nosuchfileordirectory$(pwd)localmodedsh4>dsh4>dsh4>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "check command with leading spaces works" {
    run "./dsh" <<EOF                
    echo "hello world" 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="hello worldlocal modedsh4> dsh4> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "check command with trailing spaces works" {
    run "./dsh" <<EOF                
echo "hello world"       
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="hello worldlocal modedsh4> dsh4> cmd loop returned 0"

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
    expected_output="2localmodedsh4>dsh4>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipes with the max of 8 commands" {
    run ./dsh <<EOF
echo "test1 test2 test3" | cat | sort | uniq | wc -l | cat | cat | cat
exit
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="1localmodedsh4>dsh4>cmdloopreturned-7"

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
    expected_output="localmodedsh4>error:pipinglimitedto8commandsdsh4>cmdloopreturned-7"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "check redirection with echo" {
    run ./dsh <<EOF
echo "hello" > test.txt
cat test.txt
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="hellolocalmodedsh4>dsh4>dsh4>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "client can connect to server" {
    run ./dsh -c <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

