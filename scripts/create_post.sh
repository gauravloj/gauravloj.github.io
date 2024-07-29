#!/bin/sh -e

# Create a new post with the given title
# in the current directory
# Note: The title should be in kebab-case
# and without extension

if [ -z "$1" ]; then
    echo "Usage: $0 <post-title>"
    exit 1
fi

file_name="$(date "+%Y-%m-%d")-$1.md"

if [ -e "$file_name" ]; then
    echo "File already exists: $file_name"
    exit 1
fi


echo "---
title: ${1//[-]/ }
time: $(date "+%Y-%m-%d %H:%M:%S")
categories: [general, default]
tags: [default]
---
" > "$file_name"

echo "$file_name created successfully"

