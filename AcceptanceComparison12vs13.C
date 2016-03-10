// compares pT and E distributions for run 12 and 13

void AcceptanceComparison12vs13(){
  gStyle->SetOptStat(0);
  enum year_enum {k12,k13};
  Int_t year[2] = {12,13};
  Int_t col[2] = {kRed,kBlue};
  Double_t pt_th[2] = {2.5,2.0};
  Double_t en_th[2] = {30,30};
  TString en_t = "E distributions (run12:red  run13:blue)";
  TString pt_t = "p_{T} distributions (run12:red  run13:blue)";

  TString infile_n[2];
  TFile * infile[2];
  TH2D * acc[2];
  TH1D * en[2];
  TH1D * pt[2];
  int r;
  TLine * pt_line[2];
  TLine * en_line[2];
  Int_t binn;
  for(r=0;r<2;r++){
    infile_n[r] = Form("/home/dilks/h%d/root12fms/spin%d/diag.full_range.root",year[r]-8,year[r]);
    infile[r] = new TFile(infile_n[r].Data(),"READ");
    acc[r] = (TH2D*) infile[r]->Get("pi0_en_vs_pt");

    pt[r] = acc[r]->ProjectionX();
    en[r] = acc[r]->ProjectionY();

    pt[r]->SetTitle(pt_t.Data());
    en[r]->SetTitle(en_t.Data());

    pt[r]->SetLineColor(col[r]);
    en[r]->SetLineColor(col[r]);
    pt[r]->SetLineWidth(2);
    en[r]->SetLineWidth(2);

    binn = pt[r]->FindBin(pt_th[r]);
    pt_line[r] = new TLine(pt_th[r],0,pt_th[r],pt[r]->GetBinContent(binn));
    binn = en[r]->FindBin(en_th[r]);
    en_line[r] = new TLine(en_th[r],0,en_th[r],en[r]->GetBinContent(binn));

    pt_line[r]->SetLineColor(col[r]);
    en_line[r]->SetLineColor(col[r]);
    pt_line[r]->SetLineWidth(2);
    en_line[r]->SetLineWidth(2);

    printf("en[%d]->GetNbinsX() = %d\n",year[r],en[r]->GetNbinsX());
    printf("pt[%d]->GetNbinsX() = %d\n",year[r],pt[r]->GetNbinsX());
  };

  TH1D * en_rat = new TH1D("en_rat","run 12 / run 13 E ratio",
    en[k12]->GetNbinsX(),en[k12]->GetXaxis()->GetXmin(),en[k12]->GetXaxis()->GetXmax());
  TH1D * pt_rat = new TH1D("pt_rat","run 12 / run 13 p_{T} ratio"
    ,pt[k12]->GetNbinsX(),pt[k12]->GetXaxis()->GetXmin(),pt[k12]->GetXaxis()->GetXmax());
  en_rat->Divide(en[k12],en[k13]);
  pt_rat->Divide(pt[k12],pt[k13]);
  en_rat->SetLineColor(kGreen+2);
  pt_rat->SetLineColor(kGreen+2);
  en_rat->SetLineWidth(2);
  pt_rat->SetLineWidth(2);

  TLine * pt_unity = new TLine(pt[k12]->GetXaxis()->GetXmin(),1,pt[k12]->GetXaxis()->GetXmax(),1);
  TLine * en_unity = new TLine(en[k12]->GetXaxis()->GetXmin(),1,en[k12]->GetXaxis()->GetXmax(),1);
  pt_unity->SetLineWidth(2);
  en_unity->SetLineWidth(2);
  pt_unity->SetLineColor(kMagenta);
  en_unity->SetLineColor(kMagenta);

  TCanvas * canv_pt = new TCanvas("canv_pt","canv_pt",800,800);
  canv_pt->Divide(1,2);
  for(int c=1;c<=2;c++) canv_pt->GetPad(c)->SetGrid(1,1);
  canv_pt->cd(1);
  pt[k13]->Draw(); pt[k12]->Draw("same");
  for(r=0;r<2;r++){ pt_line[r]->Draw(); };
  canv_pt->cd(2);
  pt_rat->Draw();
  pt_unity->Draw();
  
  TCanvas * canv_en = new TCanvas("canv_en","canv_en",800,800);
  canv_en->Divide(1,2);
  for(int c=1;c<=2;c++) canv_en->GetPad(c)->SetGrid(1,1);
  canv_en->cd(1);
  en[k13]->Draw(); en[k12]->Draw("same");
  for(r=0;r<2;r++){ en_line[r]->Draw(); };
  canv_en->cd(2);
  en_rat->Draw();
  en_unity->Draw();
};
