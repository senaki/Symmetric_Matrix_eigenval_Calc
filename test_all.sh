#!/usr/bin/env bash
for fich in $1/*
do
	 # echo "$inPath/${file}"
	 ./rotjacobi ${fich} "$2/eigval_"$(basename ${fich}) ;
done | sed -n -r 's/^.*:[[:blank:]]([[:digit:]]+)[[:blank:]]([[:digit:]]+)[[:blank:]]([[:digit:]]+) ms/\1\t\2\t\3/g;p' > $2/test_all.log
echo -e "Congratulations, it's all done !"
exit 0 ;
