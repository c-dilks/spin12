// shows the dependence of various kinematics on geometry
// e.g., pt vs. eta
// --> outputs all plots to diag.pdf

void Diagnostics()
{
  gSystem->Load("src/RunData12.so");
  RunData12 * RD = new RunData12();
  const Int_t NBINS=500; // NUMBER OF BINS

  // open chain
  TChain * tr = new TChain("str");
  tr->Add("./redset/Red*.root");
  Float_t E12,Pt,Eta,Phi,M12,Z,b_pol,y_pol;
  Bool_t kicked,isConsistent;
  Int_t TrigBits,runnum,bx;
  Float_t N12;

  Float_t E12_min,Pt_min,Eta_min,Phi_min;
  Float_t E12_max,Pt_max,Eta_max,Phi_max;
  E12_min=Pt_min=Eta_min=Phi_min=1000;
  E12_max=Pt_max=Eta_max=Phi_max=0;

  str->SetBranchAddress("runnum",&runnum);
  str->SetBranchAddress("Bunchid7bit",&bx);
  str->SetBranchAddress("E12",&E12);
  str->SetBranchAddress("Pt",&Pt);
  str->SetBranchAddress("Eta",&Eta);
  str->SetBranchAddress("Phi",&Phi);
  str->SetBranchAddress("M12",&M12);
  str->SetBranchAddress("Z",&Z);
  str->SetBranchAddress("TrigBits",&TrigBits);
  str->SetBranchAddress("N12",&N12);
  Int_t runnum_tmp=0;

  
  // get kinematics ranges for eta,Pt,Phi using the reduced data set and looking for maxima and minima
  // -- this is useful for looking at kinematic distrbutions beyond the kinematic boundaries set in Bin_Splitter.C
  // -- below there is a section to get the kinematic ranges using Bin_Splitter.C; comment either this section
  //    or the next to choose which one to use
  /*
  for(Int_t x=0; x<tr->GetEntries(); x++)
  {
    if((x%100000)==0) printf("computing kin ranges: %.2f%%\n",100*((Float_t)x)/((Float_t)tr->GetEntries()));
    tr->GetEntry(x);
    kicked = RD->Kicked(runnum,bx);
    if(runnum!=runnum_tmp)
    {
      b_pol = RD->BluePol(runnum);
      y_pol = RD->YellPol(runnum);
      isConsistent = RD->RellumConsistent(runnum);
      runnum_tmp=runnum;
    }
    // n photon cut
    //if(N12==1 && kicked==0 && isConsistent==1 && b_pol*y_pol!=0)
    // pi0 cut
    if(fabs(M12-0.135)<0.1 && Z<0.8 && (TrigBits&0x200) && N12==2 && kicked==0 && isConsistent==1 && b_pol*y_pol!=0 && Pt<15)
    {
      E12_min = (E12 < E12_min) ? E12:E12_min;
      Pt_min  = (Pt  < Pt_min)  ? Pt:Pt_min;
      Eta_min = (Eta < Eta_min) ? Eta:Eta_min;
      Phi_min = (Phi < Phi_min) ? Phi:Phi_min;

      E12_max = (E12 > E12_max) ? E12:E12_max;
      Pt_max  = (Pt  > Pt_max)  ? Pt:Pt_max;
      Eta_max = (Eta > Eta_max) ? Eta:Eta_max;
      Phi_max = (Phi > Phi_max) ? Phi:Phi_max;
    };
  };
  */


  // get kinematic ranges using the current binning set by Bin_Splitter.C
  // -- choose this section or the previous one to set the kinematic ranges to plot in diag.root 
  ///*
  Int_t phi_bins0, eta_bins0, pt_bins0, en_bins0;
  if(gSystem->Getenv("PHI_BINS")==NULL){fprintf(stderr,"ERROR: source env vars\n"); return;};
  sscanf(gSystem->Getenv("PHI_BINS"),"%d",&phi_bins0);
  sscanf(gSystem->Getenv("ETA_BINS"),"%d",&eta_bins0);
  sscanf(gSystem->Getenv("PT_BINS"),"%d",&pt_bins0);
  sscanf(gSystem->Getenv("EN_BINS"),"%d",&en_bins0);
  const Int_t phi_bins = phi_bins0;
  const Int_t eta_bins = eta_bins0;
  const Int_t pt_bins = pt_bins0;
  const Int_t en_bins = en_bins0;
  Float_t phi_div[phi_bins+1];
  Float_t eta_div[eta_bins+1];
  Float_t pt_div[pt_bins+1];
  Float_t en_div[en_bins+1];
  char phi_div_env[phi_bins+1][20];
  char eta_div_env[eta_bins+1][20];
  char pt_div_env[pt_bins+1][20];
  char en_div_env[en_bins+1][20];
  for(Int_t i=0; i<=phi_bins; i++)
  {
    sprintf(phi_div_env[i],"PHI_DIV_%d",i);
    sscanf(gSystem->Getenv(phi_div_env[i]),"%f",&(phi_div[i]));
    printf("%s %f\n",phi_div_env[i],phi_div[i]);
  };
  for(Int_t i=0; i<=eta_bins; i++)
  {
    sprintf(eta_div_env[i],"ETA_DIV_%d",i);
    sscanf(gSystem->Getenv(eta_div_env[i]),"%f",&(eta_div[i]));
    printf("%s %f\n",eta_div_env[i],eta_div[i]);
  };
  for(Int_t i=0; i<=pt_bins; i++)
  {
    sprintf(pt_div_env[i],"PT_DIV_%d",i);
    sscanf(gSystem->Getenv(pt_div_env[i]),"%f",&(pt_div[i]));
    printf("%s %f\n",pt_div_env[i],pt_div[i]);
  };
  for(Int_t i=0; i<=en_bins; i++)
  {
    sprintf(en_div_env[i],"EN_DIV_%d",i);
    sscanf(gSystem->Getenv(en_div_env[i]),"%f",&(en_div[i]));
    printf("%s %f\n",en_div_env[i],en_div[i]);
  };
  sscanf(gSystem->Getenv("PHI_LOW"),"%f",&Phi_min);
  sscanf(gSystem->Getenv("PHI_HIGH"),"%f",&Phi_max);
  sscanf(gSystem->Getenv("ETA_LOW"),"%f",&Eta_min);
  sscanf(gSystem->Getenv("ETA_HIGH"),"%f",&Eta_max);
  sscanf(gSystem->Getenv("PT_LOW"),"%f",&Pt_min);
  sscanf(gSystem->Getenv("PT_HIGH"),"%f",&Pt_max);
  sscanf(gSystem->Getenv("EN_LOW"),"%f",&E12_min);
  sscanf(gSystem->Getenv("EN_HIGH"),"%f",&E12_max);
  //*/

  
  // load run exclusion trees
  TTree * exclusion_sph = new TTree("exclusion_sph","exclusion_sph");
  TTree * exclusion_pi0 = new TTree("exclusion_pi0","exclusion_pi0");
  TTree * exclusion_thr = new TTree("exclusion_thr","exclusion_thr");
  exclusion_sph->ReadFile("exclusion_list_sph","runnum/I");
  exclusion_pi0->ReadFile("exclusion_list_pi0","runnum/I");
  exclusion_thr->ReadFile("exclusion_list_thr","runnum/I");
  Bool_t exclude_sph,exclude_pi0,exclude_thr;
  Int_t rn_sph,rn_pi0,rn_thr;
  exclusion_sph->SetBranchAddress("runnum",&rn_sph);
  exclusion_pi0->SetBranchAddress("runnum",&rn_pi0);
  exclusion_thr->SetBranchAddress("runnum",&rn_thr);


  TH2D * sph_pt_vs_eta = new TH2D("sph_pt_vs_eta","single #gamma :: p_{T} vs. #eta",NBINS,Eta_min,Eta_max,NBINS,0,Pt_max);
  TH2D * sph_en_vs_eta = new TH2D("sph_en_vs_eta","single #gamma :: E vs. #eta",NBINS,Eta_min,Eta_max,NBINS,0,E12_max);
  TH2D * sph_pt_vs_phi = new TH2D("sph_pt_vs_phi","single #gamma :: p_{T} vs. #phi",NBINS,-3.14,3.14,NBINS,0,Pt_max);
  TH2D * sph_en_vs_phi = new TH2D("sph_en_vs_phi","single #gamma :: E vs. #phi",NBINS,-3.14,3.14,NBINS,0,E12_max);
  TH2D * sph_eta_vs_phi = new TH2D("sph_eta_vs_phi","single #gamma :: #eta vs. #phi",NBINS,-3.14,3.14,NBINS,Eta_min,Eta_max);
  TH2D * sph_en_vs_pt = new TH2D("sph_en_vs_pt","single #gamma :: E vs. p_{T}",NBINS,0,Pt_max,NBINS,0,E12_max);

  TH2D * pi0_pt_vs_eta = new TH2D("pi0_pt_vs_eta","#pi^{0} (naive M cut) :: p_{T} vs. #eta",NBINS,Eta_min,Eta_max,NBINS,0,Pt_max);
  TH2D * pi0_en_vs_eta = new TH2D("pi0_en_vs_eta","#pi^{0} (naive M cut) :: E vs. #eta",NBINS,Eta_min,Eta_max,NBINS,0,E12_max);
  TH2D * pi0_pt_vs_phi = new TH2D("pi0_pt_vs_phi","#pi^{0} (naive M cut) :: p_{T} vs. #phi",NBINS,-3.14,3.14,NBINS,0,Pt_max);
  TH2D * pi0_en_vs_phi = new TH2D("pi0_en_vs_phi","#pi^{0} (naive M cut) :: E vs. #phi",NBINS,-3.14,3.14,NBINS,0,E12_max);
  TH2D * pi0_eta_vs_phi = new TH2D("pi0_eta_vs_phi","#pi^{0} (naive M cut) :: #eta vs. #phi",NBINS,-3.14,3.14,NBINS,Eta_min,Eta_max);
  TH2D * pi0_en_vs_pt = new TH2D("pi0_en_vs_pt","#pi^{0} (naive M cut) :: E vs. p_{T}",NBINS,0,Pt_max,NBINS,0,E12_max);

  TH2D * thr_pt_vs_eta = new TH2D("thr_pt_vs_eta","N_{#gamma}>2 :: p_{T} vs. #eta",NBINS,Eta_min,Eta_max,NBINS,0,Pt_max);
  TH2D * thr_en_vs_eta = new TH2D("thr_en_vs_eta","N_{#gamma}>2 :: E vs. #eta",NBINS,Eta_min,Eta_max,NBINS,0,E12_max);
  TH2D * thr_pt_vs_phi = new TH2D("thr_pt_vs_phi","N_{#gamma}>2 :: p_{T} vs. #phi",NBINS,-3.14,3.14,NBINS,0,Pt_max);
  TH2D * thr_en_vs_phi = new TH2D("thr_en_vs_phi","N_{#gamma}>2 :: E vs. #phi",NBINS,-3.14,3.14,NBINS,0,E12_max);
  TH2D * thr_eta_vs_phi = new TH2D("thr_eta_vs_phi","N_{#gamma}>2 :: #eta vs. #phi",NBINS,-3.14,3.14,NBINS,Eta_min,Eta_max);
  TH2D * thr_en_vs_pt = new TH2D("thr_en_vs_pt","N_{#gamma}>2 :: E vs. p_{T}",NBINS,0,Pt_max,NBINS,0,E12_max);

  TH2D * pi0_z_vs_eta = new TH2D("pi0_z_vs_eta","#pi^{0} (naive M cut) :: Z vs. #eta",NBINS,Eta_min,Eta_max,NBINS,0,1);
  TH2D * pi0_z_vs_phi = new TH2D("pi0_z_vs_phi","#pi^{0} (naive M cut) :: Z vs. #phi",NBINS,-3.14,3.14,NBINS,0,1);

  TH1D * mass_dist = new TH1D("mass_dist","M_{#gamma#gamma} distribution (#pi^{0} cuts without mass cut)",NBINS,0,1);
  TH1D * z_dist = new TH1D("z_dist","Z distribution (N12==2, jet1, abs(M12-0.135)<0.1, kicked==0)",NBINS,0,1);
  TH1D * trig_dist = new TH1D("trig_dist","TrigBits distribution (N12==2)",NBINS,7500,33500);
  
  TH2D * mass_vs_en = new TH2D("mass_vs_en","M_{#gamma#gamma} vs. E_{#gamma#gamma} (#pi^{0} cuts without mass cut)",
    NBINS,0,E12_max,NBINS,0,1);
  TH2D * mass_vs_pt = new TH2D("mass_vs_pt","M_{#gamma#gamma} vs. p_{T} (#pi^{0} cuts without mass cut)",
    NBINS,0,Pt_max,NBINS,0,1);

  TH1D * mass_dist_for_enbin[10];
  char mass_dist_for_enbin_n[10][64];
  char mass_dist_for_enbin_t[10][256];
  for(Int_t ee=0; ee<10; ee++)
  {
    sprintf(mass_dist_for_enbin_n[ee],"mass_dist_for_enbin%d",ee);
    sprintf(mass_dist_for_enbin_t[ee],"M_{#gamma#gamma} distribution for E_{#gamma#gamma}#in[%d,%d) GeV",ee*10,(ee+1)*10);
    mass_dist_for_enbin[ee] = new TH1D(mass_dist_for_enbin_n[ee],mass_dist_for_enbin_t[ee],NBINS,0,1);
  };

  runnum_tmp=0;
  for(Int_t x=0; x<tr->GetEntries(); x++)
  {
    if((x%100000)==0) printf("filling histograms: %.2f%%\n",100*((Float_t)x)/((Float_t)tr->GetEntries()));
    tr->GetEntry(x);
    kicked = RD->Kicked(runnum,bx);
    if(runnum!=runnum_tmp)
    {
      b_pol = RD->BluePol(runnum);
      y_pol = RD->YellPol(runnum);
      isConsistent = RD->RellumConsistent(runnum);
      runnum_tmp=runnum;
      exclude_sph=0;
      exclude_pi0=0;
      exclude_thr=0;
      for(Int_t xx=0; xx<exclusion_sph->GetEntries(); xx++) { exclusion_sph->GetEntry(xx); if(runnum==rn_sph) exclude_sph=1; };
      for(Int_t xx=0; xx<exclusion_pi0->GetEntries(); xx++) { exclusion_pi0->GetEntry(xx); if(runnum==rn_pi0) exclude_pi0=1; };
      for(Int_t xx=0; xx<exclusion_thr->GetEntries(); xx++) { exclusion_thr->GetEntry(xx); if(runnum==rn_thr) exclude_thr=1; };
    }
    // rellum / pol cut
    if( kicked==0 && isConsistent==1 && b_pol>0 && y_pol>0)
    {
      // IF YOU CHANGE THE CUTS HERE, CHANGE THEM IN THE PLOT TITLES TOO!!!!!
      if(exclude_pi0==0 && 
         (TrigBits&0x200) &&
         fabs(N12-2)<0.01 &&
         Z<0.8 &&
         ( (((runnum/1000000)-1 == 12) && Pt>=2.5 && Pt<10) ||
           (((runnum/1000000)-1 == 13) && Pt>=2.0 && Pt<10) ) &&
         E12>=30 && E12<100 &&
         M12>0) 
      {
        mass_dist->Fill(M12);
        mass_vs_en->Fill(E12,M12);
        mass_vs_pt->Fill(Pt,M12);
        for(Int_t ee=0; ee<10; ee++)
        {
          if(E12>=(ee*10) && E12<((ee+1)*10)) mass_dist_for_enbin[ee]->Fill(M12);
        };
        // naive mass cut for correlation plots
        if(fabs(M12-0.135)<0.1)
        {
          pi0_pt_vs_eta->Fill(Eta,Pt);
          pi0_en_vs_eta->Fill(Eta,E12);
          pi0_pt_vs_phi->Fill(Phi,Pt);
          pi0_en_vs_phi->Fill(Phi,E12);
          pi0_eta_vs_phi->Fill(Phi,Eta);
          pi0_en_vs_pt->Fill(Pt,E12);
          pi0_z_vs_eta->Fill(Eta,Z);
          pi0_z_vs_phi->Fill(Phi,Z);
        };
      }
      if(exclude_pi0==0 && fabs(N12-2)<0.01 && (TrigBits&0x200) && fabs(M12-0.135)<0.1) z_dist->Fill(Z);
      if(exclude_pi0==0 && fabs(N12-2)<0.01) trig_dist->Fill(TrigBits);

      // single photon cut
      if(exclude_sph==0 && fabs(N12-1)<0.01)
      {
        sph_pt_vs_eta->Fill(Eta,Pt);
        sph_en_vs_eta->Fill(Eta,E12);
        sph_pt_vs_phi->Fill(Phi,Pt);
        sph_en_vs_phi->Fill(Phi,E12);
        sph_eta_vs_phi->Fill(Phi,Eta);
        sph_en_vs_pt->Fill(Pt,E12);
      };

      // three or more photons cut
      if(exclude_thr==0 && N12>2.5)
      {
        thr_pt_vs_eta->Fill(Eta,Pt);
        thr_en_vs_eta->Fill(Eta,E12);
        thr_pt_vs_phi->Fill(Phi,Pt);
        thr_en_vs_phi->Fill(Phi,E12);
        thr_eta_vs_phi->Fill(Phi,Eta);
        thr_en_vs_pt->Fill(Pt,E12);
      };
    };
  };


  /*
  // draw output -- MOVED TO DrawDiagnostics.C
  gStyle->SetOptStat(0);

  TCanvas * cc0 = new TCanvas("cc0","cc0",1000,1200);
  cc0->Divide(1,3);
  cc0->cd(1); mass_dist->Draw();
  cc0->cd(2); z_dist->Draw();
  cc0->cd(3); cc0->GetPad(3)->SetLogy(); trig_dist->Draw();


  TCanvas * cc1 = new TCanvas("cc1","cc1",1000,1200);
  cc1->Divide(2,3);
  cc1->cd(1); pt_vs_eta->Draw("colz");
  cc1->cd(2); pt_vs_phi->Draw("colz");
  cc1->cd(3); en_vs_eta->Draw("colz");
  cc1->cd(4); en_vs_phi->Draw("colz");
  cc1->cd(5); z_vs_eta->Draw("colz");
  cc1->cd(6); z_vs_phi->Draw("colz");

  TCanvas * cc2 = new TCanvas("cc2","cc2",1000,1200);
  cc2->Divide(2,3);
  cc2->cd(1); eta_vs_phi->Draw("colz");
  cc2->cd(2); en_vs_pt->Draw("colz");


  cc0->Print("diag_lin.pdf(","pdf");
  cc1->Print("diag_lin.pdf","pdf");
  cc2->Print("diag_lin.pdf)","pdf");
  printf("\ndiag_lin.pdf diagnostic kinematics file produced\n");

  for(Int_t i=1; i<=3; i++) cc0->GetPad(i)->SetLogy();
  for(Int_t i=1; i<=6; i++)
  {
    cc1->GetPad(i)->SetLogz();
    cc2->GetPad(i)->SetLogz();
  };

  cc0->Print("diag_log.pdf(","pdf");
  cc1->Print("diag_log.pdf","pdf");
  cc2->Print("diag_log.pdf)","pdf");
  printf("\ndiag_log.pdf diagnostic kinematics file produced\n");

  printf("E12 range: %f -- %f\n",E12_min,E12_max);
  printf("Pt range: %f -- %f\n",Pt_min,Pt_max);
  printf("Eta range: %f -- %f\n",Eta_min,Eta_max);
  printf("Phi range: %f -- %f\n",Phi_min,Phi_max);
  */

  // write output
  TFile * outfile = new TFile("diag.root","RECREATE");

  mass_dist->Write();
  z_dist->Write();
  trig_dist->Write();
  mass_vs_en->Write();
  mass_vs_pt->Write();

  for(Int_t ee=0; ee<10; ee++) mass_dist_for_enbin[ee]->Write();

  sph_pt_vs_eta->Write();
  sph_en_vs_eta->Write();
  sph_pt_vs_phi->Write();
  sph_en_vs_phi->Write();
  sph_eta_vs_phi->Write();
  sph_en_vs_pt->Write();

  pi0_pt_vs_eta->Write();
  pi0_en_vs_eta->Write();
  pi0_pt_vs_phi->Write();
  pi0_en_vs_phi->Write();
  pi0_eta_vs_phi->Write();
  pi0_en_vs_pt->Write();

  thr_pt_vs_eta->Write();
  thr_en_vs_eta->Write();
  thr_pt_vs_phi->Write();
  thr_en_vs_phi->Write();
  thr_eta_vs_phi->Write();
  thr_en_vs_pt->Write();

  pi0_z_vs_eta->Write();
  pi0_z_vs_phi->Write();


}
  
