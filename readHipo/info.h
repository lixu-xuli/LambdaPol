#ifndef _INFO_H
#define _INFO_H

#include <vector>
#include <TMath.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

const Int_t iniVal = -999;
const Float_t electronMass = 0.000511;
const Float_t protonMass = 0.938272;
const Float_t neutronMass = 0.939565;
const Float_t pionMass = 0.139570;
const Float_t kaonMass = 0.493677;
const Float_t lambdaMass = 1.115683;
ROOT::Math::PxPyPzEVector prTarget(0, 0, 0, protonMass);
ROOT::Math::PxPyPzEVector neTarget(0, 0, 0, neutronMass);

struct globalInfo
{
	Int_t runNumber{};
	Long_t eventNumber{};
	Int_t helicity{};
	Float_t torus{};
	Float_t solenoid{};
	Long_t hipoIndex{};
	Float_t EBeam{};
	Float_t targetPol{};
	Int_t epoch{};
};

struct eventInfo
{
	Int_t nElectron{};
	Int_t nProton{};
	Int_t nNeutron{};
	Int_t nPhoton{};
	Int_t nPionP{};
	Int_t nPionM{};
	Int_t nKaonP{};
	Int_t nKaonM{};
	Int_t nOther{};
	Int_t numOther{};
	Int_t pidOther[10]{};
};

struct prePartInfo
{
	Int_t pid{};
	Float_t px{};
	Float_t py{};
	Float_t pz{};
	Float_t vx{};
	Float_t vy{};
	Float_t vz{};
	Float_t vt{};
};

struct recPartInfo: prePartInfo
{
	Int_t charge{};
	Float_t beta{};
	Float_t chi2pid{};
	Int_t status{};
	Int_t pindex{};
};

struct mcPartInfo: prePartInfo
{
	Int_t mcindex{};
};

struct calInfo
{
	Int_t sectorCAL[3]{};
	Float_t energyCAL[3]{};
	Float_t xCAL[3]{};
	Float_t yCAL[3]{};
	Float_t zCAL[3]{};
	Float_t luCAL[3]{};
	Float_t lvCAL[3]{};
	Float_t lwCAL[3]{};
	Float_t phiCAL[3]{};
	Float_t thetaCAL[3]{};
	Float_t timeCAL[3]{};
	Float_t pathCAL[3]{};
	Float_t energySumCAL{};
};

struct trackInfo
{
	Int_t sectorDC[3]{};
	Float_t xDC[3]{};
	Float_t yDC[3]{};
	Float_t zDC[3]{};
	Float_t phiDC[3]{};
	Float_t thetaDC[3]{};
	Float_t edgeDC[3]{};
	Int_t sectorTrack{};
	Float_t chi2Track{};
	Int_t ndfTrack{};
	Float_t chi2ndfTrack{};
};

struct cheInfo
{
	Int_t detectorChe[3]{};
	Int_t sectorChe[3]{};
	Float_t npheChe[3]{};
	Float_t timeChe[3]{};
	Float_t pathChe[3]{};
	Float_t chi2Che[3]{};
	Float_t xChe[3]{};
	Float_t yChe[3]{};
	Float_t zChe[3]{};
	Float_t dThetaChe[3]{};
	Float_t dPhiChe[3]{};
	Int_t statusChe[3]{};
};

struct richInfo
{
	Int_t pidRICH{};
	Float_t RQRICH{};
	Float_t ReQRICH{};
	Float_t elLogLRICH{};
	Float_t piLogLRICH{};
	Float_t kLogLRICH{};
	Float_t prLogLRICH{};
	Float_t bestChRICH{};
	Float_t bestC2RICH{};
	Float_t bestRLRICH{};
	Float_t bestNtotRICH{};
	Float_t bestMassRICH{};
};

struct radPhInfo
{
	Int_t nPh{};
	Int_t numPh{};
	Int_t pindexPh[10]{};
	Float_t pPh[10]{};
	Float_t pxPh[10]{};
	Float_t pyPh[10]{};
	Float_t pzPh[10]{};
	Float_t anglePh[10]{};
	Float_t dThetaPh[10]{};
	Float_t dPhiPh[10]{};
};

struct elDeteInfo: calInfo, trackInfo, cheInfo, radPhInfo {};

struct kaDeteInfo: calInfo, trackInfo, cheInfo, richInfo {};

struct prDeteInfo: calInfo, trackInfo, cheInfo {};

class preParticle
{
	public:
		preParticle() = default;
		preParticle(Int_t paraPid, Float_t paraPx, Float_t paraPy, Float_t paraPz, Float_t paraVx, Float_t paraVy, Float_t paraVz, Float_t paraVt): pid(paraPid), px(paraPx), py(paraPy), pz(paraPz), vx(paraVx), vy(paraVy), vz(paraVz), vt(paraVt) {
			setPVec();
		}
		void setPVec() {
			M = 0;
			if (pid == 11)
				M = electronMass;
			else if (pid == 2112)
				M = neutronMass;
			else if (pid == 2212)
				M = protonMass;
			else if (pid == 22)
				M = 0;
			else if (pid == 211 || pid == -211)
				M = pionMass;
			else if (pid == 321 || pid == -321)
				M = kaonMass;
			P = TMath::Sqrt(px*px + py*py + pz*pz);
			E = TMath::Sqrt(P*P + M*M);
			PVec.SetPxPyPzE(px, py, pz, E);
			phi = PVec.Phi() / TMath::Pi() * 180;
			theta = PVec.Theta() / TMath::Pi() * 180;
		}
		Int_t pid{};
		Float_t px{};
		Float_t py{};
		Float_t pz{};
		Float_t vx{};
		Float_t vy{};
		Float_t vz{};
		Float_t vt{};
		Float_t M{};
		Float_t P{};
		Float_t E{};
		Float_t phi{};
		Float_t theta{};
		ROOT::Math::PxPyPzEVector PVec{};
};

class recParticle: public preParticle
{
	public:
		recParticle() = default;
		recParticle(recPartInfo paraInfo, Int_t paraMCIndex): preParticle(paraInfo.pid, paraInfo.px, paraInfo.py, paraInfo.pz, paraInfo.vx, paraInfo.vy, paraInfo.vz, paraInfo.vt), charge(paraInfo.charge), beta(paraInfo.beta), chi2pid(paraInfo.chi2pid), status(paraInfo.status), pindex(paraInfo.pindex), mcindex(paraMCIndex) {}
		Int_t charge{};
		Float_t beta{};
		Float_t chi2pid{};
		Int_t status{};
		Int_t pindex{};
		Int_t mcindex{};
};

class mcParticle: public preParticle
{
	public:
		mcParticle() = default;
		mcParticle(mcPartInfo paraInfo, Int_t paraPIndex): preParticle(paraInfo.pid, paraInfo.px, paraInfo.py, paraInfo.pz, paraInfo.vx, paraInfo.vy, paraInfo.vz, paraInfo.vt), mcindex(paraInfo.mcindex), pindex(paraPIndex) {}
		Int_t mcindex{};
		Int_t pindex{};
};

class lamNoPi
{
	public:
		lamNoPi() = default;
		lamNoPi(ROOT::Math::PxPyPzEVector paraBeam, ROOT::Math::PxPyPzEVector paraTarget, ROOT::Math::PxPyPzEVector paraEl, ROOT::Math::PxPyPzEVector paraKa, ROOT::Math::PxPyPzEVector paraPr): elBeam(paraBeam), prTarget(paraTarget), elPVec(paraEl), kaPVec(paraKa), prPVec(paraPr) {
			calculateVars();
		}
		void calculateVars() {
			virPhPVec = elBeam - elPVec;
			Q2 = -virPhPVec.M2();
			W = (prTarget + elBeam - elPVec).M();
			Xbj = Q2 / (2 * prTarget.M() * (elBeam.E() - elPVec.E()));
			tKaon = (elBeam - elPVec - kaPVec).M2();
			virPhPx = virPhPVec.Px();
			virPhPy = virPhPVec.Py();
			virPhPz = virPhPVec.Pz();
			virPhP = virPhPVec.P();
			virPhE = virPhPVec.E();
			virPhPhi = virPhPVec.Phi() / TMath::Pi() * 180;
			virPhTheta = virPhPVec.Theta() / TMath::Pi() * 180;
			ROOT::Math::XYZVector zAxis = virPhPVec.Vect();  //virtual photon direction
			ROOT::Math::XYZVector yAxis = elBeam.Vect().Cross(elPVec.Vect());  //vertical to leptonic plane
			ROOT::Math::XYZVector xAxis = yAxis.Cross(zAxis);
			ROOT::Math::XYZVector y1Axis = kaPVec.Vect().Cross(zAxis);  //vertical to hadronic plane
			ROOT::Math::XYZVector y2Axis = kaPVec.Vect().Cross(prPVec.Vect());  //vertical to hadronic plane
			phiVirPh = ROOT::Math::VectorUtil::Angle(y1Axis, yAxis) / TMath::Pi() * 180;
			if (y1Axis.Dot(xAxis) > 0)  phiVirPh = 360 - phiVirPh;
			phiProton = ROOT::Math::VectorUtil::Angle(y2Axis, yAxis) / TMath::Pi() * 180;
			if (y2Axis.Dot(xAxis) > 0)  phiProton = 360 - phiProton;
			deltaPhi = phiVirPh - phiProton;
			if (deltaPhi > 180)  deltaPhi -= 360;
			else if (deltaPhi < -180)  deltaPhi += 360;
			misLamPVec = elBeam + prTarget - elPVec - kaPVec;
			misLamPx = misLamPVec.Px();
			misLamPy = misLamPVec.Py();
			misLamPz = misLamPVec.Pz();
			misLamM2 = misLamPVec.M2();
			misLamM = misLamPVec.M();
			misLamP = misLamPVec.P();
			misLamE = misLamPVec.E();
			misLamPhi = misLamPVec.Phi() / TMath::Pi() * 180;
			misLamTheta = misLamPVec.Theta() / TMath::Pi() * 180;
			misPiPVec = elBeam + prTarget - elPVec - kaPVec - prPVec;
			misPiPx = misPiPVec.Px();
			misPiPy = misPiPVec.Py();
			misPiPz = misPiPVec.Pz();
			misPiM2 = misPiPVec.M2();
			misPiM = misPiPVec.M();
			misPiP = misPiPVec.P();
			misPiE = misPiPVec.E();
			misPiPhi = misPiPVec.Phi() / TMath::Pi() * 180;
			misPiTheta = misPiPVec.Theta() / TMath::Pi() * 180;
			ROOT::Math::PxPyPzEVector CMSys = virPhPVec + prTarget;
			ROOT::Math::Boost boostCM(CMSys.BoostToCM());
			ROOT::Math::PxPyPzEVector virPhPVecCM = boostCM(virPhPVec);
			ROOT::Math::PxPyPzEVector kaPVecCM = boostCM(kaPVec);
			cosThetaK = kaPVecCM.Vect().Dot(virPhPVecCM.Vect()) / (kaPVecCM.P() * virPhPVecCM.P());
			exclChi2 = pow(misLamM2-lambdaMass*lambdaMass, 2) + pow(misPiM2-pionMass*pionMass, 2);
		}
		ROOT::Math::PxPyPzEVector elBeam{};
		ROOT::Math::PxPyPzEVector prTarget{};
		ROOT::Math::PxPyPzEVector elPVec{};
		ROOT::Math::PxPyPzEVector kaPVec{};
		ROOT::Math::PxPyPzEVector prPVec{};
		ROOT::Math::PxPyPzEVector virPhPVec{};
		ROOT::Math::PxPyPzEVector misLamPVec{};
		ROOT::Math::PxPyPzEVector misPiPVec{};
		Float_t virPhPx{};
		Float_t virPhPy{};
		Float_t virPhPz{};
		Float_t virPhM2{};
		Float_t virPhP{};
		Float_t virPhE{};
		Float_t virPhPhi{};
		Float_t virPhTheta{};
		Float_t misLamPx{};
		Float_t misLamPy{};
		Float_t misLamPz{};
		Float_t misLamM2{};
		Float_t misLamM{};
		Float_t misLamP{};
		Float_t misLamE{};
		Float_t misLamPhi{};
		Float_t misLamTheta{};
		Float_t misPiPx{};
		Float_t misPiPy{};
		Float_t misPiPz{};
		Float_t misPiM2{};
		Float_t misPiM{};
		Float_t misPiP{};
		Float_t misPiE{};
		Float_t misPiPhi{};
		Float_t misPiTheta{};
		Float_t Q2{};
		Float_t W{};
		Float_t Xbj{};
		Float_t tKaon{};
		Float_t phiVirPh{};
		Float_t phiProton{};
		Float_t deltaPhi{};
		Float_t cosThetaK{};
		Float_t exclChi2{};
};

class lamHasPi: public lamNoPi
{
	public:
		lamHasPi() = default;
		lamHasPi(ROOT::Math::PxPyPzEVector paraBeam, ROOT::Math::PxPyPzEVector paraTarget, ROOT::Math::PxPyPzEVector paraEl, ROOT::Math::PxPyPzEVector paraKa, ROOT::Math::PxPyPzEVector paraPr, ROOT::Math::PxPyPzEVector paraPi): lamNoPi(paraBeam, paraTarget, paraEl, paraKa, paraPr), piPVec(paraPi) {
			calculateVarsWithPi();
		}
		void calculateVarsWithPi() {
			lambdaPVec = prPVec + piPVec;
			lambdaPx = lambdaPVec.Px();
			lambdaPy = lambdaPVec.Py();
			lambdaPz = lambdaPVec.Pz();
			lambdaM2 = lambdaPVec.M2();
			lambdaM = lambdaPVec.M();
			lambdaP = lambdaPVec.P();
			lambdaE = lambdaPVec.E();
			lambdaPhi = lambdaPVec.Phi() / TMath::Pi() * 180;
			lambdaTheta = lambdaPVec.Theta() / TMath::Pi() * 180;
			tLambda = (prPVec + piPVec - prTarget).M2();
			ROOT::Math::PxPyPzEVector misX = elBeam + prTarget - elPVec - kaPVec - prPVec - piPVec;
			mM2ep2ekpPiX = misX.M2();
			mPep2ekpPiX = misX.P();
		}
		ROOT::Math::PxPyPzEVector piPVec{};
		ROOT::Math::PxPyPzEVector lambdaPVec{};
		Float_t lambdaPx{};
		Float_t lambdaPy{};
		Float_t lambdaPz{};
		Float_t lambdaM2{};
		Float_t lambdaM{};
		Float_t lambdaP{};
		Float_t lambdaE{};
		Float_t lambdaPhi{};
		Float_t lambdaTheta{};
		Float_t tLambda{};
		Float_t mM2ep2ekpPiX{};
		Float_t mPep2ekpPiX{};
};

class sigmaZero: public lamHasPi
{
	public:
		sigmaZero() = default;
		sigmaZero(ROOT::Math::PxPyPzEVector paraBeam, ROOT::Math::PxPyPzEVector paraTarget, ROOT::Math::PxPyPzEVector paraEl, ROOT::Math::PxPyPzEVector paraKa, ROOT::Math::PxPyPzEVector paraPr, ROOT::Math::PxPyPzEVector paraPi, ROOT::Math::PxPyPzEVector paraPh): lamHasPi(paraBeam, paraTarget, paraEl, paraKa, paraPr, paraPi), phPVec(paraPh) {
			calculateVarsSigmaZero();
		}
		void calculateVarsSigmaZero() {
			sigmaPVec = prPVec + piPVec + phPVec;
			sigmaPx = sigmaPVec.Px();
			sigmaPy = sigmaPVec.Py();
			sigmaPz = sigmaPVec.Pz();
			sigmaM2 = sigmaPVec.M2();
			sigmaM = sigmaPVec.M();
			sigmaP = sigmaPVec.P();
			sigmaE = sigmaPVec.E();
			sigmaPhi = sigmaPVec.Phi() / TMath::Pi() * 180;
			sigmaTheta = sigmaPVec.Theta() / TMath::Pi() * 180;
			tSigma = (prPVec + piPVec + phPVec - prTarget).M2();
			ROOT::Math::PxPyPzEVector misX = elBeam + prTarget - elPVec - kaPVec - prPVec - piPVec - phPVec;
			mM2ep2ekpPigX = misX.M2();
			mPep2ekpPigX = misX.P();
		}
		ROOT::Math::PxPyPzEVector phPVec{};
		ROOT::Math::PxPyPzEVector sigmaPVec{};
		Float_t sigmaPx{};
		Float_t sigmaPy{};
		Float_t sigmaPz{};
		Float_t sigmaM2{};
		Float_t sigmaM{};
		Float_t sigmaP{};
		Float_t sigmaE{};
		Float_t sigmaPhi{};
		Float_t sigmaTheta{};
		Float_t tSigma{};
		Float_t mM2ep2ekpPigX{};
		Float_t mPep2ekpPigX{};
};

class pipiBkg: public lamNoPi
{
	public:
		pipiBkg() = default;
		pipiBkg(ROOT::Math::PxPyPzEVector paraBeam, ROOT::Math::PxPyPzEVector paraTarget, ROOT::Math::PxPyPzEVector paraEl, ROOT::Math::PxPyPzEVector paraKa, ROOT::Math::PxPyPzEVector paraPr, ROOT::Math::PxPyPzEVector paraPip): lamNoPi(paraBeam, paraTarget, paraEl, paraKa, paraPr), pipPVec(paraPip) {
			calculateVarsPipi();
		}
		void calculateVarsPipi() {
			ROOT::Math::PxPyPzEVector realMisPrPiPVec = elBeam + prTarget - elPVec - pipPVec;
			ROOT::Math::PxPyPzEVector realMisPiPVec = elBeam + prTarget - elPVec - prPVec - pipPVec;
			realMisPrPiPx = realMisPrPiPVec.Px();
			realMisPrPiPy = realMisPrPiPVec.Py();
			realMisPrPiPz = realMisPrPiPVec.Pz();
			realMisPrPiM2 = realMisPrPiPVec.M2();
			realMisPrPiM = realMisPrPiPVec.M();
			realMisPrPiP = realMisPrPiPVec.P();
			realMisPrPiE = realMisPrPiPVec.E();
			realMisPrPiPhi = realMisPrPiPVec.Phi() / TMath::Pi() * 180;
			realMisPrPiTheta = realMisPrPiPVec.Theta() / TMath::Pi() * 180;
			realMisPiPx = realMisPiPVec.Px();
			realMisPiPy = realMisPiPVec.Py();
			realMisPiPz = realMisPiPVec.Pz();
			realMisPiM2 = realMisPiPVec.M2();
			realMisPiM = realMisPiPVec.M();
			realMisPiP = realMisPiPVec.P();
			realMisPiE = realMisPiPVec.E();
			realMisPiPhi = realMisPiPVec.Phi() / TMath::Pi() * 180;
			realMisPiTheta = realMisPiPVec.Theta() / TMath::Pi() * 180;
		}
		ROOT::Math::PxPyPzEVector pipPVec{};
		ROOT::Math::PxPyPzEVector realMisPrPiPVec{};
		ROOT::Math::PxPyPzEVector realMisPiPVec{};
		Float_t realMisPiPx{};
		Float_t realMisPiPy{};
		Float_t realMisPiPz{};
		Float_t realMisPiM2{};
		Float_t realMisPiM{};
		Float_t realMisPiP{};
		Float_t realMisPiE{};
		Float_t realMisPiPhi{};
		Float_t realMisPiTheta{};
		Float_t realMisPrPiPx{};
		Float_t realMisPrPiPy{};
		Float_t realMisPrPiPz{};
		Float_t realMisPrPiM2{};
		Float_t realMisPrPiM{};
		Float_t realMisPrPiP{};
		Float_t realMisPrPiE{};
		Float_t realMisPrPiPhi{};
		Float_t realMisPrPiTheta{};
};

#endif
