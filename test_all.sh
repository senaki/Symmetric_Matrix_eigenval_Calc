#!/usr/bin/env bash
inPath=$(readlink -f $1);
for fich in $(ls $1)
do
	 # echo "$inPath/${file}"
	 ./rotjacobi "$1/${fich}" $2 ;
done | sed -n -r 's/^.*[[:blank:]]Dim=([[:digit:]]+)[[:blank:]]Rotations=([[:digit:]]+)[[:blank:]]Duration=([[:digit:]]+) ms/\1\t\2\t\3/g;p' > $2/test_all.log
echo -e "Congratulations, it's all done !"
exit 0 ;
