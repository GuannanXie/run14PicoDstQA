/*

   Written by Christopher B. Powell
   2010
   CBPowell@lbl.gov
   --------------------------------

   Code loads StPicoDst and loops through events (StPicoEvent)
   and tracks (StPicoTrack) within accepted events. Acceptance
   and quality cuts are applied to tracks and histograms are
   stored using the class StHistogramsQa. Electron id
   is performed using TPC, TOF, BEMC and BSMD cuts, and
   dielectrons are reconstructed event-wise, after which
   electron and dielectron histograms are filled.

*/

#include "StQaAnalysisMaker.h"
#include "StRoot/StPicoDstMaker/StPicoDst.h"
#include "StRoot/StPicoDstMaker/StPicoEvent.h"
#include "StRoot/StPicoDstMaker/StPicoTrack.h"
#include "StRoot/StPicoDstMaker/StPicoDstMaker.h"
#include "StRoot/StPicoDstMaker/StPicoEmcTrigger.h"
#include "StRoot/StPicoDstMaker/StPicoMtdTrigger.h"
#include "StRoot/StPicoDstMaker/StPicoBTOWHit.h"
#include "StRoot/StPicoDstMaker/StPicoBTofHit.h"
#include "StRoot/StPicoDstMaker/StPicoMtdHit.h"
#include "StRoot/StPicoDstMaker/StPicoEmcPidTraits.h"
#include "StRoot/StPicoDstMaker/StPicoBTofPidTraits.h"
#include "StRoot/StPicoDstMaker/StPicoMtdPidTraits.h"
#include "StThreeVectorF.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include <TProfile.h>
#include "Stiostream.h"
//#include "fstream.h"
#include <math.h>

//StEmc
#include "StEmcClusterCollection.h"
#include "StEmcPoint.h"
#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/others/emcDetectorName.h"
#include "StEmcADCtoEMaker/StBemcData.h"
#include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"

#include "StEmcRawMaker/StBemcRaw.h"
#include "StEmcRawMaker/StBemcTables.h"
#include "StEmcRawMaker/StEmcRawMaker.h"
#include "StEmcRawMaker/defines.h"

#include "tables/St_emcStatus_Table.h"
#include "tables/St_smdStatus_Table.h"

#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"
#include "StEmcCollection.h"
#include "StEmcCluster.h"
#include "StMuDSTMaker/COMMON/StMuEmcPoint.h"
#include "StEmcUtil/projection/StEmcPosition.h"
#include "StEmcUtil/filters/StEmcFilter.h"

#include "StEmcRawHit.h"
#include "StEmcModule.h"
#include "StEmcDetector.h"
#include "StEmcClusterCollection.h"
#include "StDaqLib/EMC/StEmcDecoder.h"

#include "StMuDSTMaker/COMMON/StMuMtdHit.h"
#include "StMuDSTMaker/COMMON/StMuMtdPidTraits.h"
//#include "tables/St_mtdModuleToQTmap_Table.h"

#include "StTriggerUtilities/StTriggerSimuMaker.h"
#include "StTriggerUtilities/Bemc/StBemcTriggerSimu.h"
#include "StTriggerUtilities/Eemc/StEemcTriggerSimu.h"
#include "StTriggerUtilities/Emc/StEmcTriggerSimu.h"
#include "StTriggerData.h"

#include "StBTofHeader.h"
#include "TF1.h"
#include "TFile.h"
#include "StarClassLibrary/StThreeVectorD.hh"
#include "StarClassLibrary/StElectron.hh"
#include "StarClassLibrary/StPhysicalHelix.hh"
#include "PhysicalConstants.h"
#include "StMcEventTypes.hh"
#include "StMcEvent.hh"
#include "StMcEventMaker/StMcEventMaker.h"
#include "TMath.h"
#include "StGlobals.hh"
#include "StGlobalTrack.h"
#include "StPhysicalHelixD.hh"
//#include "StDaqLib/TRG/trgStructures2005.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeHelper.h"
#include "TBranch.h"
#include "StEvent/StDcaGeometry.h"

#include "StTriggerConstants.h"


ClassImp(StQaAnalysisMaker)

//-----------------------------------------------------------------------------
StQaAnalysisMaker::StQaAnalysisMaker(const char* name, StPicoDstMaker *picoMaker, const char* outName)
   : StMaker(name)
{
   mPicoDstMaker = picoMaker;
   mPicoDst = 0;
   mOutName = outName;
}

//-----------------------------------------------------------------------------
StQaAnalysisMaker::~StQaAnalysisMaker()
{
   /*  */
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
void StQaAnalysisMaker::setOutputName(Char_t* dir, Char_t* file)
{
   outDir = dir;
   TString dirName = (TString) dir;
   dirName =  dirName + "/";
   TString fileName = (TString) file;
   TString qaOutFileName = (TString) file;
   fileName = "picoQA.AuAu200GeV." + fileName + ".root";
   mOutputName = dirName + fileName;

   return;
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
Float_t StQaAnalysisMaker::getEventInfo(StPicoDst* mPicoDst)
{

   acceptedEvent = false;

   if (bDEBUG)cout << "\nStarting function getEventInfo..." << endl;

   if (!mPicoDst)return 0;

   //Load event
   mPicoEvent = (StPicoEvent*)mPicoDst->event();
   if (!mPicoEvent)
   {
      cerr << "Error opening picoDst Event, skip!" << endl;
      return 0;
   }

   if (bDEBUG)cout << "\nEvent and Vertex loaded." << endl;

   bool isVPDMB5 = kFALSE;
   //for test triggers
   // cout<<"*********"<<mPicoEvent->triggerIds().size()<<endl;
   // for(int i=0;i<mPicoEvent->triggerIds().size();i++) cout<<"Triggers= "<<mPicoEvent->triggerIds()[i]<<endl;
   for (int i = 0; i < 5; i++)
   {
      if (mPicoEvent->isTrigger(TriggerConst::mTriggerId[i])) isVPDMB5 = kTRUE ;  //Select vpdMB5-p trigger
   }
   if (!(isVPDMB5))
   {
      return 0;
   }

   //Remove bad vertices
   vertexPos = mPicoEvent->primaryVertex();
   if ((TMath::Abs(vertexPos.x()) < Verror) && (TMath::Abs(vertexPos.y()) < Verror) && (TMath::Abs(vertexPos.z()) < Verror))
   {
      cout << "Bad Vertex (0, 0, 0)" << endl;
      return 0;
   }

   if (bDEBUG)cout << "\nFilling event histograms." << endl;

   //Vertex histograms
   hist->hVzVsVpdVz->Fill(vertexPos.z(), mPicoEvent->vzVpd());
   hist->hDeltaVz->Fill(vertexPos.z() - mPicoEvent->vzVpd());
   hist->hVxVy->Fill(vertexPos.x(), vertexPos.y());
   hist->hRef->Fill(mPicoEvent->refMult());
   hist->hRefVsTofRef->Fill(mPicoEvent->btofTrayMultiplicity(), mPicoEvent->refMult());
   hist->hRefVsGlobalRef->Fill(mPicoEvent->numberOfGlobalTracks(), mPicoEvent->refMult());
   hist->hRefVsGRef->Fill(mPicoEvent->refMult(), mPicoEvent->grefMult());//xgn
   hist->hRefVsBemcMatch->Fill(mPicoEvent->nBEMCMatch(), mPicoEvent->refMult());
   hist->hRefVsBtofMatch->Fill(mPicoEvent->nBTOFMatch(), mPicoEvent->refMult());
   hist->hgRefVsBemcMatch->Fill(mPicoEvent->nBEMCMatch(), mPicoEvent->grefMult());
   hist->hgRefVsBtofMatch->Fill(mPicoEvent->nBTOFMatch(), mPicoEvent->grefMult());
   hist->hRefVsBbcX->Fill(mPicoEvent->BBCx() / 1.0e3, mPicoEvent->refMult());
   hist->hRefVsZdcX->Fill(mPicoEvent->ZDCx() / 1.0e3, mPicoEvent->refMult());
   hist->hRefVsVz->Fill(vertexPos.z(), mPicoEvent->refMult());
   hist->hgRefVsZdcX->Fill(mPicoEvent->ZDCx() / 1.0e3, mPicoEvent->grefMult());
   hist->hgRefVsVz->Fill(vertexPos.z(), mPicoEvent->grefMult());
   hist->hVzVsDeltaVz->Fill(vertexPos.z(), vertexPos.z() - mPicoEvent->vzVpd());
   hist->hVpdVzVsDeltaVz->Fill(mPicoEvent->vzVpd(), vertexPos.z() - mPicoEvent->vzVpd());
   hist->hRefVsRunId->Fill(mPicoEvent->runId() / 1.0e4, mPicoEvent->refMult());
   hist->hgRefVsRunId->Fill(mPicoEvent->runId() / 1.0e4, mPicoEvent->grefMult()); //xgn
   hist->hVzVsRunId->Fill(mPicoEvent->runId() / 1.0e4, vertexPos.z()); //xgn
   //hist->hRefVsDeltaVz->Fill(mPicoEvent->refMult(), vertexPos.z() - mPicoEvent->vzVpd());
   //hist->hRefVsFillId->Fill(mPicoEvent->refMult(), mPicoEvent->fillId());

   //nPXLInner hits (recently added in PicoEvent)  vs gRefMult
   //nPXLOuter hits (recently added in PicoEvent)  vs gRefMult
   //nIST hits (recently added in PicoEvent)  vs gRefMult
   //cout<<"mPicoEvent->numberOfPxlInnerHits()"<<mPicoEvent->numberOfPxlInnerHits()<<endl;
   //cout<<mPicoEvent->numberOfPxlOuterHits()<<endl;
   //cout<<mPicoEvent->numberOfIstHits()<<endl;
   hist->hgRefVsPxlInnerHits->Fill(mPicoEvent->numberOfPxlInnerHits(), mPicoEvent->grefMult());
   hist->hgRefVsPxlOuterHits->Fill(mPicoEvent->numberOfPxlOuterHits(), mPicoEvent->grefMult());
   hist->hgRefVsIstHits->Fill(mPicoEvent->numberOfIstHits(), mPicoEvent->grefMult());



   if (bDEBUG)cout << "\nEvent histograms filled." << endl;

   //Event Plane variables
   //Empty

   if (bDEBUG)cout << "\nApplying event cuts." << endl;

   //Apply vertex cuts
   VzMaxHFTRatioCut = kFALSE;
   if (TMath::Abs(vertexPos.z()) < VzMaxHFTRatio) VzMaxHFTRatioCut = kTRUE;
   if (TMath::Abs(vertexPos.z()) > VzMax) return 0;
   if (mPicoEvent->refMult() < refMultMin) return 0;
   if (TMath::Abs(vertexPos.z() - mPicoEvent->vzVpd()) > deltaVzMax) return 0;
   acceptedEvent = true;
   hist->hVzVsVpdVzAccept->Fill(vertexPos.z(), mPicoEvent->vzVpd());

   //Vertex histograms after Events Cut
// hist->hVzVsVpdVzEvtAccept->Fill(vertexPos.z(), mPicoEvent->vzVpd());
   hist->hDeltaVzEvtAccept->Fill(vertexPos.z() - mPicoEvent->vzVpd());
   hist->hVxVyEvtAccept->Fill(vertexPos.x(), vertexPos.y());
   hist->hRefEvtAccept->Fill(mPicoEvent->refMult());
   hist->hRefVsTofRefEvtAccept->Fill(mPicoEvent->btofTrayMultiplicity(), mPicoEvent->refMult());
   hist->hRefVsGlobalRefEvtAccept->Fill(mPicoEvent->numberOfGlobalTracks(), mPicoEvent->refMult());
   hist->hRefVsGRefEvtAccept->Fill(mPicoEvent->grefMult(), mPicoEvent->refMult()); //xgn
   hist->hRefVsBemcMatchEvtAccept->Fill(mPicoEvent->nBEMCMatch(), mPicoEvent->refMult());
   hist->hRefVsBtofMatchEvtAccept->Fill(mPicoEvent->nBTOFMatch(), mPicoEvent->refMult());
   hist->hgRefVsBemcMatchEvtAccept->Fill(mPicoEvent->nBEMCMatch(), mPicoEvent->grefMult());
   hist->hgRefVsBtofMatchEvtAccept->Fill(mPicoEvent->nBTOFMatch(), mPicoEvent->grefMult());
   hist->hRefVsBbcXEvtAccept->Fill(mPicoEvent->BBCx() / 1.0e3, mPicoEvent->refMult());
   hist->hRefVsZdcXEvtAccept->Fill(mPicoEvent->ZDCx() / 1.0e3, mPicoEvent->refMult());
   hist->hRefVsVzEvtAccept->Fill(vertexPos.z(), mPicoEvent->refMult());
   hist->hgRefVsZdcXEvtAccept->Fill(mPicoEvent->ZDCx() / 1.0e3, mPicoEvent->grefMult());
   hist->hgRefVsVzEvtAccept->Fill(vertexPos.z(), mPicoEvent->grefMult());
   hist->hVzVsDeltaVzEvtAccept->Fill(vertexPos.z(), vertexPos.z() - mPicoEvent->vzVpd());
   hist->hVpdVzVsDeltaVzEvtAccept->Fill(mPicoEvent->vzVpd(), vertexPos.z() - mPicoEvent->vzVpd());
   hist->hRefVsRunIdEvtAccept->Fill(mPicoEvent->runId() / 1.0e4, mPicoEvent->refMult());
   hist->hgRefVsRunIdEvtAccept->Fill(mPicoEvent->runId() / 1.0e4, mPicoEvent->grefMult()); //xgn
   hist->hVzVsRunIdEvtAccept->Fill(mPicoEvent->runId() / 1.0e4, vertexPos.z()); //xgn
   //hist->hRefVsDeltaVzEvtAccept->Fill(vertexPos.z() - mPicoEvent->vzVpd(),mPicoEvent->refMult());
   //hist->hRefVsFillIdEvtAccept->Fill( mPicoEvent->fillId(),mPicoEvent->refMult());


   return 0;
}
/*----------------------------------------------------------------------------------------------------------------------*/
Float_t StQaAnalysisMaker::getTrackInfo(StPicoDst* mPicoDst)
{
   if (bDEBUG)cout << "\nBegin looping through particles..." << endl;

   //Load tracks for default vertex index
   int iTrack = 0;
   while (1)
   {
      mPicoTrack = (StPicoTrack*) mPicoDst->track(iTrack);
      if (!mPicoTrack)
      {
         break;
      }
      int tofIndex = mPicoTrack->bTofPidTraitsIndex();
      //    if(tofIndex >=0 ) mPicoBTofPidTraits = mPicoDst->btofPidTraits(tofIndex);//GNX
      int emcIndex = mPicoTrack->emcPidTraitsIndex();
      //    if(emcIndex >=0 ) mPicoEmcPidTraits = mPicoDst->emcPidTraits(emcIndex);//GNX
      int mtdIndex = mPicoTrack->mtdPidTraitsIndex();
      //    if(mtdIndex >=0 ) mPicoMtdPidTraits = mPicoDst->mtdPidTraits(mtdIndex);//GNX

      iTrack++;

      //Track acceptance cuts
      // momentum = mPicoTrack->gMom();
//    momentum = mPicoTrack->pMom();
//     StThreeVectorF momentum = mPicoTrack->gMom();
//    if(TMath::Abs(momentum.pseudoRapidity()) > etaMax) continue;
//    if(momentum.perp() < ptMin) continue;
      double magn = mPicoEvent->bField();

      StThreeVectorF vertexPosErr(0., 0., 0.);
      vertexPosErr = mPicoEvent->primaryVertexError();

      StDcaGeometry dcaG;
      dcaG.set(mPicoTrack->params(), mPicoTrack->errMatrix());
      StPhysicalHelixD helix = dcaG.helix();
      double dca = helix.geometricSignedDistance(vertexPos);

//     StThreeVectorF pmomentum = mPicoTrack->pMom();
//     cout<<"pmomentum="<<pmomentum.x()<<" "<<pmomentum.y()<<" "<<pmomentum.z()<<endl;

//     StThreeVectorF momentum = helix.momentum(magn*kilogauss);//From MuTrack shoudl be confirm
//     cout<<"momentum="<<momentum.x()<<" "<<momentum.y()<<" "<<momentum.z()<<endl;
//
//     StThreeVectorF momentum2 = dcaG->momentum();//From dcaG shoudl be confirm
//     cout<<"momentum2="<<momentum2.x()<<" "<<momentum2.y()<<" "<<momentum2.z()<<endl;
//
//     double s = helix.pathLength(vertexPos);
//     StThreeVectorF momentum3 = helix.momentumAt(s,magn*kilogauss);//From StHelix
//     cout<<"momentum3="<<momentum3.x()<<" "<<momentum3.y()<<" "<<momentum3.z()<<endl;

      StThreeVectorF momentum = mPicoTrack->gMom(vertexPos, magn); //From StPicoDst
//       cout<<"momentum4="<<momentum4.x()<<" "<<momentum4.y()<<" "<<momentum4.z()<<endl;

      etaMaxHFTRatioCut = kFALSE;
      if (TMath::Abs(momentum.pseudoRapidity()) < etaMaxHFTRatio) etaMaxHFTRatioCut = kTRUE;
      if (TMath::Abs(momentum.pseudoRapidity()) > etaMax) continue;
      if (momentum.perp() < ptMin) continue;

      StThreeVectorF dcaPoint = helix.at(helix.pathLength(vertexPos.x(), vertexPos.y()));
      double dcaZ = dcaPoint.z() - vertexPos.z();
      StThreeVectorF dcaP = helix.momentumAt(vertexPos.x(), vertexPos.y());
      // double dcaXy = ((dcaPoint-vertexPos).x()*dcaP.y()-(dcaPoint-vertexPos).y()*dcaP.x())/dcaP.perp();
      double dcaXy = helix.geometricSignedDistance(vertexPos.x(), vertexPos.y());

      // double dcafun = helix.geometricSignedDistance(vertexPos.x(),vertexPos.y());
//         cout<<"DcaDcaDcaFun="<<dcafun<<"DcaDcaDca="<<dca<<"DcaDcaDcaXy="<<dcaXy<<";;;;;;;;;DcaZDcaZDcaZ="<<dcaZ<<endl;
      //if(TMath::Abs(dca) > 0.1) continue;//temp cut for Zplus
      //double Zplus=vertexPos.z()+2.8*tan(3.14159265/2.-momentum.theta());
      //cout<<"||||||||||||||||"<<endl;
      //cout<<"eta="<<momentum.pseudoRapidity()<<"Vxyz="<<momentum.x()<<" "<<momentum.y()<<" "<<momentum.z()<<endl;
      //cout<<"Zplus="<<Zplus<<endl;
      //pair<double,double> s = helix.pathLength(2.93,-0.2397,-0.1522);
      pair<double, double> ss;
      // if(mPicoEvent->runId()<15109006) pair<double,double> ss = helix.pathLength(2.93,0.04183,-0.3365);
      // else pair<double,double> ss = helix.pathLength(2.93,-0.2397,-0.1522);
      ss = helix.pathLength(2.93, vertexPos.x(), vertexPos.y());
      //pair<double,double> s = helix.pathLength(2.93);//2.8
      //StThreeVectorF ZplusPoint = helix.at(s.first);
      StThreeVectorF ZplusPoint = helix.at(ss.second);
      //cout<<"ZplusPoint.xyz()="<<ZplusPoint.x()<<" "<<ZplusPoint.y()<<" "<<ZplusPoint.z()<<endl;
      double Zplus = ZplusPoint.z();

      pair<double, double> s2;
      // if(mPicoEvent->runId()<15109006) s2 = helix.pathLength(8.08,0.04183,-0.3365);
      // else  s2 = helix.pathLength(8.08,-0.2397,-0.1522);
      s2 = helix.pathLength(8.08, vertexPos.x(), vertexPos.y());
      StThreeVectorF ZplusPoint2 = helix.at(s2.second);
      double Zplus2 = ZplusPoint2.z();

      //Track quality cuts
      // if ((Float_t)mPicoTrack->nHitsFit() / (Float_t)mPicoTrack->nHitsMax() < nHitsRatioMin) continue;
      if (mPicoTrack->nHitsFit() < nHitsFitMin) continue;
      if (mPicoTrack->nHitsDedx() < nDedxHitsMin) continue;
      //if(TMath::Abs(mPicoTrack->dca()) > dcaMax) continue;
      dcaMaxHFTRatioCut = kFALSE;
      if (TMath::Abs(dca) < dcaMaxHFTRatio) dcaMaxHFTRatioCut = kTRUE;
      if (TMath::Abs(dca) > dcaMax) continue;
      //Fill a histogram to get the Tof Match Histogram
      hist->hPtNoTof->Fill(momentum.perp());
      hist->hPtNoTofHao->Fill(momentum.perp());

      bool TofMatch = kFALSE;
      if (tofIndex >= 0)  mPicoBTofPidTraits = mPicoDst->btofPidTraits(tofIndex); //GNX
      if (tofIndex >= 0 && mPicoBTofPidTraits && mPicoBTofPidTraits->btofMatchFlag() > 0)  TofMatch = kTRUE; //GNX
      if (!TofMatch) continue;
//    hist->hPtTof->Fill(momentum.perp());
      if (bDEBUG)cout << "\n\tFilling track histograms." << endl;

      //Fill TPC Histograms
      hist->hnHits->Fill(mPicoTrack->nHitsFit()*mPicoTrack->charge());
      // hist->hnHitsToMaxPts->Fill((Float_t)mPicoTrack->nHitsFit()/(Float_t)mPicoTrack->nHitsMax());
      hist->hnDedx->Fill(mPicoTrack->nHitsDedx());
      if (etaMaxHFTRatioCut && VzMaxHFTRatioCut && dcaMaxHFTRatioCut) hist->hPt->Fill(momentum.perp());//Mar13
      if (etaMaxHFTRatioCut && VzMaxHFTRatioCut && dcaMaxHFTRatioCut) hist->hPtHao->Fill(momentum.perp());//Mar13
//    if(tofIndex >=0 ) {
//       mPicoBTofPidTraits = mPicoDst->btofPidTraits(tofIndex);//GNX
//       if(mPicoBTofPidTraits && mPicoBTofPidTraits->btofMatchFlag()>0)//GNX
//       {
//       hist->hPtTof->Fill(momentum.perp());
//       }
//    }
      hist->hPhiVsEta->Fill(momentum.phi(), momentum.pseudoRapidity());
      hist->hDedxVsP->Fill(momentum.mag(), mPicoTrack->dEdx());
      hist->hSigmaEvsP->Fill(momentum.mag(), mPicoTrack->nSigmaElectron());//Mar13
      hist->hSigmaPvsP->Fill(momentum.mag(), mPicoTrack->nSigmaProton());//Mar13
      hist->hSigmaPivsP->Fill(momentum.mag(), mPicoTrack->nSigmaPion());//Mar13
      hist->hSigmaKvsP->Fill(momentum.mag(), mPicoTrack->nSigmaKaon());//Mar13
//    hist->hChi2Prob->Fill(mPicoTrack->chi2prob());
      hist->hDca->Fill(dca);
      //hist->hDca->Fill(mPicoTrack->dca());
      hist->hnDedxVsP->Fill(mPicoTrack->nHitsDedx(), momentum.mag());
      hist->hPtVsEta->Fill(momentum.perp(), momentum.pseudoRapidity());
      hist->hPhiVsPt->Fill(momentum.phi(), momentum.perp());
      hist->hnHitsVsEta->Fill(mPicoTrack->nHitsFit(), momentum.pseudoRapidity());
      //April 23 Ratio Vs Z
      hist->hZVsPt->Fill(vertexPos.z(), momentum.perp());
      hist->hZplusVsPt->Fill(Zplus, momentum.perp());
      hist->hZplus2VsPt->Fill(Zplus2, momentum.perp());
      //April 23 Ratio Vs Z Vs Phi
      hist->hZVsPhi->Fill(vertexPos.z(), momentum.phi());
      hist->hZplusVsPhi->Fill(Zplus, momentum.phi());
      hist->hZplus2VsPhi->Fill(Zplus2, momentum.phi());
//    hist->hnHitsVsMaxPts->Fill(mPicoTrack->nHitsFit(), mPicoTrack->nHitsMax());
      //    hist->hPhiVsEtaVsPt->Fill(momentum.phi(), momentum.pseudoRapidity(), momentum.perp());

      //---------------dca information //  Add by GNX
      // StThreeVectorF momentum = mPicoTrack->gMom();
//         StThreeVectorF origin = mPicoTrack->origin();
      // mPicoEvent = (StPicoEvent*)mPicoDst->event();
      //  double magn = mPicoEvent->bField();
      // double magn =  mPicoDst->event()->bField();

//         StPhysicalHelix helix(momentum, origin, magn*kilogauss, mPicoTrack->charge());
//         StThreeVectorF dcaPoint = helix.at(helix.pathLength(vertexPos.x(), vertexPos.y()));
//         double dcaZ = dcaPoint.z() - vertexPos.z();
//         StThreeVectorF dcaP = helix.momentumAt(vertexPos.x(), vertexPos.y());
//         double dcaXy = ((dcaPoint-vertexPos).x()*dcaP.y()-(dcaPoint-vertexPos).y()*dcaP.x())/dcaP.perp();

//    double R=sqrt(pow(origin.x(),2)+pow(origin.y(),2) );
//    hist->hTrackR->Fill(R);

      //hist->hDcaVsPt->Fill(mPicoTrack->dca(), momentum.perp());
      hist->hDcaVsPt->Fill(dca, momentum.perp());
      hist->hDcaXyVsPt->Fill(dcaXy, momentum.perp());
      hist->hDcaZVsPt->Fill(dcaZ, momentum.perp());
      hist->hDcaXyVsPhi->Fill(dcaXy, momentum.phi());
      hist->hDcaZVsPhi->Fill(dcaZ, momentum.phi());
      hist->hDcaXyVsEta->Fill(dcaXy, momentum.pseudoRapidity());
      hist->hDcaZVsEta->Fill(dcaZ, momentum.pseudoRapidity());
      hist->hDcaXyVsZdc->Fill(dcaXy, mPicoEvent->ZDCx() / 1.0e3);
      hist->hDcaZVsZdc->Fill(dcaZ, mPicoEvent->ZDCx() / 1.0e3);

      //----------------------------------
      //Fill nHitsHFt Histograms  add by GNX
      bool isPxlInner = kFALSE;
      bool isPxlOuter = kFALSE;
      bool isIst = kFALSE;

      Int_t sum = 0;
      UInt_t nHitsMapHFT = mPicoTrack->nHitsMapHFT();
      hist->hnHitsMapHFT->Fill(nHitsMapHFT);
      while (nHitsMapHFT)
      {
         sum += (nHitsMapHFT & 0x01);
         nHitsMapHFT >>= 1;
      }
      hist->hnHitsHFT->Fill(sum * mPicoTrack->charge());
      //Inner pxl Hits
      sum = 0;
      nHitsMapHFT = mPicoTrack->nHitsMapHFT();
      if (nHitsMapHFT) sum += (nHitsMapHFT & 0x01);
      if (sum > 0) isPxlInner = kTRUE;
      hist->hnHitsPxlInner->Fill(sum * mPicoTrack->charge());
      //Outer pxl Hits
      sum = 0;
      nHitsMapHFT = mPicoTrack->nHitsMapHFT();
      nHitsMapHFT >>= 1;
      for (int i = 0; i < 2; nHitsMapHFT)
      {
         sum += (nHitsMapHFT & 0x01);
         nHitsMapHFT >>= 1;
         i++;
      }
      if (sum > 0) isPxlOuter = kTRUE;
      hist->hnHitsPxlOuter->Fill(sum * mPicoTrack->charge());
      //ISt Hits
      sum = 0;
      nHitsMapHFT = mPicoTrack->nHitsMapHFT();
      nHitsMapHFT >>= 3;
      for (int i = 0; i < 2; nHitsMapHFT)
      {
         sum += (nHitsMapHFT & 0x01);
         nHitsMapHFT >>= 1;
         i++;
      }
      if (sum > 0) isIst = kTRUE;
      hist->hnHitsIst->Fill(sum * mPicoTrack->charge());
      //SSD Hits
      sum = 0;
      nHitsMapHFT = mPicoTrack->nHitsMapHFT();
      nHitsMapHFT >>= 5;
      for (int i = 0; i < 2; nHitsMapHFT)
      {
         sum += (nHitsMapHFT & 0x01);
         nHitsMapHFT >>= 1;
         i++;
      }
      hist->hnHitsSsd->Fill(sum * mPicoTrack->charge());

      if (bDEBUG)cout << "\n\tTPC Track histos filled." << endl;

      if (isPxlInner && isPxlOuter && isIst)
      {
         if (etaMaxHFTRatioCut && VzMaxHFTRatioCut && dcaMaxHFTRatioCut)  hist->hHFTPt->Fill(momentum.perp());//Mar13
         if (etaMaxHFTRatioCut && VzMaxHFTRatioCut && dcaMaxHFTRatioCut)  hist->hHFTPtHao->Fill(momentum.perp());   //Mar13
//    if(tofIndex >=0 ) {
//       mPicoBTofPidTraits = mPicoDst->btofPidTraits(tofIndex);//GNX
//       if(mPicoBTofPidTraits && mPicoBTofPidTraits->btofMatchFlag()>0)//GNX
//       {
//       hist->hHFTPtTof->Fill(momentum.perp());
//       }
//    }
         //hist->hHFTTrackR->Fill(R);

         hist->hHFTPtVsEta->Fill(momentum.perp(), momentum.pseudoRapidity());
         hist->hHFTPhiVsEta->Fill(momentum.phi(), momentum.pseudoRapidity());
         hist->hHFTPhiVsPt->Fill(momentum.phi(), momentum.perp());
         //hist->hHFTDca->Fill(mPicoTrack->dca());
         //hist->hHFTDcaVsPt->Fill(mPicoTrack->dca(), momentum.perp());
         hist->hHFTDca->Fill(dca);
         hist->hHFTDcaVsPt->Fill(dca, momentum.perp());
         hist->hHFTDcaXyVsPt->Fill(dcaXy, momentum.perp());
         hist->hHFTDcaZVsPt->Fill(dcaZ, momentum.perp());
         hist->hHFTDcaXyVsPhi->Fill(dcaXy, momentum.phi());
         hist->hHFTDcaZVsPhi->Fill(dcaZ, momentum.phi());
         hist->hHFTDcaXyVsEta->Fill(dcaXy, momentum.pseudoRapidity());
         hist->hHFTDcaZVsEta->Fill(dcaZ, momentum.pseudoRapidity());
         hist->hHFTDcaXyVsZdc->Fill(dcaXy, mPicoEvent->ZDCx() / 1.0e3);
         hist->hHFTDcaZVsZdc->Fill(dcaZ, mPicoEvent->ZDCx() / 1.0e3);
         hist->hHFTDcaZoomIn->Fill(dca);
         hist->hHFTDcaVsPtZoomIn->Fill(dca, momentum.perp());
         hist->hHFTDcaXyVsPtZoomIn->Fill(dcaXy, momentum.perp());
         hist->hHFTDcaZVsPtZoomIn->Fill(dcaZ, momentum.perp());
         hist->hHFTDcaXyVsPhiZoomIn->Fill(dcaXy, momentum.phi());
         hist->hHFTDcaZVsPhiZoomIn->Fill(dcaZ, momentum.phi());
         hist->hHFTDcaXyVsEtaZoomIn->Fill(dcaXy, momentum.pseudoRapidity());
         hist->hHFTDcaZVsEtaZoomIn->Fill(dcaZ, momentum.pseudoRapidity());
         hist->hHFTDcaXyVsZdcZoomIn->Fill(dcaXy, mPicoEvent->ZDCx() / 1.0e3);
         hist->hHFTDcaZVsZdcZoomIn->Fill(dcaZ, mPicoEvent->ZDCx() / 1.0e3);
         //Vz Ratio
         hist->hHFTZVsPt->Fill(vertexPos.z(), momentum.perp());
         hist->hHFTZplusVsPt->Fill(Zplus, momentum.perp());
         hist->hHFTZplus2VsPt->Fill(Zplus2, momentum.perp());
         //Vz Ratio Vs Phi
         hist->hHFTZVsPhi->Fill(vertexPos.z(), momentum.phi());
         hist->hHFTZplusVsPhi->Fill(Zplus, momentum.phi());
         hist->hHFTZplus2VsPhi->Fill(Zplus2, momentum.phi());


         double phi;
         if (momentum.phi() < 0.338) phi = momentum.phi() + 2 * 3.14159265358;
         else phi = momentum.phi();
         // hist->hHFTDcaXyVsZplusVsPhiZoomIn->Fill(dcaXy,Zplus,phi); //comment Mar14, just save displace
         // hist->hHFTDcaZVsZplusVsPhiZoomIn->Fill(dcaZ,Zplus,phi);  //comment Mar14
      }

      // Require No NoHFT Hits //April22
      if ((!isPxlInner) && (!isPxlOuter) && (!isIst))
      {
         // if(abs(dcaXy-dcafun)>1e-5)  cout<<"not hft hits::******dcaXy="<<dcaXy<<" ;dcafun="<<dcafun<<endl;
         hist->hNoHFTPt->Fill(momentum.perp());//Mar13
         hist->hNoHFTPtHao->Fill(momentum.perp());   //Mar13

         hist->hNoHFTPtVsEta->Fill(momentum.perp(), momentum.pseudoRapidity());
         hist->hNoHFTPhiVsEta->Fill(momentum.phi(), momentum.pseudoRapidity());
         hist->hNoHFTPhiVsPt->Fill(momentum.phi(), momentum.perp());
         //hist->hNoHFTDca->Fill(mPicoTrack->dca());
         //hist->hNoHFTDcaVsPt->Fill(mPicoTrack->dca(), momentum.perp());
         hist->hNoHFTDca->Fill(dca);
         hist->hNoHFTDcaVsPt->Fill(dca, momentum.perp());
         hist->hNoHFTDcaXyVsPt->Fill(dcaXy, momentum.perp());
         hist->hNoHFTDcaZVsPt->Fill(dcaZ, momentum.perp());
         hist->hNoHFTDcaXyVsPhi->Fill(dcaXy, momentum.phi());
         hist->hNoHFTDcaZVsPhi->Fill(dcaZ, momentum.phi());
         hist->hNoHFTDcaXyVsEta->Fill(dcaXy, momentum.pseudoRapidity());
         hist->hNoHFTDcaZVsEta->Fill(dcaZ, momentum.pseudoRapidity());
         hist->hNoHFTDcaXyVsZdc->Fill(dcaXy, mPicoEvent->ZDCx() / 1.0e3);
         hist->hNoHFTDcaZVsZdc->Fill(dcaZ, mPicoEvent->ZDCx() / 1.0e3);

         //Vz Ratio
         hist->hNoHFTZVsPt->Fill(vertexPos.z(), momentum.perp());
         hist->hNoHFTZplusVsPt->Fill(Zplus, momentum.perp());
         hist->hNoHFTZplus2VsPt->Fill(Zplus2, momentum.perp());
         //Vz Ratio Vs Phi
         hist->hNoHFTZVsPhi->Fill(vertexPos.z(), momentum.phi());
         hist->hNoHFTZplusVsPhi->Fill(Zplus, momentum.phi());
         hist->hNoHFTZplus2VsPhi->Fill(Zplus2, momentum.phi());
      }


      //Fill TOF Histograms
      if (tofIndex >= 0)
      {
         mPicoBTofPidTraits = mPicoDst->btofPidTraits(tofIndex);//GNX
         //if(mPicoBTofPidTraits && mPicoBTofPidTraits->btofMatchFlag()>0)
         //-------------------------------------+
         //  add by Zebo Tang
         Int_t cellId = mPicoBTofPidTraits->btofCellId();
         Int_t boardId = cellId / 24;
         Int_t trayId = boardId / 8;
         Int_t moduleId = (cellId % 192) / 6;
         Double_t tof = mPicoBTofPidTraits->btof();
         Double_t beta = mPicoBTofPidTraits->btofBeta();
         Double_t beta_Pion = momentum.mag() / sqrt(pow(momentum.mag(), 2) + pow(0.13957, 2));
         Double_t beta_Proton = momentum.mag() / sqrt(pow(momentum.mag(), 2) + pow(0.93827, 2));
         Double_t tof_Pion = tof * beta / beta_Pion;
         Double_t tof_Proton = tof * beta / beta_Proton;

         hist->hTofModulevsTray->Fill(trayId, moduleId);
         hist->hTofvsBoardId->Fill(boardId, mPicoBTofPidTraits->btof());
         //hist->hTofPhivsZ->Fill(mPicoBTofPidTraits->btofHisPos().z(), mPicoBTofPidTraits->btofHisPos().phi());
         if (mPicoTrack->nHitsFit() >= 20 && TMath::Abs(momentum.pseudoRapidity()) < 1) //track quality cuts
         {
            if (momentum.perp() > 0.2 && momentum.mag() > 0.3 && momentum.mag() < 1.0 && mPicoTrack->nSigmaPion() < 1) //pion, cut nSigmaPion to get rid of electrons
            {
               hist->hBetaDiffvsTrayIdPi->Fill(trayId, 1. / beta - 1. / beta_Pion);
               hist->hTofDiffvsTrayIdPi->Fill(trayId, tof - tof_Pion);
               /*
                  if(mPicoTrack->charge()>0) {
                  hist->hBetaDiffvsTrayIdPiPlus->Fill(trayId, 1./beta - 1./beta_Pion);
                  hist->hTofDiffvsTrayIdPiPlus->Fill(trayId, tof - tof_Pion);
                  } else {
                  hist->hBetaDiffvsTrayIdPiMinus->Fill(trayId, 1./beta - 1./beta_Pion);
                  hist->hTofDiffvsTrayIdPiMinus->Fill(trayId, tof - tof_Pion);
                  }
                  */
            }
            //if(momentum.mag()>1.5&&momentum.mag()<2.5) {//proton
            if (momentum.mag() > 0.5 && momentum.mag() < 2.5 && mPicoTrack->nSigmaProton() < 1) //proton
            {
               hist->hBetaDiffvsTrayIdProton->Fill(trayId, 1. / beta - 1. / beta_Proton);
               hist->hTofDiffvsTrayIdProton->Fill(trayId, tof - tof_Proton);
               /*
                  if(mPicoTrack->charge()>0) {
                  hist->hBetaDiffvsTrayIdProton->Fill(trayId, 1./beta - 1./beta_Proton);
                  hist->hTofDiffvsTrayIdProton->Fill(trayId, tof - tof_Proton);
                  }
                  else {
                  hist->hBetaDiffvsTrayIdPbar->Fill(trayId, 1./beta - 1./beta_Proton);
                  hist->hTofDiffvsTrayIdPbar->Fill(trayId, tof - tof_Proton);
                  }
                  */
            }
            if (beta > -998) hist->hMassvsp->Fill(momentum.mag(), pow(momentum.mag(), 2) * (pow(1 / beta, 2) - 1.));
         }
         //-------------------------------------+
         //hist->hPvsBeta->Fill(momentum.mag(), mPicoBTofPidTraits->btofBeta());
         //hist->hPvsTimeOfFlight->Fill(momentum.mag(), mPicoBTofPidTraits->btof());
         if (TMath::Abs(mPicoBTofPidTraits->btofBeta() > Verror))hist->hPvsInvBeta->Fill(momentum.mag(), 1.0 / mPicoBTofPidTraits->btofBeta());
         else hist->hPvsInvBeta->Fill(momentum.mag(), 0.0);
      }

      if (bDEBUG)cout << "\n\tTOF Track histos filled." << endl;

      //Fill BEMC Histograms
      StEmcGeom *mEmcGeom = StEmcGeom::getEmcGeom(detname[3].Data());

      if (emcIndex >= 0) //Add by GNX
      {
         mPicoEmcPidTraits = mPicoDst->emcPidTraits(emcIndex);//GNX
         hist->hEmcTowerId->Fill(mPicoEmcPidTraits->btowId());
         if (mPicoEmcPidTraits->e0() > 0) hist->hEmcEnergyHitVsP->Fill(mPicoEmcPidTraits->e0(), momentum.mag());
         if (mPicoEmcPidTraits->e() > 0) hist->hEmcEnergySumVsP->Fill(mPicoEmcPidTraits->e(), momentum.mag());
         if (mPicoEmcPidTraits->e1() > 0) hist->hEmcEnergyTowVsP->Fill(mPicoEmcPidTraits->e1(), momentum.mag());
         if (mPicoEmcPidTraits->e1() > 0) hist->hEmcDzVsDphi->Fill(mPicoEmcPidTraits->zDist(), mEmcGeom->Radius()*mPicoEmcPidTraits->phiDist());
         //hist->hEoPvsP_Hit->Fill(mPicoEmcPidTraits->e0()/momentum.mag(), momentum.mag());
         //hist->hEoPvsP_Sum->Fill(mPicoEmcPidTraits->e()/momentum.mag(), momentum.mag());
         //hist->hEoPvsP_Tow->Fill(mPicoEmcPidTraits->e1()/momentum.mag(), momentum.mag());
         //double distance = sqrt(mPicoEmcPidTraits->zDist()*mPicoEmcPidTraits->zDist() + mPicoEmcPidTraits->phiDist()*mPicoEmcPidTraits->phiDist()*mEmcGeom->Radius()*mEmcGeom->Radius());
         //hist->hEmcDistVsP->Fill(distance, momentum.mag());
         //if(mPicoEmcPidTraits->e3() > energyMin && mPicoEmcPidTraits->e() > energyMin) hist->hEmcEnergyRatioVsDist->Fill(mPicoEmcPidTraits->e()/mPicoEmcPidTraits->e3(), distance);
         if (bDEBUG)cout << "\n\tBEMC Track histos filled." << endl;

         //Fill BSMD Histograms
         hist->hSmdHitsEtaVsPhi->Fill(mPicoEmcPidTraits->nEta(), mPicoEmcPidTraits->nPhi());

         if (bDEBUG)cout << "\n\tBSMD Track histos filled." << endl;
      }
      //Fill MTD matching Histograms
      //Add by GNX
      if (mtdIndex >= 0)
      {
         mPicoMtdPidTraits = mPicoDst->mtdPidTraits(mtdIndex);//GNX
         hist->hMtdDeltaYVspt->Fill(mPicoMtdPidTraits->deltaY(), momentum.perp());
         hist->hMtdDeltaZVspt->Fill(mPicoMtdPidTraits->deltaZ(), momentum.perp());
         hist->hMtdDeltaTOFVspt->Fill(mPicoMtdPidTraits->deltaTimeOfFlight(), momentum.perp());
         hist->hMtdMatchFlag->Fill(mPicoMtdPidTraits->matchFlag());
      }
      //getElectronInfo(mPicoTrack);

   }//loop thru mPicoTracks



   // Multiplicity and cell vs backleg
   // 1) all the MTD hits;
   // 2) MTD hits that are matched to tracks;
   // 3) MTD hits that fired the trigger.
   // cell = 12*(module()-1)+cell() and backleg=backleg()

   int iMtdHit = 0;
   int nMtdHits = mPicoDst->numberOfMtdHits();
   hist->hMultMtdHit_1->Fill(nMtdHits);
   int nMtdhitsFireTrig = 0;
   int nMtdhitsMatchTrack = 0;
   int cell;
   int backleg;

   while (1)
   {
      StPicoMtdHit *mPicoMtdHit = (StPicoMtdHit*) mPicoDst->mtdHit(iMtdHit);//GNX
      if (!mPicoMtdHit)
      {
         break;
      }
      //for(int iMtdHit=0;iMtdHit<nMtdHits;iMtdHit++) mPicoMtdHit = mPicoDst->mtdHit(iMtdHit);//GNX
      iMtdHit++;

      cell = 12 * (mPicoMtdHit->module() - 1) + mPicoMtdHit->cell();
      backleg = mPicoMtdHit->backleg();

      hist->hCellVsbackleg_1->Fill(backleg, cell);
      hist->hMtdTriggerFlag->Fill(mPicoMtdHit->triggerFlag());

      if (mPicoMtdHit->triggerFlag() > 0)
      {
         nMtdhitsFireTrig++;
         hist->hCellVsbackleg_3->Fill(backleg, cell);
      }

   }

   iTrack = 0;
   while (1)
   {
      mPicoTrack = (StPicoTrack*) mPicoDst->track(iTrack);
      if (!mPicoTrack)
      {
         break;
      }
      int mtdIndex = mPicoTrack->mtdPidTraitsIndex();
      iTrack++;
      if (mtdIndex >= 0)
      {
         mPicoMtdPidTraits = mPicoDst->mtdPidTraits(mtdIndex);//GNX
         cell = 12 * (mPicoMtdPidTraits->module() - 1) + mPicoMtdPidTraits->cell();
         backleg = mPicoMtdPidTraits->backleg();
         nMtdhitsMatchTrack++;
         hist->hCellVsbackleg_2->Fill(backleg, cell);
      }
   }
   // iTrack = 0;
   // while(1){
   //    mPicoTrack =(StPicoTrack*) mPicoDst->track(iTrack);
   //    if(!mPicoTrack){
   //       break;
   //    }
   //    int mtdIndex = mPicoTrack->mtdPidTraitsIndex();
   //       if(mtdIndex >=0 ) mPicoMtdPidTraits = mPicoDst->mtdPidTraits(mtdIndex);//GNX
   //       iTrack++;
   //       cell=12*(mPicoMtdPidTraits->module()-1)+mPicoMtdPidTraits->cell();
   //       backleg=mPicoMtdPidTraits->backleg();
   //       if(mPicoMtdPidTraits){
   //          nMtdhitsMatchTrack++;
   //          hist->hCellVsbackleg_2->Fill(backleg,cell);
   //       }
   // }

   hist->hMultMtdHit_11->Fill(iMtdHit);
   hist->hMultMtdHit_2->Fill(nMtdhitsMatchTrack);
   hist->hMultMtdHit_3->Fill(nMtdhitsFireTrig);



   return 0;
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
   void StQaAnalysisMaker::getElectronInfo(StPicoTrack* mPicoTrack)
   {
   if(!mPicoTrack) return;

   if(bDEBUG)cout<<"\nBegin looping through particles..."<<endl;

   Int_t ncut = 0;

//Perform Electron Cuts and store track variables, with each set of cuts adding to the last
StThreeVectorF momentum = mPicoTrack->pMom();
StEmcGeom *mEmcGeom = StEmcGeom::getEmcGeom(detname[3].Data());
double distance = sqrt(mPicoTrack->zDist()*mPicoTrack->zDist() + mPicoTrack->phiDist()*mPicoTrack->phiDist()*mEmcGeom->Radius()*mEmcGeom->Radius());

//perform electron PID, store all electron candidates event-wise
//if(momentum.mag() < pElecMin) return;
if(momentum.perp() < ptElecMin) return;

if(fabs(mPicoTrack->nSigmaElectron()) > nSigmaElectronMax || TMath::Abs(1.0/mPicoTrack->btofBeta() - 1.0) > invBetaMax) return;
if(TMath::Abs(mPicoTrack->nSigmaPion()) < nSigmaPionMin || TMath::Abs(mPicoTrack->nSigmaProton()) < nSigmaProtonMin || TMath::Abs(mPicoTrack->nSigmaKaon()) < nSigmaKaonMin) return;
electronArray[ncut][ea[ncut]][0] = (Float_t)mPicoTrack->charge();
electronArray[ncut][ea[ncut]][1] = (Float_t)momentum.x();
electronArray[ncut][ea[ncut]][2] = (Float_t)momentum.y();
electronArray[ncut][ea[ncut]][3] = (Float_t)momentum.z();
electronArray[ncut][ea[ncut]][4] = (Float_t)mPicoTrack->e1();
electronArray[ncut][ea[ncut]][5] = (Float_t)mPicoTrack->dEdx();
electronArray[ncut][ea[ncut]][6] = (Float_t)mPicoTrack->nSigmaElectron();
electronArray[ncut][ea[ncut]][7] = (Float_t)mPicoTrack->btofBeta();
electronArray[ncut][ea[ncut]][8] = (Float_t)mPicoTrack->nEta();
electronArray[ncut][ea[ncut]][9] = (Float_t)mPicoTrack->nPhi();
electronArray[ncut][ea[ncut]][10] = (Float_t)distance;
ea[ncut]++;
ncut++;

return;
}
*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
void StQaAnalysisMaker::reconstructDielectrons()
{
   //Dielectron reconstruction
   //electronArray = {0 = charge, 1 = px, 2 = py, 3 = pz, 4 = e, 5 = dEdx, 6 = nsigmaE, 7 = btofBeta, 8 = smdHitsEta, 9 = smdHitsPhi, 10 = smdDist};
   bool likesign;
   for (int icut = 0; icut < nelectronCuts; icut ++)
   {
      for (int ie1 = 0; ie1 < ea[icut] - 1; ie1++)
      {
         for (int ie2 = ie1 + 1; ie2 < ea[icut]; ie2++)
         {

            //Reconstruct electron pairs
            StThreeVectorF elec1(electronArray[icut][ie1][1], electronArray[icut][ie1][2], electronArray[icut][ie1][3]);
            StLorentzVectorF elecL1(electronArray[icut][ie1][1], electronArray[icut][ie1][2], electronArray[icut][ie1][3], electronMass);
            StThreeVectorF elec2(electronArray[icut][ie2][1], electronArray[icut][ie2][2], electronArray[icut][ie2][3]);
            StLorentzVectorF elecL2(electronArray[icut][ie2][1], electronArray[icut][ie2][2], electronArray[icut][ie2][3], electronMass);
            StLorentzVectorF dielec = elecL1 + elecL2;
            double angle = elec1.angle(elec2);
            double mass = sqrt(2.0 * elec1.mag() * elec2.mag() * (1.0 - cos(angle)));

            likesign = true;
            if (electronArray[icut][ie1][0]*electronArray[icut][ie2][0] < 0) likesign = false;
            if (likesign)
            {
               //e- e- pairs
               if (electronArray[icut][ie1][0] < 0)
               {
                  hist->hMassVsPt_NN[icut]->Fill(mass, dielec.perp());
                  //hist->hMassVsPhi_NN[icut]->Fill(mass, dielec.phi());
                  //hist->hYvsPt_NN[icut]->Fill(dielec.rapidity(), dielec.perp());
               }
               //e+ e+ pairs
               else
               {
                  hist->hMassVsPt_PP[icut]->Fill(mass, dielec.perp());
                  //hist->hMassVsPhi_PP[icut]->Fill(mass, dielec.phi());
                  //hist->hYvsPt_PP[icut]->Fill(dielec.rapidity(), dielec.perp());
               }
            }
            //e+ e- pairs
            else
            {
               hist->hMassVsPt_NP[icut]->Fill(mass, dielec.perp());
               //hist->hPhiVsPt_NP[icut]->Fill(mass, dielec.phi());
               //hist->hYvsPt_NP[icut]->Fill(dielec.rapidity(), dielec.perp());
            }

         }//2nd electron
      }//1st electron

      //single electron plots
      for (int ie1 = 0; ie1 < ea[icut]; ie1++)
      {
         StThreeVectorF elec1(electronArray[icut][ie1][1], electronArray[icut][ie1][2], electronArray[icut][ie1][3]);
         hist->hElectronDedxVsP[icut]->Fill(elec1.mag(), electronArray[icut][ie1][5]);
         hist->hElectronSigmaEvsP[icut]->Fill(electronArray[icut][ie1][6], elec1.mag());
         hist->hElectronEvsP[icut]->Fill(electronArray[icut][ie1][4], elec1.mag());
         hist->hElectronEoPvsP[icut]->Fill(electronArray[icut][ie1][4] / elec1.mag(), elec1.mag());
         if (electronArray[icut][ie1][7] > 0)hist->hElectronPvsInvBeta[icut]->Fill(elec1.mag(), 1.0 / electronArray[icut][ie1][7]);
         else hist->hElectronPvsInvBeta[icut]->Fill(elec1.mag(), 0.0);
         hist->hElectronSmdEtaVsPhi[icut]->Fill(electronArray[icut][ie1][8], electronArray[icut][ie1][9]);
         hist->hElectronSmdDistVsP[icut]->Fill(electronArray[icut][ie1][10], elec1.mag());
      }//electron loop


   }//cut loop

   return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Int_t StQaAnalysisMaker::Init()
{

   acceptedEvent = false;
   bDEBUG = false;//true;
   if (bDEBUG)cout << "\nBeginning Init().." << endl;

   hist = new HistogramsQa();
   if (bDEBUG)cout << "\nHistograms Initialised.." << endl;

   hist->imgOutDir = outDir;
   hist->outputQaName = mQaOutputName;

   if (bDEBUG)cout << "HistogramsQa Loaded.";

   mNEvent = 0;
   nTriggerTypes = 4; //number of trigger ids
   nelectronCuts = 1; //number of cut types

   //Event Cuts
   Verror = 1e-5;
   refMultMin = 1;
   VzMax = 6.0;
   VzMaxHFTRatio = 4.0;
   //VzMax = 12.0; //For Hao Check
   // deltaVzMax = 6.0;
    deltaVzMax = 3.0;
   //Track Cuts
   ptMin = 0.15;
   // ptMin = 2.0;// this is for pt 2GeV check Mar23,2016
   etaMax = 1.0;
   etaMaxHFTRatio = 0.4;
   nHitsFitMin = 25;
   nHitsRatioMin = 0.52;
   nDedxHitsMin = 12;
   dcaMax = 3.0;
   dcaMaxHFTRatio = 1.5;
   energyMin = 0.15;

   //Electron Cuts
   electronMass = 0.511e-3;//StElectron::instance()->mass();
   pElecMin = 0.2;
   ptElecMin = 0.2;
   nSigmaElectronMax = 2.0;
   nSigmaPionMin = 2.5;
   nSigmaProtonMin = 2.5;
   nSigmaKaonMin = 2.5;
   invBetaMax = 0.03;
   eopMin = 0.5;
   eopMax = 2.0;
   nHitsSmdEtaMin = 2;
   nHitsSmdPhiMin = 2;

   if (bDEBUG)cout << "\nInit() done." << endl;

   return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StQaAnalysisMaker::Finish()
{

   saveHistograms();
   return kStOK;
}

//-----------------------------------------------------------------------------
void StQaAnalysisMaker::Clear(Option_t *opt)
{
}

//-----------------------------------------------------------------------------
Int_t StQaAnalysisMaker::Make()
{
   if (!mPicoDstMaker)
   {
      LOG_WARN << " No PicoDstMaker! Skip! " << endm;
      return kStWarn;
   }

   mPicoDst = mPicoDstMaker->picoDst();
   if (!mPicoDst)
   {
      LOG_WARN << " No PicoDst! Skip! " << endm;
      return kStWarn;
   }

   //Remove stored electrons from previous event
   for (int cut = 0; cut < 5; cut++)
   {
      ea[cut] = 0;
      for (int cj = 0; cj < 300; cj++)
      {
         for (int ck = 0; ck < 15; ck++)
         {
            electronArray[cut][cj][ck] = 0;
         }
      }
   }
   getEventInfo(mPicoDst);//get event info
   if (acceptedEvent) //if event was accepted
   {
      getTrackInfo(mPicoDst);//loop through all tracks and store info. For each track, preform electron identification using getElectronInfo(mPicoTrack);
      //reconstructDielectrons();//reconstruct tracks in the event passing electron id cuts into pairs
      // getV0Info(mPicoDst); // get V0(KShort Lambda ,anti-Lambda) information
   }
   mNEvent++;
   //if((mNEvent)%10000==0 && mNEvent>0){
   // cout <<"Saving Histograms..." << endl;
   // saveHistograms();
   // cout<<"Histograms saved after "<<mNEvent<<" events\n";
   // }

   //  mPicoDst->Print();
   //  mPicoDst->printTracks();

   return kStOK;
}

//--------------------------------------------------------------
void StQaAnalysisMaker::saveHistograms()
{
   TFile *f = new TFile(mOutputName, "RECREATE");
   f->cd();
   hist->writeData(f);
   f->Close();
   f->Delete();
   return;
}
float StQaAnalysisMaker::Centrality(int mult)
{
   int central;
   float centFull[9] = {10, 21, 41, 72, 118, 182, 266, 375, 441};
   if (mult >= centFull[8]) central = 9;
   else if (mult >= centFull[7]) central = 8;
   else if (mult >= centFull[6]) central = 7;
   else if (mult >= centFull[5]) central = 6;
   else if (mult >= centFull[4]) central = 5;
   else if (mult >= centFull[3]) central = 4;
   else if (mult >= centFull[2]) central = 3;
   else if (mult >= centFull[1]) central = 2;
   else if (mult >= centFull[0]) central = 1;
   else central = 0;

   return central;
}

