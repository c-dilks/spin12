// draws STAR preliminary plots for SPIN 2014
// -- draws 35mr and 100mr on same plot

void DrawPreliminaryPlots2(const char * kin="pt")
{
  const Double_t SYSTEMATIC = 0.000282; // FOR RUN 12 !!!!!!!!!!!!!!!!!!!!!


  if(!(!strcmp(kin,"en")||!strcmp(kin,"pt")))
  {
    fprintf(stderr,"ERROR: kin must be \"en\" or \"pt\"");
    return;
  };

  // open files
  char fileS_n[128]; // 35mr
  char fileL_n[128]; // 100mr 
  sprintf(fileS_n,"output_for_spin/output_%s_35mr/spin_pi0.root",kin);
  sprintf(fileL_n,"output_for_spin/output_%s_100mr/spin_pi0.root",kin);
  TFile * fileS = new TFile(fileS_n,"READ");
  TFile * fileL = new TFile(fileL_n,"READ");


  // get graphs and set title strings
  TGraphErrors * grS;
  TGraphErrors * grL;
  char title_str[128];
  char axis_str[128];
  Double_t offset;
  if(!strcmp(kin,"pt")) 
  {
    grS = (TGraphErrors*) fileS->Get("A_LL/pt_dep_a3_g0_e0");
    grL = (TGraphErrors*) fileL->Get("A_LL/pt_dep_a3_g0_e0");
    strcpy(title_str,"#pi^{0} Double Helicity Asymmetry A_{LL} vs. p_{T}");
    strcpy(axis_str,"p_{T} [GeV/c]");
    offset = 0.025;
  }
  else if(!strcmp(kin,"en"))
  {
    grS = (TGraphErrors*) fileS->Get("A_LL/en_dep_a3_g0_p0");
    grL = (TGraphErrors*) fileL->Get("A_LL/en_dep_a3_g0_p0");
    strcpy(title_str,"#pi^{0} Double Helicity Asymmetry A_{LL} vs. E_{#gamma#gamma}");
    strcpy(axis_str,"E_{#gamma#gamma} [GeV]");
    offset = 0.25;
  };


  // get wdist sub graphs & integrate them
  Int_t MAX_BINS_tmp = grL->GetN();
  const Int_t MAX_BINS=MAX_BINS_tmp;
  TH1D * subgrS[MAX_BINS];
  TH1D * subgrL[MAX_BINS];
  char subgr_n[MAX_BINS][128];
  Double_t integralS[MAX_BINS];
  Double_t integralL[MAX_BINS];
  for(Int_t b=0; b<MAX_BINS; b++)
  {
    if(!strcmp(kin,"pt")) sprintf(subgr_n[b],"bin_weighting/pt_wdist_sub_g0_e0_ptbin%d",b);
    else if(!strcmp(kin,"en")) sprintf(subgr_n[b],"bin_weighting/en_wdist_sub_g0_p0_enbin%d",b);
    subgrS[b] = (TH1D*) fileS->Get(subgr_n[b]);
    subgrL[b] = (TH1D*) fileL->Get(subgr_n[b]);
    integralS[b] = subgrS[b]->Integral();
    integralL[b] = subgrL[b]->Integral();
  };


  // graph of number of pi0s vs. kinematic (pt or E)
  TGraph * numgrS = new TGraphErrors();
  TGraph * numgrL = new TGraphErrors();
  Double_t x,y,x_e,y_e;
  for(Int_t n=0; n<MAX_BINS; n++)
  {
    grS->GetPoint(n,x,y);
    numgrS->SetPoint(n,x,integralS[n]);
    grL->GetPoint(n,x,y);
    numgrL->SetPoint(n,x,integralL[n]);
  };
  numgrS->SetMarkerSize(1);
  numgrS->SetMarkerStyle(kFullCircle);
  numgrL->SetMarkerSize(1);
  numgrL->SetMarkerStyle(kFullCircle);


  // offset the 100mr graph
  TGraphErrors * grLoffset = new TGraphErrors();
  for(Int_t n=0; n<MAX_BINS; n++)
  {
    grL->GetPoint(n,x,y);
    x_e = grL->GetErrorX(n);
    y_e = grL->GetErrorY(n);
    grLoffset->SetPoint(n,x+offset,y);
    grLoffset->SetPointError(n,x_e,y_e);
  };


  // systematic uncertainties
  TGraphErrors * grSsys = new TGraphErrors();
  TGraphErrors * grLsys = new TGraphErrors();
  for(Int_t n=0; n<MAX_BINS; n++)
  {
    grS->GetPoint(n,x,y);
    x_e = grS->GetErrorX(n);
    y_e = SYSTEMATIC;
    grSsys->SetPoint(n,x,y);
    grSsys->SetPointError(n,x_e,y_e);

    grLoffset->GetPoint(n,x,y); // use offset here
    x_e = grL->GetErrorX(n);
    y_e = SYSTEMATIC;
    grLsys->SetPoint(n,x,y);
    grLsys->SetPointError(n,x_e,y_e);
  };

  grSsys->SetFillColor(kRed);
  grLsys->SetFillColor(kBlue);
  grSsys->SetFillStyle(3001);
  grLsys->SetFillStyle(3001);
    



  // zero line
  TLine * zero_line = new TLine(grL->GetXaxis()->GetXmin(),0,grL->GetXaxis()->GetXmax(),0);
  zero_line->SetLineColor(kCyan+3);
  zero_line->SetLineWidth(3);
  zero_line->SetLineStyle(2);


  // plot style
  grS->SetMarkerColor(kRed);
  grS->SetLineColor(kRed);
  grS->SetMarkerSize(2);
  grS->SetMarkerStyle(kFullCircle);
  grS->SetLineWidth(3);
  grL->SetMarkerColor(kBlue);
  grL->SetLineColor(kBlue);
  grL->SetMarkerSize(2);
  grL->SetMarkerStyle(kFullSquare);
  grL->SetLineWidth(3);
  grLoffset->SetMarkerColor(kBlue);
  grLoffset->SetLineColor(kBlue);
  grLoffset->SetMarkerSize(2);
  grLoffset->SetMarkerStyle(kFullSquare);
  grLoffset->SetLineWidth(3);


  // create multigraph
  TMultiGraph * mg = new TMultiGraph();
  mg->Add(grS);
  //mg->Add(grL);
  mg->Add(grLoffset);


  // plot tiles & axes titles
  grS->SetTitle(title_str);
  grS->GetXaxis()->SetTitle(axis_str);
  grS->GetYaxis()->SetTitle("A_{LL}");
  grS->GetXaxis()->SetLabelSize(0.05);
  grS->GetYaxis()->SetLabelSize(0.05);
  grS->GetXaxis()->SetTitleOffset(1.0);
  grS->GetYaxis()->SetTitleOffset(1.0);
  grS->GetXaxis()->SetTitleSize(0.05);
  grS->GetYaxis()->SetTitleSize(0.05);
  grL->SetTitle(title_str);
  grL->GetXaxis()->SetTitle(axis_str);
  grL->GetYaxis()->SetTitle("A_{LL}");
  grL->GetXaxis()->SetLabelSize(0.05);
  grL->GetYaxis()->SetLabelSize(0.05);
  grL->GetXaxis()->SetTitleOffset(1.0);
  grL->GetYaxis()->SetTitleOffset(1.0);
  grL->GetXaxis()->SetTitleSize(0.05);
  grL->GetYaxis()->SetTitleSize(0.05);


  // fit
  printf("\n\n35mr FIT RESULTS");
  grS->Fit("pol0","N","",grS->GetXaxis()->GetXmin(),grS->GetXaxis()->GetXmax());
  printf("\n\n100mr FIT RESULTS");
  grL->Fit("pol0","N","",grL->GetXaxis()->GetXmin(),grL->GetXaxis()->GetXmax());



  // draw
  TCanvas * prelim_plot = new TCanvas("prelim_plot","prelim_plot",1000,500);
  mg->Draw("ape");
  grLsys->Draw("2");
  grSsys->Draw("2");
  mg->Draw("pe");
  zero_line->Draw();


  // set multigraph parameters & update tcanvas
  mg->SetTitle(title_str);
  mg->GetXaxis()->SetTitle(axis_str);
  mg->GetYaxis()->SetTitle("A_{LL}");
  mg->GetXaxis()->SetLabelSize(0.05);
  mg->GetYaxis()->SetLabelSize(0.05);
  mg->GetXaxis()->SetTitleOffset(1.0);
  mg->GetYaxis()->SetTitleOffset(1.0);
  mg->GetXaxis()->SetTitleSize(0.05);
  mg->GetYaxis()->SetTitleSize(0.05);
  mg->GetXaxis()->SetLimits(grS->GetXaxis()->GetXmin(),grS->GetXaxis()->GetXmax());
  prelim_plot->Update();


  TFile * outfile = new TFile("prelim.root","RECREATE");
  prelim_plot->Write("prelim_plot");
  numgrS->Write("numgrS");
  numgrL->Write("numgrL");
};

