#!/bin/bash
set +x
#set -- ${1:-xyz} ${@:2:$#} ; echo $*

optstring=""
while getopts dlf opt
do
    case $opt in
	f) optstring="$optstring""$opt" ;; 
	d) optstring="$optstring""$opt" ;;
	l) optstring="$optstring""$opt" ;;
	*)	echo "unknown $opt"; exit 1 ;;
    esac    
done

firstdir="${1}"
lastdir="${@:$#:1}" 
searchdir="./"

echo firstdir
echo $firstdir
echo
echo lastdir
echo $lastdir
echo

if [[ -d $firstdir ]]
then
    searchdir=$firstdir 
else
    optstring="$optstring""$firstdir"
fi

if [[ -d $lastdir ]]
then
    searchdir=$lastdir
else
    optstring="$optstring""$lastdir"
fi

echo searchdir
echo $searchdir
echo 

#while getopts dfl opt
#do
#    case $opt in
#        *x*) optstring="$optstring""$opt" ;;
#        *l*) optstring="$optstring""$opt" ;;
#        *d*) optstring="$optstring""$opt" ;;
#        *) printf >&2 "Unrecognized option $opt\n"; exit 1 ;;
#    esac
#done

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
if [ mask == "" ]
then
    mask="ld-"
fi
echo 
    #echo file permissions
for entry in "$search_dir"	
do
    echo entry
    echo $entry
    curopt=$(stat -c%A --format=%A "$entry")
    curopt=${curopt:0:1}
    
    case "$mask" in 
	*"$curopt"*)
	     echo $entry
	#     echo $(stat -c%A --format=%A "$entry")
	    ;;
    esac
done