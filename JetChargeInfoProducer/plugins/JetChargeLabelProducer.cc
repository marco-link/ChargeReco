// system include files
#include <memory>
#include <vector>
#include <unordered_map>
// user include files

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "ChargeReco/DataFormats/interface/JetChargeLabelInfo.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/angle.h"

#include "DataFormats/PatCandidates/interface/Jet.h"



class JetChargeLabelProducer:
    public edm::stream::EDProducer<>
    
{
    private:
        edm::EDGetTokenT<edm::View<pat::Jet>> jetToken_;
        edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> jetFlavourInfoMatchingToken_;

        virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

    public:
        explicit JetChargeLabelProducer(const edm::ParameterSet&);
        ~JetChargeLabelProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};

JetChargeLabelProducer::JetChargeLabelProducer(const edm::ParameterSet& iConfig):
    jetToken_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcJets"))),
    jetFlavourInfoMatchingToken_(consumes<reco::JetFlavourInfoMatchingCollection>(iConfig.getParameter<edm::InputTag>("customFlavourAssociation"))) 
{
    produces<reco::JetChargeLabelInfoCollection>();
}


JetChargeLabelProducer::~JetChargeLabelProducer()
{
}



// ------------ method called to produce the data  ------------
void
JetChargeLabelProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    edm::Handle<edm::View<pat::Jet>> jetCollection;
    iEvent.getByToken(jetToken_, jetCollection);
    
    edm::Handle<reco::JetFlavourInfoMatchingCollection> jetFlavourInfoMatchingCollection;
    iEvent.getByToken(jetFlavourInfoMatchingToken_, jetFlavourInfoMatchingCollection);
    
    std::cout<<jetCollection->size()<<", "<<jetFlavourInfoMatchingCollection->size()<<std::endl;
    
    auto outputJetChargeLabelInfo = std::make_unique<reco::JetChargeLabelInfoCollection>();
    
    for (std::size_t ijet = 0; ijet < jetCollection->size(); ijet++) 
    {
        const pat::Jet& jet = jetCollection->at(ijet);
        edm::RefToBase<reco::Jet> jet_ref(jetCollection->refAt(ijet));
        wbwbx::JetChargeLabel label;
        
        if (jet.genJet())
        {

            label.partonFlavor = jet.partonFlavour();
            if (label.partonFlavor==5) label.bPartonCharge = -1; //b = -1/3
            if (label.partonFlavor==-5) label.bPartonCharge = 1; //bbar = +1/3
            
            label.hadronFlavor = jet.hadronFlavour();
            
            const reco::JetFlavourInfo& flavourInfo = (*jetFlavourInfoMatchingCollection)[jet_ref];
            
            for (const auto& bhadron: flavourInfo.getbHadrons())
            {
                std::cout<<bhadron->pt()<<std::endl;
            }
            
            label.matchedGenJetDeltaR = reco::deltaR(jet.p4(),jet.genJet()->p4());
            label.matchedGenJetPt = jet.genJet()->pt();
            
            /*
            if (std::abs(jet.partonFlavour())==5)
            {
                label.type = jet.partonFlavour()>0 ? wbwbx::JetChargeLabel::Type::isB_pos : wbwbx::JetChargeLabel::Type::isB_neg;
            }
  
            else
            {
                label.type = wbwbx::JetChargeLabel::Type::isUndefined;
            }*/
            
        }
        
        outputJetChargeLabelInfo->emplace_back(label,jet_ref);
    }

    iEvent.put(std::move(outputJetChargeLabelInfo));
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetChargeLabelProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(JetChargeLabelProducer);

