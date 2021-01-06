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
    
    
    std::array<std::vector<int>,wbwbx::JetChargeLabel::BHadronTypes.size()> bhadronType;
    
    std::array<std::vector<int>,wbwbx::JetChargeLabel::HadronDecays.size()> bhadronDecay;
    std::array<std::vector<int>,wbwbx::JetChargeLabel::HadronDecays.size()> chadronDecay;
    std::array<std::vector<int>,wbwbx::JetChargeLabel::HadronDecays.size()> dhadronDecay;
    
    std::vector<int> bHadronCharge;
    std::vector<int> bPartonCharge;
    
    std::vector<int> partonFlavor;
    std::vector<int> hadronFlavor;
    
    std::vector<float> matchedGenJetDeltaR;
    std::vector<float> matchedGenJetPt;
        
    std::vector<float> matchedBHadronDeltaR;
    std::vector<float> matchedBHadronPt;
    
            
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
        
        for (size_t i = 0; i < wbwbx::JetChargeLabel::BHadronTypes.size(); ++i)
        {
            bhadronType[i].push_back(labels.bhadronType == wbwbx::JetChargeLabel::BHadronTypes[i] ? 1 : 0);
        }
        
        for (size_t i = 0; i < wbwbx::JetChargeLabel::HadronDecays.size(); ++i)
        {
            bhadronDecay[i].push_back(labels.bhadronDecay == wbwbx::JetChargeLabel::HadronDecays[i] ? 1 : 0);
            chadronDecay[i].push_back(labels.chadronDecay == wbwbx::JetChargeLabel::HadronDecays[i] ? 1 : 0);
            dhadronDecay[i].push_back(labels.dhadronDecay == wbwbx::JetChargeLabel::HadronDecays[i] ? 1 : 0);
        }
 
    }

    
    jetOriginTable->addColumn<int>("jetIdx", truth_jetIdx, "doc", nanoaod::FlatTable::IntColumn);
    
    for (size_t i = 0; i < wbwbx::JetChargeLabel::BHadronTypes.size(); ++i)
    {
        jetOriginTable->addColumn<int>("BHadronType_"+wbwbx::JetChargeLabel::typeToString(wbwbx::JetChargeLabel::BHadronTypes[i]), bhadronType[i], "doc", nanoaod::FlatTable::IntColumn);
    }
    
    for (size_t i = 0; i < wbwbx::JetChargeLabel::HadronDecays.size(); ++i)
    {
        jetOriginTable->addColumn<int>("BHadronDecay_"+wbwbx::JetChargeLabel::typeToString(wbwbx::JetChargeLabel::HadronDecays[i]), bhadronDecay[i], "doc", nanoaod::FlatTable::IntColumn);
        jetOriginTable->addColumn<int>("CHadronDecay_"+wbwbx::JetChargeLabel::typeToString(wbwbx::JetChargeLabel::HadronDecays[i]), chadronDecay[i], "doc", nanoaod::FlatTable::IntColumn);
        jetOriginTable->addColumn<int>("DHadronDecay_"+wbwbx::JetChargeLabel::typeToString(wbwbx::JetChargeLabel::HadronDecays[i]), dhadronDecay[i], "doc", nanoaod::FlatTable::IntColumn);
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

