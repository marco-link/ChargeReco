#ifndef ChargeReco_DataFormats_JetChargeTagInfo_h
#define ChargeReco_DataFormats_JetChargeTagInfo_h

#include "ChargeReco/DataFormats/interface/JetChargeTagFeatures.h"
#include "DataFormats/BTauReco/interface/FeaturesTagInfo.h"

namespace reco {

    typedef FeaturesTagInfo<wbwbx::JetChargeTagFeatures> JetChargeTagInfo;

    DECLARE_EDM_REFS(JetChargeTagInfo)

}

#endif

