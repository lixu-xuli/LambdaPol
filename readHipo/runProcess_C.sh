#!/bin/bash

for runNumber in $(cat Files_C.txt)
do
	echo ${runNumber}
	read energy polar epoch <<< $(python getRunInfo.py ${runNumber})
	echo ${energy} ${polar} ${epoch}
	inputPre="/cache/clas12/rg-c/production/fall22/pass1/C/dst/train/sidisdvcs/sidisdvcs_0${runNumber}.hipo"
	if [ "$runNumber" -lt 16800 ]; then
		inputPre="/cache/clas12/rg-c/production/summer22/pass1/10.5gev/C/dst/train/sidisdvcs/sidisdvcs_0${runNumber}.hipo"
	elif [ "$runNumber" -ge 17450 ]; then
		inputPre="/cache/clas12/rg-c/production/spring23/pass1/C/dst/train/sidisdvcs/sidisdvcs_0${runNumber}.hipo"
	fi
	outputPre="C/data_${runNumber}.root"
	echo ${inputPre}
	echo ${outputPre}
	clas12root -l -b -q processData.C\(\"${inputPre}\"\,\"${outputPre}\"\,${energy}\,${polar}\,${epoch}\)
done
