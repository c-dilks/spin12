// compares A_LL determined using VPD vs. ZDC for spin meeting 13 Aug 2015

void CompareAsymByRellum(Int_t year=13)
{
  TString det[2];// [0=vpd, 1=zdc]
  det[0] = "vpd";
  det[1] = "zdc";

  TString filename[2]; 
  TFile * infile[2];
  TGraphErrors * graph[2];
  Int_t i;
  for(i=0; i<2; i++)
  {
    filename[i] = Form("%d-output-%s/spin_pi0.root",year,det[i].Data());
    cout << filename[i] << endl;
    infile[i] = new TFile(filename[i].Data(),"READ");
    infile[i]->cd("A_LL");
    graph[i] = (TGraphErrors*) infile[i]->Get("/A_LL/en_dep_a3_g0_p0");
  };
  
  for(i=0;i<2;i++) printf("%p\n",(void*)graph[i]);

  graph[0]->SetMarkerColor(kRed);
  graph[1]->SetMarkerColor(kBlue);
  graph[0]->SetLineColor(kRed);
  graph[1]->SetLineColor(kBlue);
  for(i=0;i<2;i++) 
  {
    graph[i]->SetMarkerStyle(kFullCircle);
    graph[i]->GetXaxis()->SetLabelSize(0.08);
    graph[i]->GetYaxis()->SetLabelSize(0.08);
  };

  TMultiGraph * mg = new TMultiGraph();
  for(i=0;i<2;i++) mg->Add(graph[i]);
  TString mg_t = Form("A_{LL} vs. E for run %d -- red via VPD -- blue via ZDC",year);
  mg->SetTitle(mg_t.Data());

  TCanvas * c1 = new TCanvas("c1","c1",800,800);
  c1->Divide(1,2);
  c1->cd(1);
  mg->Draw("APE");
  mg->GetXaxis()->SetLabelSize(0.08);
  mg->GetYaxis()->SetLabelSize(0.08);

  TGraph * diff = new TGraphErrors();
  Double_t x[2];
  Double_t y[2];
  for(Int_t k=0;k<graph[0]->GetN();k++)
  {
    for(i=0;i<2;i++) graph[i]->GetPoint(k,x[i],y[i]);
    diff->SetPoint(k,x[0],y[0]-y[1]);
  };

  diff->SetMarkerStyle(kFullCircle);
  diff->SetMarkerColor(kRed);
  diff->SetTitle("A_{LL} via VPD minus A_{LL} for ZDC");

  c1->cd(2);
  diff->GetXaxis()->SetLabelSize(0.08);
  diff->GetYaxis()->SetLabelSize(0.08);
  diff->Fit("pol0","","",10,100);
  gStyle->SetOptFit(1);
  diff->Draw("AP");

};
  
