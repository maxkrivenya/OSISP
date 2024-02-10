#!/bin/bash
optstring="ldf"
curopt=""
search_dir=./

#for entry in "$search_rit"/*
#do
#    echo "$entry"
#done
while getopts ${optstring} curopt
do
    echo $curopt
done
