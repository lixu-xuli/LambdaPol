#!/bin/bash

for runNumber in $(cat Files_NH3_summer22.txt)
do
	echo ${runNumber}
	read energy polar epoch <<< $(python getRunInfo.py ${runNumber})
	echo ${energy} ${polar} ${epoch}
	inputPre="/cache/clas12/rg-c/production/summer22/pass1/10.5gev/NH3/dst/train/sidisdvcs/sidisdvcs_0${runNumber}.hipo"
	outputPre="NH3/data_${runNumber}.root"
	echo ${inputPre}
	echo ${outputPre}
	clas12root -l -b -q processData.C\(\"${inputPre}\"\,\"${outputPre}\"\,${energy}\,${polar}\,${epoch}\)
done
