// determines energy-dependent mass cuts 

void MassCutter(const char * filename="diag.root")
{
  // load mass dists for each en bin
  TFile * infile = new TFile(filename,"READ");
  TH1D  * mdist[10];
  char mdist_n[10][32];
  for(Int_t ee=0; ee<10; ee++)
  {
    sprintf(mdist_n[ee],"mass_dist_for_enbin%d",ee);
    mdist[ee] = (TH1D*) infile->Get(mdist_n[ee]);
  };

  // find bin with maximum bin and determine upper and lower bounds 
  // of energy-dependent mass cut by setting the bounds in the distribution
  // where the mass is reduced from the maximum by a factor of "factor"
  Double_t mdist_max[10];
  Double_t mdist_max_mass[10];
  Int_t mdist_max_bin[10];
  Int_t mdist_nbins[10];
  Double_t bc;
  Double_t factor=0.5; // "factor"
  Double_t ub[10];
  Double_t lb[10];
  Bool_t found;
  for(Int_t ee=0; ee<10; ee++)
  {
    if(mdist[ee]->GetEntries()>0)
    {
      mdist_max[ee] = mdist[ee]->GetMaximum();
      mdist_max_bin[ee] = mdist[ee]->GetMaximumBin();
      mdist_max_mass[ee] = mdist[ee]->GetBinCenter(mdist_max_bin[ee]);
      mdist_nbins[ee] = mdist[ee]->GetNbinsX();
      
      // cheap hack to get ignore mass spikes (hot towers)
      // -- hot towers are all for masses > 0.3
      // -- pi0 peak really never goes > 0.3
      // -- thus if we find a maximum with mass > 0.3, we simply set the 
      //    bin contents to 0... this is not a nice thing to do, but I needed 
      //    to write this script quickly; TO BE UPDATED SOMEDAY!
      if(mdist_max_mass[ee]>0.3)
      {
        while(mdist_max_mass[ee]>0.3)
        {
          mdist[ee]->SetBinContent(mdist_max_bin[ee],0);
          mdist_max[ee] = mdist[ee]->GetMaximum();
          mdist_max_bin[ee] = mdist[ee]->GetMaximumBin();
          mdist_max_mass[ee] = mdist[ee]->GetBinCenter(mdist_max_bin[ee]);
        };
        mdist[ee] = (TH1D*) infile->Get(mdist_n[ee]);
      };


      // search for upper bound
      found=false;
      for(Int_t b=mdist_max_bin[ee]; b<=mdist_nbins[ee]; b++)
      {
        bc = mdist[ee]->GetBinContent(b);
        if(bc<factor*mdist_max[ee] && found==false)
        {
          ub[ee] = mdist[ee]->GetBinCenter(b);
          found=true;
        };
      };

      // search for lower bound
      found=false;
      for(Int_t b=mdist_max_bin[ee]; b>=1; b--)
      {
        bc = mdist[ee]->GetBinContent(b);
        if(bc<factor*mdist_max[ee] && found==false)
        {
          lb[ee] = mdist[ee]->GetBinCenter(b);
          found=true;
        };
      };
    };
  };


  // define TLines and draw TCanvases
  TLine * max_line[10];
  TLine * lb_line[10];
  TLine * ub_line[10];
  TCanvas * cc[10];
  char cc_n[10][32];
  gROOT->ProcessLine(".! touch mass_cuts; rm mass_cuts; touch mass_cuts");
  printf("en_low en_high mass_low mass_at_max_bin mass_high\n");
  for(Int_t ee=0; ee<10; ee++)
  {
    if(mdist[ee]->GetEntries()>0)
    {
      sprintf(cc_n[ee],"cc%d",ee);
      cc[ee] = new TCanvas(cc_n[ee],cc_n[ee],700,500);
      max_line[ee] = new TLine(mdist_max_mass[ee],0,mdist_max_mass[ee],mdist_max[ee]);
      lb_line[ee] = new TLine(lb[ee],0,lb[ee],mdist_max[ee]);
      ub_line[ee] = new TLine(ub[ee],0,ub[ee],mdist_max[ee]);
      max_line[ee]->SetLineColor(kRed);
      lb_line[ee]->SetLineColor(kBlue);
      ub_line[ee]->SetLineColor(kGreen+2);
      mdist[ee]->Draw();
      max_line[ee]->Draw();
      lb_line[ee]->Draw();
      ub_line[ee]->Draw();
      gSystem->RedirectOutput("mass_cuts","a");
      printf("%f %f %f %f %f\n",ee*10,(ee+1)*10,lb[ee],mdist_max_mass[ee],ub[ee]);
      gSystem->RedirectOutput(0);
      printf("%f %f %f %f %f\n",ee*10,(ee+1)*10,lb[ee],mdist_max_mass[ee],ub[ee]);
    };
  };
};
