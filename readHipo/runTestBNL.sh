#!/bin/bash

runNumber=17366
echo ${runNumber}
read energy polar epoch <<< $(python getRunInfo.py ${runNumber})
echo ${energy} ${polar} ${epoch}
inputPre="/gpfs02/eic/ztu/CLAS12/forLi/sidisdvcs_0${runNumber}.hipo"
outputPre="Test/data_${runNumber}.root"
echo ${inputPre}
echo ${outputPre}
clas12root -l -b -q processData.C\(\"${inputPre}\"\,\"${outputPre}\"\,${energy}\,${polar}\,${epoch}\)
