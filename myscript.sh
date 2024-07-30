#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Enter file name:"
    read file
    echo "Enter directory:"
    read dir
elif [ ! -d "$2" ]; then
    echo "Invalid directory."
    exit 1
else
    file="$1"
    dir="$2"
fi

cd "$dir" || exit 1
cat $(ls | grep .txt) > "$file"

exit 0
