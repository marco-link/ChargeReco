#ifndef ChargeReco_DataFormats_NeutralCandidateFeatures_h
#define ChargeReco_DataFormats_NeutralCandidateFeatures_h

namespace wbwbx {

struct NeutralCandidateFeatures 
{
    int npf_jetIdx;
    float npf_ptrel;
    float npf_deta;
    float npf_dphi;
    float npf_deltaR;
    int npf_isGamma;
    float npf_hcal_fraction;
    float npf_drminsv;
    float npf_puppi_weight;
    float npf_relmassdrop;
};

}

#endif
