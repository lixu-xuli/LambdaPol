#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include "hipo4/reader.h"
#include "QADB.h"

using namespace std;

void processQADB() {
	auto time_start = chrono::high_resolution_clock::now();
	TChain myChain;
	int run;
	string path;
	vector<int> runList;
	vector<string> typeList;
	ifstream inputFile1("Files_NH3_summer22.txt");
	while (inputFile1 >> run) {
		cout << run << endl;
		path = "/cache/clas12/rg-c/production/summer22/pass1/10.5gev/NH3/dst/train/sidisdvcs/";
		myChain.Add((path+"sidisdvcs_0"+to_string(run)+".hipo").c_str());
		runList.push_back(run);
		typeList.push_back("NH3");
	}
	ifstream inputFile2("Files_NH3_fall22.txt");
	while (inputFile2 >> run) {
		cout << run << endl;
		path = "/cache/clas12/rg-c/production/fall22/pass1/NH3/dst/train/sidisdvcs/";
		myChain.Add((path+"sidisdvcs_0"+to_string(run)+".hipo").c_str());
		runList.push_back(run);
		typeList.push_back("NH3");
	}
	ifstream inputFile3("Files_NH3_spring23.txt");
	while (inputFile3 >> run) {
		cout << run << endl;
		path = "/cache/clas12/rg-c/production/spring23/pass1/NH3/dst/train/sidisdvcs/";
		myChain.Add((path+"sidisdvcs_0"+to_string(run)+".hipo").c_str());
		runList.push_back(run);
		typeList.push_back("NH3");
	}
	ifstream inputFile4("Files_C.txt");
	while (inputFile4 >> run) {
		cout << run << endl;
		path = "/cache/clas12/rg-c/production/fall22/pass1/C/dst/train/sidisdvcs/";
		if (run < 16800)
			path = "/cache/clas12/rg-c/production/summer22/pass1/10.5gev/C/dst/train/sidisdvcs/";
		else if (run > 17450)
			path = "/cache/clas12/rg-c/production/spring23/pass1/C/dst/train/sidisdvcs/";
		myChain.Add((path+"sidisdvcs_0"+to_string(run)+".hipo").c_str());
		runList.push_back(run);
		typeList.push_back("C");
	}
	ifstream inputFile5("Files_CH2.txt");
	while (inputFile5 >> run) {
		cout << run << endl;
		path = "/cache/clas12/rg-c/production/fall22/pass1/CH2/dst/train/sidisdvcs/";
		if (run < 16800)
			path = "/cache/clas12/rg-c/production/summer22/pass1/10.5gev/CH2/dst/train/sidisdvcs/";
		else if (run > 17450)
			path = "/cache/clas12/rg-c/production/spring23/pass1/CH2/dst/train/sidisdvcs/";
		myChain.Add((path+"sidisdvcs_0"+to_string(run)+".hipo").c_str());
		runList.push_back(run);
		typeList.push_back("CH2");
	}
	ifstream inputFile6("Files_Empty.txt");
	while (inputFile6 >> run) {
		cout << run << endl;
		path = "/cache/clas12/rg-c/production/fall22/pass1/ET/dst/train/sidisdvcs/";
		if (run < 16800)
			path = "/cache/clas12/rg-c/production/summer22/pass1/10.5gev/ET/dst/train/sidisdvcs/";
		else if (run > 17450)
			path = "/cache/clas12/rg-c/production/spring23/pass1/ET/dst/train/sidisdvcs/";
		myChain.Add((path+"sidisdvcs_0"+to_string(run)+".hipo").c_str());
		runList.push_back(run);
		typeList.push_back("Empty");
	}
	ifstream inputFile7("Files_Foil.txt");
	while (inputFile7 >> run) {
		cout << run << endl;
		path = "/cache/clas12/rg-c/production/fall22/pass1/ET/dst/train/sidisdvcs/";
		if (run < 16800)
			path = "/cache/clas12/rg-c/production/summer22/pass1/10.5gev/ET/dst/train/sidisdvcs/";
		else if (run > 17450)
			path = "/cache/clas12/rg-c/production/spring23/pass1/ET/dst/train/sidisdvcs/";
		myChain.Add((path+"sidisdvcs_0"+to_string(run)+".hipo").c_str());
		runList.push_back(run);
		typeList.push_back("Foil");
	}
	auto files = myChain.GetListOfFiles();
	cout << "Size of files: " << files->GetLast()+1 << endl;
	ofstream outputFile;
	outputFile.open("FCup_qadb.txt");
	outputFile << "RunNumber, FCup_run, Type" << endl;
	for (int iFile=0; iFile<files->GetLast()+1; iFile++) {
		run = runList[iFile];
		cout << run << " " << files->At(iFile)->GetTitle() << endl;
		hipo::reader reader;
		reader.setTags(1);
		hipo::dictionary factory;
		hipo::event event;
		reader.open(files->At(iFile)->GetTitle());
		reader.readDictionary(factory);
		//factory.show();
		hipo::bank configBank(factory.getSchema("RUN::config"));
		int runNumber = 0;
		long eventNumber = 0;
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
		if (typeList[iFile] == "Empty" || typeList[iFile] == "Foil")
  			qa.CheckForDefect("Misc", false);
		else
  			qa.CheckForDefect("Misc", true);
		while (reader.next()) {
			reader.read(event);
			event.getStructure(configBank);
			runNumber = configBank.getInt("run", 0);
			eventNumber = configBank.getInt("event", 0);
			if(qa.Pass(runNumber, eventNumber)) {
				qa.AccumulateCharge();
			}
		}
		cout << runNumber << " " << qa.GetAccumulatedCharge() << " nC" << endl;
		outputFile << runNumber << " " << qa.GetAccumulatedCharge() << " " << typeList[iFile] << endl;
		if (run != runNumber) {
			cout << "Run number is wrong!" << endl;
			return;
		}
	}
	outputFile.close();
	auto time_end = chrono::high_resolution_clock::now();
	double elapsed_time_ms = chrono::duration<double, milli> (time_end-time_start).count();
	cout << elapsed_time_ms/1000 << endl;
}
