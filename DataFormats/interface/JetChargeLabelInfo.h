#ifndef ChargeReco_DataFormats_JetChargeLabelInfo_h
#define ChargeReco_DataFormats_JetChargeLabelInfo_h

#include "ChargeReco/DataFormats/interface/JetChargeLabel.h"
#include "DataFormats/BTauReco/interface/FeaturesTagInfo.h"

namespace reco {

typedef FeaturesTagInfo<wbwbx::JetChargeLabel> JetChargeLabelInfo;

DECLARE_EDM_REFS(JetChargeLabelInfo)

}

#endif
