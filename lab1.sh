#!/bin/bash
set +x

optstring="-dl"
curopt=""
search_dir="./"


    #echo file permissions
for entry in "$search_rit"/*
do
    curopt=$(stat -c%A --format=%A "$entry")
    curopt=${curopt:0:1}
    
#    if [ "$optstring"==*"$curopt"* ]
#    then
#	echo "$entry"
#    fi

    case "$optstring" in 
	*"$curopt"*)
	     echo $entry
	     echo $(stat -c%A --format=%A "$entry")
	    ;;
    esac
done