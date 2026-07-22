#ifndef _READBANK_H
#define _READBANK_H

#include "hipo4/reader.h"
#include "info.h"

globalInfo readGlobalInfo(const hipo::bank &configBank, const hipo::bank &eventBank, const Int_t &index, const Float_t &EBeam, const Float_t &targetPol, const Int_t &epoch) {
	globalInfo myInfo{};
	myInfo.runNumber = configBank.getInt("run", 0);
	myInfo.eventNumber = configBank.getInt("event", 0);
	myInfo.helicity = eventBank.getInt("helicity", 0);
	myInfo.torus = configBank.getFloat("torus", 0);
	myInfo.solenoid = configBank.getFloat("solenoid", 0);
	myInfo.hipoIndex = index;
	myInfo.EBeam = EBeam;
	myInfo.targetPol = targetPol;
	myInfo.epoch = epoch;
	return myInfo;
}

eventInfo readEventInfo(const hipo::bank &particleBank) {
	eventInfo myInfo{};
	Int_t nParticle = particleBank.getRows();
	for (Int_t iPart=0; iPart<nParticle; iPart++) {
		Int_t pid = particleBank.getInt("pid", iPart);
		if (pid == 11)  myInfo.nElectron ++;
		else if (pid == 2212)  myInfo.nProton ++;
		else if (pid == 2112)  myInfo.nNeutron ++;
		else if (pid == 22)  myInfo.nPhoton ++;
		else if (pid == 211)  myInfo.nPionP ++;
		else if (pid == -211)  myInfo.nPionM ++;
		else if (pid == 321)  myInfo.nKaonP ++;
		else if (pid == -321)  myInfo.nKaonM ++;
		else {
			myInfo.nOther ++;
			if (myInfo.numOther < 10) {
				myInfo.pidOther[myInfo.numOther] = pid;
				myInfo.numOther ++;
			}
		}
	}
	return myInfo;
}

recPartInfo readRecParticle(const hipo::bank &particleBank, Int_t iPart) {
	recPartInfo myInfo{};
	myInfo.pid = particleBank.getInt("pid", iPart);
	myInfo.px = particleBank.getFloat("px", iPart);
	myInfo.py = particleBank.getFloat("py", iPart);
	myInfo.pz = particleBank.getFloat("pz", iPart);
	myInfo.vx = particleBank.getFloat("vx", iPart);
	myInfo.vy = particleBank.getFloat("vy", iPart);
	myInfo.vz = particleBank.getFloat("vz", iPart);
	myInfo.vt = particleBank.getFloat("vt", iPart);
	myInfo.charge = particleBank.getInt("charge", iPart);
	myInfo.beta = particleBank.getFloat("beta", iPart);
	myInfo.chi2pid = particleBank.getFloat("chi2pid", iPart);
	myInfo.status = particleBank.getInt("status", iPart);
	myInfo.pindex = iPart;
	return myInfo;
}

Int_t readRecMatch(const hipo::bank &matchBank, Int_t iPart) {
	return matchBank.getInt("mcindex", iPart);
}

void addPartLambda(vector<recParticle> &elVect, vector<recParticle> &kaVect, vector<recParticle> &prVect, vector<recParticle> &phVect, recPartInfo paraInfo, Int_t mcindex = iniVal) {
	if (paraInfo.pid == 0)  return;
	if (abs(paraInfo.px) < 1e-6 && abs(paraInfo.py) < 1e-6 && abs(paraInfo.pz) < 1e-6)  return;
	if (paraInfo.beta < 0)  return;
	recParticle newPart(paraInfo, mcindex);
	if (paraInfo.charge==-1 && paraInfo.pid==11)
	    elVect.emplace_back(newPart);
	else if (paraInfo.charge==1 && paraInfo.pid==321)
	    kaVect.emplace_back(newPart);
	else if (paraInfo.charge==1 && paraInfo.pid==2212)
	    prVect.emplace_back(newPart);
	else if (paraInfo.charge==0 && paraInfo.pid==22)
	    phVect.emplace_back(newPart);
}

void readRecLambda(const hipo::bank &particleBank, vector<recParticle> &elVect, vector<recParticle> &kaVect, vector<recParticle> &prVect, vector<recParticle> &phVect) {
	elVect.clear();
	kaVect.clear();
	prVect.clear();
	phVect.clear();
	Int_t nParticle = particleBank.getRows();
	for (Int_t iPart=0; iPart<nParticle; iPart++) {
		recPartInfo myInfo = readRecParticle(particleBank, iPart);
		addPartLambda(elVect, kaVect, prVect, phVect, myInfo);
	}
}

void readRecLambda(const hipo::bank &particleBank, const hipo::bank &matchBank, vector<recParticle> &elVect, vector<recParticle> &kaVect, vector<recParticle> &prVect, vector<recParticle> &phVect) {
	elVect.clear();
	kaVect.clear();
	prVect.clear();
	phVect.clear();
	Int_t nParticle = particleBank.getRows();
	for (Int_t iPart=0; iPart<nParticle; iPart++) {
		recPartInfo myInfo = readRecParticle(particleBank, iPart);
		Int_t mcindex = readRecMatch(matchBank, iPart);
		addPartLambda(elVect, kaVect, prVect, phVect, myInfo, mcindex);
	}
}

void addPartPipiBkg(vector<recParticle> &elVect, vector<recParticle> &prVect, vector<recParticle> &pipVect, vector<recParticle> &phVect, recPartInfo paraInfo, Int_t mcindex = iniVal) {
	if (paraInfo.pid == 0)  return;
	if (abs(paraInfo.px) < 1e-6 && abs(paraInfo.py) < 1e-6 && abs(paraInfo.pz) < 1e-6)  return;
	if (paraInfo.beta < 0)  return;
	recParticle newPart(paraInfo, mcindex);
	if (paraInfo.charge==-1 && paraInfo.pid==11)
	    elVect.emplace_back(newPart);
	else if (paraInfo.charge==1 && paraInfo.pid==2212)
	    prVect.emplace_back(newPart);
	else if (paraInfo.charge==1 && paraInfo.pid==211)
	    pipVect.emplace_back(newPart);
	else if (paraInfo.charge==0 && paraInfo.pid==22)
	    phVect.emplace_back(newPart);
}

void readRecPipiBkg(const hipo::bank &particleBank, vector<recParticle> &elVect, vector<recParticle> &prVect, vector<recParticle> &pipVect, vector<recParticle> &phVect) {
	elVect.clear();
	prVect.clear();
	pipVect.clear();
	phVect.clear();
	Int_t nParticle = particleBank.getRows();
	for (Int_t iPart=0; iPart<nParticle; iPart++) {
		recPartInfo myInfo = readRecParticle(particleBank, iPart);
		addPartPipiBkg(elVect, prVect, pipVect, phVect, myInfo);
	}
}

void readMCParticleBank(const hipo::bank &particleBank, const hipo::bank &matchBank, vector<mcParticle> &mcPartVect) {
	mcPartVect.clear();
	Int_t nParticle = particleBank.getRows();
	for (Int_t iPart=0; iPart<nParticle; iPart++) {
		mcPartInfo myInfo{};
		myInfo.pid = particleBank.getInt("pid", iPart);
		myInfo.px = particleBank.getFloat("px", iPart);
		myInfo.py = particleBank.getFloat("py", iPart);
		myInfo.pz = particleBank.getFloat("pz", iPart);
		myInfo.vx = particleBank.getFloat("vx", iPart);
		myInfo.vy = particleBank.getFloat("vy", iPart);
		myInfo.vz = particleBank.getFloat("vz", iPart);
		myInfo.vt = particleBank.getFloat("vt", iPart);
		myInfo.mcindex = iPart;
		Int_t pindex = matchBank.getInt("pindex", iPart);
		mcParticle newPart(myInfo, pindex);
        mcPartVect.emplace_back(newPart);
	}
}

Int_t trajSector(Float_t phi) {
    if (phi < 30 && phi >= -30)
        return 1;
    else if (phi < 90 && phi >= 30)
        return 2;
    else if (phi < 150 && phi >= 90)
        return 3;
    else if (phi >= 150 || phi < -150)
        return 4;
    else if (phi < -90 && phi >= -150)
        return 5;
    else if (phi < -30 && phi >= -90)
        return 6;
    return 0;
}

void readCALInfo(const hipo::bank &calBank, Int_t pindex, Int_t (&sectorCAL)[3], Float_t (&energyCAL)[3], Float_t (&xCAL)[3], Float_t (&yCAL)[3], Float_t (&zCAL)[3], Float_t (&luCAL)[3], Float_t (&lvCAL)[3], Float_t (&lwCAL)[3], Float_t (&thetaCAL)[3], Float_t (&phiCAL)[3], Float_t (&timeCAL)[3], Float_t (&pathCAL)[3], Float_t &energySumCAL) {
	Int_t nCal = calBank.getRows();
	for (Int_t iCal=0; iCal<nCal; iCal++) {
		Int_t pindexCal = calBank.getInt("pindex", iCal);
		if (pindexCal != pindex)  continue;
		Int_t layerCal = calBank.getInt("layer", iCal);
		Int_t iLayer = -1;
		if (layerCal == 1)  iLayer = 0;
		else if (layerCal == 4)  iLayer = 1;
		else if (layerCal == 7)  iLayer = 2;
		else  continue;
		sectorCAL[iLayer] = calBank.getInt("sector", iCal);
		energyCAL[iLayer] = calBank.getFloat("energy", iCal);
		xCAL[iLayer] = calBank.getFloat("x", iCal);
		yCAL[iLayer] = calBank.getFloat("y", iCal);
		zCAL[iLayer] = calBank.getFloat("z", iCal);
		luCAL[iLayer] = calBank.getFloat("lu", iCal);
		lvCAL[iLayer] = calBank.getFloat("lv", iCal);
		lwCAL[iLayer] = calBank.getFloat("lw", iCal);
		thetaCAL[iLayer] = getTheta(xCAL[iLayer], yCAL[iLayer], zCAL[iLayer]);
		phiCAL[iLayer] = getPhi(xCAL[iLayer], yCAL[iLayer]);
		timeCAL[iLayer] = calBank.getFloat("time", iCal);
		pathCAL[iLayer] = calBank.getFloat("path", iCal);
	}
	energySumCAL = energyCAL[0] + energyCAL[1] + energyCAL[2];
}

void readDCInfo(const hipo::bank &trajBank, Int_t pindex, Int_t (&sectorDC)[3], Float_t (&xDC)[3], Float_t (&yDC)[3], Float_t (&zDC)[3], Float_t (&thetaDC)[3], Float_t (&phiDC)[3], Float_t (&edgeDC)[3]) {
	Int_t nTraj = trajBank.getRows();
	for (Int_t iTraj=0; iTraj<nTraj; iTraj++) {
		Int_t pindexTraj = trajBank.getInt("pindex", iTraj);
		if (pindexTraj != pindex)  continue;
		Int_t detectorTraj = trajBank.getInt("detector", iTraj);
		if (detectorTraj != 6)  continue;
		Int_t layerTraj = trajBank.getInt("layer", iTraj);
		Int_t iLayer = -1;
		if (layerTraj == 6)  iLayer = 0;
		else if (layerTraj == 18)  iLayer = 1;
		else if (layerTraj == 36)  iLayer = 2;
		else  continue;
		xDC[iLayer] = trajBank.getFloat("x", iTraj);
		yDC[iLayer] = trajBank.getFloat("y", iTraj);
		zDC[iLayer] = trajBank.getFloat("z", iTraj);
		edgeDC[iLayer] = trajBank.getFloat("edge", iTraj);
		thetaDC[iLayer] = getTheta(xDC[iLayer], yDC[iLayer], zDC[iLayer]);
		phiDC[iLayer] = getPhi(xDC[iLayer], yDC[iLayer]);
		sectorDC[iLayer] = trajSector(phiDC[iLayer]);
	}
}

void readTrackInfo(const hipo::bank &trackBank, Int_t pindex, Int_t &sectorTrack, Float_t &chi2Track, Int_t &ndfTrack, Float_t &chi2ndfTrack) {
	Int_t nTrack = trackBank.getRows();
	for (Int_t iTrack=0; iTrack<nTrack; iTrack++) {
		Int_t pindexTrack = trackBank.getInt("pindex", iTrack);
		if (pindexTrack != pindex)  continue;
		Int_t detectorTrack = trackBank.getInt("detector", iTrack);
		if (detectorTrack != 6)  continue;
		sectorTrack = trackBank.getInt("sector", iTrack);
		chi2Track = trackBank.getFloat("chi2", iTrack);
		ndfTrack = trackBank.getInt("NDF", iTrack);
		chi2ndfTrack = chi2Track / ndfTrack;
	}
}

void readFTOFInfo(const hipo::bank &sciBank, Int_t pindex, Int_t (&sectorFTOF)[3], Float_t (&energyFTOF)[3], Float_t (&xFTOF)[3], Float_t (&yFTOF)[3], Float_t (&zFTOF)[3], Float_t (&timeFTOF)[3], Float_t (&pathFTOF)[3]) {
	Int_t nSci = sciBank.getRows();
	for (Int_t iSci=0; iSci<nSci; iSci++) {
		Int_t pindexSci = sciBank.getInt("pindex", iSci);
		if (pindexSci != pindex)  continue;
		Int_t detectorSci = sciBank.getInt("detector", iSci);
		if (detectorSci != 12)  continue;
		Int_t layerSci = sciBank.getInt("layer", iSci);
		Int_t iLayer = -1;
		if (layerSci == 1)  iLayer = 0;
		else if (layerSci == 2)  iLayer = 1;
		else if (layerSci == 3)  iLayer = 2;
		else  continue;
		sectorFTOF[iLayer] = sciBank.getInt("sector", iSci);
		energyFTOF[iLayer] = sciBank.getFloat("energy", iSci);
		xFTOF[iLayer] = sciBank.getFloat("x", iSci);
		yFTOF[iLayer] = sciBank.getFloat("y", iSci);
		zFTOF[iLayer] = sciBank.getFloat("z", iSci);
		timeFTOF[iLayer] = sciBank.getFloat("time", iSci);
		pathFTOF[iLayer] = sciBank.getFloat("path", iSci);
	}
}

void readCheInfo(const hipo::bank &cheBank, Int_t pindex, Int_t (&detectorChe)[3], Int_t (&sectorChe)[3], Float_t (&npheChe)[3], Float_t (&timeChe)[3], Float_t (&pathChe)[3], Float_t (&chi2Che)[3], Float_t (&xChe)[3], Float_t (&yChe)[3], Float_t (&zChe)[3], Float_t (&dThetaChe)[3], Float_t (&dPhiChe)[3], Int_t (&statusChe)[3]) {
	Int_t nChe = cheBank.getRows();
	for (Int_t iChe=0; iChe<nChe; iChe++) {
		Int_t pindexChe = cheBank.getInt("pindex", iChe);
		if (pindexChe != pindex)  continue;
		Int_t detector = cheBank.getInt("detector", iChe);
		Int_t iCC = -1;
		if (detector == 15)  iCC = 0;
		else if (detector == 16)  iCC = 1;
		else if (detector == 18)  iCC = 2;
		else  continue;
		detectorChe[iCC] = detector;
		sectorChe[iCC] = cheBank.getInt("sector", iChe);
		npheChe[iCC] = cheBank.getFloat("nphe", iChe);
		timeChe[iCC] = cheBank.getFloat("time", iChe);
		pathChe[iCC] = cheBank.getFloat("path", iChe);
		chi2Che[iCC] = cheBank.getFloat("chi2", iChe);
		xChe[iCC] = cheBank.getFloat("x", iChe);
		yChe[iCC] = cheBank.getFloat("y", iChe);
		dThetaChe[iCC] = cheBank.getFloat("dtheta", iChe);
		dPhiChe[iCC] = cheBank.getFloat("dphi", iChe);
		statusChe[iCC] = cheBank.getInt("status", iChe);
	}
}

void readRichInfo(const hipo::bank &richBank, Int_t pindex, Int_t &pidRICH, Float_t &RQRICH, Float_t &ReQRICH, Float_t &elLogLRICH, Float_t &piLogLRICH, Float_t &kLogLRICH, Float_t &prLogLRICH, Float_t &bestChRICH, Float_t &bestC2RICH, Float_t &bestRLRICH, Float_t &bestNtotRICH, Float_t &bestMassRICH) {
	Int_t nRich = richBank.getRows();
	for (Int_t iRich=0; iRich<nRich; iRich++) {
		Int_t pindexRich = richBank.getInt("pindex", iRich);
		if (pindexRich != pindex)  continue;
		pidRICH = richBank.getInt("best_PID", iRich);
		RQRICH = richBank.getFloat("RQ", iRich);
		ReQRICH = richBank.getFloat("ReQ", iRich);
		elLogLRICH = richBank.getFloat("el_logl", iRich);
		piLogLRICH = richBank.getFloat("pi_logl", iRich);
		kLogLRICH = richBank.getFloat("k_logl", iRich);
		prLogLRICH = richBank.getFloat("pr_logl", iRich);
		bestChRICH = richBank.getFloat("best_ch", iRich);
		bestC2RICH = richBank.getFloat("best_c2", iRich);
		bestRLRICH = richBank.getFloat("best_RL", iRich);
		bestNtotRICH = richBank.getFloat("best_ntot", iRich);
		bestMassRICH = richBank.getFloat("best_mass", iRich);
	}
}

void readRadPhInfo(const vector<recParticle> &phVect, const recParticle &myPart, Int_t &nPh, Int_t &numPh, Int_t (&pindex)[10], Float_t (&pPh)[10], Float_t (&pxPh)[10], Float_t (&pyPh)[10], Float_t (&pzPh)[10], Float_t (&anglePh)[10], Float_t (&dThetaPh)[10], Float_t (&dPhiPh)[10]) {
	Int_t num = phVect.size();
	nPh = 0;
	numPh = 0;
	for (Int_t iPh=0; iPh<num; iPh++) {
		Float_t dTheta = getTheta(phVect[iPh].px, phVect[iPh].py, phVect[iPh].pz) - getTheta(myPart.px, myPart.py, myPart.pz);
		Float_t dPhi = getPhi(phVect[iPh].px, phVect[iPh].py) - getPhi(myPart.px, myPart.py);
		if (dPhi < -180)  dPhi += 360;
		else if (dPhi > 180)  dPhi -= 360;
		if (abs(dTheta) < 2 && abs(dPhi) < 45) {
			if (nPh < 10) {
				pindex[numPh] = phVect[iPh].pindex;
				pxPh[numPh] = phVect[iPh].px;
				pyPh[numPh] = phVect[iPh].py;
				pzPh[numPh] = phVect[iPh].pz;
				pPh[numPh] = TMath::Sqrt(pxPh[nPh]*pxPh[nPh] + pyPh[nPh]*pyPh[nPh] + pzPh[nPh]*pzPh[nPh]);
				anglePh[numPh] = getConeAngle(myPart.px, myPart.py, myPart.pz, phVect[iPh].px, phVect[iPh].py, phVect[iPh].pz);
				dThetaPh[numPh] = dTheta;
				dPhiPh[numPh] = dPhi;
				numPh ++;
			}
			nPh ++;
		}
	}
}

void readElDeteBank(const hipo::bank &calBank, const hipo::bank &trajBank, const hipo::bank &trackBank, const hipo::bank &sciBank, const hipo::bank &cheBank, const vector<recParticle> &phVect, const vector<recParticle> &partVect, vector<elDeteInfo> &partDeteVect) {
	partDeteVect.clear();
	Int_t num = partVect.size();
	for (Int_t iPart=0; iPart<num; iPart++) {
		elDeteInfo myInfo{};
		Int_t pindex = partVect[iPart].pindex;
		readCALInfo(calBank, pindex, myInfo.sectorCAL, myInfo.energyCAL, myInfo.xCAL, myInfo.yCAL, myInfo.zCAL, myInfo.luCAL, myInfo.lvCAL, myInfo.lwCAL, myInfo.thetaCAL, myInfo.phiCAL, myInfo.timeCAL, myInfo.pathCAL, myInfo.energySumCAL);
		readDCInfo(trajBank, pindex, myInfo.sectorDC, myInfo.xDC, myInfo.yDC, myInfo.zDC, myInfo.thetaDC, myInfo.phiDC, myInfo.edgeDC);
		readTrackInfo(trackBank, pindex, myInfo.sectorTrack, myInfo.chi2Track, myInfo.ndfTrack, myInfo.chi2ndfTrack);
		readFTOFInfo(sciBank, pindex, myInfo.sectorFTOF, myInfo.energyFTOF, myInfo.xFTOF, myInfo.yFTOF, myInfo.zFTOF, myInfo.timeFTOF, myInfo.pathFTOF);
		readCheInfo(cheBank, pindex, myInfo.detectorChe, myInfo.sectorChe, myInfo.npheChe, myInfo.timeChe, myInfo.pathChe, myInfo.chi2Che, myInfo.xChe, myInfo.yChe, myInfo.zChe, myInfo.dThetaChe, myInfo.dPhiChe, myInfo.statusChe);
		readRadPhInfo(phVect, partVect[iPart], myInfo.nPh, myInfo.numPh, myInfo.pindexPh, myInfo.pPh, myInfo.pxPh, myInfo.pyPh, myInfo.pzPh, myInfo.anglePh, myInfo.dThetaPh, myInfo.dPhiPh);
		partDeteVect.emplace_back(myInfo);
	}
}

void readKaDeteBank(const hipo::bank &calBank, const hipo::bank &trajBank, const hipo::bank &trackBank, const hipo::bank &sciBank, const hipo::bank &cheBank, const hipo::bank &richBank, const vector<recParticle> &partVect, vector<kaDeteInfo> &partDeteVect) {
	partDeteVect.clear();
	Int_t num = partVect.size();
	for (Int_t iPart=0; iPart<num; iPart++) {
		kaDeteInfo myInfo{};
		Int_t pindex = partVect[iPart].pindex;
		readCALInfo(calBank, pindex, myInfo.sectorCAL, myInfo.energyCAL, myInfo.xCAL, myInfo.yCAL, myInfo.zCAL, myInfo.luCAL, myInfo.lvCAL, myInfo.lwCAL, myInfo.thetaCAL, myInfo.phiCAL, myInfo.timeCAL, myInfo.pathCAL, myInfo.energySumCAL);
		readDCInfo(trajBank, pindex, myInfo.sectorDC, myInfo.xDC, myInfo.yDC, myInfo.zDC, myInfo.thetaDC, myInfo.phiDC, myInfo.edgeDC);
		readTrackInfo(trackBank, pindex, myInfo.sectorTrack, myInfo.chi2Track, myInfo.ndfTrack, myInfo.chi2ndfTrack);
		readFTOFInfo(sciBank, pindex, myInfo.sectorFTOF, myInfo.energyFTOF, myInfo.xFTOF, myInfo.yFTOF, myInfo.zFTOF, myInfo.timeFTOF, myInfo.pathFTOF);
		readCheInfo(cheBank, pindex, myInfo.detectorChe, myInfo.sectorChe, myInfo.npheChe, myInfo.timeChe, myInfo.pathChe, myInfo.chi2Che, myInfo.xChe, myInfo.yChe, myInfo.zChe, myInfo.dThetaChe, myInfo.dPhiChe, myInfo.statusChe);
		readRichInfo(richBank, pindex, myInfo.pidRICH, myInfo.RQRICH, myInfo.ReQRICH, myInfo.elLogLRICH, myInfo.piLogLRICH, myInfo.kLogLRICH, myInfo.prLogLRICH, myInfo.bestChRICH, myInfo.bestC2RICH, myInfo.bestRLRICH, myInfo.bestNtotRICH, myInfo.bestMassRICH);
		partDeteVect.emplace_back(myInfo);
	}
}

void readPrDeteBank(const hipo::bank &calBank, const hipo::bank &trajBank, const hipo::bank &trackBank, const hipo::bank &sciBank, const hipo::bank &cheBank, const vector<recParticle> &partVect, vector<prDeteInfo> &partDeteVect) {
	partDeteVect.clear();
	Int_t num = partVect.size();
	for (Int_t iPart=0; iPart<num; iPart++) {
		prDeteInfo myInfo{};
		Int_t pindex = partVect[iPart].pindex;
		readCALInfo(calBank, pindex, myInfo.sectorCAL, myInfo.energyCAL, myInfo.xCAL, myInfo.yCAL, myInfo.zCAL, myInfo.luCAL, myInfo.lvCAL, myInfo.lwCAL, myInfo.thetaCAL, myInfo.phiCAL, myInfo.timeCAL, myInfo.pathCAL, myInfo.energySumCAL);
		readDCInfo(trajBank, pindex, myInfo.sectorDC, myInfo.xDC, myInfo.yDC, myInfo.zDC, myInfo.thetaDC, myInfo.phiDC, myInfo.edgeDC);
		readTrackInfo(trackBank, pindex, myInfo.sectorTrack, myInfo.chi2Track, myInfo.ndfTrack, myInfo.chi2ndfTrack);
		readFTOFInfo(sciBank, pindex, myInfo.sectorFTOF, myInfo.energyFTOF, myInfo.xFTOF, myInfo.yFTOF, myInfo.zFTOF, myInfo.timeFTOF, myInfo.pathFTOF);
		readCheInfo(cheBank, pindex, myInfo.detectorChe, myInfo.sectorChe, myInfo.npheChe, myInfo.timeChe, myInfo.pathChe, myInfo.chi2Che, myInfo.xChe, myInfo.yChe, myInfo.zChe, myInfo.dThetaChe, myInfo.dPhiChe, myInfo.statusChe);
		partDeteVect.emplace_back(myInfo);
	}
}

#endif
