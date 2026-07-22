#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include "readBank.h"
#include "saveLambda.h"
#include "QADB.h"

using namespace std;

void processData(string inputFile, string outputFile, Float_t EBeam, Float_t targetPol, Int_t epoch) {
	auto time_start = chrono::high_resolution_clock::now();
	TChain myChain;
	myChain.Add(inputFile.c_str());
	auto files = myChain.GetListOfFiles();
	cout << files->At(0)->GetTitle() << endl;
	hipo::reader reader;
	reader.open(files->At(0)->GetTitle());
	hipo::dictionary factory;
	reader.readDictionary(factory);
	//factory.show();
	hipo::bank configBank(factory.getSchema("RUN::config"));
	hipo::bank eventBank(factory.getSchema("REC::Event"));
	hipo::bank particleBank(factory.getSchema("REC::Particle"));
	hipo::bank calBank(factory.getSchema("REC::Calorimeter"));
	hipo::bank trajBank(factory.getSchema("REC::Traj"));
	hipo::bank trackBank(factory.getSchema("REC::Track"));
	hipo::bank sciBank(factory.getSchema("REC::Scintillator"));
	hipo::bank cheBank(factory.getSchema("REC::Cherenkov"));
	hipo::bank richBank(factory.getSchema("RICH::Particle"));
	hipo::event event;
	TFile myFile(outputFile.c_str(), "recreate");
	recLambdaTree myLambdaTree("recTree");
	int iEvent = 0;
	QA::QADB qa("latest");
  	qa.CheckForDefect("TotalOutlier", true);
  	qa.CheckForDefect("TerminalOutlier", true);
  	qa.CheckForDefect("MarginalOutlier", true);
  	qa.CheckForDefect("SectorLoss", true);
  	qa.CheckForDefect("LowLiveTime", true);
  	qa.CheckForDefect("ChargeHigh", true);
  	qa.CheckForDefect("ChargeNegative", true);
  	qa.CheckForDefect("ChargeUnknown", true);
  	qa.CheckForDefect("PossiblyNoBeam", true);
	if (epoch == -1)
  		qa.CheckForDefect("Misc", false);
	else
  		qa.CheckForDefect("Misc", true);
	while (reader.next()) {
	//while (reader.next() && iEvent < 1000) {
		reader.read(event);
		event.getStructure(configBank);
		event.getStructure(eventBank);
		event.getStructure(particleBank);
		event.getStructure(calBank);
		event.getStructure(trajBank);
		event.getStructure(trackBank);
		event.getStructure(sciBank);
		event.getStructure(cheBank);
		event.getStructure(richBank);
		globalInfo myGlobalInfo = readGlobalInfo(configBank, eventBank, iEvent, EBeam, targetPol, epoch);
		iEvent ++;
		if(!qa.Pass(myGlobalInfo.runNumber, myGlobalInfo.eventNumber))  continue;
		eventInfo myEventInfo = readEventInfo(particleBank);
		myLambdaTree.initialEvent(myGlobalInfo, myEventInfo);
		vector<recParticle> elVect;
		vector<recParticle> kaVect;
		vector<recParticle> prVect;
		vector<recParticle> phVect;
		readRecLambda(particleBank, elVect, kaVect, prVect, phVect);
		vector<elDeteInfo> elDeteVect;
		vector<kaDeteInfo> kaDeteVect;
		vector<prDeteInfo> prDeteVect;
		readElDeteBank(calBank, trajBank, trackBank, sciBank, cheBank, phVect, elVect, elDeteVect);
		readKaDeteBank(calBank, trajBank, trackBank, sciBank, cheBank, richBank, kaVect, kaDeteVect);
		readPrDeteBank(calBank, trajBank, trackBank, sciBank, cheBank, prVect, prDeteVect);
		myLambdaTree.fill(elVect, kaVect, prVect, elDeteVect, kaDeteVect, prDeteVect);
	}
	myLambdaTree.write();
	myFile.Close();
	auto time_end = chrono::high_resolution_clock::now();
	double elapsed_time_ms = chrono::duration<double, milli> (time_end-time_start).count();
	cout << elapsed_time_ms/1000 << endl;
}
