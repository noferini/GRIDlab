{
  TFile *f = new TFile("out.root");
  TTree *t = (TTree *) f->Get("tree"); 

  // spec: 0=pi+, 1=pi-, 2=K+, 3=K-, 4=p, 5=pbar, ..., 8=phi
  // phi -> K+ K-

  Int_t ispec1 = 2; // K+
  Int_t ispec2 = 3; // K-

  Int_t iev=0;
  Int_t np=0;
  Int_t np1=0;
  Int_t np2=0;
  Float_t etapart[10000];
  Float_t px[10000];
  Float_t py[10000];
  Float_t pz[10000];
  Float_t e[10000];
  Int_t id[10000];

  Int_t i1list[10000];
  Int_t i2list[10000];

  Float_t pt,phi,m;

  TH2F *h = new TH2F("hPhi","Inv mass distr;p_{T} (GeV/c);m_{KK} (GeV/c^{2});N",20,0,5,1000,0.9,1.9);

  for(Int_t i=0;i< t->GetEntries();i++){
    t->GetEvent(i);
    if(t->GetLeaf("ev")->GetValue() == iev){
      // fill particles in the event
      etapart[np]=t->GetLeaf("eta")->GetValue();
      if(TMath::Abs(etapart[np]) < 1){// && t->GetLeaf("mother")->GetValue() == 8){
	pt = t->GetLeaf("pt")->GetValue();
	phi = t->GetLeaf("phi")->GetValue();
	m = t->GetLeaf("mass")->GetValue();
	id[np] = t->GetLeaf("id")->GetValue();

	px[np] = pt*TMath::Cos(phi);
	py[np] = pt*TMath::Sin(phi);
	pz[np] = t->GetLeaf("pz")->GetValue();
	e[np] = TMath::Sqrt(m*m + px[np]*px[np] + py[np]*py[np] + pz[np]*pz[np]);

	if(id[np] == ispec1){
	  i1list[np1]=np;
	  np1++;
	}
	if(id[np] == ispec2){
	  i2list[np2]=np;
	  np2++;
	}

	np++;
      }
    }
    else{
      // process particles filled
      for(Int_t ii=0;ii < np1;ii++){
	for(Int_t jj=0;jj < np2;jj++){
	  pt = (px[i1list[ii]] + px[i2list[jj]]) * (px[i1list[ii]] + px[i2list[jj]]);
	  pt += (py[i1list[ii]] + py[i2list[jj]]) * (py[i1list[ii]] + py[i2list[jj]]);

	  m = (e[i1list[ii]] + e[i2list[jj]]) * (e[i1list[ii]] + e[i2list[jj]]);
	  m -= (pz[i1list[ii]] + pz[i2list[jj]]) * (pz[i1list[ii]] + pz[i2list[jj]]);
	  m -= pt;

	  pt = TMath::Sqrt(pt);
	  m = TMath::Sqrt(m);

	  h->Fill(pt,m);
	}
      }

      // prepare to analyze next event
      iev = t->GetLeaf("ev")->GetValue();
      i--;
      np=0;
      np1=0;
      np2=0;
    }
  }




  h->Draw("colz");

}
