Double_t GenerateCorrMatr(const char* mcfile, const char* mctask, const char* idstring ,const char* outfile, const char* gifdir = 0)
//Double_t GenerateCorrMatr()
{

//tmp setting
//   const char* mcfile = "/lustre/alice/train/V005.PbPb_MC/2010-11-29_0108.4251/mergedPeriods/MC/PbPb/LHC10h8/mknichel_dNdPtPbPb_gt_v0_c0_syst4.root";
//     const char* mctask = "mknichel_dNdPtPbPb_gt_v0_c0_syst4";
//     const char* idstring = "gt_v0_c0_syst4";
//     const char* outfile = "/u/mknichel/alice/dNdPt/2010-11-29_0318/corrMatr_gt_v0_c0_syst4.root";
//     const char* gifdir = "/u/mknichel/alice/dNdPt/2010-11-29";
    
    // settings vor zVertex cut (event and track level)
    Double_t zVert = 10.0;
    
    // setting on eta cut (track level)
    Double_t eta = 0.8;
    
    // strangeness scaling factor (for secondaries from strange decays)
    Double_t sscale = 1.4;
    
    //load required libraries
    //load required libraries    
    gSystem->AddIncludePath("-I$ROOTSYS/include -I$ALICE_ROOT/ -I$ALICE_ROOT/include -I$ALICE_ROOT/STEER  -I$ALICE_ROOT/ANALYSIS -I$ALICE_ROOT/PWG0 -I$ALICE_ROOT/PWGPP -I$ALICE_ROOT/PWG2 -I$ALICE_ROOT/PWG3 -I$ALICE_ROOT/PWG3/vertexingHF -I$ALICE_ROOT/PWG4 -I$ALICE_ROOT/CORRFW -I$ALICE_ROOT/TPC -I$ALICE_ROOT/TRD -I$ALICE_ROOT/PWG3/muon -I$ALICE_ROOT/JETAN -I$ALICE_ROOT/TENDER/Tender");
    
  gSystem->Load("libCore");
  gSystem->Load("libPhysics");
  gSystem->Load("libMinuit");
  gSystem->Load("libGui");
  gSystem->Load("libXMLParser");

  gSystem->Load("libGeom");
  gSystem->Load("libVMC");

  gSystem->Load("libNet");


  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libCDB");
  gSystem->Load("libRAWDatabase");
  gSystem->Load("libRAWDatarec");
  gSystem->Load("libANALYSIS");

    
    
    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISalice");
    gSystem->Load("libTender");
    gSystem->Load("libCORRFW");
    gSystem->Load("libPWG0base");
    gSystem->Load("libPWG0dep"); 
    gSystem->Load("libPWG0selectors");
    




    // make plots nicer
    gROOT->SetStyle("Plain");
    gStyle->SetPalette(1);
    
    // array for all correction matrices
    TObjArray* CorrMatr = new TObjArray();
    
    // array for all control histograms
    TObjArray* ContHist = new TObjArray();
    

    // load mc information
    TFile* fmc = TFile::Open(mcfile,"READ");
    if (!fmc) return -1;
    TList* lmc = dynamic_cast<TList*>(fmc->Get(mctask));
    if (!lmc) return -1;
    AlidNdPtAnalysisPbPb *obj = dynamic_cast<AlidNdPtAnalysisPbPb*>(lmc->FindObject("dNdPtAnalysisPbPb"));
    if (!obj) return -1;

    //Event statistics
    THnSparse *fRecEventMatrix = obj->GetRecEventMatrix(); //all reconstructed events	
    TH2D* h2RecEventAll = fRecEventMatrix->Projection(0,1)->Clone("h2RecEventAll");
    fRecEventMatrix->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    TH2D* h2RecEvent = fRecEventMatrix->Projection(0,1)->Clone("h2RecEvent");
    Double_t MCReconstructedEvents = h2RecEvent->Integral();
    Double_t MCReconstructedEventsAll = h2RecEventAll->Integral();
    ContHist->Add(h2RecEvent);
    ContHist->Add(h2RecEventAll);
        
    THnSparse *fTriggerEventMatrix = obj->GetTriggerEventMatrix(); //all triggered events
    TH2D* h2TriggerEventAll = fTriggerEventMatrix->Projection(0,1)->Clone("h2TriggerEventAll");
    fTriggerEventMatrix->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    TH2D* h2TriggerEvent = fTriggerEventMatrix->Projection(0,1)->Clone("h2TriggerEvent");
    Double_t MCTriggeredEvents = h2TriggerEvent->Integral();
    Double_t MCTriggeredEventsAll = h2TriggerEventAll->Integral();
    ContHist->Add(h2TriggerEvent);
    ContHist->Add(h2TriggerEventAll);    
        
    THnSparse *fGenEventMatrix = obj->GetGenEventMatrix(); //all generated events
    TH2D* h2GenEventAll = fGenEventMatrix->Projection(0,1)->Clone("h2GenEventAll");
    fGenEventMatrix->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    TH2D* h2GenEvent = fGenEventMatrix->Projection(0,1)->Clone("h2GenEvent");
    Double_t MCGeneratedEvents = h2GenEvent->Integral();
    Double_t MCGeneratedEventsAll = h2GenEventAll->Integral(); 
    ContHist->Add(h2RecEvent);
    ContHist->Add(h2RecEventAll);
	
    printf("=== generated MC events for correction matrices    %lf ===\n",MCGeneratedEvents);
    printf("=== triggered MC events for correction matrices    %lf ===\n",MCTriggeredEvents);
    printf("=== recontructed MC events for correction matrices %lf ===\n",MCReconstructedEvents);
    printf("\n");
    printf("=== cut on the zVertex +- %lf ===\n",zVert);
    printf("=== generated MC events (before zVertex cut)       %lf ===\n",MCGeneratedEventsAll);
    printf("=== triggered MC events (before zVertex cut)       %lf ===\n",MCTriggeredEventsAll);
    printf("=== recontructed MC events (before zVertex cut)    %lf ===\n",MCReconstructedEventsAll);
    
    TH1D* h1MCGeneratedEvents = new TH1D("h1MCGeneratedEvents","h1MCGeneratedEvents",1,0,1);
    TH1D* h1MCTriggeredEvents = new TH1D("h1MCTriggeredEvents","h1MCTriggeredEvents",1,0,1);
    TH1D* h1MCReconstructedEvents = new TH1D("h1MCReconstructedEvents","h1MCReconstructedEvents",1,0,1);
    TH1D* h1MCGeneratedEventsAll = new TH1D("h1MCGeneratedEventsAll","h1MCGeneratedEventsAll",1,0,1);
    TH1D* h1MCTriggeredEventsAll = new TH1D("h1MCTriggeredEventsAll","h1MCTriggeredEventsAll",1,0,1);
    TH1D* h1MCReconstructedEventsAll = new TH1D("h1MCReconstructedEventsAll","h1MCReconstructedEventsAll",1,0,1);
    
    h1MCGeneratedEvents->Fill(0,MCGeneratedEvents);
    h1MCGeneratedEvents->SetEntries(MCGeneratedEvents);
    h1MCTriggeredEvents->Fill(0,MCTriggeredEvents);
    h1MCTriggeredEvents->SetEntries(MCTriggeredEvents);
    h1MCReconstructedEvents->Fill(0,MCReconstructedEvents);
    h1MCReconstructedEvents->SetEntries(MCReconstructedEvents);
    h1MCGeneratedEventsAll->Fill(0,MCGeneratedEventsAll);
    h1MCGeneratedEventsAll->SetEntries(MCGeneratedEventsAll);
    h1MCTriggeredEventsAll->Fill(0,MCTriggeredEventsAll);
    h1MCTriggeredEventsAll->SetEntries(MCTriggeredEventsAll);
    h1MCReconstructedEventsAll->Fill(0,MCReconstructedEventsAll);
    h1MCReconstructedEventsAll->SetEntries(MCReconstructedEventsAll);
    
    ContHist->Add(h1MCGeneratedEvents);
    ContHist->Add(h1MCTriggeredEvents);
    ContHist->Add(h1MCReconstructedEvents);
    ContHist->Add(h1MCGeneratedEventsAll);
    ContHist->Add(h1MCTriggeredEventsAll);
    ContHist->Add(h1MCReconstructedEventsAll);

    // efficienfy and correction matrices for tigger and vertex efficiency
    TH2D* h2EventTriggerEffAll  = AlidNdPtHelper::GenerateCorrMatrix(h2TriggerEventAll,h2GenEventAll,"h2EventTriggerEffAll");
    TH2D* h2EventTriggerCorrAll = AlidNdPtHelper::GenerateCorrMatrix(h2GenEventAll,h2TriggerEventAll,"h2EventTriggerCorrAll"); 
    TH2D* h2EventTriggerEff  = AlidNdPtHelper::GenerateCorrMatrix(h2TriggerEvent,h2GenEvent,"h2EventTriggerEff");
    TH2D* h2EventTriggerCorr = AlidNdPtHelper::GenerateCorrMatrix(h2GenEvent,h2TriggerEvent,"h2EventTriggerCorr"); 

    TH2D* h2EventRecEffAll  = AlidNdPtHelper::GenerateCorrMatrix(h2RecEventAll,h2TriggerEventAll,"h2EventRecEffAll");
    TH2D* h2EventRecCorrAll = AlidNdPtHelper::GenerateCorrMatrix(h2TriggerEventAll,h2RecEventAll,"h2EventRecCorrAll");
    TH2D* h2EventRecEff  = AlidNdPtHelper::GenerateCorrMatrix(h2RecEvent,h2TriggerEvent,"h2EventRecEff");
    TH2D* h2EventRecCorr = AlidNdPtHelper::GenerateCorrMatrix(h2TriggerEvent,h2RecEvent,"h2EventRecCorr");

    TH2D* h2EventEffAll  = AlidNdPtHelper::GenerateCorrMatrix(h2RecEventAll,h2GenEventAll,"h2EventEffAll");
    TH2D* h2EventCorrAll = AlidNdPtHelper::GenerateCorrMatrix(h2GenEventAll,h2RecEventAll,"h2EventCorrAll");
    TH2D* h2EventEff  = AlidNdPtHelper::GenerateCorrMatrix(h2RecEvent,h2GenEvent,"h2EventEff");
    TH2D* h2EventCorr = AlidNdPtHelper::GenerateCorrMatrix(h2GenEvent,h2RecEvent,"h2EventCorr");

    CorrMatr->Add(h2EventTriggerEffAll);
    CorrMatr->Add(h2EventTriggerCorrAll);
    CorrMatr->Add(h2EventTriggerEff);
    CorrMatr->Add(h2EventTriggerCorr);
    CorrMatr->Add(h2EventRecEffAll);
    CorrMatr->Add(h2EventRecCorrAll);
    CorrMatr->Add(h2EventRecEff);
    CorrMatr->Add(h2EventRecCorr);
    CorrMatr->Add(h2EventEffAll);
    CorrMatr->Add(h2EventCorrAll);
    CorrMatr->Add(h2EventEff);
    CorrMatr->Add(h2EventCorr);



    // all recontructed
    THnSparse *fRecTrackMatrix = obj->GetRecTrackMatrix();
    fRecTrackMatrix->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    fRecTrackMatrix->GetAxis(2)->SetRangeUser(-eta, eta-0.01);//eta
    TH3D* h3RecTrack = fRecTrackMatrix->Projection(0,1,2)->Clone("h3RecTrack");
    TH2D* h2RecTrack_zv_pt  = fRecTrackMatrix->Projection(0,1)->Clone("h2RecTrack_zv_pt");
    TH2D* h2RecTrack_zv_eta = fRecTrackMatrix->Projection(0,2)->Clone("h2RecTrack_zv_eta");
    TH2D* h2RecTrack_pt_eta = fRecTrackMatrix->Projection(1,2)->Clone("h2RecTrack_pt_eta");
    TH1D* h1RecTrack_zv  = fRecTrackMatrix->Projection(0)->Clone("h1RecTrack_zv");
    TH1D* h1RecTrack_pt  = fRecTrackMatrix->Projection(1)->Clone("h1RecTrack_pt");
    TH1D* h1RecTrack_eta = fRecTrackMatrix->Projection(2)->Clone("h1RecTrack_eta");
    Double_t MCReconstructedTracks = h3RecTrack->Integral();

    ContHist->Add(h3RecTrack);
    ContHist->Add(h2RecTrack_zv_pt);
    ContHist->Add(h2RecTrack_zv_eta);
    ContHist->Add(h2RecTrack_pt_eta);
    ContHist->Add(h1RecTrack_zv);
    ContHist->Add(h1RecTrack_pt);
    ContHist->Add(h1RecTrack_eta);

     // recontructed primary tracks
    THnSparse *fRecPrimTrackMatrix = obj->GetRecPrimTrackMatrix();
    THnSparse *fRecTrackMatrixScaled = fRecPrimTrackMatrix->Clone("fRecTrackMatrixScaled"); //used later for secondaries scaling
    fRecPrimTrackMatrix->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    fRecPrimTrackMatrix->GetAxis(2)->SetRangeUser(-eta, eta-0.01);//eta
    TH3D* h3RecPrimTrack = fRecPrimTrackMatrix->Projection(0,1,2)->Clone("h3RecPrimTrack");
    TH2D* h2RecPrimTrack_zv_pt  = fRecPrimTrackMatrix->Projection(0,1)->Clone("h2RecPrimTrack_zv_pt");
    TH2D* h2RecPrimTrack_zv_eta = fRecPrimTrackMatrix->Projection(0,2)->Clone("h2RecPrimTrack_zv_eta");
    TH2D* h2RecPrimTrack_pt_eta = fRecPrimTrackMatrix->Projection(1,2)->Clone("h2RecPrimTrack_pt_eta");
    TH1D* h1RecPrimTrack_zv  = fRecPrimTrackMatrix->Projection(0)->Clone("h1RecPrimTrack_zv");
    TH1D* h1RecPrimTrack_pt  = fRecPrimTrackMatrix->Projection(1)->Clone("h1RecPrimTrack_pt");
    TH1D* h1RecPrimTrack_eta = fRecPrimTrackMatrix->Projection(2)->Clone("h1RecPrimTrack_eta");
    Double_t MCReconstructedPrimTracks = h3RecPrimTrack->Integral();

    ContHist->Add(h3RecPrimTrack);
    ContHist->Add(h2RecPrimTrack_zv_pt);
    ContHist->Add(h2RecPrimTrack_zv_eta);
    ContHist->Add(h2RecPrimTrack_pt_eta);
    ContHist->Add(h1RecPrimTrack_zv);
    ContHist->Add(h1RecPrimTrack_pt);
    ContHist->Add(h1RecPrimTrack_eta);
    
     // recontructed secondary tracks
    THnSparse *fRecSecTrackMatrix = obj->GetRecSecTrackMatrix();
    THnSparse *fRecSecTrackMatrixScaled = fRecSecTrackMatrix->Clone("fRecSecTrackMatrixScaled"); //used later for secondaries scaling
    fRecSecTrackMatrix->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    fRecSecTrackMatrix->GetAxis(2)->SetRangeUser(-eta, eta-0.01);//eta
    TH3D* h3RecSecTrack = fRecSecTrackMatrix->Projection(0,1,2)->Clone("h3RecSecTrack");
    TH2D* h2RecSecTrack_zv_pt  = fRecSecTrackMatrix->Projection(0,1)->Clone("h2RecSecTrack_zv_pt");
    TH2D* h2RecSecTrack_zv_eta = fRecSecTrackMatrix->Projection(0,2)->Clone("h2RecSecTrack_zv_eta");
    TH2D* h2RecSecTrack_pt_eta = fRecSecTrackMatrix->Projection(1,2)->Clone("h2RecSecTrack_pt_eta");
    TH1D* h1RecSecTrack_zv  = fRecSecTrackMatrix->Projection(0)->Clone("h1RecSecTrack_zv");
    TH1D* h1RecSecTrack_pt  = fRecSecTrackMatrix->Projection(1)->Clone("h1RecSecTrack_pt");
    TH1D* h1RecSecTrack_eta = fRecSecTrackMatrix->Projection(2)->Clone("h1RecSecTrack_eta");
    Double_t MCReconstructedSecTracks = h3RecSecTrack->Integral();

    ContHist->Add(h3RecSecTrack);
    ContHist->Add(h2RecSecTrack_zv_pt);
    ContHist->Add(h2RecSecTrack_zv_eta);
    ContHist->Add(h2RecSecTrack_pt_eta);
    ContHist->Add(h1RecSecTrack_zv);
    ContHist->Add(h1RecSecTrack_pt);
    ContHist->Add(h1RecSecTrack_eta);
    
    // generated primary tracks
    THnSparse *fGenPrimTrackMatrix = obj->GetGenPrimTrackMatrix();
    fGenPrimTrackMatrix->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    fGenPrimTrackMatrix->GetAxis(2)->SetRangeUser(-eta, eta-0.01);//eta
    TH3D* h3GenPrimTrack = fGenPrimTrackMatrix->Projection(0,1,2)->Clone("h3GenPrimTrack");
    TH2D* h2GenPrimTrack_zv_Pt  = fGenPrimTrackMatrix->Projection(0,1)->Clone("h2GenPrimTrack_zv_pt");
    TH2D* h2GenPrimTrack_zv_eta = fGenPrimTrackMatrix->Projection(0,2)->Clone("h2GenPrimTrack_zv_eta");
    TH2D* h2GenPrimTrack_pt_eta = fGenPrimTrackMatrix->Projection(1,2)->Clone("h2GenPrimTrack_pt_eta");
    TH1D* h1GenPrimTrack_zv  = fGenPrimTrackMatrix->Projection(0)->Clone("h1GenPrimTrack_zv");
    TH1D* h1GenPrimTrack_pt  = fGenPrimTrackMatrix->Projection(1)->Clone("h1GenPrimTrack_pt");
    TH1D* h1GenPrimTrack_eta = fGenPrimTrackMatrix->Projection(2)->Clone("h1GenPrimTrack_eta");
    Double_t MCGeneratedPrimTracks = h3GenPrimTrack->Integral();

    ContHist->Add(h3GenPrimTrack);
    ContHist->Add(h2GenPrimTrack_zv_pt);
    ContHist->Add(h2GenPrimTrack_zv_eta);
    ContHist->Add(h2GenPrimTrack_pt_eta);
    ContHist->Add(h1GenPrimTrack_zv);
    ContHist->Add(h1GenPrimTrack_pt);
    ContHist->Add(h1GenPrimTrack_eta);
    printf("\n");
    printf("==============================================================\n");    
    printf("=== recontructed MC tracks              %lf ===\n",MCReconstructedTracks);
    printf("=== recontructed MC secondary tracks    %lf ===\n",MCReconstructedSecTracks);
    printf("=== recontructed MC primary tracks      %lf ===\n",MCReconstructedPrimTracks);
    printf("=== generated MC primary track          %lf ===\n",MCGeneratedPrimTracks);
    printf("==============================================================\n");    
    printf("\n");
	
//    THnSparse *fSparseTriggerTrackEvent = obj->GetTriggerTrackEventMatrix();//Tracks from triggered events
//    THnSparse *fSparseVtxTrackEvent = obj->GetRecTrackEventMatrix();//Tracks from events with rec. vtx
//    THnSparse *fSparseGenTrackEvent = obj->GetGenTrackEventMatrix();//generated TrackEvent matrix

    // tracking efficiencies + corrections
   TH3D* h3TrackEff  = AlidNdPtHelper::GenerateCorrMatrix(h3RecPrimTrack,h3GenPrimTrack,"h3TrackEff");
   TH3D* h3TrackCorr = AlidNdPtHelper::GenerateCorrMatrix(h3GenPrimTrack,h3RecPrimTrack,"h3TrackCorr");
   
   TH2D* h2TrackEff_zv_pt   = AlidNdPtHelper::GenerateCorrMatrix(h2RecPrimTrack_zv_pt,h2GenPrimTrack_zv_pt,"h2TrackEff_zv_pt");
   TH2D* h2TrackCorr_zv_pt  = AlidNdPtHelper::GenerateCorrMatrix(h2GenPrimTrack_zv_pt,h2RecPrimTrack_zv_pt,"h2TrackCorr_zv_pt");
   TH2D* h2TrackEff_zv_eta  = AlidNdPtHelper::GenerateCorrMatrix(h2RecPrimTrack_zv_eta,h2GenPrimTrack_zv_eta,"h2TrackEff_zv_eta");
   TH2D* h2TrackCorr_zv_eta = AlidNdPtHelper::GenerateCorrMatrix(h2GenPrimTrack_zv_eta,h2RecPrimTrack_zv_eta,"h2TrackCorr_zv_eta");
   TH2D* h2TrackEff_pt_eta  = AlidNdPtHelper::GenerateCorrMatrix(h2RecPrimTrack_pt_eta,h2GenPrimTrack_pt_eta,"h2TrackEff_pt_eta");
   TH2D* h2TrackCorr_pt_eta = AlidNdPtHelper::GenerateCorrMatrix(h2GenPrimTrack_pt_eta,h2RecPrimTrack_pt_eta,"h2TrackCorr_pt_eta");
    
   TH1D* h1TrackEff_zv   = AlidNdPtHelper::GenerateCorrMatrix(h1RecPrimTrack_zv,h1GenPrimTrack_zv,"h1TrackEff_zv");
   TH1D* h1TrackCorr_zv  = AlidNdPtHelper::GenerateCorrMatrix(h1GenPrimTrack_zv,h1RecPrimTrack_zv,"h1TrackCorr_zv");
   TH1D* h1TrackEff_pt   = AlidNdPtHelper::GenerateCorrMatrix(h1RecPrimTrack_pt,h1GenPrimTrack_pt,"h1TrackEff_pt");
   TH1D* h1TrackCorr_pt  = AlidNdPtHelper::GenerateCorrMatrix(h1GenPrimTrack_pt,h1RecPrimTrack_pt,"h1TrackCorr_pt");
   TH1D* h1TrackEff_eta  = AlidNdPtHelper::GenerateCorrMatrix(h1RecPrimTrack_eta,h1GenPrimTrack_eta,"h1TrackEff_eta");
   TH1D* h1TrackCorr_eta = AlidNdPtHelper::GenerateCorrMatrix(h1GenPrimTrack_eta,h1RecPrimTrack_eta,"h1TrackCorr_eta");
   
   CorrMatr->Add(h3TrackEff);
   CorrMatr->Add(h3TrackCorr);
   CorrMatr->Add(h2TrackEff_zv_pt);
   CorrMatr->Add(h2TrackCorr_zv_pt);
   CorrMatr->Add(h2TrackEff_zv_eta);
   CorrMatr->Add(h2TrackCorr_zv_eta);
   CorrMatr->Add(h2TrackEff_pt_eta);
   CorrMatr->Add(h2TrackCorr_pt_eta);
   CorrMatr->Add(h1TrackEff_zv);
   CorrMatr->Add(h1TrackCorr_zv);
   CorrMatr->Add(h1TrackEff_pt);
   CorrMatr->Add(h1TrackCorr_pt);
   CorrMatr->Add(h1TrackEff_eta);
   CorrMatr->Add(h1TrackCorr_eta);

   // scale the secondaries before calculating correction matrices
   for (Long64_t i = 0; i < fRecSecTrackMatrixScaled->GetNbins(); i++) {
       Int_t c[3];
       Double_t val = fRecSecTrackMatrixScaled->GetBinContent(i,c);
       Double_t err = fRecSecTrackMatrixScaled->GetBinError(i);
       Double_t pt =  fRecSecTrackMatrixScaled->GetAxis(1)->GetBinCenter(c[1]);
       Double_t scale = GetStrangenessCorrFactorPbPb(pt, sscale);
       fRecSecTrackMatrixScaled->SetBinContent(c,val*scale);
       fRecSecTrackMatrixScaled->SetBinError(c,err*scale);
    }
    
    // for correct determination of secondaries contamination, also the total total tracks have to be scaled
    // this is done by taking primaries and adding the scaled secondaries
    fRecTrackMatrixScaled->Add(fRecSecTrackMatrixScaled);

    fRecSecTrackMatrixScaled->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    fRecSecTrackMatrixScaled->GetAxis(2)->SetRangeUser(-eta, eta-0.01);//eta
    
    TH3D* h3RecSecTrackScaled = fRecSecTrackMatrixScaled->Projection(0,1,2)->Clone("h3RecSecTrackScaled");
    TH2D* h2RecSecTrackScaled_zv_pt  = fRecSecTrackMatrixScaled->Projection(0,1)->Clone("h2RecSecTrackScaled_zv_pt");
    TH2D* h2RecSecTrackScaled_zv_eta = fRecSecTrackMatrixScaled->Projection(0,2)->Clone("h2RecSecTrackScaled_zv_eta");
    TH2D* h2RecSecTrackScaled_pt_eta = fRecSecTrackMatrixScaled->Projection(1,2)->Clone("h2RecSecTrackScaled_pt_eta");
    TH1D* h1RecSecTrackScaled_zv  = fRecSecTrackMatrixScaled->Projection(0)->Clone("h1RecSecTrackScaled_zv");
    TH1D* h1RecSecTrackScaled_pt  = fRecSecTrackMatrixScaled->Projection(1)->Clone("h1RecSecTrackScaled_pt");
    TH1D* h1RecSecTrackScaled_eta = fRecSecTrackMatrixScaled->Projection(2)->Clone("h1RecSecTrackScaled_eta");

    ContHist->Add(h3RecSecTrackScaled);
    ContHist->Add(h2RecSecTrackScaled_zv_pt);
    ContHist->Add(h2RecSecTrackScaled_zv_eta);
    ContHist->Add(h2RecSecTrackScaled_pt_eta);
    ContHist->Add(h1RecSecTrackScaled_zv);
    ContHist->Add(h1RecSecTrackScaled_pt);
    ContHist->Add(h1RecSecTrackScaled_eta);
    
    fRecTrackMatrixScaled->GetAxis(0)->SetRangeUser(-zVert, zVert-0.01);//zVer
    fRecTrackMatrixScaled->GetAxis(2)->SetRangeUser(-eta, eta-0.01);//eta
    
    TH3D* h3RecTrackScaled = fRecTrackMatrixScaled->Projection(0,1,2)->Clone("h3RecTrackScaled");
    TH2D* h2RecTrackScaled_zv_pt  = fRecTrackMatrixScaled->Projection(0,1)->Clone("h2RecTrackScaled_zv_pt");
    TH2D* h2RecTrackScaled_zv_eta = fRecTrackMatrixScaled->Projection(0,2)->Clone("h2RecTrackScaled_zv_eta");
    TH2D* h2RecTrackScaled_pt_eta = fRecTrackMatrixScaled->Projection(1,2)->Clone("h2RecTrackScaled_pt_eta");
    TH1D* h1RecTrackScaled_zv  = fRecTrackMatrixScaled->Projection(0)->Clone("h1RecTrackScaled_zv");
    TH1D* h1RecTrackScaled_pt  = fRecTrackMatrixScaled->Projection(1)->Clone("h1RecTrackScaled_pt");
    TH1D* h1RecTrackScaled_eta = fRecTrackMatrixScaled->Projection(2)->Clone("h1RecTrackScaled_eta");

    ContHist->Add(h3RecTrackScaled);
    ContHist->Add(h2RecTrackScaled_zv_pt);
    ContHist->Add(h2RecTrackScaled_zv_eta);
    ContHist->Add(h2RecTrackScaled_pt_eta);
    ContHist->Add(h1RecTrackScaled_zv);
    ContHist->Add(h1RecTrackScaled_pt);
    ContHist->Add(h1RecTrackScaled_eta);
    
    // create histograms for secondaries contamination and correction
    
    TH3D* h3SecCont = AlidNdPtHelper::GenerateCorrMatrix(h3RecSecTrackScaled,h3RecTrackScaled,"h3SecCont");
    TH3D* h3SecCorr = AlidNdPtHelper::GenerateContCorrMatrix(h3RecSecTrackScaled,h3RecTrackScaled,"h3SecCorr");
    TH2D* h2SecCont_zv_pt  = AlidNdPtHelper::GenerateCorrMatrix(h2RecSecTrackScaled_zv_pt,h2RecTrackScaled_zv_pt,"h2SecCont_zv_pt");
    TH2D* h2SecCorr_zv_pt  = AlidNdPtHelper::GenerateContCorrMatrix(h2RecSecTrackScaled_zv_pt,h2RecTrackScaled_zv_pt,"h2SecCorr_zv_pt");
    TH2D* h2SecCont_zv_eta = AlidNdPtHelper::GenerateCorrMatrix(h2RecSecTrackScaled_zv_eta,h2RecTrackScaled_zv_eta,"h2SecCont_zv_eta");
    TH2D* h2SecCorr_zv_eta = AlidNdPtHelper::GenerateContCorrMatrix(h2RecSecTrackScaled_zv_eta,h2RecTrackScaled_zv_eta,"h2SecCorr_zv_eta");
    TH2D* h2SecCont_pt_eta = AlidNdPtHelper::GenerateCorrMatrix(h2RecSecTrackScaled_pt_eta,h2RecTrackScaled_pt_eta,"h2SecCont_pt_eta");
    TH2D* h2SecCorr_pt_eta = AlidNdPtHelper::GenerateContCorrMatrix(h2RecSecTrackScaled_pt_eta,h2RecTrackScaled_pt_eta,"h2SecCorr_pt_eta");
    TH1D* h1SecCont_zv = AlidNdPtHelper::GenerateCorrMatrix(h1RecSecTrackScaled_zv,h1RecTrackScaled_zv,"h1SecCont_zv");
    TH1D* h1SecCorr_zv = AlidNdPtHelper::GenerateContCorrMatrix(h1RecSecTrackScaled_zv,h1RecTrackScaled_zv,"h1SecCorr_zv");
    TH1D* h1SecCont_pt = AlidNdPtHelper::GenerateCorrMatrix(h1RecSecTrackScaled_pt,h1RecTrackScaled_pt,"h1SecCont_pt");
    TH1D* h1SecCorr_pt = AlidNdPtHelper::GenerateContCorrMatrix(h1RecSecTrackScaled_pt,h1RecTrackScaled_pt,"h1SecCorr_pt");
    TH1D* h1SecCont_eta = AlidNdPtHelper::GenerateCorrMatrix(h1RecSecTrackScaled_eta,h1RecTrackScaled_eta,"h1SecCont_eta");
    TH1D* h1SecCorr_eta = AlidNdPtHelper::GenerateContCorrMatrix(h1RecSecTrackScaled_eta,h1RecTrackScaled_eta,"h1SecCorr_eta");

    CorrMatr->Add(h3SecCont);
    CorrMatr->Add(h3SecCorr);
    CorrMatr->Add(h2SecCont_zv_pt);
    CorrMatr->Add(h2SecCorr_zv_pt);
    CorrMatr->Add(h2SecCont_zv_eta);
    CorrMatr->Add(h2SecCorr_zv_eta);
    CorrMatr->Add(h2SecCont_pt_eta);
    CorrMatr->Add(h2SecCorr_pt_eta);
    CorrMatr->Add(h1SecCont_zv);
    CorrMatr->Add(h1SecCorr_zv);
    CorrMatr->Add(h1SecCont_pt);
    CorrMatr->Add(h1SecCorr_pt);
    CorrMatr->Add(h1SecCont_eta);
    CorrMatr->Add(h1SecCorr_eta);

    // plot pictures and save to gifdir
    for (i=0; i < CorrMatr->LastIndex(); i++) {    
        TCanvas* ctmp = PlotHist(CorrMatr->At(i),idstring);
        if (gifdir && ctmp) {
            TString gif(gifdir);
            gif += '/';
            gif += ctmp->GetName();
            gif += ".gif";
            ctmp->SaveAs(gif.Data(),"gif");     
            delete ctmp;
        }
    }
    for (i=0; i < ContHist->LastIndex(); i++) {    
        TCanvas* ctmp = PlotHist(ContHist->At(i),idstring);
        if (gifdir && ctmp) {
            TString gif(gifdir);
            gif += '/';
            gif += ctmp->GetName();
            gif += ".gif";
            ctmp->SaveAs(gif.Data(),"gif");     
            delete ctmp;
        }
   }    

    // save all correction matrices and control histograms to file
    if (!outfile) { return; }
    TFile *out = TFile::Open(outfile,"RECREATE");
    CorrMatr->Write();
    ContHist->Write();
    out->Close();
    
    return MCReconstructedEvents;

}


//_____________________________________________________________________________
Double_t GetStrangenessCorrFactorPbPb(Double_t pt, Double_t s)
{
    // data driven correction factor for secondaries (PbPb)

    if (pt <= 0.25) return 1.0;
    if (pt <= 0.5) return GetLinearInterpolationValue(0.25,1.0,0.5,0.60+0.40*s, pt);
    if (pt <= 1.0) return GetLinearInterpolationValue(0.5,0.60+0.40*s,1.0,0.53+0.47*s, pt);
    if (pt <= 2.0) return GetLinearInterpolationValue(1.0,0.53+0.47*s,2.0,0.44+0.56*s, pt);
    if (pt <= 5.0) return GetLinearInterpolationValue(2.0,0.44+0.56*s,5.0,0.33+0.67*s, pt);
    return 0.33+0.67*s;
}


//___________________________________________________________________________
Double_t GetLinearInterpolationValue(const Double_t x1,const  Double_t y1,const  Double_t x2,const  Double_t y2, const Double_t pt)
{
    //
    // linear interpolation
    //
    return ((y2-y1)/(x2-x1))*pt+(y2-(((y2-y1)/(x2-x1))*x2)); 
}

//___________________________________________________________________________
TCanvas* PlotHist(TObject* hobj, const char* label=0)
{
    TH1* h = dynamic_cast<TH1*>(hobj);
    if (!h) return 0;
    if (h->GetDimension() > 2) return 0;
    h->SetStats(0);
    if ( TString(h->GetName()).Contains("Events")) { h->SetStats(1); } 
    TString t(label);
    if (label) t += "_";
    t += h->GetName();
    h->SetTitle(t.Data());
    TCanvas* c = new TCanvas(t.Data(),t.Data());
    if (h->GetDimension() >= 1) {
        TString xlabel(h->GetXaxis()->GetTitle());
        if (xlabel.Contains("Pt")) { c->SetLogx();  h->GetXaxis()->SetRangeUser(0.1 , 50.); }
    }
    if (h->GetDimension() == 1) {
        if (xlabel.Contains("p_{T}")) { c->SetLogx();  c->SetLogy();  h->GetXaxis()->SetRangeUser(0.1 , 50.); }
    }
    if (h->GetDimension() == 2) {  
        TString ylabel(h->GetYaxis()->GetTitle());
        if (ylabel.Contains("Pt")) { c->SetLogy(); h->GetYaxis()->SetRangeUser(0.1 , 50.); }
        if (ylabel.Contains("p_{T}")) { c->SetLogy(); h->GetYaxis()->SetRangeUser(0.1 , 50.); }
        h->Draw("COLZ");
    }        
    if (h->GetDimension() == 1) {
        h->Draw();
    }
    return c;

}

Int_t CheckLoadLibrary(const char* library)
{
  // checks if a library is already loaded, if not loads the library

  if (strlen(gSystem->GetLibraries(library, "", kFALSE)) > 0)
    return 1;

  return gSystem->Load(library);
}
