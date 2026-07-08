#!/bin/bash

for i in $(cat Files_RGA.txt)
do
	inputPre="/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass2/main/train/nSidis/nSidis_00${i}.hipo"
	outputPre="RGA/data_${i}.root"
	echo ${inputPre}
	echo ${outputPre}
	clas12root -l -b -q processData.C\(\"${inputPre}\"\,\"${outputPre}\"\,10.6\,0\,0\)
done
