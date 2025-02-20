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
    expected_output="forkerror:Nosuchfileordirectorydsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "check multiple invalid command fails" {
    run ./dsh <<EOF                
invalidCommand
tehehe
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="forkerror:Nosuchfileordirectorydsh2>forkerror:Nosuchfileordirectorydsh2>dsh2>dsh2>dsh2>dsh2>cmdloopreturned0"

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
    expected_output="forkerror:Nosuchfileordirectorydsh2>dsh2>dsh2>cmdloopreturned0"

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
    expected_output="dsh2>cmdloopreturned0"

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

@test "check mkdir runs without errors" {
    run ./dsh <<EOF                
mkdir test4Errors
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
    expected_output="/tmpdsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "cd with linux shell behavior (~1) fails" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
cd ~1
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output when HOME is unset
    expected_output="cdfailed:Nosuchfileordirectorydsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
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
    expected_output="cdfailed:Nosuchfileordirectory$(pwd)dsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "cd to root (/) works" {
    current=$(pwd)
    run "${current}/dsh" <<EOF
cd /
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/dsh2>dsh2>dsh2>cmdloopreturned0"

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
    expected_output="hello worlddsh2> dsh2> cmd loop returned 0"

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
    expected_output="hello worlddsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "check command with lots of spaces between command and args works" {
    run "./dsh" <<EOF                
echo          "hello world"       
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="hello worlddsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "check command with numerous of spaces between quotes works" {
    run "./dsh" <<EOF                
echo "hello           world"       
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="hello           worlddsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "check command with spaces between quotes works" {
    run "./dsh" <<EOF                
echo "    hello           world   "       
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="    hello           world   dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}



