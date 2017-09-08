/*

Written by Christopher B. Powell
2010
CBPowell@lbl/gov
--------------------------------

Header for StQaAnalysisMaker.cxx

*/

#ifndef STAR_picoMaker
#define STAR_picoMaker
#include "StMaker.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TNtuple.h"
#include "TF1.h"
#include "StarClassLibrary/StThreeVectorF.hh"
#include "StarClassLibrary/StThreeVectorD.hh"
#include "StarClassLibrary/StLorentzVectorF.hh"
#include "StarClassLibrary/StLorentzVectorD.hh"
#include "StarClassLibrary/StHelixD.hh"
#include "TString.h"
#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/filters/StEmcFilter.h"
#include "StEmcUtil/projection/StEmcPosition.h"
#include "StHistogramsQa/HistogramsQa.h"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoEmcTrigger.h"
#include "StPicoDstMaker/StPicoMtdTrigger.h"
#include "StPicoDstMaker/StPicoBTOWHit.h"
#include "StPicoDstMaker/StPicoBTofHit.h"
#include "StPicoDstMaker/StPicoMtdHit.h"
#include "StPicoDstMaker/StPicoEmcPidTraits.h"
#include "StPicoDstMaker/StPicoBTofPidTraits.h"
#include "StPicoDstMaker/StPicoMtdPidTraits.h"
#include "StEmcRawHit.h"
#include <map>

#include "StMuDSTMaker/COMMON/StMuMtdHit.h"
#include "StMuDSTMaker/COMMON/StMuMtdPidTraits.h"
#include "tables/St_mtdModuleToQTmap_Table.h"

class StPicoDst;
class StPicoTrack;
class StPicoEvent;
class StPicoDstMaker;
class TString;
class TH1F;
class TH2F;
class StPicoBTofPidTraits;
class StPicoEmcPidTraits;
class StPicoMtdPidTraits;
class StPicoBTofHit;
class StPicoMtdHit;

class StQaAnalysisMaker : public StMaker
{
public:
   StQaAnalysisMaker(const char *name, StPicoDstMaker *picoMaker, const char *outName);
   virtual ~StQaAnalysisMaker();

   virtual Int_t Init();
   virtual Int_t Make();
   virtual void  Clear(Option_t *opt = "");
   virtual Int_t Finish();

   Float_t getEventInfo(StPicoDst* mPicoDst);
   Float_t getTrackInfo(StPicoDst* mPicoDst);
   Float_t getV0Info(StPicoDst* mPicoDst);
   void    getElectronInfo(StPicoTrack* mPicoTrack);
   void    reconstructDielectrons();
   void    saveHistograms();
   HistogramsQa* hist;
   char    *outDir;
   char    *mTriggerName;
   int     mTriggerId[4];
   int     nTriggerTypes;
   int     nelectronCuts;
   void   setOutputName(Char_t* dir, Char_t* name = "electronFinder");
   float Centrality(int mult);
private:
   StPicoDstMaker *mPicoDstMaker;
   StPicoDst      *mPicoDst;
   StPicoEvent      *mPicoEvent;
   StPicoTrack      *mPicoTrack;

   StPicoBTofPidTraits  *mPicoBTofPidTraits;
   StPicoEmcPidTraits   *mPicoEmcPidTraits;
   StPicoMtdPidTraits   *mPicoMtdPidTraits;

   TString    mOutName;
   TString    mQaOutputName;
   TString mOutputName;
   double  p10;
   double     phiConv;
   int     mNEvent;
   char mFileName[50];
   bool    bDEBUG;
   bool acceptedEvent;
   Float_t electronArray[5][300][15];//cuts, tracks, variables
   Int_t   ea[5];

   Float_t  VzMax;
   Float_t  VzMaxHFTRatio;
   Float_t refMultMin;
   Float_t  Verror;
   Float_t deltaVzMax;
   Float_t  ptMin;
   UShort_t nHitsFitMin;
   Float_t  nHitsRatioMin;
   UShort_t nDedxHitsMin;
   Float_t dcaMax;
   Float_t dcaMaxHFTRatio;
   Float_t  etaMax;
   Float_t  etaMaxHFTRatio;
   Float_t energyMin;

   bool VzMaxHFTRatioCut;
   bool etaMaxHFTRatioCut;
   bool dcaMaxHFTRatioCut;


   Double_t electronMass;
   Float_t pElecMin;
   Float_t ptElecMin;
   Float_t invBetaMax;
   Float_t eopMin;
   Float_t eopMax;
   Float_t nSigmaElectronMax;
   Float_t nSigmaProtonMin;
   Float_t nSigmaPionMin;
   Float_t nSigmaKaonMin;
   Int_t nHitsSmdEtaMin;
   Int_t nHitsSmdPhiMin;

   StThreeVectorF  vertexPos;
   StThreeVectorF momentum;
   StThreeVectorF dcaGlobal, dcaPrimary;


   ClassDef(StQaAnalysisMaker, 1)
};

#endif
