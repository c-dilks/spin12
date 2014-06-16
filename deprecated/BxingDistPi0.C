// takes pi0 sample in redset and builds bXing distribution (number of pi0s)
// for each spin bit, and a sum
//
// this was written to see if there is any structure in the bXing distributions

void BxingDistPi0()
{
  TChain * tc = new TChain("str");
  tc->Add("redset/Red*.root");
  TH1D * bxing_dist[5];
  char bxing_dist_n[5][64];
  char spin_cut[5][128];
  for(Int_t s=0; s<5; s++)
  {
    sprintf(bxing_dist_n[s],"bxing_dist_%d",s);
    bxing_dist[s] = new TH1D(bxing_dist_n[s],
                             "pi0 bXing dist (Grn:-- Orn:-+ Red:+- Blue:++ Blk:all)",
                             120,0,120);
    if(s<4) sprintf(spin_cut[s],"!kicked && abs(M12-0.135)<0.07 && E12>40 && spin==%d",s);
    else sprintf(spin_cut[s],"!kicked && abs(M12-0.135)<0.07 && E12>40 && spin>=0 && spin<=3");
    tc->Project(bxing_dist_n[s],"Bunchid7bit",spin_cut[s]);
  };
  bxing_dist[0]->SetLineColor(kGreen+2);
  bxing_dist[1]->SetLineColor(kOrange+7);
  bxing_dist[2]->SetLineColor(kRed);
  bxing_dist[3]->SetLineColor(kBlue);
  bxing_dist[4]->SetLineColor(kBlack);
  TCanvas * canv = new TCanvas("canv","canv",1100,500);
  bxing_dist[4]->Draw();
  for(Int_t s=0; s<4; s++) bxing_dist[s]->Draw("same");
}



