{
  TFile f("Galet.root");
  H1_2->Draw("h");
  c1->SetLogy();

  TF1* func = new TF1("func","gaus+[3]*exp([4]*x)");
  func->SetParameter(0,1000.);
  func->SetParameter(1,662.);
  func->SetParameter(2,16.);
  func->SetParameter(3,100.);
  func->SetParameter(4,-0.001);

  H1_2->Fit("func","","",450,750);
}
