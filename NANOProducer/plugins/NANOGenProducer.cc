// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "ChargeReco/DataFormats/interface/JetChargeLabel.h"
#include "ChargeReco/DataFormats/interface/JetChargeLabelInfo.h"

#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include <Math/Vector4D.h>




//
// class declaration
//

class NANOGenProducer : public edm::stream::EDProducer<> {
   public:
      explicit NANOGenProducer(const edm::ParameterSet&);
      ~NANOGenProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      const edm::EDGetTokenT<edm::View<pat::Jet>> _jet_src;
      const edm::EDGetTokenT<std::vector<reco::JetChargeLabelInfo>> _label_src;
     
      
      virtual void beginStream(edm::StreamID) override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
};

NANOGenProducer::NANOGenProducer(const edm::ParameterSet& iConfig) :
    _jet_src(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcJets"))),
    _label_src(consumes<std::vector<reco::JetChargeLabelInfo>>(iConfig.getParameter<edm::InputTag>("srcLabels")))
{
    produces<nanoaod::FlatTable>("jetorigin");
}


NANOGenProducer::~NANOGenProducer()
{
}


// ------------ method called to produce the data  ------------
void
NANOGenProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    edm::Handle<edm::View<pat::Jet>> jets;
    iEvent.getByToken(_jet_src, jets);

    edm::Handle<std::vector<reco::JetChargeLabelInfo>> label_infos;
    iEvent.getByToken(_label_src, label_infos);

    std::size_t njets = jets->size();
    std::size_t ntruth = label_infos->size();


    auto jetOriginTable = std::make_unique<nanoaod::FlatTable>(ntruth, "jetorigin", false, false);

    std::vector<int> truth_jetIdx;
    
    std::vector<int> bHadronId;
    std::vector<int> cHadronId;
    
    std::array<std::vector<int>,wbwbx::JetChargeLabel::HadronDecays.size()> bHadronDecay;
    std::array<std::vector<int>,wbwbx::JetChargeLabel::HadronDecays.size()> cHadronDecay;
    
    std::vector<int> nBHadrons;
    std::vector<int> nCHadrons;
    
    std::vector<int> bHadronCharge;
    std::vector<int> cHadronCharge;
    std::vector<int> bPartonCharge;
    
    std::vector<int> partonFlavor;
    std::vector<int> hadronFlavor;
    
    std::vector<float> matchedGenJetDeltaR;
    std::vector<float> matchedGenJetPt;
        
    std::vector<float> matchedBHadronDeltaR;
    std::vector<float> matchedBHadronPt;
    
    std::vector<float> matchedCHadronDeltaR;
    std::vector<float> matchedCHadronPt;
    
            
    for (std::size_t itag = 0; itag < ntruth; itag++) {
    	const auto& labels = label_infos->at(itag).features();
        int jetIdx = -1;
        auto base_jet_ref = label_infos->at(itag).jet();
        if (base_jet_ref.isAvailable() and base_jet_ref.isNonnull()){
        	const auto& base_jet = base_jet_ref.get();
        	for (std::size_t ijet = 0; ijet < njets; ijet++) {
            	auto jet = jets->at(ijet);
            	if (reco::deltaR(base_jet->p4(),jet.p4()) < 1e-4){
                	jetIdx = ijet;
                	break;
            	}
        	}
        } 


        truth_jetIdx.push_back(jetIdx);
        
        bHadronId.push_back(labels.bHadronId);
        cHadronId.push_back(labels.cHadronId);
        
        nBHadrons.push_back(labels.nBHadrons);
        nCHadrons.push_back(labels.nCHadrons);
        
        bHadronCharge.push_back(labels.bHadronCharge);
        cHadronCharge.push_back(labels.cHadronCharge);
        bPartonCharge.push_back(labels.bPartonCharge);
        
        partonFlavor.push_back(labels.partonFlavor);
        hadronFlavor.push_back(labels.hadronFlavor);
        
        matchedGenJetDeltaR.push_back(labels.matchedGenJetDeltaR);
        matchedGenJetPt.push_back(labels.matchedGenJetPt);
        
        matchedBHadronDeltaR.push_back(labels.matchedBHadronDeltaR);
        matchedBHadronPt.push_back(labels.matchedBHadronPt);
        
        matchedCHadronDeltaR.push_back(labels.matchedCHadronDeltaR);
        matchedCHadronPt.push_back(labels.matchedCHadronPt);
        
        for (size_t i = 0; i < wbwbx::JetChargeLabel::HadronDecays.size(); ++i)
        {
            bHadronDecay[i].push_back(labels.bHadronDecay == wbwbx::JetChargeLabel::HadronDecays[i] ? 1 : 0);
            cHadronDecay[i].push_back(labels.cHadronDecay == wbwbx::JetChargeLabel::HadronDecays[i] ? 1 : 0);
        }
    }

    
    jetOriginTable->addColumn<int>("jetIdx", truth_jetIdx, "doc", nanoaod::FlatTable::IntColumn);
    
    jetOriginTable->addColumn<int>("bHadronId", bHadronId, "doc", nanoaod::FlatTable::IntColumn);
    jetOriginTable->addColumn<int>("cHadronId", cHadronId, "doc", nanoaod::FlatTable::IntColumn);
    
    jetOriginTable->addColumn<int>("nBHadrons", nBHadrons, "doc", nanoaod::FlatTable::IntColumn);
    jetOriginTable->addColumn<int>("nCHadrons", nCHadrons, "doc", nanoaod::FlatTable::IntColumn);
    
    jetOriginTable->addColumn<int>("bHadronCharge", bHadronCharge, "doc", nanoaod::FlatTable::IntColumn);
    jetOriginTable->addColumn<int>("cHadronCharge", cHadronCharge, "doc", nanoaod::FlatTable::IntColumn);
    jetOriginTable->addColumn<int>("bPartonCharge", bPartonCharge, "doc", nanoaod::FlatTable::IntColumn);
    
    jetOriginTable->addColumn<int>("partonFlavor", partonFlavor, "doc", nanoaod::FlatTable::IntColumn);
    jetOriginTable->addColumn<int>("hadronFlavor", hadronFlavor, "doc", nanoaod::FlatTable::IntColumn);
    
    jetOriginTable->addColumn<float>("matchedGenJetDeltaR", matchedGenJetDeltaR, "doc", nanoaod::FlatTable::FloatColumn);
    jetOriginTable->addColumn<float>("matchedGenJetPt", matchedGenJetPt, "doc", nanoaod::FlatTable::FloatColumn);
    
    jetOriginTable->addColumn<float>("matchedBHadronDeltaR", matchedBHadronDeltaR, "doc", nanoaod::FlatTable::FloatColumn);
    jetOriginTable->addColumn<float>("matchedBHadronPt", matchedBHadronPt, "doc", nanoaod::FlatTable::FloatColumn);

    jetOriginTable->addColumn<float>("matchedCHadronDeltaR", matchedCHadronDeltaR, "doc", nanoaod::FlatTable::FloatColumn);
    jetOriginTable->addColumn<float>("matchedCHadronPt", matchedCHadronPt, "doc", nanoaod::FlatTable::FloatColumn);

    
    for (size_t i = 0; i < wbwbx::JetChargeLabel::HadronDecays.size(); ++i)
    {
        jetOriginTable->addColumn<int>("bHadronDecay_"+wbwbx::JetChargeLabel::typeToString(wbwbx::JetChargeLabel::HadronDecays[i]), bHadronDecay[i], "doc", nanoaod::FlatTable::IntColumn);
        jetOriginTable->addColumn<int>("cHadronDecay_"+wbwbx::JetChargeLabel::typeToString(wbwbx::JetChargeLabel::HadronDecays[i]), cHadronDecay[i], "doc", nanoaod::FlatTable::IntColumn);
    }
         
    iEvent.put(std::move(jetOriginTable), "jetorigin");
}

void
NANOGenProducer::beginStream(edm::StreamID)
{
}

void
NANOGenProducer::endStream() {
}

 
void
NANOGenProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(NANOGenProducer);

