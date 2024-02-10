#!/bin/bash
set +x

optstring=""
searchdir="./"

while getopts dxl opt
do
    case $opt in
        *x*) optstring="$optstring""$opt" ;;
        *l*) optstring="$optstring""$opt" ;;
        *d*) optstring="$optstring""$opt" ;;
        *) printf >&2 "Unrecognized option $opt\n"; exit 1 ;;
    esac
done

echo directory
echo $searchdir

mask=""

if [[ $optstring =~ "x" ]] 
then
    mask="$mask""-"
fi

if [[ $optstring =~ "l" ]] 
then
    mask="$mask""l"
fi

if [[ $optstring =~ "d" ]]
then
    mask="$mask""d"
fi

echo mask
echo $mask

curopt=""


    #echo file permissions
for entry in "$search_rit"/*
do
    curopt=$(stat -c%A --format=%A "$entry")
    curopt=${curopt:0:1}
    
    case "$mask" in 
	*"$curopt"*)
#	     echo $entry
#	     echo $(stat -c%A --format=%A "$entry")
	    ;;
    esac
done