//
// Dump TH1 dose data to ASCII data file.
//   2013-11-23 T.Aso 
//
void DumpTH1ToAscii(TString rootFileName,TString hName, TString asciiFileName){

  cout << "Input  : "<<rootFileName<<endl;
  cout << "Output : "<<asciiFileName<<endl;
  cout << "TH1    : "<<hName<<endl;

  TFile f(rootFileName);
  TH1* h1 = (TH1*)f.Get(hName); 

  std::ofstream ofs(asciiFileName);

  Int_t nx = h1->GetNbinsX();

  cout << "Nx = "<<nx<<endl;

      for (Int_t ix = 0; ix < nx; ix++){
        Double_t x = h1->GetBinCenter(ix+1);
        Double_t value = h1->GetBinContent(ix+1);
        ofs<< x << "," <<value << endl;;
      }
  ofs.close();
  f.Close();
}
