#ifndef ChargeReco_DataFormats_JetChargeLabel_h
#define ChargeReco_DataFormats_JetChargeLabel_h

#include <array>

namespace wbwbx {

class JetChargeLabel {

    public:
        
        int jetIdx;
        
        enum class BHadronType {
            Bpm, //B+ or B-
            B0,
            Bs,
            Bc,
            Undefined //when no b hadron is found
        };
        constexpr static std::array<BHadronType,5> BHadronTypes{{
            BHadronType::Bpm,
            BHadronType::B0,
            BHadronType::Bs,
            BHadronType::Bc,
            BHadronType::Undefined
        }};
        
        enum class HadronDecay {
            Electron,
            Muon,
            Tau,
            Hadronic,
            Undefined //when no b/c/d hadron is found
        };
        constexpr static std::array<HadronDecay,5> HadronDecays{{
            HadronDecay::Electron,
            HadronDecay::Muon,
            HadronDecay::Tau,
            HadronDecay::Hadronic,
            HadronDecay::Undefined
        }};
        
        BHadronType bhadronType;
        
        HadronDecay bhadronDecay;
        HadronDecay chadronDecay;
        HadronDecay dhadronDecay;
        
        int bHadronCharge; //charge of b quark in hadron (particle level); can differ due to oscillations
        int bPartonCharge; //parton-level b quark
        
        int partonFlavor; //generic parton flavour of jet
        int hadronFlavor; //generic hadron flavour of jet
        
        float matchedGenJetDeltaR;
        float matchedGenJetPt;
        
        float matchedBHadronDeltaR;
        float matchedBHadronPt;
        
        JetChargeLabel():
            bhadronType(BHadronType::Undefined),
            bhadronDecay(HadronDecay::Undefined),
            chadronDecay(HadronDecay::Undefined),
            dhadronDecay(HadronDecay::Undefined),
            bHadronCharge(0),
            bPartonCharge(0),
            partonFlavor(0),
            hadronFlavor(0),
            matchedGenJetDeltaR(-1),
            matchedGenJetPt(-1),
            matchedBHadronDeltaR(-1),
            matchedBHadronPt(-1)
        {
        }
        
        inline static std::string typeToString(const BHadronType& bHadronType)
        {
            switch (bHadronType)
            {
                case BHadronType::Bpm: return "Bpm";
                case BHadronType::B0: return "B0";
                case BHadronType::Bs: return "Bs";
                case BHadronType::Bc: return "Bc";
                case BHadronType::Undefined: return "Undefined";
            }
            return "Undefined";
        }
        
        inline static std::string typeToString(const HadronDecay& hadronDecay)
        {
            switch (hadronDecay)
            {
                case HadronDecay::Electron: return "Electron";
                case HadronDecay::Muon: return "Muon";
                case HadronDecay::Tau: return "Tau";
                case HadronDecay::Hadronic: return "Hadronic";
                case HadronDecay::Undefined: return "Undefined";
            }
            return "Undefined";
        }
};



}

#endif
