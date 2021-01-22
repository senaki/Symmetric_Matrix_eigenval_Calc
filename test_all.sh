#!/usr/bin/env bash
inputFolder="${PWD}/../input/";
for file in $(ls | sort $inputFolder) do
	./rotjacobi $file ;
done


