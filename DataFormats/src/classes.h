#include "DataFormats/Common/interface/Wrapper.h"
#include "ChargeReco/DataFormats/interface/XTagFeatures.h"
#include "ChargeReco/DataFormats/interface/XTagInfo.h"
#include "ChargeReco/DataFormats/interface/JetChargeLabel.h"

namespace {

    struct dictionary
    {
        std::vector<reco::FeaturesTagInfo<wbwbx::XTagFeatures>> dummy0;
        edm::Wrapper<std::vector<reco::FeaturesTagInfo<wbwbx::XTagFeatures>>> dummy1;

        reco::FeaturesTagInfo<wbwbx::XTagFeatures> dummy2;
        edm::Wrapper<reco::FeaturesTagInfo<wbwbx::XTagFeatures>> dummy3;

        wbwbx::XTagFeatures dummy4;
        wbwbx::JetFeatures dummy5;
        wbwbx::SecondaryVertexFeatures dummy6;
        wbwbx::ChargedCandidateFeatures dummy7;
        wbwbx::NeutralCandidateFeatures dummy8;
        wbwbx::ElectronCandidateFeatures dummy31;
        wbwbx::MuonCandidateFeatures dummy32;

        edm::PtrVector<reco::GenParticle> dummy17;
        edm::Wrapper<edm::PtrVector<reco::GenParticle>> dummy18;
        
        wbwbx::JetChargeLabel dummy19;
        reco::FeaturesTagInfo<wbwbx::JetChargeLabel> dummy20;

        std::vector<reco::FeaturesTagInfo<wbwbx::JetChargeLabel>> dummy21;
        edm::Wrapper<std::vector<reco::FeaturesTagInfo<wbwbx::JetChargeLabel>>> dummy22;

    };

}
