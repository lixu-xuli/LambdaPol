#ifndef _SAVELAMBDA_H
#define _SAVELAMBDA_H

#include "saveTree.h"

class preEvent
{
	public:
		preEvent() = default;
		void initial(globalInfo paraGlobalInfo, eventInfo paraEventInfo) {
			myGlobalInfo = paraGlobalInfo;
			myEventInfo = paraEventInfo;
			elBeam.SetPxPyPzE(0, 0, myGlobalInfo.EBeam, myGlobalInfo.EBeam);
			target = prTarget;
		}
		globalInfo myGlobalInfo{};
		eventInfo myEventInfo{};
		ROOT::Math::PxPyPzEVector elBeam{};
		ROOT::Math::PxPyPzEVector target{};
};

class preLambdaEvent: public preEvent
{
	public:
		preLambdaEvent() = default;
		bool findBest(const vector<recParticle> &elVect, const vector<recParticle> &kaVect, const vector<recParticle> &prVect, const vector<elDeteInfo> &elDeteVect, const vector<kaDeteInfo> &kaDeteVect, const vector<prDeteInfo> &prDeteVect) {
			Int_t nElectron = elVect.size();
			Int_t nKaon = kaVect.size();
			Int_t nProton = prVect.size();
			Float_t exclChi2 = 1e9;
			Int_t elIndex = -1;
			Int_t kaIndex = -1;
			Int_t prIndex = -1;
			for (Int_t iEl=0; iEl<nElectron; iEl++) {
				if (elVect[iEl].P < 1)  continue;
				for (Int_t iKa=0; iKa<nKaon; iKa++) {
					if (kaVect[iKa].P < 0.3)  continue;
					for (Int_t iPr=0; iPr<nProton; iPr++) {
						if (prVect[iPr].P < 0.3)  continue;
						Float_t newExclChi = getExclChi2(elVect[iEl].PVec, kaVect[iKa].PVec, prVect[iPr].PVec);
						if (newExclChi < exclChi2) {
							exclChi2 = newExclChi;
							elIndex = iEl;
							kaIndex = iKa;
							prIndex = iPr;
						}
					}
				}
			}
			if (elIndex!=-1 && kaIndex!=-1 && prIndex!=-1) {
				elPart = elVect[elIndex];
				kaPart = kaVect[kaIndex];
				prPart = prVect[prIndex];
				elDete = elDeteVect[elIndex];
				kaDete = kaDeteVect[kaIndex];
				prDete = prDeteVect[prIndex];
				return true;
			}
			return false;
		}
		virtual Float_t getExclChi2(const ROOT::Math::PxPyPzEVector &elPVec, const ROOT::Math::PxPyPzEVector &kaPVec, const ROOT::Math::PxPyPzEVector &prPVec) = 0;
		recParticle elPart{};
		recParticle kaPart{};
		recParticle prPart{};
		elDeteInfo elDete{};
		kaDeteInfo kaDete{};
		prDeteInfo prDete{};
};

class recLambdaEvent: public preLambdaEvent
{
	public:
		recLambdaEvent() = default;
		bool setEvent(const vector<recParticle> &elVect, const vector<recParticle> &kaVect, const vector<recParticle> &prVect, const vector<elDeteInfo> &elDeteVect, const vector<kaDeteInfo> &kaDeteVect, const vector<prDeteInfo> &prDeteVect) {
			if (findBest(elVect, kaVect, prVect, elDeteVect, kaDeteVect, prDeteVect)) {
				lamNoPi bestLamNoPi(elBeam, target, elPart.PVec, kaPart.PVec, prPart.PVec);
				myRecLamNoPi = bestLamNoPi;
				if (myRecLamNoPi.misLamM2 < -1)  return false;
				if (myRecLamNoPi.misLamM2 > 5)  return false;
				if (myRecLamNoPi.misPiM2 < -0.5)  return false;
				if (myRecLamNoPi.misPiM2 > 0.5)  return false;
				return true;
			}
			return false;
		}
		Float_t getExclChi2(const ROOT::Math::PxPyPzEVector &elPVec, const ROOT::Math::PxPyPzEVector &kaPVec, const ROOT::Math::PxPyPzEVector &prPVec) {
			ROOT::Math::PxPyPzEVector misLam = elBeam + target - elPVec - kaPVec;
			ROOT::Math::PxPyPzEVector misPi = elBeam + target - elPVec - kaPVec - prPVec;
			return pow(misLam.M2()-lambdaMass*lambdaMass, 2) + pow(misPi.M2()-pionMass*pionMass, 2);
		}
		lamNoPi myRecLamNoPi{};
};

class recPipiBkgEvent: public preLambdaEvent
{
	public:
		recPipiBkgEvent() = default;
		bool setEvent(const vector<recParticle> &elVect, const vector<recParticle> &kaVect, const vector<recParticle> &prVect, const vector<elDeteInfo> &elDeteVect, const vector<kaDeteInfo> &kaDeteVect, const vector<prDeteInfo> &prDeteVect) {
			if (findBest(elVect, kaVect, prVect, elDeteVect, kaDeteVect, prDeteVect)) {
				pipiBkg bestPipiBkg(elBeam, target, elPart.PVec, getKaPVec(kaPart.PVec), prPart.PVec, kaPart.PVec);
				myRecPipiBkg = bestPipiBkg;
				if (myRecPipiBkg.misLamM2 < -1)  return false;
				if (myRecPipiBkg.misLamM2 > 5)  return false;
				if (myRecPipiBkg.misPiM2 < -0.5)  return false;
				if (myRecPipiBkg.misPiM2 > 0.5)  return false;
				return true;
			}
			return false;
		}
		Float_t getExclChi2(const ROOT::Math::PxPyPzEVector &elPVec, const ROOT::Math::PxPyPzEVector &kaPVec, const ROOT::Math::PxPyPzEVector &prPVec) {
			ROOT::Math::PxPyPzEVector newKaPVec = getKaPVec(kaPVec);
			ROOT::Math::PxPyPzEVector misLam = elBeam + target - elPVec - newKaPVec;
			ROOT::Math::PxPyPzEVector misPi = elBeam + target - elPVec - newKaPVec - prPVec;
			return pow(misLam.M2()-lambdaMass*lambdaMass, 2) + pow(misPi.M2()-pionMass*pionMass, 2);
		}
		static ROOT::Math::PxPyPzEVector getKaPVec(const ROOT::Math::PxPyPzEVector &paraPip) {
			ROOT::Math::PxPyPzEVector paraKa = paraPip;
			paraKa.SetE(TMath::Sqrt(paraPip.P2() + kaonMass*kaonMass));
			return paraKa;
		}
		pipiBkg myRecPipiBkg{};
};

class mcLambdaEvent: public preEvent
{
	public:
		mcLambdaEvent() = default;
		bool setEvent(const vector<mcParticle> &mcPartVect) {
			Int_t nParticle = mcPartVect.size();
			if (nParticle != 4) {
				cout << myGlobalInfo.eventNumber << " " << myGlobalInfo.hipoIndex << endl;
				cout << "Error: Number of MC particles is " << mcPartVect.size() << endl;
				return false;
			}
			elPart = mcPartVect[0];
			kaPart = mcPartVect[1];
			prPart = mcPartVect[2];
			piPart = mcPartVect[3];
			lamHasPi myLamHasPi(elBeam, target, elPart.PVec, kaPart.PVec, prPart.PVec, piPart.PVec);
			myMCLamHasPi = myLamHasPi;
			return true;
		}
		lamHasPi myMCLamHasPi{};
		mcParticle elPart{};
		mcParticle kaPart{};
		mcParticle prPart{};
		mcParticle piPart{};
};

class mcSigmaEvent: public preEvent
{
	public:
		mcSigmaEvent() = default;
		bool setEvent(const vector<mcParticle> &mcPartVect) {
			Int_t nParticle = mcPartVect.size();
			if (nParticle != 5) {
				cout << myGlobalInfo.eventNumber << " " << myGlobalInfo.hipoIndex << endl;
				cout << "Error: Number of MC particles is " << mcPartVect.size() << endl;
				return false;
			}
			elPart = mcPartVect[0];
			kaPart = mcPartVect[1];
			prPart = mcPartVect[2];
			piPart = mcPartVect[3];
			phPart = mcPartVect[4];
			sigmaZero mySigmaZero(elBeam, target, elPart.PVec, kaPart.PVec, prPart.PVec, piPart.PVec, phPart.PVec);
			myMCSigmaZero = mySigmaZero;
			return true;
		}
		sigmaZero myMCSigmaZero{};
		mcParticle elPart{};
		mcParticle kaPart{};
		mcParticle prPart{};
		mcParticle piPart{};
		mcParticle phPart{};
};

class recLambdaTree: public preTree
{
	public:
		recLambdaTree(TString name): preTree(name) {
			addBranch();
		}
		void addBranch() {
			addGlobalBranch(&recEvent.myGlobalInfo);
			addEventBranch(&recEvent.myEventInfo);
			addRecPartBranch("el", &recEvent.elPart);
			addElDeteBranch("el", &recEvent.elDete);
			addRecPartBranch("ka", &recEvent.kaPart);
			addKaDeteBranch("ka", &recEvent.kaDete);
			addRecPartBranch("pr", &recEvent.prPart);
			addPrDeteBranch("pr", &recEvent.prDete);
			addLamNoPiBranch("", &recEvent.myRecLamNoPi);
		}
		void initialEvent(globalInfo paraGlobalInfo, eventInfo paraEventInfo) {
			recEvent.initial(paraGlobalInfo, paraEventInfo);
		}
		void fill(const vector<recParticle> &elVect, const vector<recParticle> &kaVect, const vector<recParticle> &prVect, const vector<elDeteInfo> &elDeteVect, const vector<kaDeteInfo> &kaDeteVect, const vector<prDeteInfo> &prDeteVect) {
			if (recEvent.setEvent(elVect, kaVect, prVect, elDeteVect, kaDeteVect, prDeteVect)) {
				myTTree.Fill();
			}
		}
		recLambdaEvent recEvent{};
};

class recPipiBkgTree: public preTree
{
	public:
		recPipiBkgTree(TString name): preTree(name) {
			addBranch();
		}
		void addBranch() {
			addGlobalBranch(&recEvent.myGlobalInfo);
			addEventBranch(&recEvent.myEventInfo);
			addRecPartBranch("el", &recEvent.elPart);
			addElDeteBranch("el", &recEvent.elDete);
			addRecPartBranch("ka", &recEvent.kaPart);
			addKaDeteBranch("ka", &recEvent.kaDete);
			addRecPartBranch("pr", &recEvent.prPart);
			addPrDeteBranch("pr", &recEvent.prDete);
			addPipiBkgBranch("", &recEvent.myRecPipiBkg);
		}
		void initialEvent(globalInfo paraGlobalInfo, eventInfo paraEventInfo) {
			recEvent.initial(paraGlobalInfo, paraEventInfo);
		}
		void fill(const vector<recParticle> &elVect, const vector<recParticle> &kaVect, const vector<recParticle> &prVect, const vector<elDeteInfo> &elDeteVect, const vector<kaDeteInfo> &kaDeteVect, const vector<prDeteInfo> &prDeteVect) {
			if (recEvent.setEvent(elVect, kaVect, prVect, elDeteVect, kaDeteVect, prDeteVect)) {
				myTTree.Fill();
			}
		}
		recPipiBkgEvent recEvent{};
};

class mcLambdaTree: public preTree
{
	public:
		mcLambdaTree(TString name): preTree(name) {
			addBranch();
		}
		void addBranch() {
			addGlobalBranch(&mcEvent.myGlobalInfo);
			addMCPartBranch("true_el", &mcEvent.elPart);
			addMCPartBranch("true_ka", &mcEvent.kaPart);
			addMCPartBranch("true_pr", &mcEvent.prPart);
			addMCPartBranch("true_pi", &mcEvent.piPart);
			addLamHasPiBranch("true_", &mcEvent.myMCLamHasPi);
		}
		void initialEvent(globalInfo paraGlobalInfo) {
			eventInfo dummy{};
			mcEvent.initial(paraGlobalInfo, dummy);
		}
		void fill(const vector<mcParticle> &mcPartVect) {
			if (mcEvent.setEvent(mcPartVect)) {
				myTTree.Fill();
			}
		}
		mcLambdaEvent mcEvent{};
};

class mcRecLambdaTree: public preTree
{
	public:
		mcRecLambdaTree(TString name): preTree(name) {
			addBranch();
		}
		void addBranch() {
			addGlobalBranch(&recEvent.myGlobalInfo);
			addEventBranch(&recEvent.myEventInfo);
			addRecPartBranch("el", &recEvent.elPart);
			addElDeteBranch("el", &recEvent.elDete);
			addRecPartBranch("ka", &recEvent.kaPart);
			addKaDeteBranch("ka", &recEvent.kaDete);
			addRecPartBranch("pr", &recEvent.prPart);
			addPrDeteBranch("pr", &recEvent.prDete);
			addLamNoPiBranch("", &recEvent.myRecLamNoPi);
			addMCPartBranch("true_el", &mcEvent.elPart);
			addMCPartBranch("true_ka", &mcEvent.kaPart);
			addMCPartBranch("true_pr", &mcEvent.prPart);
			addMCPartBranch("true_pi", &mcEvent.piPart);
			addLamHasPiBranch("true_", &mcEvent.myMCLamHasPi);
		}
		void initialEvent(globalInfo paraGlobalInfo, eventInfo paraEventInfo) {
			recEvent.initial(paraGlobalInfo, paraEventInfo);
			mcEvent.initial(paraGlobalInfo, paraEventInfo);
		}
		void fill(const vector<mcParticle> &mcVect, const vector<recParticle> &elVect, const vector<recParticle> &kaVect, const vector<recParticle> &prVect, const vector<elDeteInfo> &elDeteVect, const vector<kaDeteInfo> &kaDeteVect, const vector<prDeteInfo> &prDeteVect) {
			if (recEvent.setEvent(elVect, kaVect, prVect, elDeteVect, kaDeteVect, prDeteVect)) {
				if (mcEvent.setEvent(mcVect)) {
					myTTree.Fill();
				}
			}
		}
		recLambdaEvent recEvent{};
		mcLambdaEvent mcEvent{};
};

class mcSigmaTree: public preTree
{
	public:
		mcSigmaTree(TString name): preTree(name) {
			addBranch();
		}
		void addBranch() {
			addGlobalBranch(&mcEvent.myGlobalInfo);
			addMCPartBranch("true_el", &mcEvent.elPart);
			addMCPartBranch("true_ka", &mcEvent.kaPart);
			addMCPartBranch("true_pr", &mcEvent.prPart);
			addMCPartBranch("true_pi", &mcEvent.piPart);
			addMCPartBranch("true_ph", &mcEvent.phPart);
			addSigmaZeroBranch("true_", &mcEvent.myMCSigmaZero);
		}
		void initialEvent(globalInfo paraGlobalInfo) {
			eventInfo dummy{};
			mcEvent.initial(paraGlobalInfo, dummy);
		}
		void fill(const vector<mcParticle> &mcPartVect) {
			if (mcEvent.setEvent(mcPartVect)) {
				myTTree.Fill();
			}
		}
		mcSigmaEvent mcEvent{};
};

class mcRecSigmaTree: public preTree
{
	public:
		mcRecSigmaTree(TString name): preTree(name) {
			addBranch();
		}
		void addBranch() {
			addGlobalBranch(&recEvent.myGlobalInfo);
			addEventBranch(&recEvent.myEventInfo);
			addRecPartBranch("el", &recEvent.elPart);
			addElDeteBranch("el", &recEvent.elDete);
			addRecPartBranch("ka", &recEvent.kaPart);
			addKaDeteBranch("ka", &recEvent.kaDete);
			addRecPartBranch("pr", &recEvent.prPart);
			addPrDeteBranch("pr", &recEvent.prDete);
			addLamNoPiBranch("", &recEvent.myRecLamNoPi);
			addMCPartBranch("true_el", &mcEvent.elPart);
			addMCPartBranch("true_ka", &mcEvent.kaPart);
			addMCPartBranch("true_pr", &mcEvent.prPart);
			addMCPartBranch("true_pi", &mcEvent.piPart);
			addMCPartBranch("true_ph", &mcEvent.phPart);
			addSigmaZeroBranch("true_", &mcEvent.myMCSigmaZero);
		}
		void initialEvent(globalInfo paraGlobalInfo, eventInfo paraEventInfo) {
			recEvent.initial(paraGlobalInfo, paraEventInfo);
			mcEvent.initial(paraGlobalInfo, paraEventInfo);
		}
		void fill(const vector<mcParticle> &mcVect, const vector<recParticle> &elVect, const vector<recParticle> &kaVect, const vector<recParticle> &prVect, const vector<elDeteInfo> &elDeteVect, const vector<kaDeteInfo> &kaDeteVect, const vector<prDeteInfo> &prDeteVect) {
			if (recEvent.setEvent(elVect, kaVect, prVect, elDeteVect, kaDeteVect, prDeteVect)) {
				if (mcEvent.setEvent(mcVect)) {
					myTTree.Fill();
				}
			}
		}
		recLambdaEvent recEvent{};
		mcSigmaEvent mcEvent{};
};

#endif
