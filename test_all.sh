#!/usr/bin/env bash
inPath=$(readlink -f $1);
for file in $(dir $1)
do
	#  echo ""
	 ./rotjacobi "$(readlink -f ${file})" $2 ;
done
echo -e "Congratulations, it's all done !"
exit 0 ;
