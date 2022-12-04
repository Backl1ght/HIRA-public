DUMMY_SYSTEM_HEADER_PATH=${HIRA_ROOT}/dummy_system_headers

get_system_headers() {
    # gcc -E -M will print all headers to stdout
    # read and split output into string array
    readarray -d \\ -t all_headers <<< $(gcc $1 -E -I${HIRA_ROOT} -M ${HIRA_FLAGS})

    system_headers=()
    for header in ${all_headers[*]}
    do
        # assume system headers start with /usr
        if [[ $header == /usr/* ]]
        then
            system_headers+=($header)
        fi
    done
}

generate_dummy_system_headers() {
    get_system_headers $1

    # generate empty headers
    for header in ${system_headers[*]}
    do
        dummy_header=${DUMMY_SYSTEM_HEADER_PATH}${header}
        dummy_header_directory=${dummy_header%/*}

        if [ ! -d "$dummy_header_directory" ]
        then
            mkdir -p $dummy_header_directory
        fi

        touch ${dummy_header}
    done

    # generate include parameters for preprocess pharse later.
    include_dummy_system_headers=""
    readarray -d \ -t directories <<< $(find ${DUMMY_SYSTEM_HEADER_PATH} -type d)
    for directory in ${directories[*]}
    do
        include_dummy_system_headers="$include_dummy_system_headers -I${directory}"
    done
}