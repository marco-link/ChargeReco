#ifndef ChargeReco_DataFormats_JetChargeTagFeatures_h
#define ChargeReco_DataFormats_JetChargeTagFeatures_h

#include <vector>
#include "ChargeReco/DataFormats/interface/JetFeatures.h"
#include "ChargeReco/DataFormats/interface/SecondaryVertexFeatures.h"
#include "ChargeReco/DataFormats/interface/ShallowTagInfoFeatures.h"
#include "ChargeReco/DataFormats/interface/NeutralCandidateFeatures.h"
#include "ChargeReco/DataFormats/interface/ChargedCandidateFeatures.h"
#include "ChargeReco/DataFormats/interface/MuonCandidateFeatures.h"
#include "ChargeReco/DataFormats/interface/ElectronCandidateFeatures.h"

namespace wbwbx {

class JetChargeTagFeatures {

  public:
    JetFeatures jet_features;
    ShallowTagInfoFeatures tag_info_features;
    std::vector<SecondaryVertexFeatures> sv_features;
    std::vector<NeutralCandidateFeatures> npf_features;
    std::vector<ChargedCandidateFeatures> cpf_features;
    std::vector<MuonCandidateFeatures> mu_features;
    std::vector<ElectronCandidateFeatures> elec_features;
    
    std::size_t npv;
    
};

}  

#endif
