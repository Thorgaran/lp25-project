# lp25-repo_analyzer

## Overview

This program recursively scans a directory, and records its tree structure in a text file. 
Some data concerning the files is tracked, such as the last modification time or the MD5 sum.

## Building (linux)

Make sure that you have OpenSSL installed.  
Then, run `make repo_analyzer clean`.

## Usage

You can run the executable with the following (optional) arguments:
 - **-i** \<path\>: Path to the directory to analyze. If unspecified, the program will scan the current working directory.
 - **-o** \<path\>: Path to the output text file. If unspecified, the program will save the scan in `~/.filescanner/`.
 - **-s**: Whether or not to track the files' MD5 sum. Computing this sum may be a little slow.
