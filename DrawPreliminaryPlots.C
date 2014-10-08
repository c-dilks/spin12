// draws STAR preliminary plots for SPIN 2014

void DrawPreliminaryPlots(const char * filename="output/spin_pi0.root")
{
  // open root file
  TFile * tf = new TFile(filename,"READ");
  

  // get bins from environment
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
  Float_t phi_low; sscanf(gSystem->Getenv("PHI_LOW"),"%f",&phi_low);
  Float_t phi_high; sscanf(gSystem->Getenv("PHI_HIGH"),"%f",&phi_high);
  Float_t eta_low; sscanf(gSystem->Getenv("ETA_LOW"),"%f",&eta_low);
  Float_t eta_high; sscanf(gSystem->Getenv("ETA_HIGH"),"%f",&eta_high);
  Float_t pt_low; sscanf(gSystem->Getenv("PT_LOW"),"%f",&pt_low);
  Float_t pt_high; sscanf(gSystem->Getenv("PT_HIGH"),"%f",&pt_high);
  Float_t en_low; sscanf(gSystem->Getenv("EN_LOW"),"%f",&en_low);
  Float_t en_high; sscanf(gSystem->Getenv("EN_HIGH"),"%f",&en_high);

  // obtain graph
  TGraphErrors * gr;
  if(pt_bins==1 && en_bins>1) gr = (TGraphErrors*) tf->Get("/A_LL/en_dep_a3_g0_p0");
  else if(pt_bins>1 && en_bins==1) gr = (TGraphErrors*) tf->Get("A_LL/pt_dep_a3_g0_e0");
  else { fprintf(stderr,"error: number of either pt bins or en bins must be 1\n"); return; };
  
  // zero line
  TLine * zero_line = new TLine(gr->GetXaxis()->GetXmin(),0,gr->GetXaxis()->GetXmax(),0);
  zero_line->SetLineColor(kCyan+3);
  zero_line->SetLineWidth(3);
  zero_line->SetLineStyle(2);

  // plot style
  gr->SetMarkerColor(kBlack);
  gr->SetLineColor(kRed);
  gr->SetMarkerSize(1.6);
  gr->SetMarkerStyle(kFullCircle);
  gr->SetLineWidth(4);
  gr->GetXaxis()->SetLabelSize(0.05);
  gr->GetYaxis()->SetLabelSize(0.05);
  gr->GetXaxis()->SetTitleOffset(1.0);
  gr->GetYaxis()->SetTitleOffset(1.0);
  gr->GetXaxis()->SetTitleSize(0.05);
  gr->GetYaxis()->SetTitleSize(0.05);

  // title & axes
  if(pt_bins==1 && en_bins>1)
  {
    gr->SetTitle("#pi^{0} Double Helicity Asymmetry A_{LL} vs. E_{#gamma#gamma}");
    gr->GetXaxis()->SetTitle("E_{#gamma#gamma} [GeV]");
  }
  else if(pt_bins>1 && en_bins==1)
  {
    gr->SetTitle("#pi^{0} Double Helicity Asymmetry A_{LL} vs. p_{T}");
    gr->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  };
  gr->GetYaxis()->SetTitle("A_{LL}");

  // fit
  TF1 * fit_ftn = new TF1("fit_ftn","pol0",gr->GetXaxis()->GetXmin(),gr->GetXaxis()->GetXmax());
  gr->Fit(fit_ftn,"N","",gr->GetXaxis()->GetXmin(),gr->GetXaxis()->GetXmax());
  //char fit_result_text[64];
  //sprintf(fit_result_text,"constant fit: %.3f #pm %.3f",fit_ftn->GetParameter(0),fit_ftn->GetParError(0));
  //printf("%s\n",fit_result_text);
  //TLatex * fit_result = new TLatex(0.8,0.9,fit_result_text);

  // draw
  TCanvas * prelim_plot = new TCanvas("prelim_plot","prelim_plot",1000,500);
  gr->Draw("APE");
  zero_line->Draw();
  //fit_result->Draw();



};
