{
  Int_t npeaks = 5;
  TSpectrum * s = new TSpectrum(npeaks);
  Int_t nfound = s->Search(H1_1, 2);
  printf("Found %d candiate peaks \n", nfound);
  //
  Double_t *peaks;
  peaks = s->GetPositionX();
  //
  for ( Int_t  i = 0;  i < nfound;  i++){
    printf(" Energy %6.1f (keV)\n", peaks[i]);
  }
  //
  TH1* hb = s->Background(H1_1,20, "same");
  if (hb) c1->Update();
}
