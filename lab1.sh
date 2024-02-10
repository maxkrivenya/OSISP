#!/bin/bash
optstring="ldf"
curopt=""
search_dir=./

    #list
#for entry in "$search_rit"/*
#do
#    echo "$entry"
#done

    #echo getopt
#while getopts ${optstring} curopt
#do
#    echo $curopt
#done


    #echo file permissions
for entry in "$search_rit"/*
do
#    curopt= $((stat -c%A  $entry))
    curopt = $(stat -c%A $entry)
    echo $curopt
    firstlet=${curopt:0:1}
    echo $firstlet
#    if [ "$curopt" = "d" ]
#    then
#	echo "$entry"
#    fi
done