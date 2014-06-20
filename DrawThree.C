// draws A_LL, blue A_L, yellow A_L on one plot

void DrawThree(const char * filetype="png",const char * asym_file="spin.root")
{
  TFile * asym_tfile = new TFile(asym_file,"READ");

  Int_t phi_bins0, eta_bins0, pt_bins0, en_bins0;
  if(gSystem->Getenv("PHI_BINS")==NULL){fprintf(stderr,"ERROR: source env vars\n"); return;};
  sscanf(gSystem->Getenv("PHI_BINS"),"%d",&phi_bins0);
  sscanf(gSystem->Getenv("ETA_BINS"),"%d",&eta_bins0);
  sscanf(gSystem->Getenv("PT_BINS"),"%d",&pt_bins0);
  sscanf(gSystem->Getenv("EN_BINS"),"%d",&en_bins0);

  const Int_t asym_bins=4;
  char plotname[asym_bins][128];
  char plottitle[128];
  if(pt_bins0==1 && en_bins0!=1) 
  {
    strcpy(plotname[1],"/A_L_yellow/en_dep_a1_p0");
    strcpy(plotname[2],"/A_L_blue/en_dep_a2_p0");
    strcpy(plotname[3],"/A_LL/en_dep_a3_p0");
    strcpy(plottitle,"Helicity Asymmetry Comparison vs. E_{#gamma#gamma}");
  }
  else if(pt_bins0!=1 && en_bins0==1) 
  {
    strcpy(plotname[1],"/A_L_yellow/pt_dep_a1_e0");
    strcpy(plotname[2],"/A_L_blue/pt_dep_a2_e0");
    strcpy(plotname[3],"/A_LL/pt_dep_a3_e0");
    strcpy(plottitle,"Helicity Asymmetry Comparison vs. p_{#perp}");
  }
  else if(pt_bins0==1 && en_bins0==1)
  {
    strcpy(plotname[1],"/A_L_yellow/pt_dep_a1_e0");
    strcpy(plotname[2],"/A_L_blue/pt_dep_a2_e0");
    strcpy(plotname[3],"/A_LL/pt_dep_a3_e0");
    strcpy(plottitle,"Helicity Asymmetry Comparison -- single bin");
  }
  else
  {
    printf("\n<><><><><><><><><><>\n\n");
    printf("pt_bins>1 && en_bins>1 ----------> three.png NOT DRAWN\n");
    printf("spin.root produced\n");
    return;
  };


  TGraphErrors * A_LL_gr = (TGraphErrors*) asym_tfile->Get(plotname[3]);
  TGraphErrors * A_Lb_gr = (TGraphErrors*) asym_tfile->Get(plotname[2]);
  TGraphErrors * A_Ly_gr = (TGraphErrors*) asym_tfile->Get(plotname[1]);

  A_LL_gr->Fit("pol0","","",8,15);
  gStyle->SetOptFit(0);

  TMultiGraph * mg = new TMultiGraph();
  A_LL_gr->SetMarkerColor(kRed);
  A_Lb_gr->SetMarkerColor(kBlue);
  A_Ly_gr->SetMarkerColor(kOrange);
  A_LL_gr->SetLineColor(kRed);
  A_Lb_gr->SetLineColor(kBlue);
  A_Ly_gr->SetLineColor(kOrange);



  mg->SetTitle(plottitle);
  //->GetXaxis()->SetTitle("p_{T} (Gev/c)");
  //mg->GetYaxis()->SetTitle("Asymmetry");

  TLegend * leg = new TLegend(0.1,0.9,0.3,0.7);
  leg->AddEntry(A_LL_gr,"A_{LL}","EP");
  leg->AddEntry(A_Lb_gr,"A_{L} (blue)","EP");
  leg->AddEntry(A_Ly_gr,"A_{L} (yellow)","EP");


  // get minimum and maximum range to view
  Double_t min_range, max_range;
  min_range=10000;
  max_range=-10000;
  Double_t tmp,min_tmp,max_tmp;
  Double_t pp;
  for(Int_t p=0; p<A_LL_gr->GetN(); p++)
  {
    A_LL_gr->GetPoint(p,pp,tmp);
    min_tmp = tmp - A_LL_gr->GetErrorY(p);
    max_tmp = tmp + A_LL_gr->GetErrorY(p);
    min_range = (min_tmp<min_range)?min_tmp:min_range;
    max_range = (max_tmp>max_range)?max_tmp:max_range;
  };
  for(Int_t p=0; p<A_Lb_gr->GetN(); p++)
  {
    A_Lb_gr->GetPoint(p,pp,tmp);
    min_tmp = tmp - A_Lb_gr->GetErrorY(p);
    max_tmp = tmp + A_Lb_gr->GetErrorY(p);
    min_range = (min_tmp<min_range)?min_tmp:min_range;
    max_range = (max_tmp>max_range)?max_tmp:max_range;
  };
  for(Int_t p=0; p<A_Ly_gr->GetN(); p++)
  {
    A_Ly_gr->GetPoint(p,pp,tmp);
    min_tmp = tmp - A_Ly_gr->GetErrorY(p);
    max_tmp = tmp + A_Ly_gr->GetErrorY(p);
    min_range = (min_tmp<min_range)?min_tmp:min_range;
    max_range = (max_tmp>max_range)?max_tmp:max_range;
  };
  printf("min_range = %f\n",min_range);
  printf("max_range = %f\n",max_range);

  
  TCanvas * cc = new TCanvas("cc","cc",3*1200,3*1000);
  cc->SetGrid(1,1);
  A_Lb_gr->SetLineWidth(4);
  A_Ly_gr->SetLineWidth(4);
  A_LL_gr->SetLineWidth(4);
  A_Lb_gr->SetMarkerSize(4);
  A_Ly_gr->SetMarkerSize(4);
  A_LL_gr->SetMarkerSize(4);
  A_LL_gr->Print();
  mg->Add(A_Lb_gr);
  mg->Add(A_Ly_gr);
  mg->Add(A_LL_gr);
  mg->Draw("ape");
  //mg->GetXaxis()->SetRangeUser(0,6);
  mg->GetYaxis()->SetRangeUser(min_range-0.001,max_range+0.001); 
  //mg->GetYaxis()->SetRangeUser(-0.05,0.15); // for bx<60
  //mg->GetYaxis()->SetRangeUser(-0.07,0.07); // for bx>=60
  if(en_bins0==1 && pt_bins0==1) mg->GetYaxis()->SetRangeUser(-0.004,0.004);
  //leg->Draw();

  char print_file[32];
  sprintf(print_file,"three.%s",filetype);
  cc->Print(print_file,filetype);
  printf("\n<><><><><><><><><><>\n\n");
  printf("three.png produced\n");
  printf("spin.root produced\n");
}
