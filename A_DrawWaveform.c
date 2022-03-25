

void A_DrawWaveform(){
	//TFile *file = new TFile("../Na22_run01_JunseokLee_waveform.root", "read");
	TFile *file = new TFile("../AdvPhyLab_Co60_waveform.root", "read");
	TTree *tree = (TTree*) file->Get("wavedata");
	const int Entries = tree->GetEntries();

	int target;
	cout << "Enter the target entry (0~"<<Entries<<")"<<endl;
	cin >> target;
	if(target>Entries){
		cout << "out of range" << endl;
		return;
	}



	UInt_t          NHitCh;
	Float_t 	waveform[8][1024];
	tree->SetBranchAddress("NHitCh", &NHitCh);
	tree->SetBranchAddress("waveform", waveform);

	tree->GetEntry(target);


	TCanvas *c1 = new TCanvas("waveform", "waveform", 1800, 1000);
	c1->Divide(4,2);
	TGraph *gr[8];
	for(int ich=0; ich<8; ich++){
		gr[ich] = new TGraph();
	}

	for(int ich=0; ich<NHitCh; ich++){
		for(int jtime=0; jtime<1024; jtime++){
			printf("%d %d %.1f\n", ich, jtime, waveform[ich][jtime]);
			gr[ich]->SetPoint(jtime, (float)jtime, waveform[ich][jtime]);
		}

		c1->cd(1+ich);
		gr[ich]->Draw("al");
	}




}
