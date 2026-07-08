#!/bin/bash

for runNumber in $(cat Files_NH3_fall22.txt)
do
	echo ${runNumber}
	read energy polar epoch <<< $(python getRunInfo.py ${runNumber})
	echo ${energy} ${polar} ${epoch}
	inputPre="/cache/clas12/rg-c/production/fall22/pass1/NH3/dst/train/sidisdvcs/sidisdvcs_0${runNumber}.hipo"
	outputPre="NH3/data_${runNumber}.root"
	echo ${inputPre}
	echo ${outputPre}
	clas12root -l -b -q processData.C\(\"${inputPre}\"\,\"${outputPre}\"\,${energy}\,${polar}\,${epoch}\)
done
