/*

   Written by D. Kikola
   2009
   Edited by Christopher B. Powell
   2010
   CBPowell@lbl.gov
   --------------------------------

   Histogram class to initialise, write, 
   save and delete histograms. Histos 
   are only written if they have entries.

*/

#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH3.h>
#include <TH2.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TText.h>
#include <TLegend.h>
#include <TLine.h>
#include "HistogramsQa.h"
#include <TProfile.h>
using namespace std;

HistogramsQa::HistogramsQa() {

	cout <<"\tInitialising Histograms";

	//Event Histograms
	hVzVsVpdVz_noCut = new TH2F("hVzVsVpdVz_noCut", ";TPC Vz (cm);VPD Vz (cm)", 500, -100, 100, 500, -100, 100);
	hVzVsVpdVz = new TH2F("hVzVsVpdVz", ";TPC Vz (cm);VPD Vz (cm)", 500, -100, 100, 500, -100, 100);
	hVzVsVpdVzAccept = new TH2F("hVzVsVpdVzAccept", ";TPC Vz (cm);VPD Vz (cm)", 500, -100, 100, 500, -100, 100);
	hVzVsDeltaVz_noCut = new TH2F("hVzVsDeltaVz_noCut", ";TPC Vz (cm);#Delta Vz (cm)", 500, -100, 100, 500, -100, 100);
	hVzVsDeltaVz = new TH2F("hVzVsDeltaVz", ";TPC Vz (cm);#Delta Vz (cm)", 500, -100, 100, 500, -100, 100);
	hDeltaVz_noCut = new TH1F("hDeltaVz_noCut", ";TPC Vz - VPD Vz (cm)", 200, -10, 10);
	hDeltaVz = new TH1F("hDeltaVz", ";TPC Vz - VPD Vz (cm)", 200, -10, 10);
	hVpdVzVsDeltaVz_noCut = new TH2F("hVpdVzVsDeltaVz_noCut", ";VPD Vz (cm);#Delta Vz (cm)", 200, -100, 100, 200, -100, 100);
	hVpdVzVsDeltaVz = new TH2F("hVpdVzVsDeltaVz", ";VPD Vz (cm);#Delta Vz (cm)", 200, -100, 100, 200, -100, 100);
	hVxVy = new TH2F("hVxVy", "; X (cm);Y (cm)", 200, -1.5, 1.5, 200, -1.5, 1.5);
	hRef = new TH1F("hRef", ";TPC Refmult", 500, 0, 1000);
	hRefVsVz_noCut = new TH2F("hRefVsVz_noCut", ";TPC Vz (cm);TPC Refmult", 200, -100, 100,500,0,1000);
	hRefVsVz_vpdCut = new TH2F("hRefVsVz_vpdCut", ";TPC Vz (cm);TPC Refmult",  200, -100, 100,500,0,1000);
	hRefVsVz = new TH2F("hRefVsVz", ";TPC Vz (cm);TPC Refmult",  200, -100, 100,500,0,1000);
	hgRefVsVz = new TH2F("hgRefVsVz", ";TPC Vz (cm);gRefmult", 200, -100, 100,500,0,1000);
	hRefVsDeltaVz_noCut = new TH2F("hRefVsDeltaVz_noCut", ";#Delta Vz (cm);TPC Refmult", 200, -100, 100,500,0,1000);
	hRefVsDeltaVz = new TH2F("hRefVsDeltaVz", ";#Delta Vz (cm);TPC Refmult", 200, -100, 100,500,0,1000);

	hRefVsBemcMatch = new TH2F("hRefVsBemcMatch", ";nBEMCMatch;TPC Refmult", 500, 0, 1000, 500, 0, 1000);
	hRefVsBtofMatch = new TH2F("hRefVsBtofMatch", ";nBTOFMatch;TPC Refmult", 500, 0, 1000, 500, 0, 1000);
	hgRefVsBemcMatch = new TH2F("hgRefVsBemcMatch", ";nBEMCMatch;gRefmult", 500, 0, 1000, 500, 0, 1000);
	hgRefVsBtofMatch = new TH2F("hgRefVsBtofMatch", ";nBTOFMatch;gRefmult", 500, 0, 1000, 500, 0, 1000);
	hRefVsGlobalRef = new TH2F("hRefVsGlobalRef", ";Number of Globals;TPC Refmult", 500, 0, 5000, 500, 0, 1000);
	hRefVsGRef = new TH2F("hRefVsGRef", ";TPC Refmult;gRefMult", 500, 0, 1000, 500, 0, 1000);
	hRefVsTofRef = new TH2F("hRefVsTofRef", ";TOF Refmult;TPC Refmult", 500, 0, 5000, 500, 0, 1000);
	hRefVsBbcX = new TH2F("hRefVsBbcX", ";bbc X (kHz);TPC Refmult", 500, 0, 50, 500,0, 1000);
	hRefVsZdcX = new TH2F("hRefVsZdcX", ";zdc X (kHz);TPC Refmult", 500,0,50,500, 0, 1000);
	hgRefVsZdcX = new TH2F("hgRefVsZdcX", ";zdc X (kHz);gRefmult", 500,0,50,500, 0, 1000);
	hRefVsRunId = new TH2F("hRefVsRunId", ";run Id * 1e4;TPC Refmult", 1000, 1500, 1535, 500, 0, 1000);
	hgRefVsRunId = new TH2F("hgRefVsRunId", ";run Id * 1e4;gRefmult;", 1000, 1500, 1535, 500, 0, 1000);
	hVzVsRunId = new TH2F("hVzVsRunId", ";run Id * 1e4;Vz", 1000, 1500, 1535, 200, -100, 100);
	hRefVsFillId = new TH2F("hRefVsFillId", ";fill Id;TPC Refmult", 1000, 11000, 12000,500,0,1000);

	hRefVsBemcMatch_noCut = new TH2F("hRefVsBemcMatch_noCut", ";TPC Refmult;nBEMCMatch", 1000, 0, 1000, 1000, 0, 1000);
	hRefVsGlobalRef_noCut = new TH2F("hRefVsGlobalRef_noCut", ";TPC Refmult;Number of Globals", 1000, 0, 1000, 500, 0, 5000);
	hRefVsTofRef_noCut = new TH2F("hRefVsTofRef_noCut", ";TPC Refmult;TOF Refmult", 1000, 0, 1000, 500, 0, 5000);
	hRefVsBbcX_noCut = new TH2F("hRefVsBbcX_noCut", ";TPC Refmult;bbc X (kHz)", 1000, 0, 1000, 500, 0, 50);
	hRefVsZdcX_noCut = new TH2F("hRefVsZdcX_noCut", ";TPC Refmult;zdc X (kHz)", 1000, 0, 1000, 500, 0, 50);
	hRefVsRunId_noCut = new TH2F("hRefVsRunId_noCut", ";run Id * 1e4; TPC Refmult",  1000, 1500, 1518, 1000, 0, 1000);
	hRefVsFillId_noCut = new TH2F("hRefVsFillId_noCut", ";TPC Refmult;fill Id", 1000, 0, 1000, 500, 11000, 12000);

	hgRefVsPxlInnerHits = new TH2F("hgRefVsPxlInnerHits",";nPxlInnerHits;gRefmult;",500,0,20000,500,0,1000);
	hgRefVsPxlOuterHits = new TH2F("hgRefVsPxlOuterHits",";nPxlOuterHits;gRefmult;",500,0,10000,500,0,1000);
	hgRefVsIstHits = new TH2F("hgRefVsIstHits",";nIstHits;gRefmult;",500,0,5000,500,0,1000);



	//	 After Events level Cut
	hDeltaVzEvtAccept = new TH1F("hDeltaVzEvtAccept", ";TPC Vz - VPD Vz (cm)", 200, -10, 10);
	hVzVsDeltaVzEvtAccept = new TH2F("hVzVsDeltaVzEvtAccept", ";TPC Vz (cm);#Delta Vz (cm)", 1000, -100, 100, 1000, -100, 100);
	hVxVyEvtAccept = new TH2F("hVxVyEvtAccept", "; X (cm);Y (cm)", 200, -1.5, 1.5, 200, -1.5, 1.5);
	hRefEvtAccept = new TH1F("hRefEvtAccept", ";TPC Refmult", 500, 0, 1000);
	hRefVsVzEvtAccept = new TH2F("hRefVsVzEvtAccept", ";TPC Vz (cm);TPC Refmult", 200, -100, 100, 500, 0, 1000);
	hgRefVsVzEvtAccept = new TH2F("hgRefVsVzEvtAccept", ";TPC Vz (cm);gRefmult", 200, -100, 100, 500, 0, 1000);
	hRefVsDeltaVzEvtAccept = new TH2F("hRefVsDeltaVzEvtAccept", ";#Delta Vz (cm);TPC Refmult", 200, -100, 100, 500, 0, 1000);
	hRefVsBemcMatchEvtAccept = new TH2F("hRefVsBemcMatchEvtAccept", ";nBEMCMatch;TPC Refmult", 500, 0, 1000, 500, 0, 1000);
	hRefVsBtofMatchEvtAccept = new TH2F("hRefVsBtofMatchEvtAccept", ";nBTOFMatch;TPC Refmult", 500, 0, 1000, 500, 0, 1000);
	hgRefVsBemcMatchEvtAccept = new TH2F("hgRefVsBemcMatchEvtAccept", ";nBEMCMatch;gRefmult", 500, 0, 1000, 500, 0, 1000);
	hgRefVsBtofMatchEvtAccept = new TH2F("hgRefVsBtofMatchEvtAccept", ";nBTOFMatch;gRefmult", 500, 0, 1000, 500, 0, 1000);
	hRefVsGlobalRefEvtAccept = new TH2F("hRefVsGlobalRefEvtAccept", ";Number of Globals;TPC Refmult", 500, 0, 5000, 500, 0, 1000);
	hRefVsGRefEvtAccept = new TH2F("hRefVsGRefEvtAccept", ";gRefMult;TPC Refmult", 500, 0, 1000, 500, 0, 1000);
	hRefVsTofRefEvtAccept = new TH2F("hRefVsTofRefEvtAccept", ";TOF Refmult;TPC Refmult", 500, 0, 5000, 500, 0, 1000);
	hRefVsBbcXEvtAccept = new TH2F("hRefVsBbcXEvtAccept", ";bbc X (kHz);TPC Refmult", 500,0,50,500, 0, 1000);
	hRefVsZdcXEvtAccept = new TH2F("hRefVsZdcXEvtAccept", ";zdc X (kHz);TPC Refmult",500,0,50,500, 0, 1000);
	hgRefVsZdcXEvtAccept = new TH2F("hgRefVsZdcXEvtAccept", ";zdc X (kHz);gRefmult", 500,0,50,500, 0, 1000);
	hRefVsRunIdEvtAccept = new TH2F("hRefVsRunIdEvtAccept", ";run Id * 1e4;TPC Refmult", 1000, 1500, 1535, 500, 0, 1000);
	hgRefVsRunIdEvtAccept = new TH2F("hgRefVsRunIdEvtAccept", ";run Id * 1e4;gRefmult", 1000, 1500, 1535, 500, 0, 1000);
	hVzVsRunIdEvtAccept = new TH2F("hVzVsRunIdEvtAccept", ";run Id * 1e4;Vz", 1000, 1500, 1535, 200, -100, 100);
	hRefVsFillIdEvtAccept = new TH2F("hRefVsFillIdEvtAccept", ";fill Id;TPC Refmult", 1000, 0, 1000, 1000, 11000, 12000);
	hVpdVzVsDeltaVzEvtAccept = new TH2F("hVpdVzVsDeltaVzEvtAccept", ";VPD Vz (cm);#Delta Vz (cm)", 200, -100, 100, 200, -100, 100);



	//TPC
	hSigmaEvsP_noCut = new TH2F("hSigmaEvsP_noCut", ";p (GeV/c);n#sigma_{e}",200, 0, 5, 200, -10, 10);//Mar13
	hSigmaEvsP = new TH2F("hSigmaEvsP", ";p (GeV/c);n#sigma_{e}",200, 0, 5, 200, -10, 10);//Mar13
	hSigmaPvsP_noCut = new TH2F("hSigmaPvsP_noCut", ";p (GeV/c);n#sigma_{p}",200, 0, 5, 200, -10, 10);//Mar13
	hSigmaPvsP = new TH2F("hSigmaPvsP", ";p (GeV/c);n#sigma_{p}",200, 0, 5, 200, -10, 10);//Mar13
	hSigmaPivsP_noCut = new TH2F("hSigmaPivsP_noCut", ";p (GeV/c);n#sigma_{#pi}",200, 0, 5, 200, -10, 10);//Mar13
	hSigmaPivsP = new TH2F("hSigmaPivsP", ";p (GeV/c);n#sigma_{#pi}", 200, 0, 5, 200, -10, 10);//Mar13
	hSigmaKvsP_noCut = new TH2F("hSigmaKvsP_noCut", ";p (GeV/c);n#sigma_{K}", 200, 0, 5, 200, -10, 10);//Mar13
	hSigmaKvsP = new TH2F("hSigmaKvsP", ";p (GeV/c);n#sigma_{K}", 200, 0, 5, 200, -10, 10);//Mar13
	hnHitsVsP_noCut = new TH2F("hnHitsVsP_noCut",";nHits;p (GeV/c)",50.,0.0,50.0, 200, 0., 10.);
	hnHitsVsP = new TH2F("hnHitsVsP",";nHits;p (GeV/c)",50.,0.0,50.0, 200, 0., 10.);
	hnHits = new TH1F("hnHits",";nHits * charge",100.,-50.0,50.0);
	hnHitsVsEta_noCut = new TH2F("hnHitsVsEta_noCut",";nHits",50.,0.0,50.0, 200, -1.5, 1.5);
	hnHitsVsEta = new TH2F("hnHitsVsEta",";nHits;#eta",50.,0.0,50.0, 200, -1.5, 1.5);
	hnHitsToMaxPts_noCut = new TH1F("hnHitsToMaxPts_noCut",";nHits/nHitsPoss",100,0.0,1.0);
	hnHitsToMaxPts = new TH1F("hnHitsToMaxPts",";nHits/nHitsPoss",120,0.0,1.2);
	hnHitsVsMaxPts_noCut = new TH2F("hnHitsVsMaxPts_noCut",";nHits;nPossible", 50, 0., 50., 50., 0., 50.0);
	hnHitsVsMaxPts = new TH2F("hnHitsVsMaxPts",";nHits;nPossible", 50, 0., 50., 50., 0., 50.0);
	hPtVsEta_noCut = new TH2F("hPtVsEta_noCut",";p_{T} (GeV/c);#eta",200, 0, 5,200,-1.5,1.5);
	hPtVsEta = new TH2F("hPtVsEta",";p_{T} (GeV/c);#eta",200, 0, 5,200,-1.5,1.5);
	hHFTPtVsEta = new TH2F("hHFTPtVsEta",";p_{T} (GeV/c);#eta",200, 0, 5,200,-1.5,1.5);
	hDedxVsP_noCut = new TH2F("hDedxVsP_noCut","; momentum (GeV/c);dE/dx (keV/cm)", 200, 0., 10., 200., 0., 10.);
	hDedxVsP = new TH2F("hDedxVsP","; momentum (GeV/c);dE/dx (keV/cm)", 200, 0., 10., 200., 0., 10.);
	hnDedxVsP_noCut = new TH2F("hnDedxVsP_noCut",";nDedxPts; p (GeV/c)",50.,0.0,50.0, 200, 0., 10.);
	hnDedxVsP = new TH2F("hnDedxVsP",";nDedxPts; p (GeV/c)",50.,0.0,50.0, 200, 0., 10.);
	hnDedx = new TH1F("hnDedx",";nDedxPts",50.,0.0,50);

	hChi2Prob = new TH1F("hChi2Prob", ";#chi^{2}", 200, 0, 100);
	hPhiVsPt = new TH2F("hPhiVsPt",";#phi;p_{T} (GeV/c)",200,-4.,4.0,200,0.0,10.0);
	hPt = new TH1F("hPt",";p_{T} (GeV/c)",200,0.0,10.0);
	hPtTof = new TH1F("hPtTof","hPt with Tof Match;p_{T} (GeV/c)",200,0.0,10.0);
	hPtNoTof = new TH1F("hPtNoTof","hPt No Tof Match;p_{T} (GeV/c)",200,0.0,10.0);
	hHFTPhiVsPt = new TH2F("hHFTPhiVsPt",";#phi;p_{T} (GeV/c)",200,-4.,4.0,200,0.0,10.0);
	hHFTPt = new TH1F("hHFTPt",";p_{T} (GeV/c)",200,0.0,10.0);
	hHFTPtTof = new TH1F("hHFTPtTof","HFT with Tof;p_{T} (GeV/c)",200,0.0,10.0);
	hPhiVsEta = new TH2F("hPhiVsEta",";#phi;#eta",200,-4.,4.0, 200, -1.5, 1.5);
	hHFTPhiVsEta = new TH2F("hHFTPhiVsEta",";#phi;#eta",200,-4.,4.0, 200, -1.5, 1.5);
	//hPhiVsEtaVsPt = new TH3F("hPhiVsEtaVsPt",";#phi;#eta;p_{T} (GeV/c)",200,-4.,4.0, 200, -1.5, 1.5, 200, 0, 5);
	//Vz
	hZVsPt = new TH2F("hZVsPt",";Vertex Z;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	hZplusVsPt = new TH2F("hZplusVsPt",";Zplus pxlinner;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	hZplus2VsPt = new TH2F("hZplus2VsPt",";Zplus2 pxlouter;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	hHFTZVsPt = new TH2F("hHFTZVsPt",";Vertex Z;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	hHFTZplusVsPt = new TH2F("hHFTZplusVsPt",";Zplus pxlinner;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	hHFTZplus2VsPt = new TH2F("hHFTZplus2VsPt",";Zplus2 pxlouter;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	hNoHFTZVsPt = new TH2F("hNoHFTZVsPt",";Vertex Z;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	hNoHFTZplusVsPt = new TH2F("hNoHFTZplusVsPt",";Zplus pxlinner;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	hNoHFTZplus2VsPt = new TH2F("hNoHFTZplus2VsPt",";Zplus2 pxlouter;p_{T} (GeV/c)",1000,-10,10.0,200,0.0,10.0);
	//Vz Vs Phi
	hZVsPhi = new TH2F("hZVsPhi",";Vertex Z;#phi",1000,-10,10.0,400,-4.,4.0);
	hZplusVsPhi = new TH2F("hZplusVsPhi",";Zplus pxlinner;#phi",1000,-10,10.0,400,-4.,4.0);
	hZplus2VsPhi = new TH2F("hZplus2VsPhi",";Zplus2 pxlouter;#phi",1000,-10,10.0,400,-4.,4.0);
	hHFTZVsPhi = new TH2F("hHFTZVsPhi",";Vertex Z;#phi",1000,-10,10.0,400,-4.,4.0);
	hHFTZplusVsPhi = new TH2F("hHFTZplusVsPhi",";Zplus pxlinner;#phi",1000,-10,10.0,400,-4.,4.0);
	hHFTZplus2VsPhi = new TH2F("hHFTZplus2VsPhi",";Zplus2 pxlouter;#phi",1000,-10,10.0,400,-4.,4.0);
	hNoHFTZVsPhi = new TH2F("hNoHFTZVsPhi",";Vertex Z;#phi",1000,-10,10.0,400,-4.,4.0);
	hNoHFTZplusVsPhi = new TH2F("hNoHFTZplusVsPhi",";Zplus pxlinner;#phi",1000,-10,10.0,400,-4.,4.0);
	hNoHFTZplus2VsPhi = new TH2F("hNoHFTZplus2VsPhi",";Zplus2 pxlouter;#phi",1000,-10,10.0,400,-4.,4.0);

	
	 //Hao's Pt Bin         
	 const int nPtBinsHao=22;
	   float ptBinsHao[nPtBinsHao+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9, 1., 1.2, 1.4, 2.,4.,6.,8.,10};
	   hPtHao = new TH1F("hPtHao",";p_{T} (GeV/c)",nPtBinsHao,ptBinsHao);
	   hPtNoTofHao = new TH1F("hPtNoTofHao",";p_{T} (GeV/c)",nPtBinsHao,ptBinsHao);
	   hHFTPtHao = new TH1F("hHFTPtHao",";p_{T} (GeV/c)",nPtBinsHao,ptBinsHao);


	hDca = new TH1F("hDca", ";dca (cm)", 300, 0, 3);
	hDcaVsPt = new TH2F("hDcaVsPt", ";dca (cm);p_{T} (GeV/c)", 300, 0, 3, 200, 0, 5);
	hDcaXyVsPt = new TH2F("hDcaXyVsPt", ";dcaXY (cm);p_{T} (GeV/c)", 300, -3, 3, 200, 0, 5);
	hDcaZVsPt = new TH2F("hDcaZVsPt", ";dcaZ (cm);p_{T} (GeV/c)", 300, -3, 3, 200, 0, 5);
	hDcaXyVsPhi = new TH2F("hDcaXyVsPhi", ";dcaXY (cm);#phi", 300, -3, 3, 200, -4, 4);
	hDcaZVsPhi = new TH2F("hDcaZVsPhi", ";dcaZ (cm);#phi", 300, -3, 3, 200, -4, 4);
	hDcaXyVsEta = new TH2F("hDcaXyVsEta", ";dcaXY (cm);#eta", 300, -3, 3, 200, -1.5, 1.5);
	hDcaZVsEta = new TH2F("hDcaZVsEta", ";dcaZ (cm);#eta", 300, -3, 3, 200, -1.5, 1.5);
	hDcaXyVsZdc = new TH2F("hDcaXyVsZdc", ";dcaXY (cm);zdc x(kHz)", 300, -3, 3, 500, 0, 50);
	hDcaZVsZdc = new TH2F("hDcaZVsZdc", ";dcaZ (cm);zdc x(kHz)", 300, -3, 3, 500, 0, 50);
	
	hTrackR = new TH1F("hTrackR",";InnerMost Hit R(cm) ",500,0,200);
	hHFTTrackR = new TH1F("hHFTTrackR",";InnerMost Hit R(cm) ",500,0,5);

	hHFTDca = new TH1F("hHFTDca", ";dca (cm)", 300, 0, 3);
	hHFTDcaVsPt = new TH2F("hHFTDcaVsPt", ";dca (cm);p_{T} (GeV/c)", 300, -3, 3, 200, 0, 5);
	hHFTDcaXyVsPt = new TH2F("hHFTDcaXyVsPt", ";dcaXY (cm);p_{T} (GeV/c)", 300, -3, 3, 200, 0, 5);
	hHFTDcaZVsPt = new TH2F("hHFTDcaZVsPt", ";dcaZ (cm);p_{T} (GeV/c)", 300, -3, 3, 200, 0, 5);
	hHFTDcaXyVsPhi = new TH2F("hHFTDcaXyVsPhi", ";dcaXY (cm);#phi", 300, -3, 3, 200, -4, 4);
	hHFTDcaZVsPhi = new TH2F("hHFTDcaZVsPhi", ";dcaZ (cm);#phi", 300, -3, 3, 200, -4, 4);
	hHFTDcaXyVsEta = new TH2F("hHFTDcaXyVsEta", ";dcaXY (cm);#eta", 300, -3, 3, 200, -1.5, 1.5);
	hHFTDcaZVsEta = new TH2F("hHFTDcaZVsEta", ";dcaZ (cm);#eta", 300, -3, 3, 200, -1.5, 1.5);
	hHFTDcaXyVsZdc = new TH2F("hHFTDcaXyVsZdc", ";dcaXY (cm);zdc x(kHz)", 300, -3, 3, 500, 0, 50);
	hHFTDcaZVsZdc = new TH2F("hHFTDcaZVsZdc", ";dcaZ (cm);zdc x(kHz)", 300, -3, 3, 500, 0, 50);
	//HFT Dca related Zoom In
	hHFTDcaZoomIn = new TH1F("hHFTDcaZoomIn", ";dca (cm)", 4000, 0, 0.1);
	hHFTDcaVsPtZoomIn = new TH2F("hHFTDcaVsPtZoomIn", ";dca (cm);p_{T} (GeV/c)", 4000, -0.1, 0.1, 200, 0, 5);
	hHFTDcaXyVsPtZoomIn = new TH2F("hHFTDcaXyVsPtZoomIn", ";dcaXY (cm);p_{T} (GeV/c)", 4000, -0.1, 0.1, 200, 0, 5);
	hHFTDcaZVsPtZoomIn = new TH2F("hHFTDcaZVsPtZoomIn", ";dcaZ (cm);p_{T} (GeV/c)", 4000, -0.1, 0.1, 200, 0, 5);
	hHFTDcaXyVsPhiZoomIn = new TH2F("hHFTDcaXyVsPhiZoomIn", ";dcaXY (cm);#phi", 4000, -0.1, 0.1, 200, -4, 4);
	hHFTDcaZVsPhiZoomIn = new TH2F("hHFTDcaZVsPhiZoomIn", ";dcaZ (cm);#phi", 4000, -0.1, 0.1, 200, -4, 4);
	hHFTDcaXyVsEtaZoomIn = new TH2F("hHFTDcaXyVsEtaZoomIn", ";dcaXY (cm);#eta", 4000, -0.1, 0.1, 200, -1.5, 1.5);
	hHFTDcaZVsEtaZoomIn = new TH2F("hHFTDcaZVsEtaZoomIn", ";dcaZ (cm);#eta", 4000, -0.1, 0.1, 200, -1.5, 1.5);
	hHFTDcaXyVsZdcZoomIn = new TH2F("hHFTDcaXyVsZdcZoomIn", ";dcaXY (cm);zdc x(kHz)", 4000, -0.1, 0.1, 500, 0, 50);
	hHFTDcaZVsZdcZoomIn = new TH2F("hHFTDcaZVsZdcZoomIn", ";dcaZ (cm);zdc x(kHz)", 4000, -0.1, 0.1, 500, 0, 50);

    
	const int nPhiBins=10;
	double PhiBins[nPhiBins+1];
	for(int i=0;i<=nPhiBins;i++) PhiBins[i]=0.338+i*3.1415926/5.;
	const int nDcaBins=4000;
	double DcaBins[nDcaBins+1];
	for(int i=0;i<=nDcaBins;i++) DcaBins[i]=-0.1+i*0.2/4000.;
	const int nZBins=500;
	double ZBins[nZBins+1];
	for(int i=0;i<=nZBins;i++) ZBins[i]=-50+i*100./500.;

	hHFTDcaXyVsZplusVsPhiZoomIn = new TH3F("hHFTDcaXyVsZplusVsPhiZoomIn",";dcaXY (cm);Zplus(cm);#phi", nDcaBins,DcaBins, nZBins,ZBins, nPhiBins, PhiBins);
	hHFTDcaZVsZplusVsPhiZoomIn = new TH3F("hHFTDcaZVsZplusVsPhiZoomIn",";dcaZ (cm);Zplus(cm);#phi", nDcaBins, DcaBins, nZBins,ZBins, nPhiBins, PhiBins );

	//Fill nHitsHFt Histograms  add by GNX
	//const Int_t nMapHFTBins =35 ;
	//Double_t MapHFTEdge[nMapHFTBins+1];
	//for(int i=0;i<nMapHFTBins;i++) MapHFTEdge[i]=i;
	//MapHFTEdge[nMapHFTBins]=129;
	//hnHitsMapHFT = new TH1F("hnHitsMapHFT",";nHitsMapHFT",nMapHFTBins,MapHFTEdge);
	hnHitsMapHFT = new TH1F("hnHitsMapHFT",";nHitsMapHFT",130,0,130);
	hnHitsHFT = new TH1F("hnHitsHFT",";nHitsHFT * charge",20.,-10.0,10.0);
	hnHitsPxlInner = new TH1F("hnHitsPxlInner",";nHitsPxlInner * charge",20.,-10.0,10.0);
	hnHitsPxlOuter = new TH1F("hnHitsPxlOuter",";nHitsPxlOuter * charge",20.,-10.0,10.0);
	hnHitsIst = new TH1F("hnHitsIst",";nHitsIst * charge",20.,-10.0,10.0);
	hnHitsSsd = new TH1F("hnHitsSsd",";nHitsSsd * charge",20.,-10.0,10.0);

// TPC require No HFT hits
	hNoHFTPtVsEta = new TH2F("hNoHFTPtVsEta",";p_{T} (GeV/c);#eta",200, 0, 5,200,-1.5,1.5);
	hNoHFTPhiVsPt = new TH2F("hNoHFTPhiVsPt",";#phi;p_{T} (GeV/c)",200,-4.,4.0,200,0.0,10.0);
	hNoHFTPt = new TH1F("hNoHFTPt",";p_{T} (GeV/c)",200,0.0,10.0);
	hNoHFTPtTof = new TH1F("hNoHFTPtTof","NoHFT with Tof;p_{T} (GeV/c)",200,0.0,10.0);
	hNoHFTPhiVsEta = new TH2F("hNoHFTPhiVsEta",";#phi;#eta",200,-4.,4.0, 200, -1.5, 1.5);
	hNoHFTPtHao = new TH1F("hNoHFTPtHao",";p_{T} (GeV/c)",nPtBinsHao,ptBinsHao);
	hNoHFTDca = new TH1F("hNoHFTDca", ";dca (cm)", 300, 0, 3);
	hNoHFTDcaVsPt = new TH2F("hNoHFTDcaVsPt", ";dca (cm);p_{T} (GeV/c)", 300, -3, 3, 200, 0, 5);
	hNoHFTDcaXyVsPt = new TH2F("hNoHFTDcaXyVsPt", ";dcaXY (cm);p_{T} (GeV/c)", 300, -3, 3, 200, 0, 5);
	hNoHFTDcaZVsPt = new TH2F("hNoHFTDcaZVsPt", ";dcaZ (cm);p_{T} (GeV/c)", 300, -3, 3, 200, 0, 5);
	hNoHFTDcaXyVsPhi = new TH2F("hNoHFTDcaXyVsPhi", ";dcaXY (cm);#phi", 300, -3, 3, 200, -4, 4);
	hNoHFTDcaZVsPhi = new TH2F("hNoHFTDcaZVsPhi", ";dcaZ (cm);#phi", 300, -3, 3, 200, -4, 4);
	hNoHFTDcaXyVsEta = new TH2F("hNoHFTDcaXyVsEta", ";dcaXY (cm);#eta", 300, -3, 3, 200, -1.5, 1.5);
	hNoHFTDcaZVsEta = new TH2F("hNoHFTDcaZVsEta", ";dcaZ (cm);#eta", 300, -3, 3, 200, -1.5, 1.5);
	hNoHFTDcaXyVsZdc = new TH2F("hNoHFTDcaXyVsZdc", ";dcaXY (cm);zdc x(kHz)", 300, -3, 3, 500, 0, 50);
	hNoHFTDcaZVsZdc = new TH2F("hNoHFTDcaZVsZdc", ";dcaZ (cm);zdc x(kHz)", 300, -3, 3, 500, 0, 50);


	//  MTD matching Histograms //Add by GNX
	hMtdDeltaYVspt = new TH2F("hMtdDeltaYVspt",";MtdDeltaY;p_{T} (GeV/c)",1000,-1000,1000,200,0,10);
	hMtdDeltaZVspt = new TH2F("hMtdDeltaZVspt",";MtdDeltaZ;p_{T} (GeV/c)",1000,-1000,1000,200,0,10);
	hMtdDeltaTOFVspt = new TH2F("hMtdDeltaTOFVspt",";MtdDeltaTOF;p_{T} (GeV/c)",1000,-1000,1000,200,0,10);
	hMtdMatchFlag=new TH1F("hMtdMatchFlag",";MtdMatchFlag",16,0,16);
	// Multiplicity and cell vs backleg
	// 1) all the MTD hits;
	// 2) MTD hits that are matched to tracks;
	// 3) MTD hits that fired the trigger.
	hMultMtdHit_1= new TH1F("all_Mtd_Hits",";Number of Mtd Hits",100,0,100);
	hMultMtdHit_11= new TH1F("all_Mtd_Hits11",";Number of Mtd Hits",100,0,100);
	hCellVsbackleg_1= new TH2F("hCellVsbackleg_all_Mtd_Hits",";backleg;Cell",35,0,35,70,0,70);
	hMultMtdHit_2= new TH1F("Mtd_Hits_mathch_tracks",";Number of Mtd Hits",100,0,100);
	hCellVsbackleg_2= new TH2F("hCellVsbackleg_Mtd_Hits_matvh_tracks",";backleg;Cell",35,0,35,70,0,70);
	hMultMtdHit_3= new TH1F("Mtd_Hits_fire_trigger",";Number of Mtd Hits",100,0,100);
	hCellVsbackleg_3= new TH2F("hCellVsbackleg_Mtd_Hits_fire_trigger",";backleg;Cell",35,0,35,70,0,70);
	hMtdTriggerFlag=new TH1F("hMtdTriggerFlag",";MtdTriggerFlag",20,0,20);


	//TOF and BSMD
	//------------------------------+
	//  add by Zebo Tang
	hTofModulevsTray =  new TH2F("hTofModulevsTray",";trayId;moduleId;Counts",120,0,120,32,0,32);
	hTofvsBoardId =  new TH2F("hTofvsBoardId",";TDig BoardId;Tof (ns);Counts",1000,0,1000,100,0,100);
	hBetaDiffvsTrayIdPi = new TH2F("hBetaDiffvsTrayIdPi",";TOF TrayId;1/#beta-1/#beta_expected;Counts",120,0,120,100,-0.1,0.1);
	hBetaDiffvsTrayIdPiPlus = new TH2F("hBetaDiffvsTrayIdPiPlus",";TOF TrayId;1/#beta-1/#beta_expected;Counts",120,0,120,100,-0.1,0.1);
	hBetaDiffvsTrayIdPiMinus = new TH2F("hBetaDiffvsTrayIdPiMinus",";TOF TrayId;1/#beta-1/#beta_expected;Counts",120,0,120,100,-0.1,0.1);
	hBetaDiffvsTrayIdProton = new TH2F("hBetaDiffvsTrayIdProton",";TOF TrayId;1/#beta-1/#beta_expected;Counts",120,0,120,100,-0.1,0.1);
	hBetaDiffvsTrayIdPbar = new TH2F("hBetaDiffvsTrayIdPbar",";TOF TrayId;1/#beta-1/#beta_expected;Counts",120,0,120,100,-0.1,0.1);
	hTofPhivsZ = new TH2F("hTofPhivsZ",";TOF Hits Z (cm); TOF Hits #phi;Counts",1000,-250,250,1800,-3.14159265,3.14159265);
	hTofDiffvsTrayIdPi = new TH2F("hTofDiffvsTrayIdPi",";TOF TrayId;Tof-Tof_expected;Counts",120,0,120,100,-1,1);
	hTofDiffvsTrayIdPiPlus = new TH2F("hTofDiffvsTrayIdPiPlus",";TOF TrayId;Tof-Tof_expected;Counts",120,0,120,100,-1,1);
	hTofDiffvsTrayIdPiMinus = new TH2F("hTofDiffvsTrayIdPiMinus",";TOF TrayId;Tof-Tof_expected;Counts",120,0,120,100,-1,1);
	hTofDiffvsTrayIdProton = new TH2F("hTofDiffvsTrayIdProton",";TOF TrayId;Tof-Tof_expected;Counts",120,0,120,100,-1,1);
	hTofDiffvsTrayIdPbar = new TH2F("hTofDiffvsTrayIdPbar",";TOF TrayId;Tof-Tof_expected;Counts",120,0,120,100,-1,1);
	hMassvsp = new TH2F("hMassvsp",";p (GeV/c);m^{2} (GeV^{2}/c^{4});Counts",500,0,5,500,-0.2,1.2);
	//------------------------------+

	hPvsTimeOfFlight = new TH2F("hPvsTimeOfFlight", ";p (GeV/c); time of flight", 200, 0, 5, 200, 0, 100);
	hPvsBeta = new TH2F("hPvsBeta", ";p (GeV/c);#beta;", 200, 0, 5, 200, 0, 5);
	hPvsInvBeta = new TH2F("hPvsInvBeta", ";p (GeV/c);1/#beta;", 200, 0, 5, 200, 0, 5);
	hSmdHitsEtaVsPhi = new TH2F("hSmdHitsEtaVsPhi", ";#eta;#phi", 10, 0, 10, 10, 0, 10);

	//BEMC
	hEmcTowerId = new TH1F("hEmcTowerId", ";towerId", 4801, 0, 4801);
	hEmcDzVsDphi = new TH2F("hEmcDzVsDphi", ";#Delta Z;#Delta #phi x R", 200, -50, 50, 200, -50, 50);
	hEmcDistVsP = new TH2F("hEmcDistVsP", ";distance (#Delta Z,  R #Delta #phi);p (GeV/c)", 200, 0, 100, 200, 0, 5);
	hEmcEnergyRatioVsDist = new TH2F("hEmcEnergyRatioVsDist", ";e / e3; distance", 200, 0, 5, 200, 0, 5);
	hEmcEnergyHitVsP = new TH2F("hEmcEnergyHitVsP", ";e0 (GeV); p (GeV/c)", 200, 0, 5, 200, 0, 5);
	hEmcEnergyTowVsP = new TH2F("hEmcEnergyTowVsP", ";e1 (GeV); p (GeV/c)", 200, 0, 5, 200, 0, 5);
	hEmcEnergySumVsP = new TH2F("hEmcEnergySumVsP", ";e (GeV); p (GeV/c)", 200, 0, 5, 200, 0, 5);
	hEoPvsP_Hit = new TH2F("hEoPvsP_Hit", ";e0 / p;p (GeV/c)", 200, 0, 5, 200, 0, 5);
	hEoPvsP_Tow = new TH2F("hEoPvsP_Tow", ";e / p;p (GeV/c)", 200, 0, 5, 200, 0, 5);
	hEoPvsP_Sum = new TH2F("hEoPvsP_Sum", ";e3 / p;p (GeV/c)", 200, 0, 5, 200, 0, 5);

	//Electron Plots
	for(int icut = 0;icut < 5; icut++){

		char ename[50];

		sprintf(ename, "hElectronDedxVsP_%d", icut);
		hElectronDedxVsP[icut] = new TH2F(ename, ";p (GeV/c); dE/dx (keV/cm)", 1000, 0, 10, 1000, 0, 10);
		sprintf(ename, "hElectronSigmaEvsP_%d", icut);
		hElectronSigmaEvsP[icut] = new TH2F(ename, ";n#sigma_{e};p (GeV/c)", 250, -10, 10, 1000, 0, 10);
		sprintf(ename, "hElectronEvsP_%d", icut);
		hElectronEvsP[icut] = new TH2F(ename, ";Energy (GeV);p (GeV/c)", 1000, 0, 10, 1000, 0, 10);
		sprintf(ename, "hElectronEoPvsP_%d", icut);
		hElectronEoPvsP[icut] = new TH2F(ename, ";E / p;p (GeV/c)", 1000, 0, 10, 1000, 0, 10);
		sprintf(ename, "hElectronPvsInvBeta_%d", icut);
		hElectronPvsInvBeta[icut] = new TH2F(ename, ";p (GeV/c);1 / #beta", 1000, 0, 10, 1000, 0, 10);
		sprintf(ename, "hElectronSmdEtaVsPhi_%d", icut);
		hElectronSmdEtaVsPhi[icut] = new TH2F(ename, ";SMD #eta hits;SMD #phi hits", 10, 0, 10, 10, 0, 10);
		sprintf(ename, "hElectronSmdDistVsP_%d", icut);
		hElectronSmdDistVsP[icut] = new TH2F(ename, ";Distance (Z, R #phi);p (GeV/c)", 200, 0, 100, 1000, 0, 10);

		sprintf(ename, "hMassVsPt_NN_%d", icut);
		hMassVsPt_NN[icut] = new TH2F(ename, ";mass (GeV/c^{2});p_{T} (GeV/c)", 1000, 0, 10, 1000, 0, 10);
		sprintf(ename, "hMassVsPt_PP_%d", icut);
		hMassVsPt_PP[icut] = new TH2F(ename, ";mass (GeV/c^{2});p_{T} (GeV/c)", 1000, 0, 10, 1000, 0, 10);
		sprintf(ename, "hMassVsPt_NP_%d", icut);
		hMassVsPt_NP[icut] = new TH2F(ename, ";mass (GeV/c^{2});p_{T} (GeV/c)", 1000, 0, 10, 1000, 0, 10);

		sprintf(ename, "hMassVsPhi_NN_%d", icut);
		hMassVsPhi_NN[icut] = new TH2F(ename, ";mass (GeV/c^{2});#phi", 1000, 0, 10, 200, -4, 4);
		sprintf(ename, "hMassVsPhi_PP_%d", icut);
		hMassVsPhi_PP[icut] = new TH2F(ename, ";mass (GeV/c^{2});#phi", 1000, 0, 10, 200, -4, 4);
		sprintf(ename, "hMassVsPhi_NP_%d", icut);
		hMassVsPhi_NP[icut] = new TH2F(ename, ";mass (GeV/c^{2});#phi", 1000, 0, 10, 200, -4, 4);

		sprintf(ename, "hYvsPt_NN_%d", icut);
		hYvsPt_NN[icut] = new TH2F(ename, ";Rapidity;p_{T} (GeV/c)", 200, -2, 2, 1000, 0, 10);
		sprintf(ename, "hYvsPt_PP_%d", icut);
		hYvsPt_PP[icut] = new TH2F(ename, ";Rapidity;p_{T} (GeV/c)", 200, -2, 2, 1000, 0, 10);
		sprintf(ename, "hYvsPt_NP_%d", icut);
		hYvsPt_NP[icut] = new TH2F(ename, ";Rapidity;p_{T} (GeV/c)", 200, -2, 2, 1000, 0, 10);

	}

	//V0 Histo
	hKShort = new TH2F("hKShort","K^{s}_{0} mass vs p_{T}",500,0.4,0.6,500,0.,10.);
	hLambda =new TH2F("hLambda","#Lambda mass vs p_{t}",500,1.08,1.16,500,0.,10.) ;
	hLambdabar =new TH2F("hLambdabar","#bar{#Lambda} mass vs p_{T}",500,1.08,1.16,500,0.,10.);  
	//Event plane

	EventPlane = new TH1F("EventPlane","EventPlane",180,0.,3.14159);
	CosRes_eta = new TProfile("CosRes_eta","CosRes_eta",10,0,10,-1.,1.);
	CosRes_ran = new TProfile("CosRes_ran","CosRes_ran",10,0,10,-1.,1.);
	cout << " ...done with HistogramsQa Init()!" << endl;
}

void HistogramsQa::writeData(TFile *outputFile) const {

	outputFile->cd();

	cout<<"\nWriting histos..."<<endl;

	if(hVzVsVpdVz_noCut->GetEntries())hVzVsVpdVz_noCut->Write();
	if(hVzVsVpdVz->GetEntries())hVzVsVpdVz->Write();
	if(hVzVsVpdVzAccept->GetEntries())hVzVsVpdVzAccept->Write();
	if(hVzVsDeltaVz_noCut->GetEntries())hVzVsDeltaVz_noCut->Write();
	if(hVzVsDeltaVz->GetEntries())hVzVsDeltaVz->Write();
	if(hDeltaVz_noCut->GetEntries())hDeltaVz_noCut->Write();
	if(hDeltaVz->GetEntries())hDeltaVz->Write();
	if(hVpdVzVsDeltaVz_noCut->GetEntries())hVpdVzVsDeltaVz_noCut->Write();
	if(hVpdVzVsDeltaVz->GetEntries())hVpdVzVsDeltaVz->Write();
	if(hVxVy->GetEntries())hVxVy->Write();
	if(hRef->GetEntries())hRef->Write();
	if(hRefVsVz_noCut->GetEntries())hRefVsVz_noCut->Write();
	if(hRefVsVz_vpdCut->GetEntries())hRefVsVz_vpdCut->Write();
	if(hRefVsVz->GetEntries())hRefVsVz->Write();
	if(hgRefVsVz->GetEntries())hgRefVsVz->Write();
	if(hRefVsDeltaVz_noCut->GetEntries())hRefVsDeltaVz_noCut->Write();
	if(hRefVsDeltaVz->GetEntries())hRefVsDeltaVz->Write();

	if(hRefVsGlobalRef_noCut->GetEntries())hRefVsGlobalRef_noCut->Write();
	if(hRefVsGlobalRef->GetEntries())hRefVsGlobalRef->Write();
	if(hRefVsGRef->GetEntries())hRefVsGRef->Write();
	if(hRefVsBemcMatch_noCut->GetEntries())hRefVsBemcMatch_noCut->Write();
	if(hRefVsBemcMatch->GetEntries())hRefVsBemcMatch->Write();
	if(hRefVsBtofMatch->GetEntries())hRefVsBtofMatch->Write();
	if(hgRefVsBemcMatch->GetEntries())hgRefVsBemcMatch->Write();
	if(hgRefVsBtofMatch->GetEntries())hgRefVsBtofMatch->Write();
	if(hRefVsTofRef_noCut->GetEntries())hRefVsTofRef_noCut->Write();
	if(hRefVsTofRef->GetEntries())hRefVsTofRef->Write();
	if(hRefVsBbcX_noCut->GetEntries())hRefVsBbcX_noCut->Write();
	if(hRefVsBbcX->GetEntries())hRefVsBbcX->Write();
	if(hRefVsZdcX_noCut->GetEntries())hRefVsZdcX_noCut->Write();
	if(hRefVsZdcX->GetEntries())hRefVsZdcX->Write();
	if(hgRefVsZdcX->GetEntries())hgRefVsZdcX->Write();
	if(hRefVsRunId_noCut->GetEntries())hRefVsRunId_noCut->Write();
	if(hRefVsRunId->GetEntries())hRefVsRunId->Write();
	if(hgRefVsRunId->GetEntries())hgRefVsRunId->Write();
	if(hVzVsRunId->GetEntries())hVzVsRunId->Write();
	if(hRefVsFillId_noCut->GetEntries())hRefVsFillId_noCut->Write();
	if(hRefVsFillId->GetEntries())hRefVsFillId->Write();

	if(hgRefVsPxlInnerHits->GetEntries())hgRefVsPxlInnerHits->Write();
	if(hgRefVsPxlOuterHits->GetEntries())hgRefVsPxlOuterHits->Write();
	if(hgRefVsIstHits->GetEntries())hgRefVsIstHits->Write();

	//	 After Events level Cut
	if(hDeltaVzEvtAccept->GetEntries())hDeltaVzEvtAccept->Write();
	if(hVzVsDeltaVzEvtAccept->GetEntries())hVzVsDeltaVzEvtAccept->Write();
	if(hVxVyEvtAccept->GetEntries())hVxVyEvtAccept->Write();
	if(hRefEvtAccept->GetEntries())hRefEvtAccept->Write();
	if(hRefVsVzEvtAccept->GetEntries())hRefVsVzEvtAccept->Write();
	if(hgRefVsVzEvtAccept->GetEntries())hgRefVsVzEvtAccept->Write();
	if(hRefVsDeltaVzEvtAccept->GetEntries())hRefVsDeltaVzEvtAccept->Write();
	if(hRefVsBemcMatchEvtAccept->GetEntries())hRefVsBemcMatchEvtAccept->Write();
	if(hRefVsBtofMatchEvtAccept->GetEntries())hRefVsBtofMatchEvtAccept->Write();
	if(hgRefVsBemcMatchEvtAccept->GetEntries())hgRefVsBemcMatchEvtAccept->Write();
	if(hgRefVsBtofMatchEvtAccept->GetEntries())hgRefVsBtofMatchEvtAccept->Write();
	if(hRefVsGlobalRefEvtAccept->GetEntries())hRefVsGlobalRefEvtAccept->Write();
	if(hRefVsGRefEvtAccept->GetEntries())hRefVsGRefEvtAccept->Write();
	if(hRefVsTofRefEvtAccept->GetEntries())hRefVsTofRefEvtAccept->Write();
	if(hRefVsBbcXEvtAccept->GetEntries())hRefVsBbcXEvtAccept->Write();
	if(hRefVsZdcXEvtAccept->GetEntries())hRefVsZdcXEvtAccept->Write();
	if(hgRefVsZdcXEvtAccept->GetEntries())hgRefVsZdcXEvtAccept->Write();
	if(hRefVsRunIdEvtAccept->GetEntries())hRefVsRunIdEvtAccept->Write();
	if(hgRefVsRunIdEvtAccept->GetEntries())hgRefVsRunIdEvtAccept->Write();
	if(hVzVsRunIdEvtAccept->GetEntries())hVzVsRunIdEvtAccept->Write();
	if(hRefVsFillIdEvtAccept->GetEntries())hRefVsFillIdEvtAccept->Write();
	if(hVpdVzVsDeltaVzEvtAccept->GetEntries())hVpdVzVsDeltaVzEvtAccept->Write();

	if(hSigmaEvsP_noCut->GetEntries())hSigmaEvsP_noCut->Write();
	if(hSigmaEvsP->GetEntries())hSigmaEvsP->Write();
	if(hSigmaPvsP_noCut->GetEntries())hSigmaPvsP_noCut->Write();
	if(hSigmaPvsP->GetEntries())hSigmaPvsP->Write();
	if(hSigmaPivsP_noCut->GetEntries())hSigmaPivsP_noCut->Write();
	if(hSigmaPivsP->GetEntries())hSigmaPivsP->Write();
	if(hSigmaKvsP_noCut->GetEntries())hSigmaKvsP_noCut->Write();
	if(hSigmaKvsP->GetEntries())hSigmaKvsP->Write();
	if(hDedxVsP_noCut->GetEntries())hDedxVsP_noCut->Write();
	if(hDedxVsP->GetEntries())hDedxVsP->Write();
	if(hnDedxVsP_noCut->GetEntries())hnDedxVsP_noCut->Write();
	if(hnDedxVsP->GetEntries())hnDedxVsP->Write();
	if(hnDedx->GetEntries())hnDedx->Write();
	if(hnHitsVsP_noCut->GetEntries())hnHitsVsP_noCut->Write();
	if(hnHitsVsP->GetEntries())hnHitsVsP->Write();
	if(hnHits->GetEntries())hnHits->Write();
	if(hnHitsVsEta_noCut->GetEntries())hnHitsVsEta_noCut->Write();
	if(hnHitsVsEta->GetEntries())hnHitsVsEta->Write();
	if(hnHitsToMaxPts_noCut->GetEntries())hnHitsToMaxPts_noCut->Write();
	if(hnHitsToMaxPts->GetEntries())hnHitsToMaxPts->Write();
	if(hnHitsVsMaxPts_noCut->GetEntries())hnHitsVsMaxPts_noCut->Write();
	if(hnHitsVsMaxPts->GetEntries())hnHitsVsMaxPts->Write();
	if(hPtVsEta_noCut->GetEntries())hPtVsEta_noCut->Write();
	if(hPtVsEta->GetEntries())hPtVsEta->Write();
	if(hPhiVsPt->GetEntries())hPhiVsPt->Write();
	if(hPt->GetEntries())hPt->Write();
//	if(hPtTof->GetEntries())hPtTof->Write();
	if(hPtNoTof->GetEntries())hPtNoTof->Write();
	if(hPhiVsEta->GetEntries())hPhiVsEta->Write();
	if(hHFTPtVsEta->GetEntries())hHFTPtVsEta->Write();
	if(hHFTPhiVsPt->GetEntries())hHFTPhiVsPt->Write();
	if(hHFTPt->GetEntries())hHFTPt->Write();
//	if(hHFTPtTof->GetEntries())hHFTPtTof->Write();
	if(hHFTPhiVsEta->GetEntries())hHFTPhiVsEta->Write();
	//if(hPhiVsEtaVsPt->GetEntries())hPhiVsEtaVsPt->Write();
	 //Hao Pt Bin 
    if(hPtHao->GetEntries())hPtHao->Write();
    if(hPtNoTofHao->GetEntries())hPtNoTofHao->Write();
	if(hHFTPtHao->GetEntries())hHFTPtHao->Write();
	//Ratio Vz
    if(hZVsPt->GetEntries()) hZVsPt->Write();
    if(hZplusVsPt->GetEntries()) hZplusVsPt->Write();
    if(hZplus2VsPt->GetEntries()) hZplus2VsPt->Write();
    if(hHFTZVsPt->GetEntries()) hHFTZVsPt->Write();
    if(hHFTZplusVsPt->GetEntries()) hHFTZplusVsPt->Write();
    if(hHFTZplus2VsPt->GetEntries()) hHFTZplus2VsPt->Write();
    if(hNoHFTZVsPt->GetEntries()) hNoHFTZVsPt->Write();
    if(hNoHFTZplusVsPt->GetEntries()) hNoHFTZplusVsPt->Write();
    if(hNoHFTZplus2VsPt->GetEntries()) hNoHFTZplus2VsPt->Write();
	//Ratio Vz Vs Phi
    if(hZVsPhi->GetEntries()) hZVsPhi->Write();
    if(hZplusVsPhi->GetEntries()) hZplusVsPhi->Write();
    if(hZplus2VsPhi->GetEntries()) hZplus2VsPhi->Write();
    if(hHFTZVsPhi->GetEntries()) hHFTZVsPhi->Write();
    if(hHFTZplusVsPhi->GetEntries()) hHFTZplusVsPhi->Write();
    if(hHFTZplus2VsPhi->GetEntries()) hHFTZplus2VsPhi->Write();
    if(hNoHFTZVsPhi->GetEntries()) hNoHFTZVsPhi->Write();
    if(hNoHFTZplusVsPhi->GetEntries()) hNoHFTZplusVsPhi->Write();
    if(hNoHFTZplus2VsPhi->GetEntries()) hNoHFTZplus2VsPhi->Write();


	if(hDcaVsPt->GetEntries())hDcaVsPt->Write();
	if(hDca->GetEntries())hDca->Write();
	if(hDcaXyVsPt->GetEntries())hDcaXyVsPt->Write();
	if(hDcaZVsPt->GetEntries())hDcaZVsPt->Write();
	if(hDcaXyVsPhi->GetEntries())hDcaXyVsPhi->Write();
	if(hDcaZVsPhi->GetEntries())hDcaZVsPhi->Write();
	if(hDcaXyVsEta->GetEntries())hDcaXyVsEta->Write();
	if(hDcaZVsEta->GetEntries())hDcaZVsEta->Write();
	if(hDcaXyVsZdc->GetEntries())hDcaXyVsZdc->Write();
	if(hDcaZVsZdc->GetEntries())hDcaZVsZdc->Write();

	if(hTrackR->GetEntries())hTrackR->Write();
	if(hHFTTrackR->GetEntries())hHFTTrackR->Write();

	if(hHFTDcaVsPt->GetEntries())hHFTDcaVsPt->Write();
	if(hHFTDca->GetEntries())hHFTDca->Write();
	if(hHFTDcaXyVsPt->GetEntries())hHFTDcaXyVsPt->Write();
	if(hHFTDcaZVsPt->GetEntries())hHFTDcaZVsPt->Write();
	if(hHFTDcaXyVsPhi->GetEntries())hHFTDcaXyVsPhi->Write();
	if(hHFTDcaZVsPhi->GetEntries())hHFTDcaZVsPhi->Write();
	if(hHFTDcaXyVsEta->GetEntries())hHFTDcaXyVsEta->Write();
	if(hHFTDcaZVsEta->GetEntries())hHFTDcaZVsEta->Write();
	if(hHFTDcaXyVsZdc->GetEntries())hHFTDcaXyVsZdc->Write();
	if(hHFTDcaZVsZdc->GetEntries())hHFTDcaZVsZdc->Write();

	//HFT Dca ZoomIn
	if(hHFTDcaVsPtZoomIn->GetEntries())hHFTDcaVsPtZoomIn->Write();
	if(hHFTDcaZoomIn->GetEntries())hHFTDcaZoomIn->Write();
	if(hHFTDcaXyVsPtZoomIn->GetEntries())hHFTDcaXyVsPtZoomIn->Write();
	if(hHFTDcaZVsPtZoomIn->GetEntries())hHFTDcaZVsPtZoomIn->Write();
	if(hHFTDcaXyVsPhiZoomIn->GetEntries())hHFTDcaXyVsPhiZoomIn->Write();
	if(hHFTDcaZVsPhiZoomIn->GetEntries())hHFTDcaZVsPhiZoomIn->Write();
	if(hHFTDcaXyVsEtaZoomIn->GetEntries())hHFTDcaXyVsEtaZoomIn->Write();
	if(hHFTDcaZVsEtaZoomIn->GetEntries())hHFTDcaZVsEtaZoomIn->Write();
	if(hHFTDcaXyVsZdcZoomIn->GetEntries())hHFTDcaXyVsZdcZoomIn->Write();
	if(hHFTDcaZVsZdcZoomIn->GetEntries())hHFTDcaZVsZdcZoomIn->Write();

	if(hHFTDcaXyVsZplusVsPhiZoomIn->GetEntries())hHFTDcaXyVsZplusVsPhiZoomIn->Write();
	if(hHFTDcaZVsZplusVsPhiZoomIn->GetEntries())hHFTDcaZVsZplusVsPhiZoomIn->Write();


	//add XGN
	if(hnHitsMapHFT->GetEntries())hnHitsMapHFT->Write();
	if(hnHitsHFT->GetEntries())hnHitsHFT->Write();
	if(hnHitsPxlInner->GetEntries())hnHitsPxlInner->Write();
	if(hnHitsPxlOuter->GetEntries())hnHitsPxlOuter->Write();
	if(hnHitsIst->GetEntries())hnHitsIst->Write();
	if(hnHitsSsd->GetEntries())hnHitsSsd->Write();

//Add GNX//NO HFT Hits

	if(hNoHFTPtVsEta->GetEntries())hNoHFTPtVsEta->Write();
	if(hNoHFTPhiVsPt->GetEntries())hNoHFTPhiVsPt->Write();
	if(hNoHFTPt->GetEntries())hNoHFTPt->Write();
	if(hNoHFTPhiVsEta->GetEntries())hNoHFTPhiVsEta->Write();
	if(hNoHFTPtHao->GetEntries())hNoHFTPtHao->Write();
	if(hNoHFTDcaVsPt->GetEntries())hNoHFTDcaVsPt->Write();
	if(hNoHFTDca->GetEntries())hNoHFTDca->Write();
	if(hNoHFTDcaXyVsPt->GetEntries())hNoHFTDcaXyVsPt->Write();
	if(hNoHFTDcaZVsPt->GetEntries())hNoHFTDcaZVsPt->Write();
	if(hNoHFTDcaXyVsPhi->GetEntries())hNoHFTDcaXyVsPhi->Write();
	if(hNoHFTDcaZVsPhi->GetEntries())hNoHFTDcaZVsPhi->Write();
	if(hNoHFTDcaXyVsEta->GetEntries())hNoHFTDcaXyVsEta->Write();
	if(hNoHFTDcaZVsEta->GetEntries())hNoHFTDcaZVsEta->Write();
	if(hNoHFTDcaXyVsZdc->GetEntries())hNoHFTDcaXyVsZdc->Write();
	if(hNoHFTDcaZVsZdc->GetEntries())hNoHFTDcaZVsZdc->Write();


	// add by Guannan
	if(hMtdDeltaYVspt->GetEntries()) hMtdDeltaYVspt->Write();
	if(hMtdDeltaZVspt->GetEntries()) hMtdDeltaZVspt->Write();
	if(hMtdDeltaTOFVspt->GetEntries()) hMtdDeltaTOFVspt->Write();
	if(hMtdMatchFlag->GetEntries()) hMtdMatchFlag->Write();
	if(hMultMtdHit_1->GetEntries()) hMultMtdHit_1->Write();
	if(hMultMtdHit_11->GetEntries()) hMultMtdHit_11->Write();
	if(hCellVsbackleg_1->GetEntries()) hCellVsbackleg_1->Write();
	if(hMultMtdHit_2->GetEntries()) hMultMtdHit_2->Write();
	if(hCellVsbackleg_2->GetEntries()) hCellVsbackleg_2->Write();
	if(hMultMtdHit_3->GetEntries()) hMultMtdHit_3->Write();
	if(hCellVsbackleg_3->GetEntries()) hCellVsbackleg_3->Write();
	if(hMtdTriggerFlag->GetEntries()) hMtdTriggerFlag->Write();

	if(hPvsTimeOfFlight->GetEntries())hPvsTimeOfFlight->Write();
	if(hPvsBeta->GetEntries())hPvsBeta->Write();
	if(hPvsInvBeta->GetEntries())hPvsInvBeta->Write();
	if(hMassvsp->GetEntries())hMassvsp->Write();
	//----------------
	//add by Zebo
	if(hTofModulevsTray->GetEntries())hTofModulevsTray->Write();
	if(hTofvsBoardId->GetEntries())hTofvsBoardId->Write();
	if(hBetaDiffvsTrayIdPi->GetEntries())hBetaDiffvsTrayIdPi->Write();
	if(hTofDiffvsTrayIdPi->GetEntries())hTofDiffvsTrayIdPi->Write();
	if(hBetaDiffvsTrayIdPiPlus->GetEntries())hBetaDiffvsTrayIdPiPlus->Write();
	if(hBetaDiffvsTrayIdPiMinus->GetEntries())hBetaDiffvsTrayIdPiMinus->Write();
	if(hBetaDiffvsTrayIdProton->GetEntries())hBetaDiffvsTrayIdProton->Write();
	if(hBetaDiffvsTrayIdPbar->GetEntries())hBetaDiffvsTrayIdPbar->Write();
	if(hTofDiffvsTrayIdPiPlus->GetEntries())hTofDiffvsTrayIdPiPlus->Write();
	if(hTofDiffvsTrayIdPiMinus->GetEntries())hTofDiffvsTrayIdPiMinus->Write();
	if(hTofDiffvsTrayIdProton->GetEntries())hTofDiffvsTrayIdProton->Write();
	if(hTofDiffvsTrayIdPbar->GetEntries())hTofDiffvsTrayIdPbar->Write();
	if(hTofPhivsZ->GetEntries())hTofPhivsZ->Write();

	//----------------
	if(hEmcTowerId->GetEntries())hEmcTowerId->Write();
	if(hEmcDzVsDphi->GetEntries())hEmcDzVsDphi->Write();
	if(hEmcDistVsP->GetEntries())hEmcDistVsP->Write();

	if(hEmcEnergyRatioVsDist->GetEntries())hEmcEnergyRatioVsDist->Write();
	if(hEmcEnergyHitVsP->GetEntries())hEmcEnergyHitVsP->Write();
	if(hEmcEnergyTowVsP->GetEntries())hEmcEnergyTowVsP->Write();
	if(hEmcEnergySumVsP->GetEntries())hEmcEnergySumVsP->Write();
	if(hEoPvsP_Hit->GetEntries())hEoPvsP_Hit->Write();
	if(hEoPvsP_Tow->GetEntries())hEoPvsP_Tow->Write();
	if(hEoPvsP_Sum->GetEntries())hEoPvsP_Sum->Write();
	if(hSmdHitsEtaVsPhi->GetEntries())hSmdHitsEtaVsPhi->Write();

	//V0
	if(hKShort->GetEntries())hKShort->Write(); 
	if(hLambda->GetEntries())hLambda->Write();
	if(hLambdabar->GetEntries())hLambdabar->Write();
	//Event plane
	//	if(EventPlane->GetEntries())EventPlane->Write();
	if(CosRes_eta->GetEntries())CosRes_eta->Write();
	if(CosRes_ran->GetEntries())CosRes_ran->Write(); 

	for(int icut = 0; icut < 5; icut ++){

		if(hElectronDedxVsP[icut]->GetEntries())hElectronDedxVsP[icut]->Write();
		if(hElectronSigmaEvsP[icut]->GetEntries())hElectronSigmaEvsP[icut]->Write();
		if(hElectronEvsP[icut]->GetEntries())hElectronEvsP[icut]->Write();
		if(hElectronEoPvsP[icut]->GetEntries())hElectronEoPvsP[icut]->Write();
		if(hElectronPvsInvBeta[icut]->GetEntries())hElectronPvsInvBeta[icut]->Write();
		if(hElectronSmdEtaVsPhi[icut]->GetEntries())hElectronSmdEtaVsPhi[icut]->Write();
		if(hElectronSmdDistVsP[icut]->GetEntries())hElectronSmdDistVsP[icut]->Write();

		if(hMassVsPt_NP[icut]->GetEntries())hMassVsPt_NP[icut]->Write();
		if(hMassVsPt_NN[icut]->GetEntries())hMassVsPt_NN[icut]->Write();
		if(hMassVsPt_PP[icut]->GetEntries())hMassVsPt_PP[icut]->Write();
		if(hMassVsPhi_NP[icut]->GetEntries())hMassVsPhi_NP[icut]->Write();
		if(hMassVsPhi_NN[icut]->GetEntries())hMassVsPhi_NN[icut]->Write();
		if(hMassVsPhi_PP[icut]->GetEntries())hMassVsPhi_PP[icut]->Write();
		if(hYvsPt_NP[icut]->GetEntries())hYvsPt_NP[icut]->Write();
		if(hYvsPt_NN[icut]->GetEntries())hYvsPt_NN[icut]->Write();
		if(hYvsPt_PP[icut]->GetEntries())hYvsPt_PP[icut]->Write();
	}


	cout<<"Histos saved!\n"<<endl;

}


HistogramsQa::~HistogramsQa() {

	cout<<"Deleting histos"<<endl;

	delete hVzVsVpdVz_noCut;
	delete hVzVsVpdVz;
	delete hVzVsVpdVzAccept;
	delete hVzVsDeltaVz_noCut;
	delete hVzVsDeltaVz;
	delete hDeltaVz_noCut;
	delete hDeltaVz;
	delete hVpdVzVsDeltaVz_noCut;
	delete hVpdVzVsDeltaVz;
	delete hVxVy;
	delete hRef;
	delete hRefVsVz;
	delete hgRefVsVz;
	delete hRefVsVz_noCut;
	delete hRefVsVz_vpdCut;
	delete hRefVsDeltaVz_noCut;
	delete hRefVsDeltaVz;

	delete hRefVsBemcMatch;
	delete hRefVsBtofMatch;
	delete hgRefVsBemcMatch;
	delete hgRefVsBtofMatch;
	delete hRefVsGlobalRef;
	delete hRefVsGRef;
	delete hRefVsTofRef;
	delete hRefVsBbcX;
	delete hRefVsZdcX;
	delete hgRefVsZdcX;
	delete hRefVsRunId;
	delete hgRefVsRunId;
	delete hVzVsRunId;
	delete hRefVsFillId;
	delete hRefVsBemcMatch_noCut;
	delete hRefVsGlobalRef_noCut;
	delete hRefVsTofRef_noCut;
	delete hRefVsBbcX_noCut;
	delete hRefVsZdcX_noCut;
	delete hRefVsRunId_noCut;
	delete hRefVsFillId_noCut;

	
	delete hgRefVsPxlInnerHits;
	delete hgRefVsPxlOuterHits;
	delete hgRefVsIstHits;




	//	 After Events level Cut
	delete	hDeltaVzEvtAccept;
	delete	hVzVsDeltaVzEvtAccept;
	delete	hVxVyEvtAccept;
	delete	hRefEvtAccept;
	delete	hRefVsVzEvtAccept;
	delete	hgRefVsVzEvtAccept; 
	delete	hRefVsDeltaVzEvtAccept;
	delete	hRefVsBemcMatchEvtAccept;  
	delete	hRefVsBtofMatchEvtAccept; 
	delete	hgRefVsBemcMatchEvtAccept;
	delete	hgRefVsBtofMatchEvtAccept;
	delete	hRefVsGlobalRefEvtAccept; 
	delete	hRefVsGRefEvtAccept; 
	delete	hRefVsTofRefEvtAccept; 
	delete	hRefVsBbcXEvtAccept; 
	delete	hRefVsZdcXEvtAccept; 
	delete	hgRefVsZdcXEvtAccept;  
	delete	hRefVsRunIdEvtAccept;  
	delete	hgRefVsRunIdEvtAccept; 
	delete	hVzVsRunIdEvtAccept; 
	delete	hRefVsFillIdEvtAccept;         
	delete hVpdVzVsDeltaVzEvtAccept;



	delete hDcaVsPt;
	delete hDca;
	delete hDcaXyVsPt;
	delete hDcaZVsPt;
	delete hDcaXyVsPhi;
	delete hDcaZVsPhi;
	delete hDcaXyVsEta;
	delete hDcaZVsEta;

	delete hDcaXyVsZdc;
	delete hDcaZVsZdc;
	delete hHFTDcaXyVsZdc;
	delete hHFTDcaZVsZdc;
	delete hHFTDcaXyVsZdcZoomIn;
	delete hHFTDcaZVsZdcZoomIn;

	delete hTrackR;
	delete hHFTTrackR;

	delete hHFTDcaVsPt;
	delete hHFTDca;
	delete hHFTDcaXyVsPt;
	delete hHFTDcaZVsPt;
	delete hHFTDcaXyVsPhi;
	delete hHFTDcaZVsPhi;
	delete hHFTDcaXyVsEta;
	delete hHFTDcaZVsEta;

	//HFT Dca Zoom In
	delete hHFTDcaVsPtZoomIn;
	delete hHFTDcaZoomIn;
	delete hHFTDcaXyVsPtZoomIn;
	delete hHFTDcaZVsPtZoomIn;
	delete hHFTDcaXyVsPhiZoomIn;
	delete hHFTDcaZVsPhiZoomIn;
	delete hHFTDcaXyVsEtaZoomIn;
	delete hHFTDcaZVsEtaZoomIn;

	delete hHFTDcaXyVsZplusVsPhiZoomIn;
	delete hHFTDcaZVsZplusVsPhiZoomIn;

	delete hSigmaEvsP_noCut;
	delete hSigmaEvsP;
	delete hSigmaPvsP_noCut;
	delete hSigmaPvsP;
	delete hSigmaPivsP_noCut;
	delete hSigmaPivsP;
	delete hSigmaKvsP_noCut;
	delete hSigmaKvsP;
	delete hnHitsVsP_noCut;
	delete hnHitsVsP;
	delete hnHits;
	delete hnHitsVsEta_noCut;
	delete hnHitsVsEta;
	delete hnHitsToMaxPts_noCut;
	delete hnHitsToMaxPts;
	delete hnHitsVsMaxPts_noCut;
	delete hnHitsVsMaxPts;
	delete hnDedxVsP_noCut;
	delete hnDedxVsP;
	delete hnDedx;
	delete hDedxVsP_noCut;
	delete hDedxVsP;
	delete hPtVsEta_noCut;
	delete hPtVsEta;
	delete hPhiVsPt;
	delete hPt;
	delete hPtNoTof;
	delete hPtTof;
	delete hPhiVsEta;
	delete hHFTPtVsEta;
	delete hHFTPhiVsPt;
	delete hHFTPt;
	delete hHFTPtTof;
	delete hHFTPhiVsEta;
	//delete hPhiVsEtaVsPt;
	     // Hao Pt Bins 
	     delete hPtHao;
	     delete hPtNoTofHao;
	     delete hHFTPtHao;
	//Ratio Vz
    delete hZVsPt;
	delete hZplusVsPt;
	delete hZplus2VsPt;
    delete hHFTZVsPt;
	delete hHFTZplusVsPt;
	delete hHFTZplus2VsPt;
    delete hNoHFTZVsPt;
	delete hNoHFTZplusVsPt;
	delete hNoHFTZplus2VsPt;
	//Ratio Vz Vs Phi
    delete hZVsPhi;
	delete hZplusVsPhi;
	delete hZplus2VsPhi;
    delete hHFTZVsPhi;
	delete hHFTZplusVsPhi;
	delete hHFTZplus2VsPhi;
    delete hNoHFTZVsPhi;
	delete hNoHFTZplusVsPhi;
	delete hNoHFTZplus2VsPhi;

 //No NoHFT
	delete hNoHFTDcaXyVsZdc;
	delete hNoHFTDcaZVsZdc;
	delete hNoHFTDcaVsPt;
	delete hNoHFTDca;
	delete hNoHFTDcaXyVsPt;
	delete hNoHFTDcaZVsPt;
	delete hNoHFTDcaXyVsPhi;
	delete hNoHFTDcaZVsPhi;
	delete hNoHFTDcaXyVsEta;
	delete hNoHFTDcaZVsEta;
	delete hNoHFTPtVsEta;
	delete hNoHFTPhiVsPt;
	delete hNoHFTPt;
	delete hNoHFTPtTof;
	delete hNoHFTPhiVsEta;
	delete hNoHFTPtHao;


	delete  hnHitsMapHFT;
	delete  hnHitsHFT;
	delete	hnHitsPxlInner;
	delete	hnHitsPxlOuter;
	delete	hnHitsIst;
	delete	hnHitsSsd;

	delete hMtdDeltaYVspt;
	delete hMtdDeltaZVspt;
	delete hMtdDeltaTOFVspt;
	delete hMtdMatchFlag;

	delete hMultMtdHit_1;
	delete hMultMtdHit_11;
	delete hCellVsbackleg_1;
	delete hMultMtdHit_2;
	delete hCellVsbackleg_2;
	delete hMultMtdHit_3;
	delete hCellVsbackleg_3;
	delete hMtdTriggerFlag;

	delete hTofModulevsTray;
	delete hTofvsBoardId;
	delete hBetaDiffvsTrayIdPi;
	delete hTofDiffvsTrayIdPi;
	delete hBetaDiffvsTrayIdPiPlus;
	delete hBetaDiffvsTrayIdPiMinus;
	delete hBetaDiffvsTrayIdProton;
	delete hBetaDiffvsTrayIdPbar;
	delete hTofDiffvsTrayIdPiPlus;
	delete hTofDiffvsTrayIdPiMinus;
	delete hTofDiffvsTrayIdProton;
	delete hTofDiffvsTrayIdPbar;
	delete hTofPhivsZ;
	delete hMassvsp;


	delete hPvsTimeOfFlight;
	delete hPvsBeta;
	delete hPvsInvBeta;
	delete hSmdHitsEtaVsPhi;
	delete hEmcDzVsDphi;
	delete hEmcTowerId;
	delete hEmcDistVsP;

	delete hEmcEnergyRatioVsDist;
	delete hEmcEnergyHitVsP;
	delete hEmcEnergyTowVsP;
	delete hEmcEnergySumVsP;
	delete hEoPvsP_Hit;
	delete hEoPvsP_Tow;
	delete hEoPvsP_Sum;

	for(int icut = 0; icut < 5; icut++){

		delete hElectronDedxVsP[icut];
		delete hElectronSigmaEvsP[icut];
		delete hElectronEvsP[icut];
		delete hElectronEoPvsP[icut];
		delete hElectronPvsInvBeta[icut];
		delete hElectronSmdEtaVsPhi[icut];
		delete hElectronSmdDistVsP[icut];

		delete hMassVsPt_NP[icut];
		delete hMassVsPt_NN[icut];
		delete hMassVsPt_PP[icut];
		delete hMassVsPhi_NP[icut];
		delete hMassVsPhi_NN[icut];
		delete hMassVsPhi_PP[icut];
		delete hYvsPt_NP[icut];
		delete hYvsPt_NN[icut];
		delete hYvsPt_PP[icut];
	}

	//V0
	delete hKShort;
	delete hLambda;
	delete hLambdabar;
	//Event plane
	delete EventPlane;
	delete CosRes_eta;
	delete CosRes_ran;
	cout<<"Histos deleted!"<<endl;

}

