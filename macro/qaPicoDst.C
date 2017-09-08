/*

   Written by Christopher B. Powell
   2010
   CBPowell@lbl.gov
   -------------------------------------------

   Macro to use StQaAnalysisMaker class, which
   uses PicoDst structure in StPicoDstMaker, and 
   histograms in StHistogramsQa to create 
   an output file of relevant histograms for QA.

*/

#include <TSystem>
#include "TStopwatch.h"

class StMaker;
class StChain;
class StPicoDstMaker;
class StQaAnalysisMaker;
class StHistogramsQa;


StChain *chain;
// void qaPicoDst(const Char_t *inputFile="test.list", 
void qaPicoDst(const Char_t *inputFile="/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/physics2/P16id/107/15107008/st_physics_15107008_raw_0000010.picoDst.root", 
	 char* outDir="./",
	 const Char_t *outputFile="test",
	 Int_t nEvents=1000000
	 )
		{

		TStopwatch*   stopWatch = new TStopwatch(); 
		stopWatch->Start();
		//Load all the System libraries

		gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
		loadSharedLibraries();

		gSystem->Load("libTable");
		gSystem->Load("libPhysics");
		gSystem->Load("St_base");
		gSystem->Load("StChain");
		gSystem->Load("St_Tables");
		gSystem->Load("StUtilities");        // new addition 22jul99
		gSystem->Load("StTreeMaker");
		gSystem->Load("StIOMaker");
		gSystem->Load("StarClassLibrary");
		gSystem->Load("StTriggerDataMaker"); // new starting from April 2003
		gSystem->Load("StBichsel");
		gSystem->Load("StEvent");
		gSystem->Load("StEventUtilities");
		gSystem->Load("StDbLib");
		gSystem->Load("StEmcUtil");
		gSystem->Load("StTofUtil");
		gSystem->Load("StPmdUtil");
		gSystem->Load("StPreEclMaker");
		gSystem->Load("StStrangeMuDstMaker");
		gSystem->Load("StMuDSTMaker");
		gSystem->Load("StMagF");

		gSystem->Load("StMtdHitMaker");
		gSystem->Load("StMtdUtil");
		gSystem->Load("StMtdMatchMaker");
		gSystem->Load("StBTofUtil");
		gSystem->Load("St_db_Maker");

		gSystem->Load("StPicoDstMaker");
		gSystem->Load("StQaAnalysisMaker");
		gSystem->Load("StHistogramsQa");
		//  gROOT->Macro("/star/u/cpowell/macros/setstyle.C");
		//  gROOT->Macro("/star/u/cpowell/macros/setcolz.C");

		chain = new StChain();

		StPicoDstMaker *picoMaker = new StPicoDstMaker(0,inputFile,"picoDst");

		StQaAnalysisMaker *anaMaker = new StQaAnalysisMaker("ana",picoMaker,outputFile);
		Char_t outFile[200];
		sprintf(outFile,"%s",outputFile);
		anaMaker->setOutputName(outDir,outFile);//sets mOutputName = <outDir>/PicoQA.AuAu200GeV.<outFile>.root

		chain->Init();
		cout<<"chain->Init();"<<endl;
		int total = picoMaker->chain()->GetEntries();
		cout << " Total entries = " << total << endl;
		if(nEvents>total) nEvents = total;
		for (Int_t i=0; i<nEvents; i++){
			if(i%100==0)
				cout << "Working on eventNumber " << i << endl;

			chain->Clear();
			int iret = chain->Make(i);

			if (iret) { cout << "Bad return code!" << iret << endl; break;}

			total++;	
		}

		cout << "****************************************** " << endl;
		cout << "Work done... now its time to close up shop!"<< endl;
		cout << "****************************************** " << endl;
		chain->Finish();
		cout << "****************************************** " << endl;
		cout << "total number of events  " << nEvents << endl;
		cout << "****************************************** " << endl;

		delete chain;

		stopWatch->Stop();  
		stopWatch->Print();
		}
