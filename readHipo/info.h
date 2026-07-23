#ifndef _INFO_H
#define _INFO_H

#include <vector>
#include <TMath.h>
#include <Math/Vector3D.h>
#include <Math/Vector4D.h>

using namespace ROOT::Math;

const Int_t iniVal = -999;
const Float_t electronMass = 0.000511;
const Float_t protonMass = 0.938272;
const Float_t neutronMass = 0.939565;
const Float_t pionMass = 0.139570;
const Float_t kaonMass = 0.493677;
const Float_t lambdaMass = 1.115683;
PxPyPzEVector prTarget(0, 0, 0, protonMass);
PxPyPzEVector neTarget(0, 0, 0, neutronMass);

Float_t getTheta(Float_t x, Float_t y, Float_t z) {
	Float_t cosTheta = z / TMath::Sqrt(x*x + y*y + z*z);
	if (cosTheta > 1.0)  cosTheta = 1.0;
	if (cosTheta < -1.0)  cosTheta = -1.0;
	return TMath::ACos(cosTheta) / TMath::Pi() * 180;
}

Float_t getPhi(Float_t x, Float_t y) {
	if (std::hypot(x, y) < 1e-12)
		return 0;
	else
		return TMath::ATan2(y, x) / TMath::Pi() * 180;
}

Float_t getConeAngle(Float_t x1, Float_t y1, Float_t z1, Float_t x2, Float_t y2, Float_t z2) {
	Float_t r1 = TMath::Sqrt(x1*x1 + y1*y1 + z1*z1);
	Float_t r2 = TMath::Sqrt(x2*x2 + y2*y2 + z2*z2);
	return TMath::ACos((x1*x2 + y1*y2 + z1*z2) / (r1*r2)) / TMath::Pi() * 180;
}

void setPVecValues(const PxPyPzEVector &PVec, Float_t &Px, Float_t &Py, Float_t &Pz, Float_t &P, Float_t &E, Float_t &M2, Float_t &M, Float_t &theta, Float_t &phi) {
	Px = PVec.Px();
	Py = PVec.Py();
	Pz = PVec.Pz();
	P = PVec.P();
	E = PVec.E();
	M2 = PVec.M2();
	M = PVec.M();
	theta = PVec.Theta() / TMath::Pi() * 180;
	phi = PVec.Phi() / TMath::Pi() * 180;
}

void setPVecValues(const PxPyPzEVector &PVec, const XYZVector &xAxis, const XYZVector &yAxis, const XYZVector &zAxis, Float_t &Px, Float_t &Py, Float_t &Pz, Float_t &P, Float_t &E, Float_t &theta, Float_t &phi) {
	Px = PVec.Vect().Dot(xAxis) / xAxis.R();
	Py = PVec.Vect().Dot(yAxis) / yAxis.R();
	Pz = PVec.Vect().Dot(zAxis) / zAxis.R();
	P = PVec.P();
	E = PVec.E();
	theta = getTheta(Px, Py, Pz);
	phi = getPhi(Px, Py);
}

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
	Float_t thetaCAL[3]{};
	Float_t phiCAL[3]{};
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
	Float_t thetaDC[3]{};
	Float_t phiDC[3]{};
	Float_t edgeDC[3]{};
	Int_t sectorTrack{};
	Float_t chi2Track{};
	Int_t ndfTrack{};
	Float_t chi2ndfTrack{};
};

struct ftofInfo
{
	Int_t sectorFTOF[3]{};
	Float_t energyFTOF[3]{};
	Float_t xFTOF[3]{};
	Float_t yFTOF[3]{};
	Float_t zFTOF[3]{};
	Float_t timeFTOF[3]{};
	Float_t pathFTOF[3]{};
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

struct elDeteInfo: calInfo, trackInfo, ftofInfo, cheInfo, radPhInfo {};

struct kaDeteInfo: calInfo, trackInfo, ftofInfo, cheInfo, richInfo {};

struct prDeteInfo: calInfo, trackInfo, ftofInfo, cheInfo {};

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
			theta = PVec.Theta() / TMath::Pi() * 180;
			phi = PVec.Phi() / TMath::Pi() * 180;
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
		Float_t theta{};
		Float_t phi{};
		PxPyPzEVector PVec{};
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
		lamNoPi(PxPyPzEVector paraBeam, PxPyPzEVector paraTarget, PxPyPzEVector paraEl, PxPyPzEVector paraKa, PxPyPzEVector paraPr): elBeam(paraBeam), prTarget(paraTarget), elPVec(paraEl), kaPVec(paraKa), prPVec(paraPr) {
			calculateVars();
		}
		void calculateVars() {
			virPhPVec = elBeam - elPVec;
			setPVecValues(virPhPVec, virPhPx, virPhPy, virPhPz, virPhP, virPhE, virPhM2, virPhM, virPhTheta, virPhPhi);
			Q2 = -virPhPVec.M2();
			W = (prTarget + elBeam - elPVec).M();
			Xbj = Q2 / (2 * prTarget.M() * (elBeam.E() - elPVec.E()));
			tKaon = (elBeam - elPVec - kaPVec).M2();
			XYZVector zAxis = virPhPVec.Vect();  //virtual photon direction
			XYZVector yAxis = elBeam.Vect().Cross(elPVec.Vect());  //vertical to leptonic plane
			XYZVector xAxis = yAxis.Cross(zAxis);
			XYZVector y1Axis = kaPVec.Vect().Cross(zAxis);  //vertical to hadronic plane
			XYZVector y2Axis = kaPVec.Vect().Cross(prPVec.Vect());  //vertical to hadronic plane
			phiVirPh = VectorUtil::Angle(y1Axis, yAxis) / TMath::Pi() * 180;
			if (y1Axis.Dot(xAxis) > 0)  phiVirPh = 360 - phiVirPh;
			phiProton = VectorUtil::Angle(y2Axis, yAxis) / TMath::Pi() * 180;
			if (y2Axis.Dot(xAxis) > 0)  phiProton = 360 - phiProton;
			deltaPhi = phiVirPh - phiProton;
			if (deltaPhi > 180)  deltaPhi -= 360;
			else if (deltaPhi < -180)  deltaPhi += 360;
			misLamPVec = elBeam + prTarget - elPVec - kaPVec;
			setPVecValues(misLamPVec, misLamPx, misLamPy, misLamPz, misLamP, misLamE, misLamM2, misLamM, misLamTheta, misLamPhi);
			misPiPVec = elBeam + prTarget - elPVec - kaPVec - prPVec;
			setPVecValues(misPiPVec, misPiPx, misPiPy, misPiPz, misPiP, misPiE, misPiM2, misPiM, misPiTheta, misPiPhi);
			exclChi2 = pow(misLamM2-lambdaMass*lambdaMass, 2) + pow(misPiM2-pionMass*pionMass, 2);
			PxPyPzEVector CMSys = virPhPVec + prTarget;
			Boost boostCM(CMSys.BoostToCM());
			PxPyPzEVector virPhPVecCM = boostCM(virPhPVec);
			PxPyPzEVector targetPVecCM = boostCM(prTarget);
			PxPyPzEVector kaPVecCM = boostCM(kaPVec);
			PxPyPzEVector misLamPVecCM = boostCM(misLamPVec);
			PxPyPzEVector prPVecCM = boostCM(prPVec);
			XYZVector zPrimeAxis = kaPVecCM.Vect();
			XYZVector yPrimeAxis = virPhPVecCM.Vect().Cross(kaPVecCM.Vect());
			XYZVector xPrimeAxis = yPrimeAxis.Cross(zPrimeAxis);
			setPVecValues(virPhPVecCM, xPrimeAxis, yPrimeAxis, zPrimeAxis, virPhPxCM, virPhPyCM, virPhPzCM, virPhPCM, virPhECM, virPhThetaCM, virPhPhiCM);
			setPVecValues(targetPVecCM, xPrimeAxis, yPrimeAxis, zPrimeAxis, targetPxCM, targetPyCM, targetPzCM, targetPCM, targetECM, targetThetaCM, targetPhiCM);
			setPVecValues(kaPVecCM, xPrimeAxis, yPrimeAxis, zPrimeAxis, kaPxCM, kaPyCM, kaPzCM, kaPCM, kaECM, kaThetaCM, kaPhiCM);
			setPVecValues(misLamPVecCM, xPrimeAxis, yPrimeAxis, zPrimeAxis, misLamPxCM, misLamPyCM, misLamPzCM, misLamPCM, misLamECM, misLamThetaCM, misLamPhiCM);
			setPVecValues(prPVecCM, xPrimeAxis, yPrimeAxis, zPrimeAxis, prPxCM, prPyCM, prPzCM, prPCM, prECM, prThetaCM, prPhiCM);
			Boost boostLamRest(misLamPVecCM.BoostToCM());
			PxPyPzEVector prPVecLamRest = boostLamRest(prPVecCM);
			cosThetaK = kaPVecCM.Vect().Dot(virPhPVecCM.Vect()) / (kaPVecCM.P() * virPhPVecCM.P());
			cosThetaPrZ = prPVecLamRest.Vect().Dot(zPrimeAxis) / (prPVecLamRest.P() * zPrimeAxis.R());
			cosThetaPrX = prPVecLamRest.Vect().Dot(xPrimeAxis) / (prPVecLamRest.P() * xPrimeAxis.R());
			cosThetaPrY = prPVecLamRest.Vect().Dot(yPrimeAxis) / (prPVecLamRest.P() * yPrimeAxis.R());
		}
		PxPyPzEVector elBeam{};
		PxPyPzEVector prTarget{};
		PxPyPzEVector elPVec{};
		PxPyPzEVector kaPVec{};
		PxPyPzEVector prPVec{};
		PxPyPzEVector virPhPVec{};
		PxPyPzEVector misLamPVec{};
		PxPyPzEVector misPiPVec{};
		Float_t virPhPx{};
		Float_t virPhPy{};
		Float_t virPhPz{};
		Float_t virPhP{};
		Float_t virPhE{};
		Float_t virPhM2{};
		Float_t virPhM{};
		Float_t virPhTheta{};
		Float_t virPhPhi{};
		Float_t misLamPx{};
		Float_t misLamPy{};
		Float_t misLamPz{};
		Float_t misLamP{};
		Float_t misLamE{};
		Float_t misLamM2{};
		Float_t misLamM{};
		Float_t misLamTheta{};
		Float_t misLamPhi{};
		Float_t misPiPx{};
		Float_t misPiPy{};
		Float_t misPiPz{};
		Float_t misPiP{};
		Float_t misPiE{};
		Float_t misPiM2{};
		Float_t misPiM{};
		Float_t misPiTheta{};
		Float_t misPiPhi{};
		Float_t exclChi2{};
		Float_t Q2{};
		Float_t W{};
		Float_t Xbj{};
		Float_t tKaon{};
		Float_t phiVirPh{};
		Float_t phiProton{};
		Float_t deltaPhi{};
		Float_t virPhPxCM{};
		Float_t virPhPyCM{};
		Float_t virPhPzCM{};
		Float_t virPhPCM{};
		Float_t virPhECM{};
		Float_t virPhThetaCM{};
		Float_t virPhPhiCM{};
		Float_t targetPxCM{};
		Float_t targetPyCM{};
		Float_t targetPzCM{};
		Float_t targetPCM{};
		Float_t targetECM{};
		Float_t targetThetaCM{};
		Float_t targetPhiCM{};
		Float_t kaPxCM{};
		Float_t kaPyCM{};
		Float_t kaPzCM{};
		Float_t kaPCM{};
		Float_t kaECM{};
		Float_t kaThetaCM{};
		Float_t kaPhiCM{};
		Float_t misLamPxCM{};
		Float_t misLamPyCM{};
		Float_t misLamPzCM{};
		Float_t misLamPCM{};
		Float_t misLamECM{};
		Float_t misLamThetaCM{};
		Float_t misLamPhiCM{};
		Float_t prPxCM{};
		Float_t prPyCM{};
		Float_t prPzCM{};
		Float_t prPCM{};
		Float_t prECM{};
		Float_t prThetaCM{};
		Float_t prPhiCM{};
		Float_t cosThetaK{};
		Float_t cosThetaPrZ{};
		Float_t cosThetaPrX{};
		Float_t cosThetaPrY{};
};

class lamHasPi: public lamNoPi
{
	public:
		lamHasPi() = default;
		lamHasPi(PxPyPzEVector paraBeam, PxPyPzEVector paraTarget, PxPyPzEVector paraEl, PxPyPzEVector paraKa, PxPyPzEVector paraPr, PxPyPzEVector paraPi): lamNoPi(paraBeam, paraTarget, paraEl, paraKa, paraPr), piPVec(paraPi) {
			calculateVarsWithPi();
		}
		void calculateVarsWithPi() {
			lambdaPVec = prPVec + piPVec;
			setPVecValues(lambdaPVec, lambdaPx, lambdaPy, lambdaPz, lambdaP, lambdaE, lambdaM2, lambdaM, lambdaTheta, lambdaPhi);
			tLambda = (prPVec + piPVec - prTarget).M2();
			PxPyPzEVector misX = elBeam + prTarget - elPVec - kaPVec - prPVec - piPVec;
			mM2ep2ekpPiX = misX.M2();
			mPep2ekpPiX = misX.P();
		}
		PxPyPzEVector piPVec{};
		PxPyPzEVector lambdaPVec{};
		Float_t lambdaPx{};
		Float_t lambdaPy{};
		Float_t lambdaPz{};
		Float_t lambdaM2{};
		Float_t lambdaM{};
		Float_t lambdaP{};
		Float_t lambdaE{};
		Float_t lambdaTheta{};
		Float_t lambdaPhi{};
		Float_t tLambda{};
		Float_t mM2ep2ekpPiX{};
		Float_t mPep2ekpPiX{};
};

class sigmaZero: public lamHasPi
{
	public:
		sigmaZero() = default;
		sigmaZero(PxPyPzEVector paraBeam, PxPyPzEVector paraTarget, PxPyPzEVector paraEl, PxPyPzEVector paraKa, PxPyPzEVector paraPr, PxPyPzEVector paraPi, PxPyPzEVector paraPh): lamHasPi(paraBeam, paraTarget, paraEl, paraKa, paraPr, paraPi), phPVec(paraPh) {
			calculateVarsSigmaZero();
		}
		void calculateVarsSigmaZero() {
			sigmaPVec = prPVec + piPVec + phPVec;
			setPVecValues(sigmaPVec, sigmaPx, sigmaPy, sigmaPz, sigmaP, sigmaE, sigmaM2, sigmaM, sigmaTheta, sigmaPhi);
			tSigma = (prPVec + piPVec + phPVec - prTarget).M2();
			PxPyPzEVector misX = elBeam + prTarget - elPVec - kaPVec - prPVec - piPVec - phPVec;
			mM2ep2ekpPigX = misX.M2();
			mPep2ekpPigX = misX.P();
		}
		PxPyPzEVector phPVec{};
		PxPyPzEVector sigmaPVec{};
		Float_t sigmaPx{};
		Float_t sigmaPy{};
		Float_t sigmaPz{};
		Float_t sigmaM2{};
		Float_t sigmaM{};
		Float_t sigmaP{};
		Float_t sigmaE{};
		Float_t sigmaTheta{};
		Float_t sigmaPhi{};
		Float_t tSigma{};
		Float_t mM2ep2ekpPigX{};
		Float_t mPep2ekpPigX{};
};

class pipiBkg: public lamNoPi
{
	public:
		pipiBkg() = default;
		pipiBkg(PxPyPzEVector paraBeam, PxPyPzEVector paraTarget, PxPyPzEVector paraEl, PxPyPzEVector paraKa, PxPyPzEVector paraPr, PxPyPzEVector paraPip): lamNoPi(paraBeam, paraTarget, paraEl, paraKa, paraPr), pipPVec(paraPip) {
			calculateVarsPipi();
		}
		void calculateVarsPipi() {
			PxPyPzEVector realMisPrPiPVec = elBeam + prTarget - elPVec - pipPVec;
			setPVecValues(realMisPrPiPVec, realMisPrPiPx, realMisPrPiPy, realMisPrPiPz, realMisPrPiP, realMisPrPiE, realMisPrPiM2, realMisPrPiM, realMisPrPiTheta, realMisPrPiPhi);
			PxPyPzEVector realMisPiPVec = elBeam + prTarget - elPVec - prPVec - pipPVec;
			setPVecValues(realMisPiPVec, realMisPiPx, realMisPiPy, realMisPiPz, realMisPiP, realMisPiE, realMisPiM2, realMisPiM, realMisPiTheta, realMisPiPhi);
		}
		PxPyPzEVector pipPVec{};
		PxPyPzEVector realMisPrPiPVec{};
		PxPyPzEVector realMisPiPVec{};
		Float_t realMisPiPx{};
		Float_t realMisPiPy{};
		Float_t realMisPiPz{};
		Float_t realMisPiM2{};
		Float_t realMisPiM{};
		Float_t realMisPiP{};
		Float_t realMisPiE{};
		Float_t realMisPiTheta{};
		Float_t realMisPiPhi{};
		Float_t realMisPrPiPx{};
		Float_t realMisPrPiPy{};
		Float_t realMisPrPiPz{};
		Float_t realMisPrPiM2{};
		Float_t realMisPrPiM{};
		Float_t realMisPrPiP{};
		Float_t realMisPrPiE{};
		Float_t realMisPrPiTheta{};
		Float_t realMisPrPiPhi{};
};

#endif
