// draws A_LL, blue A_L, yellow A_L on one plot
//
// jtype is "sph" for single photons
//          "pi0" for pi0s
//          "thr" for >2 photon jets
//
// specificEbin: if > 0, then it shows pt-dependent plots for this en bin,
//               useful for looking at pt-dependenc when PT_BINS and EN_BINS are both not unity;
//               otherwise for pt-dependence, EN_BINS must equal 1

void DrawAsymmetries(const char * jtype="pi0", const char * filetype="png", const char * asym_file="spin.root",
                     Int_t specificEbin=-1)
{
  // open root file
  TFile * asym_tfile = new TFile(asym_file,"READ");

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


  // set jet type
  char jtype_str[32];
  if(!strcmp(jtype,"sph")) strcpy(jtype_str,"single #gamma");
  else if(!strcmp(jtype,"pi0")) strcpy(jtype_str,"#pi^{0}");
  else if(!strcmp(jtype,"thr")) strcpy(jtype_str,"N_{#gamma}>2");
  else strcpy(jtype_str,"");

  // asymmetry titles
  const Int_t asym_bins=4;
  char asymmetry[asym_bins][8];
  strcpy(asymmetry[1],"Y-SSA");
  strcpy(asymmetry[2],"B-SSA");
  strcpy(asymmetry[3],"DSA");
  // written out asymmetry titles
  char asymmetry_w[asym_bins][40];
  strcpy(asymmetry_w[1],"yellow single spin asymmetry");
  strcpy(asymmetry_w[2],"blue single spin asymmetry");
  strcpy(asymmetry_w[3],"double spin asymmetry");

  // define asymmetry kinematic dependence plots
  char kindep_name[asym_bins][128];
  char xaxistitle[64];
  Int_t num_bins;
  char dir_title[asym_bins][32];
  strcpy(dir_title[1],"A_L_yellow");
  strcpy(dir_title[2],"A_L_blue");
  strcpy(dir_title[3],"A_LL");
  char kindep_main_title[64];
  if(pt_bins0==1 && en_bins0!=1 && eta_bins0==1) 
  {
    for(Int_t a=1; a<asym_bins; a++) sprintf(kindep_name[a],"/%s/en_dep_a%d_g0_p0",dir_title[a],a);
    sprintf(kindep_main_title,"%s asymmetries vs. E",jtype_str);
    strcpy(xaxistitle,"E (GeV)");
    num_bins = en_bins0;
  }
  else if(pt_bins0!=1 && en_bins0==1 && eta_bins0==1) 
  {
    for(Int_t a=1; a<asym_bins; a++) sprintf(kindep_name[a],"/%s/pt_dep_a%d_g0_e0",dir_title[a],a);
    sprintf(kindep_main_title,"%s asymmetries vs. p_{T}",jtype_str);
    strcpy(xaxistitle,"p_{#perp}  (GeV/c)");
    num_bins = pt_bins0;
  }
  else if(pt_bins0==1 && en_bins0==1 && eta_bins0==1)
  {
    for(Int_t a=1; a<asym_bins; a++) sprintf(kindep_name[a],"/%s/pt_dep_a%d_g0_e0",dir_title[a],a);
    sprintf(kindep_main_title,"%s asymmetries",jtype_str);
    strcpy(xaxistitle,"single bin");
    num_bins = 1;
  }
  else if(pt_bins0>=1 && en_bins0>=1 && specificEbin>=0)
  {
    for(Int_t a=1; a<asym_bins; a++) sprintf(kindep_name[a],"/%s/pt_dep_a%d_g0_e%d",dir_title[a],a,specificEbin);
    sprintf(kindep_main_title,"%s asymmetries vs. p_{T}",jtype_str);
    strcpy(xaxistitle,"p_{#perp}  (GeV/c)");
    num_bins = pt_bins0;
  }
  else
  {
    printf("\n<><><><><><><><><><>\n\n");
    printf("pt_bins>1 && en_bins>1 ----------> three.png NOT DRAWN\n");
    printf("to do pt-dependence for a specific E bin, set parameter specificEbin\n");
    printf("spin.root produced\n");
    return;
  };
  for(Int_t a=1; a<asym_bins; a++) printf("opening %s\n",kindep_name[a]);


  // get asym kin dep plots
  TGraphErrors * kindep_gr[asym_bins];
  for(Int_t a=1; a<asym_bins; a++) 
  {
    kindep_gr[a] = (TGraphErrors*) asym_tfile->Get(kindep_name[a]);
    kindep_gr[a]->GetXaxis()->SetLabelSize(0.05);
    kindep_gr[a]->GetYaxis()->SetLabelSize(0.05);
    kindep_gr[a]->GetXaxis()->SetTitleSize(0.06);
    kindep_gr[a]->GetYaxis()->SetTitleSize(0.06);
    kindep_gr[a]->GetYaxis()->SetTitleOffset(0.8);
    kindep_gr[a]->SetMarkerSize(1.3);
    kindep_gr[a]->SetMarkerStyle(kFullCircle);
    kindep_gr[a]->SetLineWidth(2);
  };
  kindep_gr[1]->SetMarkerColor(kBlack);
  kindep_gr[2]->SetMarkerColor(kBlack);
  kindep_gr[3]->SetMarkerColor(kBlack);
  kindep_gr[1]->SetLineColor(kOrange-3);
  kindep_gr[2]->SetLineColor(kBlue);
  kindep_gr[3]->SetLineColor(kRed);


  // get analysing power vs. phi plots
  const Int_t num_bins0 = num_bins;
  char asym_name[asym_bins][num_bins0][128];
  char asym_title[asym_bins][num_bins0][256];
  TH1D * asym_hist[asym_bins][num_bins0];
  char asym_main_title[asym_bins][100];
  if(pt_bins0==1 && en_bins0!=1 && eta_bins0==1) 
  {
    for(Int_t a=1; a<asym_bins; a++) 
    {
      for(Int_t e=0; e<en_bins0; e++)
      {
        sprintf(asym_name[a][e],"/%s/asym_a%d_g0_p0_e%d",dir_title[a],a,e);
        sprintf(asym_title[a][e],"E #in [%.2f,%.2f)",en_div[e],en_div[e+1]);
        asym_hist[a][e] = (TH1D*) asym_tfile->Get(asym_name[a][e]);
        asym_hist[a][e]->SetTitle(asym_title[a][e]);
        asym_hist[a][e]->GetXaxis()->SetTitle("#phi");
        asym_hist[a][e]->GetYaxis()->SetTitle(asymmetry[a]);
        asym_hist[a][e]->GetXaxis()->SetLabelSize(0.05);
        asym_hist[a][e]->GetYaxis()->SetLabelSize(0.05);
        asym_hist[a][e]->GetXaxis()->SetTitleSize(0.06);
        asym_hist[a][e]->GetYaxis()->SetTitleSize(0.06);
        asym_hist[a][e]->GetYaxis()->SetTitleOffset(0.8);
      };
      sprintf(asym_main_title[a],"%s %s vs #phi",jtype_str,asymmetry_w[a]);
    };
  }
  else if(pt_bins0!=1 && en_bins0==1 && eta_bins0==1) 
  {
    for(Int_t a=1; a<asym_bins; a++) 
    {
      for(Int_t p=0; p<pt_bins0; p++)
      {
        sprintf(asym_name[a][p],"/%s/asym_a%d_g0_p%d_e0",dir_title[a],a,p);
        sprintf(asym_title[a][p],"p_{#perp} #in [%.2f,%.2f)",pt_div[p],pt_div[p+1]);
        asym_hist[a][p] = (TH1D*) asym_tfile->Get(asym_name[a][p]);
        asym_hist[a][p]->SetTitle(asym_title[a][p]);
        asym_hist[a][p]->GetXaxis()->SetTitle("#phi");
        asym_hist[a][p]->GetYaxis()->SetTitle(asymmetry[a]);
        asym_hist[a][p]->GetXaxis()->SetLabelSize(0.05);
        asym_hist[a][p]->GetYaxis()->SetLabelSize(0.05);
        asym_hist[a][p]->GetXaxis()->SetTitleSize(0.06);
        asym_hist[a][p]->GetYaxis()->SetTitleSize(0.06);
        asym_hist[a][p]->GetYaxis()->SetTitleOffset(0.8);
      };
      sprintf(asym_main_title[a],"%s %s vs #phi",jtype_str,asymmetry_w[a]);
    };
  }
  else if(pt_bins0==1 && en_bins0==1 && eta_bins0==1)
  {
    for(Int_t a=1; a<asym_bins; a++) 
    {
      sprintf(asym_name[a][0],"/%s/asym_a%d_g0_p0_e0",dir_title[a],a);
      sprintf(asym_title[a][0],"single bin");
      asym_hist[a][0] = (TH1D*) asym_tfile->Get(asym_name[a][0]);
      asym_hist[a][0]->SetTitle(asym_title[a][0]);
      asym_hist[a][0]->GetXaxis()->SetTitle("#phi");
      asym_hist[a][0]->GetYaxis()->SetTitle(asymmetry[a]);
      asym_hist[a][0]->GetXaxis()->SetLabelSize(0.05);
      asym_hist[a][0]->GetYaxis()->SetLabelSize(0.05);
      asym_hist[a][0]->GetXaxis()->SetTitleSize(0.06);
      asym_hist[a][0]->GetYaxis()->SetTitleSize(0.06);
      asym_hist[a][0]->GetYaxis()->SetTitleOffset(0.8);
      sprintf(asym_main_title[a],"%s %s vs #phi",jtype_str,asymmetry_w[a]);
    };
  }
  else if(pt_bins0>=1 && en_bins0>=1 && specificEbin>=0)
  {
    for(Int_t a=1; a<asym_bins; a++) 
    {
      for(Int_t p=0; p<pt_bins0; p++)
      {
        sprintf(asym_name[a][p],"/%s/asym_a%d_g0_p%d_e%d",dir_title[a],a,p,specificEbin);
        sprintf(asym_title[a][p],"p_{#perp} #in [%.2f,%.2f) and E #in [%.2f,%.2f)",
                                  pt_div[p],pt_div[p+1],en_div[specificEbin],en_div[specificEbin+1]);
        asym_hist[a][p] = (TH1D*) asym_tfile->Get(asym_name[a][p]);
        asym_hist[a][p]->SetTitle(asym_title[a][p]);
        asym_hist[a][p]->GetXaxis()->SetTitle("#phi");
        asym_hist[a][p]->GetYaxis()->SetTitle(asymmetry[a]);
        asym_hist[a][p]->GetXaxis()->SetLabelSize(0.05);
        asym_hist[a][p]->GetYaxis()->SetLabelSize(0.05);
        asym_hist[a][p]->GetXaxis()->SetTitleSize(0.06);
        asym_hist[a][p]->GetYaxis()->SetTitleSize(0.06);
        asym_hist[a][p]->GetYaxis()->SetTitleOffset(0.8);
      };
      sprintf(asym_main_title[a],"%s %s vs #phi",jtype_str,asymmetry_w[a]);
    };
  };


  // canvas sizes
  Float_t hsize = 800;
  Float_t vsize = 1000;


  // draw asym kin dep canvas
  TCanvas * kindep_canv = new TCanvas("canv_kindep","canv_kindep",hsize,vsize);
  TPad * kindep_pad[asym_bins];
  char kindep_pad_n[asym_bins][8];
  TLine * zero_line[asym_bins];
  Float_t padding = 0.05;
  Float_t extra_bottom = 0.04;
  Float_t extra_left = 0.1;
  Float_t interval = (1-2*padding-extra_bottom)/(asym_bins-1);
  TPaveText * kindep_pave = new TPaveText(0.25,0.96,0.75,0.99,"br");
  kindep_pave->AddText(kindep_main_title);
  for(Int_t a=1; a<asym_bins; a++)
  {
    zero_line[a] = new TLine(kindep_gr[a]->GetXaxis()->GetXmin(),0,kindep_gr[a]->GetXaxis()->GetXmax(),0);
    zero_line[a]->SetLineColor(kCyan+3);
    zero_line[a]->SetLineWidth(3);
    zero_line[a]->SetLineStyle(2);
    sprintf(kindep_pad_n[a],"kpad%d",a);
    if(a==asym_bins-1)
      kindep_pad[a] = new TPad(kindep_pad_n[a],kindep_pad_n[a],
        padding,padding+(asym_bins-a-1)*interval,
        1-padding,padding+extra_bottom+(asym_bins-a)*interval,
        0,0);
    else
      kindep_pad[a] = new TPad(kindep_pad_n[a],kindep_pad_n[a],
        padding,padding+extra_bottom+(asym_bins-a-1)*interval,
        1-padding,padding+extra_bottom+(asym_bins-a)*interval,
        0,0);
    if(a==asym_bins-1) 
    {
      kindep_pad[a]->SetTopMargin(0);
      kindep_pad[a]->SetBottomMargin(extra_bottom/(interval+extra_bottom));
    }
    else 
    {
      kindep_pad[a]->SetBottomMargin(0);
      kindep_pad[a]->SetTopMargin(0);
    };
    kindep_pad[a]->SetLeftMargin(extra_left);
    kindep_pad[a]->SetGrid(1,1);
    kindep_pad[a]->Draw();
    kindep_pad[a]->cd();
    kindep_gr[a]->Draw("APE"); 
    zero_line[a]->Draw();
    kindep_canv->cd();
  };
  kindep_canv->cd();
  kindep_pave->Draw();


  // draw analysing power canvases
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  TCanvas * asym_canv[asym_bins];
  char asym_canv_n[asym_bins][32];
  TPad * asym_pad[asym_bins][num_bins0];
  char asym_pad_n[asym_bins][num_bins0][8];
  interval = (1-2*padding-extra_bottom)/(num_bins0);
  TPaveText * asym_pave[asym_bins];
  for(Int_t a=1; a<asym_bins; a++) 
  {
    sprintf(asym_canv_n[a],"canv_%s",asymmetry[a]);
    asym_canv[a] = new TCanvas(asym_canv_n[a],asym_canv_n[a],hsize,num_bins0*vsize/(asym_bins-1));
    asym_pave[a] = new TPaveText(0.25,0.96,0.75,0.99,"br");
    asym_pave[a]->AddText(asym_main_title[a]);
    for(Int_t n=0; n<num_bins0; n++)
    {
      sprintf(asym_pad_n[a][n],"p%d_%d",a,n);
      if(n==num_bins0-1)
        asym_pad[a][n] = new TPad(asym_pad_n[a][n],asym_pad_n[a][n],
        padding,padding+(num_bins0-n-1)*interval,
        1-padding,padding+extra_bottom+(num_bins0-n)*interval,
        0,0);
      else
        asym_pad[a][n] = new TPad(asym_pad_n[a][n],asym_pad_n[a][n],
        padding,padding+extra_bottom+(num_bins0-n-1)*interval,
        1-padding,padding+extra_bottom+(num_bins0-n)*interval,
        0,0);
      if(n==num_bins0-1) 
      {
        asym_pad[a][n]->SetTopMargin(0);
        asym_pad[a][n]->SetBottomMargin(extra_bottom/(interval+extra_bottom));
      }
      else 
      {
        asym_pad[a][n]->SetBottomMargin(0);
        asym_pad[a][n]->SetTopMargin(0);
      };
      asym_pad[a][n]->SetLeftMargin(extra_left);
      asym_pad[a][n]->SetGrid(1,1);
      asym_pad[a][n]->Draw();
      asym_pad[a][n]->cd();
      asym_hist[a][n]->Draw();
      asym_canv[a]->cd();
    };
    asym_canv[a]->cd();
    asym_pave[a]->Draw();
  };

  char kindep_canv_png[128];
  sprintf(kindep_canv_png,"canv_kindep.%s",filetype);
  kindep_canv->Print(kindep_canv_png,filetype);
  char asym_canv_png[asym_bins][128];
  for(Int_t a=1; a<asym_bins; a++) 
  {
    sprintf(asym_canv_png[a],"%s.%s",asym_canv_n[a],filetype);
    asym_canv[a]->Print(asym_canv_png[a],filetype);
  };
}
