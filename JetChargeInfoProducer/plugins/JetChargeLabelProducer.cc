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
        
        static int getHadronFlavor(int absPdgId)
        {
            if (absPdgId<100)
            {
                if (absPdgId<6) return absPdgId; //parton
                return 0; //not a hadron
            }
            int nq3 = (absPdgId/     10)%10; //quark content
            int nq2 = (absPdgId/    100)%10; //quark content
            int nq1 = (absPdgId/   1000)%10; //quark content
            int nL  = (absPdgId/  10000)%10;
            int n   = (absPdgId/1000000)%10;
            return std::max({nq1,nq2,nq3})+n*10000+(n>0 and nL==9)*100;
        }

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
    
    auto outputJetChargeLabelInfo = std::make_unique<reco::JetChargeLabelInfoCollection>();
    
    for (std::size_t ijet = 0; ijet < jetCollection->size(); ijet++) 
    {
        const pat::Jet& jet = jetCollection->at(ijet);
        edm::RefToBase<reco::Jet> jet_ref(jetCollection->refAt(ijet));
        wbwbx::JetChargeLabel label;
        
        if (jet.genJet())
        {
            label.partonFlavor = jet.partonFlavour();
            label.hadronFlavor = jet.hadronFlavour();
            
            label.matchedGenJetDeltaR = reco::deltaR(jet.p4(),jet.genJet()->p4());
            label.matchedGenJetPt = jet.genJet()->pt();
            
            if (label.partonFlavor==5) label.bPartonCharge = -1; //b = -1/3
            if (label.partonFlavor==-5) label.bPartonCharge = 1; //bbar = +1/3
            
            const reco::JetFlavourInfo& flavourInfo = (*jetFlavourInfoMatchingCollection)[jet_ref];
            
            label.nBHadrons = flavourInfo.getbHadrons().size();
            label.nCHadrons = flavourInfo.getcHadrons().size();
            
            const reco::GenParticle* leadingBHadron(nullptr);
            for (const auto& bhadron: flavourInfo.getbHadrons())
            {
                if (not leadingBHadron or leadingBHadron->pt()<bhadron->pt()) leadingBHadron = bhadron.get();
            }
            
            const reco::GenParticle* leadingCHadron(nullptr);
            for (const auto& chadron: flavourInfo.getcHadrons())
            {
                if (not leadingCHadron or leadingCHadron->pt()<chadron->pt()) leadingCHadron = chadron.get();
            }
            
            if (leadingBHadron)
            {
                label.bHadronId = leadingBHadron->pdgId();
                label.bHadronCharge = leadingBHadron->charge();
                
                label.matchedBHadronDeltaR = reco::deltaR(leadingBHadron->p4(),jet.genJet()->p4());
                label.matchedBHadronPt = leadingBHadron->pt();

                int nC = 0;
                label.bHadronDecay = wbwbx::JetChargeLabel::HadronDecay::Undefined;
                for (const auto& daughter: leadingBHadron->daughterRefVector())
                {
                    int absDaughterId = std::abs(daughter->pdgId());
                    if (absDaughterId==11) label.bHadronDecay = wbwbx::JetChargeLabel::HadronDecay::Electron;
                    if (absDaughterId==13) label.bHadronDecay = wbwbx::JetChargeLabel::HadronDecay::Muon;
                    if (absDaughterId==15) label.bHadronDecay = wbwbx::JetChargeLabel::HadronDecay::Tau;
                    if (getHadronFlavor(absDaughterId)==4)
                    {
                        nC+=1;
                    }
                }
                if (label.bHadronDecay == wbwbx::JetChargeLabel::HadronDecay::Undefined)
                {
                    if (nC>1) label.bHadronDecay = wbwbx::JetChargeLabel::HadronDecay::DiHadronic;
                    else if (nC == 1) label.bHadronDecay = wbwbx::JetChargeLabel::HadronDecay::SingleHadronic;
                    else label.bHadronDecay = wbwbx::JetChargeLabel::HadronDecay::OtherHadronic;
                }
            }
        
        
            if (leadingCHadron)
            {
                label.cHadronId = leadingCHadron->pdgId();
                label.cHadronCharge = leadingCHadron->charge();
                
                label.matchedCHadronDeltaR = reco::deltaR(leadingCHadron->p4(),jet.genJet()->p4());
                label.matchedCHadronPt = leadingCHadron->pt();
                
                int nS = 0;
                label.cHadronDecay = wbwbx::JetChargeLabel::HadronDecay::Undefined;
                
                for (const auto& daughter: leadingCHadron->daughterRefVector())
                {
                    int absDaughterId = std::abs(daughter->pdgId());
                    if (absDaughterId==11) label.cHadronDecay = wbwbx::JetChargeLabel::HadronDecay::Electron;
                    if (absDaughterId==13) label.cHadronDecay = wbwbx::JetChargeLabel::HadronDecay::Muon;
                    if (absDaughterId==15) label.cHadronDecay = wbwbx::JetChargeLabel::HadronDecay::Tau;
                    if (getHadronFlavor(absDaughterId)==3)
                    {
                        nS+=1;
                    }
                }
                if (label.cHadronDecay == wbwbx::JetChargeLabel::HadronDecay::Undefined)
                {
                    if (nS>1) label.cHadronDecay = wbwbx::JetChargeLabel::HadronDecay::DiHadronic;
                    else if (nS == 1) label.cHadronDecay = wbwbx::JetChargeLabel::HadronDecay::SingleHadronic;
                    else label.cHadronDecay = wbwbx::JetChargeLabel::HadronDecay::OtherHadronic;
                }
            }
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

