/*

   Written by D. Kikola
   2009
   Edited by Christopher B. Powell
   2010
   CBPowell@lbl.gov
   --------------------------------

   Header for HistogramsQa.cpp

*/

#include <TObject.h>

class TFile;
class TH1F;
class TH2F;
class TH3F;

#ifndef HistogramsQa_H_
#define HistogramsQa_H_

class HistogramsQa : public TObject {
	public:

		HistogramsQa();
		virtual ~HistogramsQa();

		void writeData(TFile *outputFile) const;
		char *imgOutDir;
		TString outputQaName;

		//Event Histograms
		TH2F *hVzVsVpdVz_noCut;
		TH2F *hVzVsVpdVz;
		TH2F *hVzVsVpdVzAccept;
		TH2F *hVzVsDeltaVz_noCut;
		TH2F *hVzVsDeltaVz;
		TH1F *hDeltaVz_noCut;
		TH1F *hDeltaVz;
		TH2F *hVpdVzVsDeltaVz_noCut;
		TH2F *hVpdVzVsDeltaVz;
		TH2F *hVpdVzVsTof;
		TH2F *hVxVy;
		TH1F *hRef;
		TH2F *hRefVsVz_noCut;
		TH2F *hRefVsVz_vpdCut;
		TH2F *hRefVsVz;
		TH2F *hgRefVsVz;
		TH2F *hRefVsDeltaVz_noCut;
		TH2F *hRefVsDeltaVz;
		TH2F *hRefVsTofRef_noCut;
		TH2F *hRefVsBbcX_noCut;
		TH2F *hRefVsZdcX_noCut;
		TH2F *hRefVsRunId_noCut;
		TH2F *hRefVsBemcMatch_noCut;
		TH2F *hRefVsGlobalRef_noCut;
		TH2F *hRefVsFillId_noCut;

		TH2F *hgRefVsPxlInnerHits;
		TH2F *hgRefVsPxlOuterHits;
		TH2F *hgRefVsIstHits;

		//Event histogram after Cut
		TH1F *hDeltaVzEvtAccept;
		TH2F *hVzVsDeltaVzEvtAccept;
		TH2F *hVpdVzVsDeltaVzEvtAccept;
		TH2F *hVxVyEvtAccept; 
		TH1F *hRefEvtAccept; 
		TH2F *hRefVsVzEvtAccept;
		TH2F *hgRefVsVzEvtAccept;
		TH2F *hRefVsDeltaVzEvtAccept;
		TH2F *hRefVsBemcMatchEvtAccept;
		TH2F *hRefVsBtofMatchEvtAccept; 
		TH2F *hgRefVsBemcMatchEvtAccept;
		TH2F *hgRefVsBtofMatchEvtAccept;
		TH2F *hRefVsGlobalRefEvtAccept;
		TH2F *hRefVsGRefEvtAccept; 
		TH2F *hRefVsTofRefEvtAccept;
		TH2F *hRefVsBbcXEvtAccept;
		TH2F *hRefVsZdcXEvtAccept; 
		TH2F *hgRefVsZdcXEvtAccept;
		TH2F *hRefVsRunIdEvtAccept;
		TH2F *hgRefVsRunIdEvtAccept;
		TH2F *hVzVsRunIdEvtAccept;
		TH2F *hRefVsFillIdEvtAccept;


		// ADD GNX
		TH2F *hRefVsTofRef;
		TH2F *hRefVsBbcX;
		TH2F *hRefVsZdcX;
		TH2F *hgRefVsZdcX;
		TH2F *hRefVsRunId;
		TH2F *hgRefVsRunId;
		TH2F *hVzVsRunId;
		TH2F *hRefVsBemcMatch;
		TH2F *hRefVsBtofMatch;
		TH2F *hgRefVsBemcMatch;
		TH2F *hgRefVsBtofMatch;
		TH2F *hRefVsGlobalRef;
		TH2F *hRefVsGRef;
		TH2F *hRefVsFillId;

		//Track Histograms
		//TPC
		TH2F *hSigmaEvsP_noCut;
		TH2F *hSigmaEvsP;
		TH2F *hSigmaPvsP_noCut;
		TH2F *hSigmaPvsP;
		TH2F *hSigmaPivsP_noCut;
		TH2F *hSigmaPivsP;
		TH2F *hSigmaKvsP_noCut;
		TH2F *hSigmaKvsP;
		TH2F *hnHitsVsP_noCut;
		TH2F *hnHitsVsP;
		TH1F *hnHits;
		TH2F *hnHitsVsEta_noCut;
		TH2F *hPtVsEta_noCut;
		TH2F *hPtVsEta;
		TH2F *hHFTPtVsEta;
		TH2F *hDedxVsP_noCut;
		TH2F *hDedxVsP;
		TH2F *hnDedxVsP_noCut;
		TH2F *hnDedxVsP;
		TH1F *hnDedx;
		TH1F *hnHitsToMaxPts_noCut;
		TH1F *hnHitsToMaxPts;
		TH2F *hnHitsVsMaxPts_noCut;
		TH2F *hnHitsVsMaxPts;
		TH2F *hnHitsVsPt;
		TH2F *hnHitsVsEta;
		TH2F *hPhiVsPt;
		TH1F *hPt;
		TH1F *hPtTof;
		TH1F *hPtNoTof;
		TH2F *hPhiVsEta;
		TH2F *hHFTPhiVsPt;
		TH1F *hHFTPt;
		TH1F *hHFTPtTof;
		TH2F *hHFTPhiVsEta;
		TH3F *hPhiVsEtaVsPt;
	         TH1F *hPtHao;
	         TH1F *hPtNoTofHao;
	         TH1F *hHFTPtHao;
		//Vz Ratio
		TH2F *hZVsPt;
		TH2F *hZplusVsPt;
		TH2F *hZplus2VsPt;
		TH2F *hHFTZVsPt;
		TH2F *hHFTZplusVsPt;
		TH2F *hHFTZplus2VsPt;
		TH2F *hNoHFTZVsPt;
		TH2F *hNoHFTZplusVsPt;
		TH2F *hNoHFTZplus2VsPt;
		//Vz Ratio Vs Phi
		TH2F *hZVsPhi;
		TH2F *hZplusVsPhi;
		TH2F *hZplus2VsPhi;
		TH2F *hHFTZVsPhi;
		TH2F *hHFTZplusVsPhi;
		TH2F *hHFTZplus2VsPhi;
		TH2F *hNoHFTZVsPhi;
		TH2F *hNoHFTZplusVsPhi;
		TH2F *hNoHFTZplus2VsPhi;



		TH2F *hDcaVsPt;
		TH1F *hDca;
		TH1F *hChi2Prob;
		TH2F *hDcaXyVsPt;
		TH2F *hDcaZVsPt;
		TH2F *hDcaXyVsPhi;
		TH2F *hDcaZVsPhi;
		TH2F *hDcaXyVsEta;
		TH2F *hDcaZVsEta;
		TH2F *hDcaXyVsZdc;
		TH2F *hDcaZVsZdc;
		
		TH1F *hTrackR;
		TH1F *hHFTTrackR;

		TH2F *hHFTDcaVsPt;
		TH1F *hHFTDca;
		TH2F *hHFTDcaXyVsPt;
		TH2F *hHFTDcaZVsPt;
		TH2F *hHFTDcaXyVsPhi;
		TH2F *hHFTDcaZVsPhi;
		TH2F *hHFTDcaXyVsEta;
		TH2F *hHFTDcaZVsEta;
		TH2F *hHFTDcaXyVsZdc;
		TH2F *hHFTDcaZVsZdc;

		//HFT Dca Zoom In
		TH2F *hHFTDcaVsPtZoomIn;
		TH1F *hHFTDcaZoomIn;
		TH2F *hHFTDcaXyVsPtZoomIn;
		TH2F *hHFTDcaZVsPtZoomIn;
		TH2F *hHFTDcaXyVsPhiZoomIn;
		TH2F *hHFTDcaZVsPhiZoomIn;
		TH2F *hHFTDcaXyVsEtaZoomIn;
		TH2F *hHFTDcaZVsEtaZoomIn;
		TH2F *hHFTDcaXyVsZdcZoomIn;
		TH2F *hHFTDcaZVsZdcZoomIn;
		TH3F *hHFTDcaXyVsZplusVsPhiZoomIn;
		TH3F *hHFTDcaZVsZplusVsPhiZoomIn;

		//NO NoHFT Hits
		TH2F *hNoHFTPtVsEta;
		TH2F *hNoHFTPhiVsPt;
		TH1F *hNoHFTPt;
		TH1F *hNoHFTPtTof;
		TH2F *hNoHFTPhiVsEta;
	    TH1F *hNoHFTPtHao;
		TH2F *hNoHFTDcaVsPt;
		TH1F *hNoHFTDca;
		TH2F *hNoHFTDcaXyVsPt;
		TH2F *hNoHFTDcaZVsPt;
		TH2F *hNoHFTDcaXyVsPhi;
		TH2F *hNoHFTDcaZVsPhi;
		TH2F *hNoHFTDcaXyVsEta;
		TH2F *hNoHFTDcaZVsEta;
		TH2F *hNoHFTDcaXyVsZdc;
		TH2F *hNoHFTDcaZVsZdc;


		//Fill nHitsHFt by GNX
		TH1F*   hnHitsMapHFT;
		TH1F*   hnHitsHFT;
		TH1F* 	hnHitsPxlInner;
		TH1F* 	hnHitsPxlOuter;
		TH1F* 	hnHitsIst;
		TH1F* 	hnHitsSsd;

		//  MTD matching Histograms //Add by GNX
		TH2F* hMtdDeltaYVspt;
		TH2F* hMtdDeltaZVspt;
		TH2F* hMtdDeltaTOFVspt;
		TH1F* hMtdMatchFlag;
		// MTD Hits information // Add GNX
		TH1F* hMultMtdHit_1;
		TH1F* hMultMtdHit_11;
		TH2F* hCellVsbackleg_1;
		TH1F* hMultMtdHit_2;
		TH2F* hCellVsbackleg_2;
		TH1F* hMultMtdHit_3;
		TH2F* hCellVsbackleg_3;
		TH1F* hMtdTriggerFlag;
		//TOF
		//---------------------------+
		//|   add by Zebo Tang         
		TH2F *hTofModulevsTray;
		TH2F *hTofvsBoardId;
		TH2F *hBetaDiffvsTrayIdPi;
		TH2F *hTofDiffvsTrayIdPi;
		TH2F *hBetaDiffvsTrayIdPiPlus;
		TH2F *hBetaDiffvsTrayIdPiMinus;
		TH2F *hBetaDiffvsTrayIdProton;
		TH2F *hBetaDiffvsTrayIdPbar;
		TH2F *hTofDiffvsTrayIdPiPlus;
		TH2F *hTofDiffvsTrayIdPiMinus;
		TH2F *hTofDiffvsTrayIdProton;
		TH2F *hTofDiffvsTrayIdPbar;
		TH2F *hTofPhivsZ;
		TH2F *hMassvsp;
		//---------------------------+
		TH2F *hPvsTimeOfFlight;
		TH2F *hPvsBeta;
		TH2F *hPvsInvBeta;
		//BEMC
		TH2F *hEmcEtaPhi;
		TH2F *hEmcEnergyRatioVsDist;
		TH2F *hEoPvsP_Hit;
		TH2F *hEoPvsP_Tow;
		TH2F *hEoPvsP_Sum;
		TH2F *hEmcEnergyHitVsP;
		TH2F *hEmcEnergyTowVsP;
		TH2F *hEmcEnergySumVsP;
		TH2F *hEmcDzVsDphi;
		TH1F *hEmcTowerId;
		TH2F *hEmcDistVsP;
		//BSMD
		TH2F *hSmdHitsEtaVsPhi;

		TH2F *hMassVsPt_NP[5];
		TH2F *hMassVsPt_NN[5];
		TH2F *hMassVsPt_PP[5];
		TH2F *hMassVsPhi_NP[5];
		TH2F *hMassVsPhi_NN[5];
		TH2F *hMassVsPhi_PP[5];
		TH2F *hYvsPt_NP[5];
		TH2F *hYvsPt_NN[5];
		TH2F *hYvsPt_PP[5];

		TH2F *hElectronDedxVsP[5];
		TH2F *hElectronSigmaEvsP[5];
		TH2F *hElectronEvsP[5];
		TH2F *hElectronEoPvsP[5];
		TH2F *hElectronPvsInvBeta[5];
		TH2F *hElectronSmdEtaVsPhi[5];
		TH2F *hElectronSmdDistVsP[5];

		//V0
		TH2F *hKShort;
		TH2F *hLambda;
		TH2F *hLambdabar;

		//Event Plane and resolution related factors
		TH1F *EventPlane;
		TProfile *CosRes_eta;
		TProfile *CosRes_ran;
};

#endif /* HistogramsQa_H_ */
