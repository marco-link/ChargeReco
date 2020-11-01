#ifndef ChargeReco_DataFormats_LLPLabel_h
#define ChargeReco_DataFormats_LLPLabel_h

#include "DataFormats/BTauReco/interface/FeaturesTagInfo.h"

namespace wbwbx {

class JetChargeLabel {

    public:

        enum class Type {
            isB_pos,
            isB_neg,
            isUndefined
        };
        
        Type type;
        
        int jetIdx;
        int partonFlavor;
        int hadronFlavor;
        
        float matchedGenJetDeltaR;
        float matchedGenJetPt;
        
        JetChargeLabel():
            type(Type::isUndefined),
            partonFlavor(0),
            hadronFlavor(0),
            matchedGenJetDeltaR(-1),
            matchedGenJetPt(-1)
        {
        }
        
        inline static const std::string typeToString(const Type& type)
        {
            switch (type)
            {
                case Type::isB_pos:
                    return "isB_pos";
                case Type::isB_neg:
                    return "isB_neg";
                case Type::isUndefined:
                    return "isUndefined";
            }
            return "isUndefined";
        }
};



}

#endif
