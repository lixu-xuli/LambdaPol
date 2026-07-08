#ifndef _SAVETREE_H
#define _SAVETREE_H

#include <TTree.h>
#include <TString.h>
#include "info.h"

class preTree
{
	public:
		preTree(TString name): myTTree(name, name) {}
		void write() {
			myTTree.Write();
		}
		void addGlobalBranch(globalInfo* paraGlobal) {
			myTTree.Branch("runNumber", &(paraGlobal->runNumber), "runNumber/I");
			myTTree.Branch("eventNumber", &(paraGlobal->eventNumber), "eventNumber/L");
			myTTree.Branch("helicity", &(paraGlobal->helicity), "helicity/I");
    		myTTree.Branch("torus", &(paraGlobal->torus), "torus/F");
    		myTTree.Branch("solenoid", &(paraGlobal->solenoid), "solenoid/F");
			myTTree.Branch("hipoIndex", &(paraGlobal->hipoIndex), "hipoIndex/L");
    		myTTree.Branch("EBeam", &(paraGlobal->EBeam), "EBeam/F");
    		myTTree.Branch("targetPol", &(paraGlobal->targetPol), "targetPol/F");
			myTTree.Branch("epoch", &(paraGlobal->epoch), "epoch/I");
		}
		void addEventBranch(eventInfo* paraEvent) {
    		myTTree.Branch("nElectron", &(paraEvent->nElectron), "nElectron/I");
    		myTTree.Branch("nProton", &(paraEvent->nProton), "nProton/I");
    		myTTree.Branch("nNeutron", &(paraEvent->nNeutron), "nNeutron/I");
    		myTTree.Branch("nPhoton", &(paraEvent->nPhoton), "nPhoton/I");
    		myTTree.Branch("nPionP", &(paraEvent->nPionP), "nPionP/I");
    		myTTree.Branch("nPionM", &(paraEvent->nPionM), "nPionM/I");
    		myTTree.Branch("nKaonP", &(paraEvent->nKaonP), "nKaonP/I");
    		myTTree.Branch("nKaonM", &(paraEvent->nKaonM), "nKaonM/I");
    		myTTree.Branch("nOther", &(paraEvent->nOther), "nOther/I");
    		myTTree.Branch("numOther", &(paraEvent->numOther), "numOther/I");
    		myTTree.Branch("pidOther", &(paraEvent->pidOther), "pidOther[numOther]/I");
		}
		void addPrePartBranch(TString name, preParticle* paraPart) {
    		myTTree.Branch(name+"_pid", &(paraPart->pid), name+"_pid/I");
    		myTTree.Branch(name+"_M", &(paraPart->M), name+"_M/F");
    		myTTree.Branch(name+"_P", &(paraPart->P), name+"_P/F");
    		myTTree.Branch(name+"_E", &(paraPart->E), name+"_E/F");
    		myTTree.Branch(name+"_phi", &(paraPart->phi), name+"_phi/F");
    		myTTree.Branch(name+"_theta", &(paraPart->theta), name+"_theta/F");
    		myTTree.Branch(name+"_px", &(paraPart->px), name+"_px/F");
    		myTTree.Branch(name+"_py", &(paraPart->py), name+"_py/F");
    		myTTree.Branch(name+"_pz", &(paraPart->pz), name+"_pz/F");
    		myTTree.Branch(name+"_vx", &(paraPart->vx), name+"_vx/F");
    		myTTree.Branch(name+"_vy", &(paraPart->vy), name+"_vy/F");
    		myTTree.Branch(name+"_vz", &(paraPart->vz), name+"_vz/F");
    		myTTree.Branch(name+"_vt", &(paraPart->vt), name+"_vt/F");
		}
		void addRecPartBranch(TString name, recParticle* paraPart) {
			addPrePartBranch(name, paraPart);
    		myTTree.Branch(name+"_charge", &(paraPart->charge), name+"_charge/I");
    		myTTree.Branch(name+"_beta", &(paraPart->beta), name+"_beta/F");
    		myTTree.Branch(name+"_chi2pid", &(paraPart->chi2pid), name+"_chi2pid/F");
    		myTTree.Branch(name+"_status", &(paraPart->status), name+"_status/I");
    		myTTree.Branch(name+"_pindex", &(paraPart->pindex), name+"_pindex/I");
    		myTTree.Branch(name+"_mcindex", &(paraPart->mcindex), name+"_mcindex/I");
		}
		void addMCPartBranch(TString name, mcParticle* paraPart) {
			addPrePartBranch(name, paraPart);
    		myTTree.Branch(name+"_mcindex", &(paraPart->mcindex), name+"_mcindex/I");
    		myTTree.Branch(name+"_pindex", &(paraPart->pindex), name+"_pindex/I");
		}
		void addCalBranch(TString name, calInfo* paraDete) {
			myTTree.Branch(name+"_CAL_sector", &(paraDete->sectorCAL), name+"_CAL_sector[3]/I");
			myTTree.Branch(name+"_CAL_energy", &(paraDete->energyCAL), name+"_CAL_energy[3]/F");
			myTTree.Branch(name+"_CAL_x", &(paraDete->xCAL), name+"_CAL_x[3]/F");
			myTTree.Branch(name+"_CAL_y", &(paraDete->yCAL), name+"_CAL_y[3]/F");
			myTTree.Branch(name+"_CAL_z", &(paraDete->zCAL), name+"_CAL_z[3]/F");
			myTTree.Branch(name+"_CAL_lu", &(paraDete->luCAL), name+"_CAL_lu[3]/F");
			myTTree.Branch(name+"_CAL_lv", &(paraDete->lvCAL), name+"_CAL_lv[3]/F");
			myTTree.Branch(name+"_CAL_lw", &(paraDete->lwCAL), name+"_CAL_lw[3]/F");
			myTTree.Branch(name+"_CAL_phi", &(paraDete->phiCAL), name+"_CAL_phi[3]/F");
			myTTree.Branch(name+"_CAL_theta", &(paraDete->thetaCAL), name+"_CAL_theta[3]/F");
			myTTree.Branch(name+"_CAL_time", &(paraDete->timeCAL), name+"_CAL_time[3]/F");
			myTTree.Branch(name+"_CAL_path", &(paraDete->pathCAL), name+"_CAL_path[3]/F");
			myTTree.Branch(name+"_CAL_energySum", &(paraDete->energySumCAL), name+"_CAL_energySum/F");
		}
		void addTrackBranch(TString name, trackInfo* paraDete) {
			myTTree.Branch(name+"_DC_sector", &(paraDete->sectorDC), name+"_DC_sector[3]/I");
			myTTree.Branch(name+"_DC_x", &(paraDete->xDC), name+"_DC_x[3]/F");
			myTTree.Branch(name+"_DC_y", &(paraDete->yDC), name+"_DC_y[3]/F");
			myTTree.Branch(name+"_DC_z", &(paraDete->zDC), name+"_DC_z[3]/F");
			myTTree.Branch(name+"_DC_phi", &(paraDete->phiDC), name+"_DC_phi[3]/F");
			myTTree.Branch(name+"_DC_theta", &(paraDete->thetaDC), name+"_DC_theta[3]/F");
			myTTree.Branch(name+"_DC_edge", &(paraDete->edgeDC), name+"_DC_edge[3]/F");
			myTTree.Branch(name+"_track_sector", &(paraDete->sectorTrack), name+"_track_sector/I");
			myTTree.Branch(name+"_track_chi2", &(paraDete->chi2Track), name+"_track_chi2/F");
			myTTree.Branch(name+"_track_ndf", &(paraDete->ndfTrack), name+"_track_ndf/I");
			myTTree.Branch(name+"_track_chi2ndf", &(paraDete->chi2ndfTrack), name+"_track_chi2ndf/F");
		}
		void addCheBranch(TString name, cheInfo* paraDete) {
			myTTree.Branch(name+"_Che_detector", &(paraDete->detectorChe), name+"_Che_detector[3]/I");
			myTTree.Branch(name+"_Che_sector", &(paraDete->sectorChe), name+"_Che_sector[3]/I");
			myTTree.Branch(name+"_Che_nphe", &(paraDete->npheChe), name+"_Che_nphe[3]/F");
			myTTree.Branch(name+"_Che_time", &(paraDete->timeChe), name+"_Che_time[3]/F");
			myTTree.Branch(name+"_Che_path", &(paraDete->pathChe), name+"_Che_path[3]/F");
			myTTree.Branch(name+"_Che_chi2", &(paraDete->chi2Che), name+"_Che_chi2[3]/F");
			myTTree.Branch(name+"_Che_x", &(paraDete->xChe), name+"_Che_x[3]/F");
			myTTree.Branch(name+"_Che_y", &(paraDete->yChe), name+"_Che_y[3]/F");
			myTTree.Branch(name+"_Che_z", &(paraDete->zChe), name+"_Che_z[3]/F");
			myTTree.Branch(name+"_Che_dTheta", &(paraDete->dThetaChe), name+"_Che_dTheta[3]/F");
			myTTree.Branch(name+"_Che_dPhi", &(paraDete->dPhiChe), name+"_Che_dPhi[3]/F");
			myTTree.Branch(name+"_Che_status", &(paraDete->statusChe), name+"_Che_status[3]/I");
		}
		void addRichBranch(TString name, richInfo* paraDete) {
			myTTree.Branch(name+"_RICH_pid", &(paraDete->pidRICH), name+"_RICH_pid/I");
			myTTree.Branch(name+"_RICH_RQ", &(paraDete->RQRICH), name+"_RICH_RQ/F");
			myTTree.Branch(name+"_RICH_ReQ", &(paraDete->ReQRICH), name+"_RICH_ReQ/F");
			myTTree.Branch(name+"_RICH_elLogL", &(paraDete->elLogLRICH), name+"_RICH_elLogL/F");
			myTTree.Branch(name+"_RICH_piLogL", &(paraDete->piLogLRICH), name+"_RICH_piLogL/F");
			myTTree.Branch(name+"_RICH_kLogL", &(paraDete->kLogLRICH), name+"_RICH_kLogL/F");
			myTTree.Branch(name+"_RICH_prLogL", &(paraDete->prLogLRICH), name+"_RICH_prLogL/F");
			myTTree.Branch(name+"_RICH_bestCh", &(paraDete->bestChRICH), name+"_RICH_bestCh/F");
			myTTree.Branch(name+"_RICH_bestC2", &(paraDete->bestC2RICH), name+"_RICH_bestC2/F");
			myTTree.Branch(name+"_RICH_bestRL", &(paraDete->bestRLRICH), name+"_RICH_bestRL/F");
			myTTree.Branch(name+"_RICH_bestNtot", &(paraDete->bestNtotRICH), name+"_RICH_bestNtot/F");
			myTTree.Branch(name+"_RICH_bestMass", &(paraDete->bestMassRICH), name+"_RICH_bestMass/F");
		}
		void addRadPhBranch(TString name, radPhInfo* paraDete) {
			myTTree.Branch(name+"_nPh", &(paraDete->nPh), name+"_nPh/I");
			myTTree.Branch(name+"_numPh", &(paraDete->numPh), name+"_numPh/I");
			myTTree.Branch(name+"_pindexPh", &(paraDete->pindexPh), name+"_pindexPh["+name+"_numPh]/I");
			myTTree.Branch(name+"_pPh", &(paraDete->pPh), name+"_pPh["+name+"_numPh]/F");
			myTTree.Branch(name+"_pxPh", &(paraDete->pxPh), name+"_pxPh["+name+"_numPh]/F");
			myTTree.Branch(name+"_pyPh", &(paraDete->pyPh), name+"_pyPh["+name+"_numPh]/F");
			myTTree.Branch(name+"_pzPh", &(paraDete->pzPh), name+"_pzPh["+name+"_numPh]/F");
			myTTree.Branch(name+"_anglePh", &(paraDete->anglePh), name+"_anglePh["+name+"_numPh]/F");
			myTTree.Branch(name+"_dThetaPh", &(paraDete->dThetaPh), name+"_dThetaPh["+name+"_numPh]/F");
			myTTree.Branch(name+"_dPhiPh", &(paraDete->dPhiPh), name+"_dPhiPh["+name+"_numPh]/F");
		}
		void addElDeteBranch(TString name, elDeteInfo* paraDete) {
			addCalBranch(name, paraDete);
			addTrackBranch(name, paraDete);
			addCheBranch(name, paraDete);
			addRadPhBranch(name, paraDete);
		}
		void addKaDeteBranch(TString name, kaDeteInfo* paraDete) {
			addCalBranch(name, paraDete);
			addTrackBranch(name, paraDete);
			addCheBranch(name, paraDete);
			addRichBranch(name, paraDete);
		}
		void addPrDeteBranch(TString name, prDeteInfo* paraDete) {
			addCalBranch(name, paraDete);
			addTrackBranch(name, paraDete);
			addCheBranch(name, paraDete);
		}
		void addLamNoPiBranch(TString name, lamNoPi* paraLamNoPi) {
    		myTTree.Branch(name+"virPh_px", &(paraLamNoPi->virPhPx), name+"virPh_px/F");
    		myTTree.Branch(name+"virPh_py", &(paraLamNoPi->virPhPy), name+"virPh_py/F");
    		myTTree.Branch(name+"virPh_pz", &(paraLamNoPi->virPhPz), name+"virPh_pz/F");
    		myTTree.Branch(name+"virPh_P", &(paraLamNoPi->virPhP), name+"virPh_P/F");
    		myTTree.Branch(name+"virPh_E", &(paraLamNoPi->virPhE), name+"virPh_E/F");
    		myTTree.Branch(name+"virPh_phi", &(paraLamNoPi->virPhPhi), name+"virPh_phi/F");
    		myTTree.Branch(name+"virPh_theta", &(paraLamNoPi->virPhTheta), name+"virPh_theta/F");
    		myTTree.Branch(name+"misLam_px", &(paraLamNoPi->misLamPx), name+"misLam_px/F");
    		myTTree.Branch(name+"misLam_py", &(paraLamNoPi->misLamPy), name+"misLam_py/F");
    		myTTree.Branch(name+"misLam_pz", &(paraLamNoPi->misLamPz), name+"misLam_pz/F");
    		myTTree.Branch(name+"misLam_M2", &(paraLamNoPi->misLamM2), name+"misLam_M2/F");
    		myTTree.Branch(name+"misLam_M", &(paraLamNoPi->misLamM), name+"misLam_M/F");
    		myTTree.Branch(name+"misLam_P", &(paraLamNoPi->misLamP), name+"misLam_P/F");
    		myTTree.Branch(name+"misLam_E", &(paraLamNoPi->misLamE), name+"misLam_E/F");
    		myTTree.Branch(name+"misLam_phi", &(paraLamNoPi->misLamPhi), name+"misLam_phi/F");
    		myTTree.Branch(name+"misLam_theta", &(paraLamNoPi->misLamTheta), name+"misLam_theta/F");
    		myTTree.Branch(name+"misPi_px", &(paraLamNoPi->misPiPx), name+"misPi_px/F");
    		myTTree.Branch(name+"misPi_py", &(paraLamNoPi->misPiPy), name+"misPi_py/F");
    		myTTree.Branch(name+"misPi_pz", &(paraLamNoPi->misPiPz), name+"misPi_pz/F");
    		myTTree.Branch(name+"misPi_M2", &(paraLamNoPi->misPiM2), name+"misPi_M2/F");
    		myTTree.Branch(name+"misPi_M", &(paraLamNoPi->misPiM), name+"misPi_M/F");
    		myTTree.Branch(name+"misPi_P", &(paraLamNoPi->misPiP), name+"misPi_P/F");
    		myTTree.Branch(name+"misPi_E", &(paraLamNoPi->misPiE), name+"misPi_E/F");
    		myTTree.Branch(name+"misPi_phi", &(paraLamNoPi->misPiPhi), name+"misPi_phi/F");
    		myTTree.Branch(name+"misPi_theta", &(paraLamNoPi->misPiTheta), name+"misPi_theta/F");
    		myTTree.Branch(name+"Q2", &(paraLamNoPi->Q2), name+"Q2/F");
    		myTTree.Branch(name+"W", &(paraLamNoPi->W), name+"W/F");
    		myTTree.Branch(name+"Xbj", &(paraLamNoPi->Xbj), name+"Xbj/F");
    		myTTree.Branch(name+"t_kaon", &(paraLamNoPi->tKaon), name+"t_kaon/F");
    		myTTree.Branch(name+"phi_virPh", &(paraLamNoPi->phiVirPh), name+"phi_virPh/F");
    		myTTree.Branch(name+"phi_proton", &(paraLamNoPi->phiProton), name+"phi_proton/F");
    		myTTree.Branch(name+"delta_phi", &(paraLamNoPi->deltaPhi), name+"delta_phi/F");
    		myTTree.Branch(name+"cosThetaK", &(paraLamNoPi->cosThetaK), name+"cosThetaK/F");
    		myTTree.Branch(name+"exclusive_chi2", &(paraLamNoPi->exclChi2), name+"exclusive_chi2/F");
		}
		void addLamHasPiBranch(TString name, lamHasPi* paraLamHasPi) {
			addLamNoPiBranch(name, paraLamHasPi);
    		myTTree.Branch(name+"lambda_px", &(paraLamHasPi->lambdaPx), name+"lambda_px/F");
    		myTTree.Branch(name+"lambda_py", &(paraLamHasPi->lambdaPy), name+"lambda_py/F");
    		myTTree.Branch(name+"lambda_pz", &(paraLamHasPi->lambdaPz), name+"lambda_pz/F");
    		myTTree.Branch(name+"lambda_M2", &(paraLamHasPi->lambdaM2), name+"lambda_M2/F");
    		myTTree.Branch(name+"lambda_M", &(paraLamHasPi->lambdaM), name+"lambda_M/F");
    		myTTree.Branch(name+"lambda_P", &(paraLamHasPi->lambdaP), name+"lambda_P/F");
    		myTTree.Branch(name+"lambda_E", &(paraLamHasPi->lambdaE), name+"lambda_E/F");
    		myTTree.Branch(name+"lambda_phi", &(paraLamHasPi->lambdaPhi), name+"lambda_phi/F");
    		myTTree.Branch(name+"lambda_theta", &(paraLamHasPi->lambdaTheta), name+"lambda_theta/F");
    		myTTree.Branch(name+"t_lambda", &(paraLamHasPi->tLambda), name+"t_lambda/F");
    		myTTree.Branch(name+"mM2_ep2ekpPiX", &(paraLamHasPi->mM2ep2ekpPiX), name+"mM2_ep2ekpPiX/F");
    		myTTree.Branch(name+"mP_ep2ekpPiX", &(paraLamHasPi->mPep2ekpPiX), name+"mP_ep2ekpPiX/F");
		}
		void addSigmaZeroBranch(TString name, sigmaZero* paraSigmaZero) {
			addLamHasPiBranch(name, paraSigmaZero);
    		myTTree.Branch(name+"sigma_px", &(paraSigmaZero->sigmaPx), name+"sigma_px/F");
    		myTTree.Branch(name+"sigma_py", &(paraSigmaZero->sigmaPy), name+"sigma_py/F");
    		myTTree.Branch(name+"sigma_pz", &(paraSigmaZero->sigmaPz), name+"sigma_pz/F");
    		myTTree.Branch(name+"sigma_M2", &(paraSigmaZero->sigmaM2), name+"sigma_M2/F");
    		myTTree.Branch(name+"sigma_M", &(paraSigmaZero->sigmaM), name+"sigma_M/F");
    		myTTree.Branch(name+"sigma_P", &(paraSigmaZero->sigmaP), name+"sigma_P/F");
    		myTTree.Branch(name+"sigma_E", &(paraSigmaZero->sigmaE), name+"sigma_E/F");
    		myTTree.Branch(name+"sigma_phi", &(paraSigmaZero->sigmaPhi), name+"sigma_phi/F");
    		myTTree.Branch(name+"sigma_theta", &(paraSigmaZero->sigmaTheta), name+"sigma_theta/F");
    		myTTree.Branch(name+"t_sigma", &(paraSigmaZero->tSigma), name+"t_sigma/F");
    		myTTree.Branch(name+"mM2_ep2ekpPigX", &(paraSigmaZero->mM2ep2ekpPigX), name+"mM2_ep2ekpPigX/F");
    		myTTree.Branch(name+"mP_ep2ekpPigX", &(paraSigmaZero->mPep2ekpPigX), name+"mP_ep2ekpPigX/F");
		}
		void addPipiBkgBranch(TString name, pipiBkg* paraPipiBkg) {
			addLamNoPiBranch(name, paraPipiBkg);
    		myTTree.Branch(name+"realMisPi_px", &(paraPipiBkg->realMisPiPx), name+"realMisPi_px/F");
    		myTTree.Branch(name+"realMisPi_py", &(paraPipiBkg->realMisPiPy), name+"realMisPi_py/F");
    		myTTree.Branch(name+"realMisPi_pz", &(paraPipiBkg->realMisPiPz), name+"realMisPi_pz/F");
    		myTTree.Branch(name+"realMisPi_M2", &(paraPipiBkg->realMisPiM2), name+"realMisPi_M2/F");
    		myTTree.Branch(name+"realMisPi_M", &(paraPipiBkg->realMisPiM), name+"realMisPi_M/F");
    		myTTree.Branch(name+"realMisPi_P", &(paraPipiBkg->realMisPiP), name+"realMisPi_P/F");
    		myTTree.Branch(name+"realMisPi_E", &(paraPipiBkg->realMisPiE), name+"realMisPi_E/F");
    		myTTree.Branch(name+"realMisPi_phi", &(paraPipiBkg->realMisPiPhi), name+"realMisPi_phi/F");
    		myTTree.Branch(name+"realMisPi_theta", &(paraPipiBkg->realMisPiTheta), name+"realMisPi_theta/F");
    		myTTree.Branch(name+"realMisPrPi_px", &(paraPipiBkg->realMisPrPiPx), name+"realMisPrPi_px/F");
    		myTTree.Branch(name+"realMisPrPi_py", &(paraPipiBkg->realMisPrPiPy), name+"realMisPrPi_py/F");
    		myTTree.Branch(name+"realMisPrPi_pz", &(paraPipiBkg->realMisPrPiPz), name+"realMisPrPi_pz/F");
    		myTTree.Branch(name+"realMisPrPi_M2", &(paraPipiBkg->realMisPrPiM2), name+"realMisPrPi_M2/F");
    		myTTree.Branch(name+"realMisPrPi_M", &(paraPipiBkg->realMisPrPiM), name+"realMisPrPi_M/F");
    		myTTree.Branch(name+"realMisPrPi_P", &(paraPipiBkg->realMisPrPiP), name+"realMisPrPi_P/F");
    		myTTree.Branch(name+"realMisPrPi_E", &(paraPipiBkg->realMisPrPiE), name+"realMisPrPi_E/F");
    		myTTree.Branch(name+"realMisPrPi_phi", &(paraPipiBkg->realMisPrPiPhi), name+"realMisPrPi_phi/F");
    		myTTree.Branch(name+"realMisPrPi_theta", &(paraPipiBkg->realMisPrPiTheta), name+"realMisPrPi_theta/F");
		}
		TTree myTTree;
};

#endif
