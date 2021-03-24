// Chi2UnbinnedFit.cpp
// Compute correct chi2 from unbinned fit

#include <Riostream.h>
#include <TAxis.h>
#include <TArrayD.h>
#include <TCanvas.h>

#include <RooPlot.h>
#include <RooDataHist.h>
#include "ChiSqUnbinnedFit.h"

Double_t ChiSqUnbinnedFit(RooDataSet *data,RooAbsPdf *model,RooRealVar *var,RooArgList modelPars,Int_t nBins){
  // define binning
  TArrayD inBins(nBins+1);
  Double_t delta=(var->getMax()-var->getMin())/nBins;
  for(Int_t iB=0;iB<(nBins+1);++iB){
    inBins[iB]=var->getMin()+iB*delta;
  }

  // fill histogram fro data set
  TH1D* h=(TH1D*)data->createHistogram(var->GetName(),nBins);
  TH1D* hist=new TH1D("hist","hist",nBins,inBins.GetArray());
  for(Int_t iB=1;iB<(nBins+1);++iB){
    hist->SetBinContent(iB,h->GetBinContent(iB));
  }
  // TCanvas c0("c0","c0");
  // hist->Draw("pe");
  // c0.Print("before_rebinning.png");

  // define new binning
  Int_t iB=1;
  TArrayD newBins;
  while(iB<(hist->GetNbinsX()+1)){
    auto bins=hist->GetXaxis()->GetXbins();
    if((hist->GetBinContent(iB))<5.5f){
      newBins.Set(hist->GetNbinsX());
      Int_t iBb=0, iBbb=0;
      while(iBb<(hist->GetNbinsX())){
        if(iBbb!=(iB)){
          if(iBb==(hist->GetNbinsX()-1)){
            newBins.SetAt(var->getMax(),iBb);
          }
          else if(iBb==0){
            newBins.SetAt(var->getMin(),iBb);
          }
          else
            newBins.SetAt(bins->At(iBbb),iBb);
          ++iBb;
          ++iBbb;
        }
        else ++iBbb;
      }
      hist=(TH1D*)hist->Rebin(hist->GetNbinsX()-1,hist->GetName(),newBins.GetArray());
    }
    else ++iB;
  }
  hist=(TH1D*)hist->Rebin(hist->GetNbinsX(),hist->GetName(),newBins.GetArray());
  // TCanvas c1("c1","c1");
  // hist->Draw("pe");
  // c1.Print("after_rebinning.png");

  // compute chi square
  RooDataHist rooHist("roohist","roohist",*var,RooFit::Import(*hist));
  RooPlot* xframe=var->frame();
  rooHist.plotOn(xframe,RooFit::Name("_roohist"));
  model->plotOn(xframe,RooFit::Name("_model"));
  Double_t chi2=xframe->chiSquare("_model","_roohist");

  // TCanvas c2("c2","c2");
  // xframe->Draw("");
  // c2.Print("compute_chi2.png");

  return chi2;
}
