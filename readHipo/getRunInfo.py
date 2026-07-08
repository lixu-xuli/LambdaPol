import sys

inputNumber = int(sys.argv[1])
infoDict = {}

with open("RGC_Good_Runs_list.xlsx") as myFile:
    myFile.readline()
    for line in myFile:
        myList = line.split()
        runNumber = int(myList[0])
        energy = float(myList[3])
        polar = float(myList[6])
        epoch = int(myList[9][-2:])
        infoDict[runNumber] = (energy, polar, epoch)

#print(infoDict)
print(infoDict[inputNumber][0], infoDict[inputNumber][1], infoDict[inputNumber][2])
