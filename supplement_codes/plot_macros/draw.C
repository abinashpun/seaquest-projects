{ 
   double mixed_norm = 1.;

   auto c0 = new TCanvas();
  
 
   TFile *forig1 = TFile::Open("vertexed_sorted_run12525.root","read");

   TTree *unmixed_orig = (TTree*) forig1->Get("save_mix");
   auto unmixed_m_orig = new TH1F("unmixed_m_orig","unmixed_m_orig",100,.0,10.);
   unmixed_orig->Draw("fDimuons.mass>>unmixed_m_orig");
   //unmixed_m_orig->Sumw2();



   TFile *forig = TFile::Open("vertexed_mixed_run12525.root","read");
   TTree *mixed_orig = (TTree*) forig->Get("save_mix");

   auto mixed_m_orig = new TH1F("mixed_m_orig","mixed_m_orig",100,.0,10.);
   mixed_orig->Draw("fDimuons.mass>>mixed_m_orig","","sames");
   //mixed_m_orig->Sumw2();
   //mixed_m_orig->Scale(mixed_norm);
   unmixed_m_orig->SetLineColor(1);
   unmixed_m_orig->SetLineWidth(2);
   mixed_m_orig->SetLineColor(3);
   mixed_m_orig->SetLineWidth(2);
   mixed_m_orig->SetLineStyle(2);  



   auto c01 = new TCanvas();
   auto diff_orig = (TH1F*)unmixed_m_orig->Clone();
   //diff_orig->Sumw2();
   diff_orig->Add(mixed_m_orig, -1);
   diff_orig->Draw();

  
 
   TFile *f = TFile::Open("vertexed_embedded_mixed_run12525_pi_hold.root","read");


   TTree *mixed = (TTree*) f->Get("save_mix");
   //auto mixed_m = new TH1F("mixed_m","mixed_m",62,2.8,9);
   auto mixed_m = new TH1F("mixed_m","mixed_m",100,.0,10.);
   auto c1 = new TCanvas();
   mixed->Draw("fDimuons.mass>>mixed_m");
   //mixed_m->Sumw2();
   //mixed_m->Scale(mixed_norm);


  
   TFile *f1 = TFile::Open("vertexed_embedded_sorted_run12525_pi_hold.root","read");
 
   TTree *unmixed = (TTree*) f1->Get("save_mix");
   auto unmixed_m = new TH1F("unmixed_m","unmixed_m",100,0.,10.);
   //auto unmixed_m = new TH1F("unmixed_m","unmixed_m",62,2.8,9);
   auto c2 = new TCanvas();
   unmixed->Draw("fDimuons.mass>>unmixed_m");
   //unmixed_m->Sumw2();

  auto c3 = new TCanvas();
  auto diff = (TH1F*)unmixed_m->Clone();
  //diff->Sumw2();
  diff->Add(mixed_m, -1);
  diff->Draw();
  diff->SetTitle("Unmixed - Mixed");
  diff->SetName("signal from data");

  auto c4 = new TCanvas();
  unmixed_m->Draw();
  mixed_m->Draw("sames");
  //mixed_m->Draw("sames");
  unmixed_m->SetLineColor(1);
  unmixed_m->SetLineWidth(2);
  mixed_m->SetLineColor(3);
  mixed_m->SetLineWidth(2);
  mixed_m->SetLineStyle(2);  
   
   //TFile *f2 = TFile::Open("umixed_mcembed2mixed.root");
   //TFile *f2 = TFile::Open("../vertexed_files/mc_vertexed/vertexed_mc_run12535.root","read");
   TFile *f2 = TFile::Open("vertexed_mc_run12525_pi.root","read");
   //TFile *f2 = TFile::Open("../gmc_files/mc_vertexed/vertexed_mc_run12525.root","read");

   auto c5 = new TCanvas();
   auto orig_mc = new TH1F("orig_mc","orig_mc",100,0.,10.);
   TTree *embed = (TTree*) f2->Get("save_mc");
   diff->Draw();
   embed->Draw("fDimuons.mass>>orig_mc","fEventID<760","sames");
   orig_mc->SetLineColor(kRed+1);
   orig_mc->SetLineStyle(2);
   orig_mc->SetLineWidth(3);
   diff->SetLineColor(4);
   //diff->SetLineStyle(2);
   diff->SetLineWidth(2);


   diff_orig->SetLineColor(4);
   diff_orig->SetLineStyle(2);
   diff_orig->SetLineWidth(2);
   diff_orig->SetFillColor(4);
   diff_orig->SetFillStyle(3004);
   diff_orig->Draw("sames");
   diff_orig->SetName("signal from embedded data");

   auto c6 = new TCanvas();
   //auto add_signal = (TH1F*) diff_orig->Clone();
   auto subt_signal = (TH1F*) diff->Clone();
   embed->Draw("fDimuons.mass>>orig_mc","fEventID<760");
   subt_signal->SetName("signal recovered");
   //add_signal->Add(orig_mc,+1);
   subt_signal->Add(diff_orig,-1);
   //diff->Draw();
   //add_signal->Draw();
   orig_mc->Draw();
   subt_signal->Draw("sames");
   subt_signal->SetLineStyle(1);
   subt_signal->SetLineColor(9);
   //subt_signal->Sumw2();

   
   //diff->Draw("sames");
/*
  auto c7 = new TCanvas();
  auto ratio = (TH1F*) orig_mc->Clone();
  ratio->SetName("ratio");
  //ratio->Sumw2();
  ratio->Add(subt_signal,-1);
  //ratio->Divide(subt_signal);
  //ratio->SetMinimum(-1.5);
  //ratio->SetMaximum(5.0);
  //ratio->Draw("ep");
  //ratio->GetXaxis()->SetRangeUser(5.,7.0);
  //ratio->Fit("pol0");
  //gStyle->SetOptStat(0);
  //gStyle->SetOptFit();
  ratio->Draw("HIST");
   auto l = new TLine(0.0,0.,15.0,0.);
  l->SetLineWidth(2);
  l->Draw("same");
  gStyle->SetOptFit();


  auto c8 =  new TCanvas();
  mixed_m->Draw();
  mixed_m_orig->Draw("same");

  auto c9 =  new TCanvas();
  auto diff_mix = (TH1F*)mixed_m->Clone();
  diff_mix->Add(mixed_m_orig, -1);
  diff_mix->Draw("HIST");
*/
   
}
