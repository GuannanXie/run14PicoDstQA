/*

Written by Christopher B. Powell
2010
CBPowell@lbl.gov
---------------------------------

Macro to plot all TH1 and TH2 objects in a histogram,
and create a PDF with 2 plots per page.

Select plots are chosen for certain log axes or plot styles.
TOF mass and invbeta have predictions plotted too.
Below, several histogram FitSlices are performed and plotted.

*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "iomanip.h"
#include <math.h>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TText.h>
#include <TLine.h>
#include <TPDF.h>
#include <TBox.h>
#include <TKey.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TString.h>
#include <TROOT.h>

void setpad(TVirtualPad *pad, float left, float right, float top, float bottom, int logy = 0, int logz = 0);
TLegend* myLeg(Double_t xlow, Double_t ylow, Double_t xup, Double_t yup, Int_t textFont = 42, Double_t textSize = 0.05);

void makeQaPlotsOnPdf(const char* FileName = "picoQA.AuAu200GeV.test")
{
   gROOT->Reset();
   gStyle->SetOptStat(111);
   gROOT->SetStyle("Plain");
   gStyle->SetPalette(1);

   const int nUniformBinNames = 1;//modified by xgn
   char *UniformBinName[nUniformBinNames] = {"hnHitsMapHFT"};

   const int nLogyNames = 27;//modified by xgn
   char *logyName[nLogyNames] = {"hMtdTriggerFlag", "hMtdMatchFlag", "hDeltaVz", "hRef", "hPt", "hPtNoTof", "hDca", "hnHitsMapHFT", "hnHitsHFT", "hnHitsPxlInner", "hnHitsPxlOuter", "hnHitsIst", "hnHitsSsd", "hHFTPt", "hHFTDca", "hDeltaVzEvtAccept", "hRefEvtAccept", "hTrackR", "hHFTTrackR", "hPtHao", "hHFTPtHao", "hPtNoTofHao", "hHFTDcaZoomIn", "all_Mtd_Hits", "all_Mtd_Hits11", "Mtd_Hits_mathch_tracks", "Mtd_Hits_fire_trigger"};
   const int nLogzNames = 4;
   char *logzName[nLogzNames] = {"hPhiVsEta", "hBetaDiffvsTrayIdPi", "hBetaDiffvsTrayIdProton", "hTofModulevsTray"};
   char boxPlotName[50];
   sprintf(boxPlotName, "hTofModulevsTray");
   char betaPlotName[50];
   sprintf(betaPlotName, "hPvsInvBeta");
   char massPlotName[50];
   sprintf(massPlotName, "hMassvsp");
   char fitslicesName[50];
   sprintf(fitslicesName, "hTofDiffvsTrayIdProton");
   char DcaXyPtFitSlicesName[50];
   sprintf(DcaXyPtFitSlicesName, "hHFTDcaXyVsPt");
   char DcaXyPhiFitSlicesName[50];
   sprintf(DcaXyPhiFitSlicesName, "hHFTDcaXyVsPhi");
   char DcaXyEtaFitSlicesName[50];
   sprintf(DcaXyEtaFitSlicesName, "hHFTDcaXyVsEta");
   char DcaXyPtZoomInFitSlicesName[50];
   char DcaXyZdcFitSlicesName[50];
   sprintf(DcaXyZdcFitSlicesName, "hHFTDcaXyVsZdc");
   sprintf(DcaXyPtZoomInFitSlicesName, "hHFTDcaXyVsPtZoomIn");
   char DcaXyPhiZoomInFitSlicesName[50];
   sprintf(DcaXyPhiZoomInFitSlicesName, "hHFTDcaXyVsPhiZoomIn");
   char DcaXyEtaZoomInFitSlicesName[50];
   sprintf(DcaXyEtaZoomInFitSlicesName, "hHFTDcaXyVsEtaZoomIn");
   char DcaXyZdcZoomInFitSlicesName[50];
   sprintf(DcaXyZdcZoomInFitSlicesName, "hHFTDcaXyVsZdcZoomIn");


   //added by xgn
   char NumberOfGlobalsName[50];
   sprintf(NumberOfGlobalsName, "hRefVsGlobalRef");
   char gRefMultName[50];
   sprintf(gRefMultName, "hRefVsGRef");
   char VzIdName[50];
   sprintf(VzIdName, "hVzVsRunId");
   // add gnx
   char HFTPt[50];
   sprintf(HFTPt, "hHFTPt");
   char Pt[50];
   sprintf(Pt, "hPt");
   char HFTPtHao[50];
   sprintf(HFTPtHao, "hHFTPtHao");
   char PtHao[50];
   sprintf(PtHao, "hPtHao");



   char name[100];
   sprintf(name, "%s.root", FileName);
   TFile *f = new TFile(name);
   int nEvents = -1, nEventsCut = -1;
   TH2F *hVzVsVpdVz = (TH2F*)f->Get("hVzVsVpdVz");
   TH2F *hVzVsVpdVzAccept = (TH2F*)f->Get("hVzVsVpdVzAccept");
   if (hVzVsVpdVz) nEvents = hVzVsVpdVz->GetEntries();
   if (hVzVsVpdVzAccept) nEventsCut = hVzVsVpdVzAccept->GetEntries();
   TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 700, 800);
   setpad(c1, 0.001, 0.001, 0.001, 0.001, 0, 0);
   TPad *pad = new TPad("pad", "", 0.00, 0.00, 1.00, 1.00);
   pad->Draw();

   TIter nextkey(f->GetListOfKeys());
   TObject *t;

   gStyle->SetOptFit(0);
   gStyle->SetEndErrorSize(2);
   sprintf(name, "%s.pdf", FileName);
   TPDF *pdf = new TPDF(name);

   //Include beta predictions
   TF1 *funBeta = new TF1("funBeta", "sqrt(pow(x,2)+pow([0],2))/x", 0., 10);
   funBeta->SetLineWidth(1);
   TF1 *funMass = new TF1("funMass", "[0]*[0]", 0, 10);
   funMass->SetLineWidth(1);

   Double_t MELECTRON  = 0.000511;
   Double_t MMUON  = 0.105658;
   Double_t MPION  = 0.13957;
   Double_t MKAON  = 0.493677;
   Double_t MPROTON  = 0.93827;

   //Set front page
   c1->cd();
   pad->cd();
   setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);
   TBox *bLabel = new TBox(0.01, 0.88, 0.99, 0.99);
   bLabel->SetFillColor(kBlack);
   bLabel->Draw();
   TLatex tl;
   tl.SetNDC();
   tl.SetTextColor(kWhite);
   tl.SetTextSize(0.063);
   char tlName[100];
   char tlName2[100];
   sprintf(tlName, "PicoDst QA AuAu 200 GeV reProduction");
   tl.DrawLatex(0.02, 0.9, tlName);
   tl.SetTextSize(0.075);
   tl.SetTextColor(kBlack);
   sprintf(tlName, "Number of events: ");
   tl.DrawLatex(0.1, 0.73, tlName);
   if (nEvents < 1e3)
   {
      sprintf(tlName, "%d ", nEvents);
      sprintf(tlName2, "%d after cuts", nEventsCut);

   }
   else if (nEvents < 1e6)
   {
      sprintf(tlName, "%d K", nEvents / 1e3);
      sprintf(tlName2, "%d K after cuts", nEventsCut / 1e3);
   }
   else
   {
      sprintf(tlName, "%1.1f M", nEvents / 1e6);
      sprintf(tlName2, "%1.1f M after cuts", nEventsCut / 1e6);

   }
   tl.DrawLatex(0.25, 0.65, tlName);
   tl.DrawLatex(0.25, 0.58, tlName2);

   tl.SetTextSize(0.05);
   sprintf(tlName, "Event Cuts: Library->SL16d");
   tl.DrawLatex(0.15, 0.48, tlName);
   tl.SetTextSize(0.04);
   //sprintf(tlName, "|V_{Z}| < 6 cm,|#Delta V_{Z}| < 3cm, refMult > 0");
   sprintf(tlName, "|V_{Z}| < 6 cm,|#Delta V_{Z}| < 3cm, refMult > 0");
   tl.DrawLatex(0.25, 0.43, tlName);
   tl.SetTextSize(0.04);
   sprintf(tlName, "VPDMB-5-p Trigger");
   tl.DrawLatex(0.25, 0.38, tlName);
   tl.SetTextSize(0.05);
   sprintf(tlName, "Track Cuts:");
   tl.DrawLatex(0.15, 0.30, tlName);
   tl.SetTextSize(0.04);
   sprintf(tlName, "Dca < 3.0 cm, Match Tof");
   tl.DrawLatex(0.25, 0.25, tlName);
   sprintf(tlName, "p_{T} > 0.15 GeV, |#eta| < 1.0, nHits > 25, ");
   tl.DrawLatex(0.25, 0.2, tlName);
   // sprintf(tlName, "nHits/nPoss > 0.52, nDedxHits > 11");
   sprintf(tlName, "nDedxHits > 11");
   tl.DrawLatex(0.25, 0.15, tlName);

   TBox *bFoot = new TBox(0.01, 0.01, 0.99, 0.12);
   bFoot->SetFillColor(kBlack);
   bFoot->Draw();
   tl.SetTextColor(kWhite);
   tl.SetTextSize(0.05);
   tl.DrawLatex(0.05, 0.05, (new TDatime())->AsString());
   sprintf(tlName, "File: %s.root", FileName);
   tl.SetTextColor(kBlack);
   tl.SetTextSize(0.04);
   tl.DrawLatex(0.015, 0.82, tlName);
   c1->cd();
   c1->Update();
   pdf->NewPage();

   c1->cd();
   pad->Divide(1, 2, 0.001, 0.001);
   int nCount2 = 0;
   int nFig = 0;
   TKey *key;
   TLine line;
   line.SetLineStyle(7);
   line.SetLineWidth(1);
   TString tName;
   tl.SetTextSize(0.035);

   //Loop through TH1 and TH2 histos and place them on PDF
   while ((key = (TKey*)nextkey()))
   {
      t = dynamic_cast<TObject*>(key->ReadObj());
      if (t)
      {
         sprintf(name, "%s.gif", t->GetName());
         if (!strcmp(t->ClassName(), "TH1D") || !strcmp(t->ClassName(), "TH1F"))
         {
            TH1F *h1 = (TH1F*)t;
            if (h1->GetEntries() > 0)
            {
               if (nCount2 == 0)
               {
                  if (nFig > 0)pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);
               }
               else
               {
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 0);
               }
               h1->GetYaxis()->SetTitleOffset(1.2);
               tName = t->GetName();

               for (int ilogy = 0; ilogy < nLogyNames; ilogy++)
               {
                  if (!tName.CompareTo(logyName[ilogy])) setpad(gPad, 0.1, 0.1, 0.02, 0.12, 1, 0);
               }
               TH1F *h11;
               TH1F *h22;
               TH1F *h111;
               TH1F *h222;

               if (!tName.CompareTo(HFTPt))
               {
                  //setpad(gPad,0.1,0.1,0.02,0.12, 0, 1);
                  TH1F *h1 = (TH1F*)t;
                  h11 = (TH1F*)t;
                  h1->Draw();
                  h1->GetYaxis()->SetTitle("count");
                  sprintf(tlName, "|#eta| < 0.4, dca<1.5, Vz<4.0");
                  tl.DrawLatex(0.3, 0.7, tlName);
                  nFig++;

//          nCount2=0;
//          sprintf(tlName, "Page %i", nFig/2+1);
//          tl.DrawLatex(0.02, 0.02, tlName);
//          c1->cd();
//          c1->Update();
                  //-----
//          pdf->NewPage();
//          pad->cd();
//          pad->Clear();
//          c1->cd();
                  pad->cd();
                  //    pad->Divide(1,2,0.001,0.001);
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 1, 0);
                  h11->Sumw2();
                  h22->Sumw2();
                  h11->Divide(h22);
                  h11->Draw();
                  h11->GetYaxis()->SetRangeUser(0.09, 4);
                  h11->GetYaxis()->SetTitle("HFT Ratio");
                  h11->SetMarkerStyle(20);
                  h11->SetName("HFT Ratio");

                  sprintf(tlName, "|#eta| < 0.4, dca<1.5, Vz<4.0");
                  tl.DrawLatex(0.3, 0.7, tlName);
                  nCount2 = 0;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();

               }
               else if (!tName.CompareTo(HFTPtHao))
               {
                  //setpad(gPad,0.1,0.1,0.02,0.12, 0, 1);
                  //-----
//          pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 1, 0);
                  TH1F *h1 = (TH1F*)t;
                  h111 = (TH1F*)t;
                  h1->Draw();
//          nFig++;
                  h1->SetName("hHFTPtRebin");
                  sprintf(tlName, "|#eta| < 0.4, dca<1.5, Vz<4.0");
                  tl.DrawLatex(0.3, 0.7, tlName);

                  pad->cd(2);
                  h111->Sumw2();
                  h222->Sumw2();
                  h111->Divide(h222);
                  h111->Draw();
                  h111->GetYaxis()->SetTitle("HFT Ratio");
                  h111->GetYaxis()->SetRangeUser(0.09, 4);
                  h111->SetMarkerStyle(20);
                  h111->SetName("HFT Ratio Rebin");
                  sprintf(tlName, "|#eta| < 0.4, dca<1.5, Vz<4.0");
                  tl.DrawLatex(0.3, 0.7, tlName);
//          nFig++;
                  nCount2 = 0;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();
               }

               else if (!tName.CompareTo(Pt))
               {
                  //setpad(gPad,0.1,0.1,0.02,0.12, 0, 1);
                  TH1F *h1 = (TH1F*)t;
                  h22 = (TH1F*)t;
                  h1->Draw();
                  h1->GetYaxis()->SetTitle("counts");
                  sprintf(tlName, "|#eta| < 0.4, dca<1.5, Vz<4.0");
                  tl.DrawLatex(0.3, 0.7, tlName);

               }
               else if (!tName.CompareTo(PtHao))
               {
                  //setpad(gPad,0.1,0.1,0.02,0.12, 0, 1);
                  TH1F *h1 = (TH1F*)t;
                  h222 = (TH1F*)t;
                  h1->Draw();
                  h1->SetName("hPtRebin");
//          nFig++;
                  sprintf(tlName, "|#eta| < 0.4, dca<1.5, Vz<4.0");
                  tl.DrawLatex(0.3, 0.4, tlName);

               }

               else
                  t->Draw();
               /*
                    for(int iUniformBin=0;iUniformBin<nUniformBinNames;iUniformBin++){
                      if(!tName.CompareTo(UniformBinName[iUniformBin])) {
                        setpad(gPad,0.1,0.1,0.02,0.12, 1, 0);
                        Int_t nb = h1->GetXaxis()->GetNbins();
                        TLine *l = new TLine;
                        l->SetLineWidth(3);
                        TText *text = new TText;
                        text->SetTextAlign(22);
                        text->SetTextSize(0.02);
                        Double_t x;
                        t->Draw();
                        for (Int_t i=1; i<=nb; i++) {
                           x = h1->GetXaxis()->GetBinLowEdge(i);
                           l->DrawLine(x,0,x,0.1);
                           text->DrawText(x,-0.15,Form("%g",x));
                        }

                     }
                    }
               */

               nFig++;
               nCount2++;
               if (nCount2 == 2)
               {
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();
                  //pdf->NewPage();
                  nCount2 = 0;
               }
            }
         }
         else if (!strcmp(t->ClassName(), "TH2D") || !strcmp(t->ClassName(), "TH2F"))
         {
            TH2F *h2 = (TH2F*)t;
            if (h2->GetEntries() > 0)
            {
               if (nCount2 == 0)
               {
                  if (nFig > 0)pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 1);
               }
               else
               {
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
               }
               tName = t->GetName();
               for (int ilogz = 0; ilogz < nLogzNames; ilogz++)
               {
                  if (!tName.CompareTo(logzName[ilogz])) setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
               }
               h2->GetYaxis()->SetTitleOffset(1.2);
               if (!tName.CompareTo(boxPlotName))
               {
                  //setpad(gPad,0.1,0.1,0.02,0.12, 0, 1);
                  t->Draw("box");
               }

               //HFT pT phi eta results of FitSlicesY() //GNX
               //BetaDiff vs. TrayId results of FitSlicesY()
               else if (!tName.CompareTo(DcaXyPtFitSlicesName))
               {
                  c1->cd();
                  pad->cd();
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
                  t->Draw("colz");
                  //    nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  /*
                              //------------xxx
                           // hHFTDcaXyVsPt->RebinX(10);
                           // hHFTDcaXyVsPt->RebinY(2);
                              c1->cd();
                              c1->Update();

                              //---------------
                              pdf->NewPage();
                              pad->cd();
                              pad->Clear();
                              c1->cd();
                              pad->cd();
                              pad->Divide(1,2,0.001,0.001);
                              pad->cd(1);
                              setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);

                              TF1 *fun = new TF1("fun","gaus");
                              fun->SetRange(-0.3,0.3);
                              hHFTDcaXyVsPt->FitSlicesX(fun,0,-1,4);
                              hHFTDcaZVsPt->FitSlicesX(fun,0,-1,4);
                              TH1F* hHFTDcaXyVsPtMean = (TH1F*) gDirectory->Get("hHFTDcaXyVsPt_1");
                              TH1F* hHFTDcaXyVsPtSigma = (TH1F*) gDirectory->Get("hHFTDcaXyVsPt_2");
                              hHFTDcaXyVsPtMean->SetLineColor(1);
                              hHFTDcaXyVsPtSigma->SetLineColor(1);
                              //hHFTDcaXyVsPtMean->SetLineStyle(7);
                              hHFTDcaXyVsPtMean->Draw();
                              hHFTDcaXyVsPtMean->GetYaxis()->SetRangeUser(-0.005,0.01);
                              TH1F* hHFTDcaZVsPtMean = (TH1F*) gDirectory->Get("hHFTDcaZVsPt_1");
                              TH1F* hHFTDcaZVsPtSigma = (TH1F*) gDirectory->Get("hHFTDcaZVsPt_2");
                              hHFTDcaZVsPtMean->SetLineColor(2);
                              hHFTDcaZVsPtSigma->SetLineColor(2);
                              //hHFTDcaZVsPtMean->SetLineStyle(7);
                              hHFTDcaZVsPtMean->Draw("same");
                              TLegend *leg = myLeg(0.5,0.6,0.75,0.86,42,0.06);
                              leg->SetHeader("hHFTDcaVsPt fitSliceX Mean");
                              leg->AddEntry(hHFTDcaXyVsPtMean,"DcaXy","l");
                              leg->AddEntry(hHFTDcaZVsPtMean,"DcaZ","l");
                              leg->Draw("same");

                              pad->cd(2);
                              setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);
                              hHFTDcaXyVsPtSigma->Draw();
                              hHFTDcaXyVsPtSigma->GetYaxis()->SetRangeUser(0.,0.2);
                              hHFTDcaZVsPtSigma->Draw("same");
                              TLegend *leg2 = myLeg(0.5,0.55,0.75,0.81,42,0.06);
                              leg2->SetHeader("hHFTDcaVsPt fitSlice sigma");
                              leg2->AddEntry(hHFTDcaXyVsPtSigma,"DcaXy","l");
                              leg2->AddEntry(hHFTDcaZVsPtSigma,"DcaZ","l");
                              leg2->Draw("same");
                              nFig++;
                              sprintf(tlName, "Page %i", nFig/2+1);
                              tl.DrawLatex(0.02, 0.02, tlName);
                  */
               }
               //HFT pT phi eta results of FitSlicesX() //GNX
               else if (!tName.CompareTo(DcaXyPhiFitSlicesName))
               {
                  c1->cd();
                  pad->cd();
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
                  t->Draw("colz");
//          nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);

                  //------------xxx
                  // hHFTDcaXyVsPhi->RebinX(10);
                  // hHFTDcaXyVsPhi->RebinY(2);
                  /*
                              c1->cd();
                              c1->Update();
                              //---------------
                              pdf->NewPage();
                              pad->cd();
                              pad->Clear();
                              c1->cd();
                              pad->cd();
                              pad->Divide(1,2,0.001,0.001);
                              pad->cd(1);
                              setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);

                              TF1 *fun = new TF1("fun","gaus");
                              fun->SetRange(-0.3,0.3);
                              hHFTDcaXyVsPhi->FitSlicesX(fun,0,-1,4);
                              hHFTDcaZVsPhi->FitSlicesX(fun,0,-1,4);
                              TH1F* hHFTDcaXyVsPhiMean = (TH1F*) gDirectory->Get("hHFTDcaXyVsPhi_1");
                              TH1F* hHFTDcaXyVsPhiSigma = (TH1F*) gDirectory->Get("hHFTDcaXyVsPhi_2");
                              hHFTDcaXyVsPhiMean->SetLineColor(1);
                              hHFTDcaXyVsPhiSigma->SetLineColor(1);
                              //hHFTDcaXyVsPhiMean->SetLineStyle(7);
                              hHFTDcaXyVsPhiMean->Draw();
                              hHFTDcaXyVsPhiMean->GetYaxis()->SetRangeUser(-0.02,0.02);
                              TH1F* hHFTDcaZVsPhiMean = (TH1F*) gDirectory->Get("hHFTDcaZVsPhi_1");
                              TH1F* hHFTDcaZVsPhiSigma = (TH1F*) gDirectory->Get("hHFTDcaZVsPhi_2");
                              hHFTDcaZVsPhiMean->SetLineColor(2);
                              hHFTDcaZVsPhiSigma->SetLineColor(2);
                              //hHFTDcaZVsPhiMean->SetLineStyle(7);
                              hHFTDcaZVsPhiMean->Draw("same");
                              TLegend *leg = myLeg(0.5,0.6,0.75,0.86,42,0.06);
                              leg->SetHeader("hHFTDcaVsPhi fitSliceX Mean");
                              leg->AddEntry(hHFTDcaXyVsPhiMean,"DcaXy","l");
                              leg->AddEntry(hHFTDcaZVsPhiMean,"DcaZ","l");
                              leg->Draw("same");

                              //---------------
                              pad->cd(2);
                              setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);

                              hHFTDcaXyVsPhiSigma->Draw();
                              hHFTDcaXyVsPhiSigma->GetYaxis()->SetRangeUser(0.,0.2);
                              hHFTDcaZVsPhiSigma->Draw("same");
                              TLegend *leg2 = myLeg(0.5,0.5,0.75,0.76,42,0.06);
                              leg2->SetHeader("hHFTDcaVsPhi fitSlice sigma");
                              leg2->AddEntry(hHFTDcaXyVsPhiSigma,"DcaXy","l");
                              leg2->AddEntry(hHFTDcaZVsPhiSigma,"DcaZ","l");
                              leg2->Draw("same");
                              nFig++;
                              sprintf(tlName, "Page %i", nFig/2+1);
                              tl.DrawLatex(0.02, 0.02, tlName);
                  */
               }
               //HFT pT Eta eta results of FitSlicesX() //GNX
               else if (!tName.CompareTo(DcaXyEtaFitSlicesName))
               {
                  c1->cd();
                  pad->cd();
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
                  t->Draw("colz");
//          nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);


                  //------------xxx
                  // hHFTDcaXyVsEta->RebinX(10);
                  // hHFTDcaXyVsEta->RebinY(2);
                  /*          c1->cd();
                              c1->Update();
                              //---------------
                              pdf->NewPage();
                              pad->cd();
                              pad->Clear();
                              c1->cd();
                              pad->cd();
                              pad->Divide(1,2,0.001,0.001);
                              pad->cd(1);
                              setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);

                              TF1 *fun = new TF1("fun","gaus");
                              fun->SetRange(-0.3,0.3);
                              hHFTDcaXyVsEta->FitSlicesX(fun,0,-1,4);
                              hHFTDcaZVsEta->FitSlicesX(fun,0,-1,4);
                              TH1F* hHFTDcaXyVsEtaMean = (TH1F*) gDirectory->Get("hHFTDcaXyVsEta_1");
                              TH1F* hHFTDcaXyVsEtaSigma = (TH1F*) gDirectory->Get("hHFTDcaXyVsEta_2");
                              hHFTDcaXyVsEtaMean->SetLineColor(1);
                              hHFTDcaXyVsEtaSigma->SetLineColor(1);
                              //hHFTDcaXyVsEtaMean->SetLineStyle(7);
                              hHFTDcaXyVsEtaMean->Draw();
                              hHFTDcaXyVsEtaMean->GetYaxis()->SetRangeUser(-0.001,0.002);
                              TH1F* hHFTDcaZVsEtaMean = (TH1F*) gDirectory->Get("hHFTDcaZVsEta_1");
                              TH1F* hHFTDcaZVsEtaSigma = (TH1F*) gDirectory->Get("hHFTDcaZVsEta_2");
                              hHFTDcaZVsEtaMean->SetLineColor(2);
                              hHFTDcaZVsEtaSigma->SetLineColor(2);
                              //hHFTDcaZVsEtaMean->SetLineStyle(7);
                              hHFTDcaZVsEtaMean->Draw("same");
                              TLegend *leg = myLeg(0.5,0.6,0.75,0.86,42,0.06);
                              leg->SetHeader("hHFTDcaVsEta fitSliceX Mean");
                              leg->AddEntry(hHFTDcaXyVsEtaMean,"DcaXy","l");
                              leg->AddEntry(hHFTDcaZVsEtaMean,"DcaZ","l");
                              leg->Draw("same");

                              //---------------
                                 pad->cd(2);
                              setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);

                              hHFTDcaXyVsEtaSigma->Draw();
                              hHFTDcaXyVsEtaSigma->GetYaxis()->SetRangeUser(0.012,0.02);
                              hHFTDcaZVsEtaSigma->Draw("same");
                              TLegend *leg2 = myLeg(0.5,0.5,0.75,0.76,42,0.06);
                              leg2->SetHeader("hHFTDcaVsEta fitSlice sigma");
                              leg2->AddEntry(hHFTDcaXyVsEtaSigma,"DcaXy","l");
                              leg2->AddEntry(hHFTDcaZVsEtaSigma,"DcaZ","l");
                              leg2->Draw("same");
                              nFig++;
                              sprintf(tlName, "Page %i", nFig/2+1);
                              tl.DrawLatex(0.02, 0.02, tlName);
                  */
               }
               //HFT pT Eta eta Zdc results of FitSlicesX() //GNX
               else if (!tName.CompareTo(DcaXyZdcFitSlicesName))
               {
                  c1->cd();
                  pad->cd();
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
                  t->Draw("colz");
                  //       nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);


                  //------------xxx
                  //hHFTDcaXyVsZdc->RebinX(50);
                  //hHFTDcaXyVsZdc->RebinY(50);
                  /*          c1->cd();
                              c1->Update();
                              //---------------
                              pdf->NewPage();
                              pad->cd();
                              pad->Clear();
                              c1->cd();
                              pad->cd();
                              pad->Divide(1,2,0.001,0.001);
                              pad->cd(1);
                              setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);

                              TF1 *fun = new TF1("fun","gaus");
                              fun->SetRange(-0.3,0.3);
                              hHFTDcaXyVsZdc->FitSlicesX(fun,0,-1,4);
                              hHFTDcaZVsZdc->FitSlicesX(fun,0,-1,4);
                              TH1F* hHFTDcaXyVsZdcMean = (TH1F*) gDirectory->Get("hHFTDcaXyVsZdc_1");
                              TH1F* hHFTDcaXyVsZdcSigma = (TH1F*) gDirectory->Get("hHFTDcaXyVsZdc_2");
                              hHFTDcaXyVsZdcMean->SetLineColor(1);
                              hHFTDcaXyVsZdcSigma->SetLineColor(1);
                              //hHFTDcaXyVsZdcMean->SetLineStyle(7);
                              hHFTDcaXyVsZdcMean->Draw();
                              hHFTDcaXyVsZdcMean->GetYaxis()->SetRangeUser(-0.0004,0.001);
                              TH1F* hHFTDcaZVsZdcMean = (TH1F*) gDirectory->Get("hHFTDcaZVsZdc_1");
                              TH1F* hHFTDcaZVsZdcSigma = (TH1F*) gDirectory->Get("hHFTDcaZVsZdc_2");
                              hHFTDcaZVsZdcMean->SetLineColor(2);
                              hHFTDcaZVsZdcSigma->SetLineColor(2);
                              //hHFTDcaZVsZdcMean->SetLineStyle(7);
                              hHFTDcaZVsZdcMean->Draw("same");
                              TLegend *leg = myLeg(0.5,0.6,0.75,0.86,42,0.06);
                              leg->SetHeader("hHFTDcaVsZdc fitSliceX Mean");
                              leg->AddEntry(hHFTDcaXyVsZdcMean,"DcaXy","l");
                              leg->AddEntry(hHFTDcaZVsZdcMean,"DcaZ","l");
                              leg->Draw("same");

                              //---------------
                                 pad->cd(2);
                              setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);

                              hHFTDcaXyVsZdcSigma->Draw();
                              hHFTDcaXyVsZdcSigma->GetYaxis()->SetRangeUser(0.012,0.02);
                              hHFTDcaZVsZdcSigma->Draw("same");
                              TLegend *leg2 = myLeg(0.5,0.5,0.75,0.76,42,0.06);
                              leg2->SetHeader("hHFTDcaVsZdc fitSlice sigma");
                              leg2->AddEntry(hHFTDcaXyVsZdcSigma,"DcaXy","l");
                              leg2->AddEntry(hHFTDcaZVsZdcSigma,"DcaZ","l");
                              leg2->Draw("same");
                              nFig++;
                              sprintf(tlName, "Page %i", nFig/2+1);
                              tl.DrawLatex(0.02, 0.02, tlName);
                  */
               }


               //HFT pT phi eta results of FitSlicesY() //GNX
               //BetaDiff vs. TrayId results of FitSlicesY()
               else if (!tName.CompareTo(DcaXyPtZoomInFitSlicesName))
               {
                  c1->cd();
                  pad->cd();
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
                  t->Draw("colz");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);

                  c1->cd();
                  c1->Update();
                  //---------------
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);
                  //------------xxx
                  hHFTDcaXyVsPtZoomIn->RebinX(4);
                  hHFTDcaXyVsPtZoomIn->RebinY(4);
                  hHFTDcaZVsPtZoomIn->RebinX(4);
                  hHFTDcaZVsPtZoomIn->RebinY(4);

                  TF1 *fun = new TF1("fun", "gaus");
                  fun->SetRange(-0.03, 0.03);
                  hHFTDcaXyVsPtZoomIn->FitSlicesX(fun, 0, -1, 4);
                  hHFTDcaZVsPtZoomIn->FitSlicesX(fun, 0, -1, 4);
                  TH1F* hHFTDcaXyVsPtZoomInMean = (TH1F*) gDirectory->Get("hHFTDcaXyVsPtZoomIn_1");
                  TH1F* hHFTDcaXyVsPtZoomInSigma = (TH1F*) gDirectory->Get("hHFTDcaXyVsPtZoomIn_2");
                  hHFTDcaXyVsPtZoomInMean->SetLineColor(1);
                  hHFTDcaXyVsPtZoomInSigma->SetLineColor(1);
                  //hHFTDcaXyVsPtZoomInMean->SetLineStyle(7);
                  hHFTDcaXyVsPtZoomInMean->Draw();
                  hHFTDcaXyVsPtZoomInMean->GetYaxis()->SetRangeUser(-0.002, 0.004);
                  TH1F* hHFTDcaZVsPtZoomInMean = (TH1F*) gDirectory->Get("hHFTDcaZVsPtZoomIn_1");
                  TH1F* hHFTDcaZVsPtZoomInSigma = (TH1F*) gDirectory->Get("hHFTDcaZVsPtZoomIn_2");
                  hHFTDcaZVsPtZoomInMean->SetLineColor(2);
                  hHFTDcaZVsPtZoomInSigma->SetLineColor(2);
                  //hHFTDcaZVsPtZoomInMean->SetLineStyle(7);
                  hHFTDcaZVsPtZoomInMean->Draw("same");
                  TLegend *leg = myLeg(0.3, 0.5, 0.55, 0.76, 42, 0.06);
                  leg->SetHeader("hHFTDcaVsPtZoomIn fitSliceX Mean");
                  leg->AddEntry(hHFTDcaXyVsPtZoomInMean, "DcaXy", "l");
                  leg->AddEntry(hHFTDcaZVsPtZoomInMean, "DcaZ", "l");
                  leg->Draw("same");

                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);

                  //---------------
                  hHFTDcaXyVsPtZoomInSigma->Draw();
                  hHFTDcaXyVsPtZoomInSigma->GetYaxis()->SetRangeUser(-0.005, 0.04);
                  hHFTDcaZVsPtZoomInSigma->Draw("same");
                  TLegend *leg2 = myLeg(0.3, 0.5, 0.55, 0.76, 42, 0.06);
                  leg2->SetHeader("hHFTDcaVsPtZoomIn fitSlice sigma");
                  leg2->AddEntry(hHFTDcaXyVsPtZoomInSigma, "DcaXy", "l");
                  leg2->AddEntry(hHFTDcaZVsPtZoomInSigma, "DcaZ", "l");
                  leg2->Draw("same");

                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);

               }
               //HFT pT phi eta results of FitSlicesX() //GNX
               else if (!tName.CompareTo(DcaXyPhiZoomInFitSlicesName))
               {
                  c1->cd();
                  pad->cd();
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
                  t->Draw("colz");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);

                  c1->cd();
                  c1->Update();
                  //---------------
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);
                  //------------xxx
                  hHFTDcaXyVsPhiZoomIn->RebinX(4);
                  hHFTDcaXyVsPhiZoomIn->RebinY(4);
                  hHFTDcaZVsPhiZoomIn->RebinX(4);
                  hHFTDcaZVsPhiZoomIn->RebinY(4);

                  TF1 *fun = new TF1("fun", "gaus");
                  fun->SetRange(-0.03, 0.03);
                  hHFTDcaXyVsPhiZoomIn->FitSlicesX(fun, 0, -1, 4);
                  hHFTDcaZVsPhiZoomIn->FitSlicesX(fun, 0, -1, 4);
                  TH1F* hHFTDcaXyVsPhiZoomInMean = (TH1F*) gDirectory->Get("hHFTDcaXyVsPhiZoomIn_1");
                  TH1F* hHFTDcaXyVsPhiZoomInSigma = (TH1F*) gDirectory->Get("hHFTDcaXyVsPhiZoomIn_2");
                  hHFTDcaXyVsPhiZoomInMean->SetLineColor(1);
                  hHFTDcaXyVsPhiZoomInSigma->SetLineColor(1);
                  //hHFTDcaXyVsPhiZoomInMean->SetLineStyle(7);
                  hHFTDcaXyVsPhiZoomInMean->Draw();
                  hHFTDcaXyVsPhiZoomInMean->GetYaxis()->SetRangeUser(-0.002, 0.004);
                  TH1F* hHFTDcaZVsPhiZoomInMean = (TH1F*) gDirectory->Get("hHFTDcaZVsPhiZoomIn_1");
                  TH1F* hHFTDcaZVsPhiZoomInSigma = (TH1F*) gDirectory->Get("hHFTDcaZVsPhiZoomIn_2");
                  hHFTDcaZVsPhiZoomInMean->SetLineColor(2);
                  hHFTDcaZVsPhiZoomInSigma->SetLineColor(2);
                  //hHFTDcaZVsPhiZoomInMean->SetLineStyle(7);
                  hHFTDcaZVsPhiZoomInMean->Draw("same");
                  TLegend *leg = myLeg(0.3, 0.65, 0.55, 0.9, 42, 0.06);
                  leg->SetHeader("hHFTDcaVsPhiZoomIn fitSliceX Mean");
                  leg->AddEntry(hHFTDcaXyVsPhiZoomInMean, "DcaXy", "l");
                  leg->AddEntry(hHFTDcaZVsPhiZoomInMean, "DcaZ", "l");
                  leg->Draw("same");

                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);
                  //---------------
                  hHFTDcaXyVsPhiZoomInSigma->Draw();
                  hHFTDcaXyVsPhiZoomInSigma->GetYaxis()->SetRangeUser(0., 0.02);
                  hHFTDcaZVsPhiZoomInSigma->Draw("same");
                  TLegend *leg2 = myLeg(0.3, 0.6, 0.55, 0.86, 42, 0.06);
                  leg2->SetHeader("hHFTDcaVsPhiZoomIn fitSlice sigma");
                  leg2->AddEntry(hHFTDcaXyVsPhiZoomInSigma, "DcaXy", "l");
                  leg2->AddEntry(hHFTDcaZVsPhiZoomInSigma, "DcaZ", "l");
                  leg2->Draw("same");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);


               }
               //HFT pT Eta eta results of FitSlicesX() //GNX
               else if (!tName.CompareTo(DcaXyEtaZoomInFitSlicesName))
               {
                  c1->cd();
                  pad->cd();
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
                  t->Draw("colz");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);

                  c1->cd();
                  c1->Update();
                  //---------------
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);

                  //------------xxx
                  hHFTDcaXyVsEtaZoomIn->RebinX(2);
                  hHFTDcaXyVsEtaZoomIn->RebinY(2);
                  hHFTDcaZVsEtaZoomIn->RebinX(2);
                  hHFTDcaZVsEtaZoomIn->RebinY(2);

                  TF1 *fun = new TF1("fun", "gaus");
                  fun->SetRange(-0.03, 0.03);
                  hHFTDcaXyVsEtaZoomIn->FitSlicesX(fun, 0, -1, 4);
                  hHFTDcaZVsEtaZoomIn->FitSlicesX(fun, 0, -1, 4);
                  TH1F* hHFTDcaXyVsEtaZoomInMean = (TH1F*) gDirectory->Get("hHFTDcaXyVsEtaZoomIn_1");
                  TH1F* hHFTDcaXyVsEtaZoomInSigma = (TH1F*) gDirectory->Get("hHFTDcaXyVsEtaZoomIn_2");
                  hHFTDcaXyVsEtaZoomInMean->SetLineColor(1);
                  hHFTDcaXyVsEtaZoomInSigma->SetLineColor(1);
                  //hHFTDcaXyVsEtaZoomInMean->SetLineStyle(7);
                  hHFTDcaXyVsEtaZoomInMean->Draw();
                  hHFTDcaXyVsEtaZoomInMean->GetYaxis()->SetRangeUser(-0.002, 0.004);
                  TH1F* hHFTDcaZVsEtaZoomInMean = (TH1F*) gDirectory->Get("hHFTDcaZVsEtaZoomIn_1");
                  TH1F* hHFTDcaZVsEtaZoomInSigma = (TH1F*) gDirectory->Get("hHFTDcaZVsEtaZoomIn_2");
                  hHFTDcaZVsEtaZoomInMean->SetLineColor(2);
                  hHFTDcaZVsEtaZoomInSigma->SetLineColor(2);
                  //hHFTDcaZVsEtaZoomInMean->SetLineStyle(7);
                  hHFTDcaZVsEtaZoomInMean->Draw("same");
                  TLegend *leg = myLeg(0.3, 0.5, 0.55, 0.76, 42, 0.06);
                  leg->SetHeader("hHFTDcaVsEtaZoomIn fitSliceX Mean");
                  leg->AddEntry(hHFTDcaXyVsEtaZoomInMean, "DcaXy", "l");
                  leg->AddEntry(hHFTDcaZVsEtaZoomInMean, "DcaZ", "l");
                  leg->Draw("same");

                  //---------------
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);

                  hHFTDcaXyVsEtaZoomInSigma->Draw();
                  hHFTDcaXyVsEtaZoomInSigma->GetYaxis()->SetRangeUser(0.001, 0.02);
                  hHFTDcaZVsEtaZoomInSigma->Draw("same");
                  TLegend *leg2 = myLeg(0.3, 0.5, 0.55, 0.76, 42, 0.06);
                  leg2->SetHeader("hHFTDcaVsEtaZoomIn fitSlice sigma");
                  leg2->AddEntry(hHFTDcaXyVsEtaZoomInSigma, "DcaXy", "l");
                  leg2->AddEntry(hHFTDcaZVsEtaZoomInSigma, "DcaZ", "l");
                  leg2->Draw("same");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);

               }

               //HFT pT Eta eta Zdc results of FitSlicesX() //GNX
               else if (!tName.CompareTo(DcaXyZdcZoomInFitSlicesName))
               {
                  c1->cd();
                  pad->cd();
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 1);
                  t->Draw("colz");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);

                  c1->cd();
                  c1->Update();
                  //---------------
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);

                  //------------xxx
                  hHFTDcaXyVsZdcZoomIn->RebinX(50);
                  hHFTDcaXyVsZdcZoomIn->RebinY(50);
                  hHFTDcaZVsZdcZoomIn->RebinX(50);
                  hHFTDcaZVsZdcZoomIn->RebinY(50);

                  TF1 *fun = new TF1("fun", "gaus");
                  fun->SetRange(-0.03, 0.03);
                  hHFTDcaXyVsZdcZoomIn->FitSlicesX(fun, 0, -1, 4);
                  hHFTDcaZVsZdcZoomIn->FitSlicesX(fun, 0, -1, 4);
                  TH1F* hHFTDcaXyVsZdcZoomInMean = (TH1F*) gDirectory->Get("hHFTDcaXyVsZdcZoomIn_1");
                  TH1F* hHFTDcaXyVsZdcZoomInSigma = (TH1F*) gDirectory->Get("hHFTDcaXyVsZdcZoomIn_2");
                  hHFTDcaXyVsZdcZoomInMean->SetLineColor(1);
                  hHFTDcaXyVsZdcZoomInSigma->SetLineColor(1);
                  //hHFTDcaXyVsZdcZoomInMean->SetLineStyle(7);
                  hHFTDcaXyVsZdcZoomInMean->Draw();
                  hHFTDcaXyVsZdcZoomInMean->GetYaxis()->SetRangeUser(-0.002, 0.004);
                  TH1F* hHFTDcaZVsZdcZoomInMean = (TH1F*) gDirectory->Get("hHFTDcaZVsZdcZoomIn_1");
                  TH1F* hHFTDcaZVsZdcZoomInSigma = (TH1F*) gDirectory->Get("hHFTDcaZVsZdcZoomIn_2");
                  hHFTDcaZVsZdcZoomInMean->SetLineColor(2);
                  hHFTDcaZVsZdcZoomInSigma->SetLineColor(2);
                  //hHFTDcaZVsZdcZoomInMean->SetLineStyle(7);
                  hHFTDcaZVsZdcZoomInMean->Draw("same");
                  TLegend *leg = myLeg(0.3, 0.5, 0.55, 0.76, 42, 0.06);
                  leg->SetHeader("hHFTDcaVsZdcZoomIn fitSliceX Mean");
                  leg->AddEntry(hHFTDcaXyVsZdcZoomInMean, "DcaXy", "l");
                  leg->AddEntry(hHFTDcaZVsZdcZoomInMean, "DcaZ", "l");
                  leg->Draw("same");

                  //---------------
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);

                  hHFTDcaXyVsZdcZoomInSigma->Draw();
                  hHFTDcaXyVsZdcZoomInSigma->GetYaxis()->SetRangeUser(0.001, 0.02);
                  hHFTDcaZVsZdcZoomInSigma->Draw("same");
                  TLegend *leg2 = myLeg(0.3, 0.5, 0.55, 0.76, 42, 0.06);
                  leg2->SetHeader("hHFTDcaVsZdcZoomIn fitSlice sigma");
                  leg2->AddEntry(hHFTDcaXyVsZdcZoomInSigma, "DcaXy", "l");
                  leg2->AddEntry(hHFTDcaZVsZdcZoomInSigma, "DcaZ", "l");
                  leg2->Draw("same");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);

               }

               //TOF expected values:
               else if (!tName.CompareTo(betaPlotName))
               {
                  t->Draw("colz");
                  funBeta->SetNpx(10000);
                  funBeta->SetParameter(0, MELECTRON);
                  funBeta->DrawClone("same");
                  funBeta->SetParameter(0, MMUON);
                  funBeta->DrawClone("same");
                  funBeta->SetParameter(0, MPION);
                  funBeta->DrawClone("same");
                  funBeta->SetParameter(0, MKAON);
                  funBeta->DrawClone("same");
                  funBeta->SetParameter(0, MPROTON);
                  funBeta->Draw("same");

               }
               else if (!tName.CompareTo(massPlotName))
               {
                  t->Draw("colz");
                  funMass->SetNpx(10000);
                  funMass->SetParameter(0, MELECTRON);
                  funMass->DrawClone("same");
                  funMass->SetParameter(0, MMUON);
                  funMass->DrawClone("same");
                  funMass->SetParameter(0, MPION);
                  funMass->DrawClone("same");
                  funMass->SetParameter(0, MKAON);
                  funMass->DrawClone("same");
                  funMass->SetParameter(0, MPROTON);
                  funMass->Draw("same");

                  nFig++;
                  nFig++;//just to adjust
                  nCount2 = 0;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();
                  //-----
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 1, 0);
                  TH1F *hMass2 = (TH1F*)hMassvsp->ProjectionY("hMass2");
                  hMass2->Draw();
                  double massMax = hMass2->GetMaximum() / 1.2;
                  line.DrawLine(MELECTRON * MELECTRON, 0, MELECTRON * MELECTRON, massMax);
                  line.DrawLine(MMUON * MMUON, 0, MMUON * MMUON, massMax);
                  line.DrawLine(MKAON * MKAON, 0, MKAON * MKAON, massMax);
                  line.DrawLine(MPION * MPION, 0, MPION * MPION, massMax);
                  line.DrawLine(MPROTON * MPROTON, 0, MPROTON * MPROTON, massMax);
               }

               //BetaDiff vs. TrayId results of FitSlicesY()
               else if (!tName.CompareTo(fitslicesName))
               {
                  t->Draw("colz");
                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 0);

                  //------------xxx
                  hBetaDiffvsTrayIdProton->RebinX(10);
                  hBetaDiffvsTrayIdProton->RebinY(2);

                  TF1 *fun = new TF1("fun", "gaus");
                  fun->SetRange(-0.03, 0.03);
                  hBetaDiffvsTrayIdPi->FitSlicesY(fun);
                  fun->SetRange(-0.02, 0.03);
                  hBetaDiffvsTrayIdProton->FitSlicesY(fun);
                  TH1F* hBetaDiffvsTrayIdPiMean = (TH1F*) gDirectory->Get("hBetaDiffvsTrayIdPi_1");
                  TH1F* hBetaDiffvsTrayIdPiSigma = (TH1F*) gDirectory->Get("hBetaDiffvsTrayIdPi_2");
                  TH1F* hBetaDiffvsTrayIdProtonMean = (TH1F*) gDirectory->Get("hBetaDiffvsTrayIdProton_1");
                  TH1F* hBetaDiffvsTrayIdProtonSigma = (TH1F*) gDirectory->Get("hBetaDiffvsTrayIdProton_2");
                  hBetaDiffvsTrayIdPiMean->SetTitle("");
                  hBetaDiffvsTrayIdPiMean->SetLineColor(2);
                  hBetaDiffvsTrayIdPiMean->SetAxisRange(-0.01, 0.01, "y");
                  hBetaDiffvsTrayIdPiMean->Draw();
                  hBetaDiffvsTrayIdProtonMean->SetLineColor(1);
                  //hBetaDiffvsTrayIdProtonMean->SetLineStyle(7);
                  if (hBetaDiffvsTrayIdProton->GetEntries() > 120. / 10 * 1000) hBetaDiffvsTrayIdProtonMean->Draw("same");
                  //hBetaDiffvsTrayIdPbarMean->SetLineStyle(7);
                  TLegend *leg = myLeg(0.13, 0.7, 0.45, 0.96, 42, 0.06);
                  leg->SetHeader("Mean of 1/#beta - 1/#beta_expected");
                  leg->AddEntry(hBetaDiffvsTrayIdPiMean, "#pi", "l");
                  if (hBetaDiffvsTrayIdProton->GetEntries() > 120. / 10 * 1000) leg->AddEntry(hBetaDiffvsTrayIdProtonMean, "p", "l");
                  leg->Draw("same");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();

                  //---------------
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);

                  hBetaDiffvsTrayIdPiSigma->SetTitle("");
                  hBetaDiffvsTrayIdPiSigma->SetLineColor(2);
                  hBetaDiffvsTrayIdPiSigma->SetAxisRange(0.007, 0.016, "y");
                  hBetaDiffvsTrayIdPiSigma->Draw();
                  //hBetaDiffvsTrayIdProtonSigma->SetLineStyle(2);
                  if (hBetaDiffvsTrayIdProton->GetEntries() > 120. / 10 * 1000) hBetaDiffvsTrayIdProtonSigma->Draw("same");
                  //hBetaDiffvsTrayIdPbarSigma->SetLineStyle(2);
                  TLegend *leg2 = myLeg(0.13, 0.7, 0.45, 0.96, 42, 0.06);
                  leg2->SetHeader("#sigma of 1/#beta - 1/#beta_expected");
                  leg2->AddEntry(hBetaDiffvsTrayIdPiMean, "#pi", "l");
                  leg2->Draw("same");
                  nFig++;

                  pad->cd(2);
                  setpad(gPad, 0.1, 0.1, 0.02, 0.12, 0, 0);
                  hTofDiffvsTrayIdProton->RebinX(10);
                  hTofDiffvsTrayIdProton->RebinY(2);

                  fun->SetRange(-0.3, 0.3);
                  hTofDiffvsTrayIdPi->FitSlicesY(fun);
                  fun->SetRange(-0.2, 0.3);
                  hTofDiffvsTrayIdProton->FitSlicesY(fun);
                  TH1F* hTofDiffvsTrayIdPiMean = (TH1F*) gDirectory->Get("hTofDiffvsTrayIdPi_1");
                  TH1F* hTofDiffvsTrayIdPiSigma = (TH1F*) gDirectory->Get("hTofDiffvsTrayIdPi_2");
                  TH1F* hTofDiffvsTrayIdProtonMean = (TH1F*) gDirectory->Get("hTofDiffvsTrayIdProton_1");
                  TH1F* hTofDiffvsTrayIdProtonSigma = (TH1F*) gDirectory->Get("hTofDiffvsTrayIdProton_2");
                  hTofDiffvsTrayIdPiMean->SetTitle("");
                  hTofDiffvsTrayIdPiMean->SetLineColor(2);
                  hTofDiffvsTrayIdPiMean->SetAxisRange(-0.1, 0.1, "y");
                  hTofDiffvsTrayIdPiMean->Draw();
                  //hTofDiffvsTrayIdProtonMean->SetLineStyle(7);
                  if (hTofDiffvsTrayIdProton->GetEntries() > 120. / 10 * 1000) hTofDiffvsTrayIdProtonMean->Draw("same");
                  //hTofDiffvsTrayIdPbarMean->SetLineStyle(7);
                  leg->SetHeader("Mean of Tof - Tof_expected");
                  leg->Draw("same");
                  nFig++;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();
                  pdf->NewPage();

                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 0, 0);
                  hTofDiffvsTrayIdPiSigma->SetTitle("");
                  hTofDiffvsTrayIdPiSigma->SetLineColor(2);
                  hTofDiffvsTrayIdPiSigma->SetAxisRange(0.07, 0.15, "y");
                  hTofDiffvsTrayIdPiSigma->Draw();
                  //hTofDiffvsTrayIdProtonSigma->SetLineStyle(2);
                  if (hTofDiffvsTrayIdProton->GetEntries() > 120. / 10 * 1000) hTofDiffvsTrayIdProtonSigma->Draw("same");
                  //hTofDiffvsTrayIdPbarSigma->SetLineStyle(2);
                  leg2->SetHeader("#sigma of Tof - Tof_expected");
                  leg2->Draw("same");
                  nFig++;

               }
               else if (!tName.CompareTo(NumberOfGlobalsName))   //Added by xgn
               {
                  t->Draw("colz");

                  nFig++;
                  nCount2 = 0;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();
                  //-----
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 1, 0);
                  TH1F *NumberOfGlobals = (TH1F*)hRefVsGlobalRef->ProjectionY("NumberOfGlobals");
                  NumberOfGlobals->Draw();
               }
               else if (!tName.CompareTo(gRefMultName))   //Added by xgn
               {
                  t->Draw("colz");

                  nFig++;
                  nCount2 = 0;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();
                  //-----
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 1, 0);
                  TH1F *gRefMult = (TH1F*)hRefVsGRef->ProjectionY("gRefMult");
                  gRefMult->Draw();
                  gRefMult->GetYaxis()->SetTitle("counts");
               }
               else if (!tName.CompareTo(VzIdName))   //Added by xgn
               {
                  t->Draw("colz");

                  nFig++;
                  nCount2 = 0;
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();
                  //-----
                  pdf->NewPage();
                  pad->cd();
                  pad->Clear();
                  c1->cd();
                  pad->cd();
                  pad->Divide(1, 2, 0.001, 0.001);
                  pad->cd(1);
                  setpad(gPad, 0.1, 0.1, 0.05, 0.12, 1, 0);
                  TH1F *VzId = (TH1F*)hVzVsRunId->ProjectionY("VzId");
                  VzId->Draw();
                  VzId->GetYaxis()->SetTitle("counts");
               }


               else
               {
                  t->Draw("colz");
               }
               nFig++;
               nCount2++;
               if (nCount2 == 2)
               {
                  sprintf(tlName, "Page %i", nFig / 2 + 1);
                  tl.DrawLatex(0.02, 0.02, tlName);
                  c1->cd();
                  c1->Update();
                  //pdf->NewPage();
                  nCount2 = 0;
               }
            }
         }
      }
   }
   //-----------
   pdf->Close();
   f->Close();
   return;
}
void setpad(TVirtualPad *pad, float left, float right, float top, float bottom, int logy, int logz)
{
   pad->SetFillColor(10);
   pad->SetBorderMode(0);
   pad->SetBorderSize(0);
   pad->SetFrameFillColor(10);
   pad->SetFrameBorderMode(0);
   pad->SetFrameBorderSize(0);
   pad->SetLeftMargin(left);
   pad->SetRightMargin(right);
   pad->SetTopMargin(top);
   pad->SetBottomMargin(bottom);
   pad->SetLogy(logy);
   pad->SetLogz(logz);
   return;
}
TLegend* myLeg(Double_t xlow, Double_t ylow, Double_t xup, Double_t yup, Int_t textFont, Double_t textSize)
{
   TLegend *leg = new TLegend(xlow, ylow, xup, yup);
   leg->SetBorderSize(0);
   leg->SetFillStyle(0);
   leg->SetFillColor(10);
   leg->SetTextFont(textFont);
   leg->SetTextSize(textSize);
   return leg;
}

