#ifndef ChargeReco_DataFormats_XTagInfo_h
#define ChargeReco_DataFormats_XTagInfo_h

#include "ChargeReco/DataFormats/interface/XTagFeatures.h"
#include "DataFormats/BTauReco/interface/FeaturesTagInfo.h"

namespace reco {

    typedef FeaturesTagInfo<wbwbx::XTagFeatures> XTagInfo;

    DECLARE_EDM_REFS(XTagInfo)

}

#endif

