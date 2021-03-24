#include <TRandom3.h>

#include <RooArgSet.h>
#include <RooGaussian.h>

#include "ChiSqUnbinnedFit.h"

void Main(){
  gRandom->SetSeed(1);

  // killing RooFit output
  RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  RooMsgService::instance().setSilentMode(true);
  gErrorIgnoreLevel = kError; // Suppressing warning outputs

  // data
  RooRealVar *x=new RooRealVar("x","x",-5.,5.);
  RooDataSet *data=new RooDataSet("data","data",RooArgSet(*x));

  // fill data set
  for(int iE=0;iE<1000;++iE){
    *x=gRandom->Gaus(0,1);
    data->add(RooArgSet(*x));
  }
  RooPlot *xframe=x->frame(100);
  data->plotOn(xframe);

  // model
  RooRealVar mean("mean","mean",-1.,1.);
  RooRealVar sigma("sigma","sigma",0.,2.);
  RooAbsPdf *model=new RooGaussian("model","model",*x,mean,sigma);

  // fit model
  model->fitTo(*data);
  model->plotOn(xframe);
  TCanvas c("c","c");
  xframe->Draw("");
  c.Print("c.png");

  // get chi sq
  Double_t chiSq=ChiSqUnbinnedFit(data,model,x,RooArgList(mean,sigma),100);
  std::cout<<"chi square = "<<chiSq<<std::endl;
}
