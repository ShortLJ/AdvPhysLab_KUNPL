float pedestal(const float waveform[1024]){
	double sum=0;
	for(int i=0; i<80; i++){
		sum+=waveform[i];
	}
	sum/=80.;
	return sum;
}

float rPHf(const float ped, const float waveform[1024]){
	float min=ped;
	for(int i=80; i<950; i++){
		if(min>waveform[i]) min=waveform[i];
	}
	return ped-min;
}

float QDCf(const float ped, const float waveform[1024]){
	float sum=0;
	for(int i=80; i<680; i++){
		sum+=ped-waveform[i];
	}
	return sum;
}








void B_rPH_QDC(){
	TFile *file = new TFile("../AdvPhyLab_Co60_waveform.root", "read");
	TTree *tree = (TTree*) file->Get("wavedata");
	const int Entries = tree->GetEntries();

	UInt_t          NHitCh;
	UInt_t          Channel[8];   //[NHitCh]
	Float_t 	waveform[8][1024];
	tree->SetBranchAddress("NHitCh", &NHitCh);
	tree->SetBranchAddress("Channel", Channel);
	tree->SetBranchAddress("waveform", waveform);


	TCanvas *c1 = new TCanvas("Pulse_Height", "Pulse_Height", 1800, 1000);
	c1->Divide(4,2);
	TCanvas *c2 = new TCanvas("Pulse_Area", "Pulse_Area", 1800, 1000);
	c2->Divide(4,2);

	TH1D *h1_rPH[8];
	TH1D *h1_QDC[8];
	for (int ich=0; ich<8; ich++){
		h1_rPH[ich] = new TH1D(
				Form("h1_rPH_ch%d", ich),
				Form("h1_rPH_ch%d; rPH, count", ich),
				2000, 0, 4000
				);
		h1_QDC[ich] = new TH1D(
				Form("h1_QDC_ch%d", ich),
				Form("h1_QDC_ch%d; QDC, count", ich),
				2000, 0, 300000
				);
	}

	Double_t ped[8];
	Double_t rPH[8];
	Double_t QDC[8];

	TFile *ofile = new TFile("QDC_rPH.root", "recreate");
	TTree *tree2 = new TTree("QDC_rPH", "QDC_rPH");
	tree2->Branch("rPH", rPH, "rPH[8]/F");	
	tree2->Branch("QDC", QDC, "QDC[8]/F");	



	for(int ient=0; ient<Entries; ient++){
		tree->GetEntry(ient);
		for(int ich=0; ich<NHitCh; ich++){
			ped[ich] = pedestal(waveform[ich]);
			rPH[ich] = rPHf(ped[ich], waveform[ich]);
			QDC[ich] = QDCf(ped[ich], waveform[ich]);

			if(rPH[ich]<80) continue;
			h1_rPH[ich]->Fill(rPH[ich]);
			h1_QDC[ich]->Fill(QDC[ich]);
		}
		tree2->Fill();
	}
	
	tree2->Write();



	for(int ich=0; ich<NHitCh; ich++){
		c1->cd(1+ich)->SetLogy();
		h1_rPH[ich]->Draw();
		h1_QDC[ich]->Write();
		c2->cd(1+ich)->SetLogy();
		h1_QDC[ich]->Draw();
		h1_QDC[ich]->Write();
	}


	ofile->Close();


}
