#ifndef SimG4CMS_ShowerLibraryProducer_HcalForwardLibWriter_h
#define SimG4CMS_ShowerLibraryProducer_HcalForwardLibWriter_h

#include <memory>
#include <string>
#include <fstream>
#include <utility>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/CaloHit/interface/HFShowerPhoton.h"
#include "SimDataFormats/CaloHit/interface/HFShowerLibraryEventInfo.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TFile.h"
#include "TTree.h"

class HcalForwardLibWriter : public edm::one::EDAnalyzer<> {
public:
  struct FileHandle {
    std::string name;
    std::string id;
    int momentum;
  };
  explicit HcalForwardLibWriter(const edm::ParameterSet&);
  ~HcalForwardLibWriter() override;

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;
  int readUserData();
  int nbins;
  int nshowers;
  int bsize;
  int splitlevel;
  int compressionAlgo;
  int compressionLevel;

  TFile* theFile;
  TTree* theTree;
  TFile* LibFile;
  TTree* LibTree;

  edm::Service<TFileService> fs;
  std::string theDataFile;
  std::vector<FileHandle> theFileHandle;

  HFShowerLibraryEventInfo evtInfo;
  HFShowerPhotonCollection emColl;
  HFShowerPhotonCollection hadColl;
};
#endif
