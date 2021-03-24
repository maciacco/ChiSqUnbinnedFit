#ifndef CHI2_UNBINNED_FIT
#define CHI2_UNBINNED_FIT

#include <TH1D.h>

#include <RooDataSet.h>
#include <RooAbsPdf.h>
#include <RooRealVar.h>
#include <RooArgList.h>

Double_t ChiSqUnbinnedFit(RooDataSet *data,RooAbsPdf *model,RooRealVar *var,RooArgList modelPars,Int_t nBins);

#endif // CHI2_UNBINNED_FIT
