#!/bin/bash
#script to make lists
DIR_IN=/path/to/input_files/
LIST=list.txt

echo "DIR_IN = $DIR_IN"
echo "LIST = $LIST"

ls $DIR_IN |xargs -n 1 basename |cat -n >$LIST
#ls $DIR_IN/*messy.root |xargs -n 1 basename |cat -n >$LIST 
#xargs -n 1 basename: to getrid of fullpath
#cat -n to get the numbering

