#!/bin/bash

runNumber=17366
echo ${runNumber}
read energy polar epoch <<< $(python getRunInfo.py ${runNumber})
echo ${energy} ${polar} ${epoch}
inputPre="/cache/clas12/rg-c/production/fall22/pass1/NH3/dst/train/sidisdvcs/sidisdvcs_0${runNumber}.hipo"
outputPre="Test/data_${runNumber}.root"
echo ${inputPre}
echo ${outputPre}
clas12root -l -b -q processData.C\(\"${inputPre}\"\,\"${outputPre}\"\,${energy}\,${polar}\,${epoch}\)
