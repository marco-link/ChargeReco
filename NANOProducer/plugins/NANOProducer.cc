// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "ChargeReco/DataFormats/interface/JetChargeTagInfo.h"

#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include <Math/Vector4D.h>

#include "FlatTableFiller.h"

//
// class declaration
//

class NANOProducer : public edm::stream::EDProducer<> {
    public:
        explicit NANOProducer(const edm::ParameterSet&);
        ~NANOProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      
        PropertyList<wbwbx::JetFeatures> globalProperties;
        PropertyList<wbwbx::ShallowTagInfoFeatures> csvProperties;
        PropertyList<wbwbx::ChargedCandidateFeatures> cpfProperties;
        PropertyList<wbwbx::NeutralCandidateFeatures> npfProperties;
        PropertyList<wbwbx::SecondaryVertexFeatures> svProperties;
        PropertyList<wbwbx::MuonCandidateFeatures> muonProperties;
        PropertyList<wbwbx::ElectronCandidateFeatures> electronProperties;

    private:
        const edm::EDGetTokenT<edm::View<pat::Jet>> _jet_src;
        const edm::EDGetTokenT<std::vector<reco::JetChargeTagInfo>> _tag_src;
        virtual void beginStream(edm::StreamID) override;
        virtual void produce(edm::Event&, const edm::EventSetup&) override;
        virtual void endStream() override;
};

NANOProducer::NANOProducer(const edm::ParameterSet& iConfig) :
    _jet_src(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcJets"))),
    _tag_src(consumes<std::vector<reco::JetChargeTagInfo>>(iConfig.getParameter<edm::InputTag>("srcTags")))
{
    globalProperties = {
        PROPERTY(wbwbx::JetFeatures, pt, "doc"),
        PROPERTY(wbwbx::JetFeatures, eta, "doc"),
        PROPERTY(wbwbx::JetFeatures, phi, "doc"),
        PROPERTY(wbwbx::JetFeatures, mass, "doc"),
        PROPERTY(wbwbx::JetFeatures, energy, "doc"),

        PROPERTY(wbwbx::JetFeatures, area, "doc"),

        PROPERTY(wbwbx::JetFeatures, beta, "doc"),
        PROPERTY(wbwbx::JetFeatures, dR2Mean, "doc"),
        PROPERTY(wbwbx::JetFeatures, frac01, "doc"),
        PROPERTY(wbwbx::JetFeatures, frac02, "doc"),
        PROPERTY(wbwbx::JetFeatures, frac03, "doc"),
        PROPERTY(wbwbx::JetFeatures, frac04, "doc"),

        PROPERTY(wbwbx::JetFeatures, jetR, "doc"),
        PROPERTY(wbwbx::JetFeatures, jetRchg, "doc"),

        PROPERTY(wbwbx::JetFeatures, n60, "doc"),
        PROPERTY(wbwbx::JetFeatures, n90, "doc"),

        PROPERTY(wbwbx::JetFeatures, chargedEmEnergyFraction, "doc"),
        PROPERTY(wbwbx::JetFeatures, chargedHadronEnergyFraction, "doc"),
        PROPERTY(wbwbx::JetFeatures, chargedMuEnergyFraction, "doc"),
        PROPERTY(wbwbx::JetFeatures, electronEnergyFraction, "doc"),

        PROPERTY(wbwbx::JetFeatures, tau1, "doc"),
        PROPERTY(wbwbx::JetFeatures, tau2, "doc"),
        PROPERTY(wbwbx::JetFeatures, tau3, "doc"),

        PROPERTY(wbwbx::JetFeatures, relMassDropMassAK, "doc"),
        PROPERTY(wbwbx::JetFeatures, relMassDropMassCA, "doc"),
        PROPERTY(wbwbx::JetFeatures, relSoftDropMassAK, "doc"),
        PROPERTY(wbwbx::JetFeatures, relSoftDropMassCA, "doc"),

        PROPERTY(wbwbx::JetFeatures, thrust, "doc"),
        PROPERTY(wbwbx::JetFeatures, sphericity, "doc"),
        PROPERTY(wbwbx::JetFeatures, circularity, "doc"),
        PROPERTY(wbwbx::JetFeatures, isotropy, "doc"),
        PROPERTY(wbwbx::JetFeatures, eventShapeC, "doc"),
        PROPERTY(wbwbx::JetFeatures, eventShapeD, "doc"),

        PROPERTY(wbwbx::JetFeatures, numberCpf, "doc"),
        PROPERTY(wbwbx::JetFeatures, numberNpf, "doc"),
        PROPERTY(wbwbx::JetFeatures, numberSv, "doc"),
        PROPERTY(wbwbx::JetFeatures, numberMuon, "doc"),
        PROPERTY(wbwbx::JetFeatures, numberElectron, "doc")
    
    };
    
    csvProperties = {
        PROPERTY(wbwbx::ShallowTagInfoFeatures, trackSumJetEtRatio, "ratio of track sum transverse energy over jet energy"),
        PROPERTY(wbwbx::ShallowTagInfoFeatures, trackSumJetDeltaR, "pseudoangular distance between jet axis and track fourvector sum"),
        PROPERTY(wbwbx::ShallowTagInfoFeatures, vertexCategory, "category of secondary vertex (Reco, Pseudo, No)"),
        PROPERTY(wbwbx::ShallowTagInfoFeatures, trackSip2dValAboveCharm, "track 2D signed impact parameter of first track lifting mass above charm"),
        PROPERTY(wbwbx::ShallowTagInfoFeatures, trackSip2dSigAboveCharm, "track 2D signed impact parameter significance of first track lifting mass above charm"),
        PROPERTY(wbwbx::ShallowTagInfoFeatures, trackSip3dValAboveCharm, "track 3D signed impact parameter of first track lifting mass above charm"),
        PROPERTY(wbwbx::ShallowTagInfoFeatures, trackSip3dSigAboveCharm, "track 3D signed impact parameter significance of first track lifting mass above charm"),
        PROPERTY(wbwbx::ShallowTagInfoFeatures, jetNTracksEtaRel, "tracks associated to jet for which trackEtaRel is calculated"),
        PROPERTY(wbwbx::ShallowTagInfoFeatures, jetNSelectedTracks, "doc")
    };
    
    cpfProperties = {
        PROPERTY(wbwbx::ChargedCandidateFeatures, ptrel, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, deta, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, dphi, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, deltaR, "doc"),

        PROPERTY(wbwbx::ChargedCandidateFeatures, px, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, py, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, pz, "doc"),
        
        PROPERTY(wbwbx::ChargedCandidateFeatures, charge, "doc"),

        PROPERTY(wbwbx::ChargedCandidateFeatures, trackEtaRel, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackPtRel, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackPPar, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackDeltaR, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackPParRatio, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackPtRatio, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackSip2dVal, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackSip2dSig, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackSip3dVal, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackSip3dSig, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackJetDistVal, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackJetDistSig, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, drminsv, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, vertex_association, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, fromPV, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, puppi_weight, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, track_chi2, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, track_quality, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, track_numberOfValidPixelHits, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, track_pixelLayersWithMeasurement, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, track_numberOfValidStripHits, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, track_stripLayersWithMeasurement, "doc"), 
        PROPERTY(wbwbx::ChargedCandidateFeatures, relmassdrop, "doc"),
        
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackSip2dValSV, "doc"), 
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackSip2dSigSV, "doc"), 
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackSip3dValSV, "doc"), 
        PROPERTY(wbwbx::ChargedCandidateFeatures, trackSip3dSigSV, "doc"), 

        PROPERTY(wbwbx::ChargedCandidateFeatures, matchedMuon, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, matchedElectron, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, matchedSV, "doc"),
        
        PROPERTY(wbwbx::ChargedCandidateFeatures, track_ndof, "doc"),
        PROPERTY(wbwbx::ChargedCandidateFeatures, dZmin, "doc")
    };
    
    
    npfProperties = {
        PROPERTY(wbwbx::NeutralCandidateFeatures, ptrel, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, deta, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, dphi, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, deltaR, "doc"),

        PROPERTY(wbwbx::NeutralCandidateFeatures, px, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, py, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, pz, "doc"),

        PROPERTY(wbwbx::NeutralCandidateFeatures, isGamma, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, hcal_fraction, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, drminsv, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, puppi_weight, "doc"),
        PROPERTY(wbwbx::NeutralCandidateFeatures, relmassdrop, "doc")
    };
    
    svProperties = {
        PROPERTY(wbwbx::SecondaryVertexFeatures, ptrel, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, deta, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, dphi, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, deltaR, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, mass, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, ntracks, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, chi2, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, ndof, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, dxy, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, dxysig, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, d3d, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, d3dsig, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, costhetasvpv, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, enratio, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, vx, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, vy, "doc"),
        PROPERTY(wbwbx::SecondaryVertexFeatures, vz, "doc")
    };
    
    muonProperties = {
        PROPERTY(wbwbx::MuonCandidateFeatures, isGlobal, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, isTight, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, isMedium, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, isLoose, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, isStandAlone, "doc"),

        PROPERTY(wbwbx::MuonCandidateFeatures, ptrel, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, deta, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, dphi, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, px, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, py, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, pz, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, charge, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, energy, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, et, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, deltaR, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, numberOfMatchedStations, "doc"),

        PROPERTY(wbwbx::MuonCandidateFeatures, IP2d, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, IP2dSig, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, IP3d, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, IP3dSig, "doc"),

        PROPERTY(wbwbx::MuonCandidateFeatures, EtaRel, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, dxy, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, dxyError, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, dxySig, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, dz, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, dzError, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, dzSig, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, numberOfValidPixelHits, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, numberOfpixelLayersWithMeasurement, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, numberOfstripLayersWithMeasurement, "doc"), //that does not help. needs to be discussed.

        PROPERTY(wbwbx::MuonCandidateFeatures, chi2, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, ndof, "doc"),

        PROPERTY(wbwbx::MuonCandidateFeatures, caloIso, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, ecalIso, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, hcalIso, "doc"),

        PROPERTY(wbwbx::MuonCandidateFeatures, sumPfChHadronPt, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, sumPfNeuHadronEt, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, Pfpileup, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, sumPfPhotonEt, "doc"),

        PROPERTY(wbwbx::MuonCandidateFeatures, sumPfChHadronPt03, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, sumPfNeuHadronEt03, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, Pfpileup03, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, sumPfPhotonEt03, "doc"),
    
        PROPERTY(wbwbx::MuonCandidateFeatures, timeAtIpInOut, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, timeAtIpInOutErr, "doc"),
        PROPERTY(wbwbx::MuonCandidateFeatures, timeAtIpOutIn, "doc")
    };
    
    
    electronProperties = {
        PROPERTY(wbwbx::ElectronCandidateFeatures, ptrel, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures, deltaR, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,deta, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dphi, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,px, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,py, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,pz, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,charge, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,energy, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,EtFromCaloEn, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures, isEB, "doc"), 
        PROPERTY(wbwbx::ElectronCandidateFeatures, isEE, "doc"), 
        PROPERTY(wbwbx::ElectronCandidateFeatures,ecalEnergy, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures, isPassConversionVeto, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,convDist, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,   convFlags, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,convRadius, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,hadronicOverEm, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,ecalDrivenSeed, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,IP2d, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,IP2dSig, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,IP3d, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,IP3dSig, "doc"),

        PROPERTY(wbwbx::ElectronCandidateFeatures,elecSC_energy, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,elecSC_deta, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,elecSC_dphi, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,elecSC_et, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,elecSC_eSuperClusterOverP, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,scPixCharge, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,superClusterFbrem, "doc"),

        PROPERTY(wbwbx::ElectronCandidateFeatures,eSeedClusterOverP, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,eSeedClusterOverPout, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,eSuperClusterOverP, "doc"),

        // shower shape
        PROPERTY(wbwbx::ElectronCandidateFeatures,sigmaEtaEta, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,sigmaIetaIeta, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,sigmaIphiIphi, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,e5x5, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,e5x5Rel, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,e1x5Overe5x5, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,e2x5MaxOvere5x5, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,r9, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,hcalOverEcal, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,hcalDepth1OverEcal, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,hcalDepth2OverEcal, "doc"),

        // Track-Cluster Matching Attributes
        PROPERTY(wbwbx::ElectronCandidateFeatures,deltaEtaEleClusterTrackAtCalo, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,deltaEtaSeedClusterTrackAtCalo, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,deltaPhiSeedClusterTrackAtCalo, "doc"), 
        PROPERTY(wbwbx::ElectronCandidateFeatures,deltaEtaSeedClusterTrackAtVtx, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,deltaEtaSuperClusterTrackAtVtx, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,deltaPhiEleClusterTrackAtCalo, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,deltaPhiSuperClusterTrackAtVtx, "doc"),

        PROPERTY(wbwbx::ElectronCandidateFeatures,sCseedEta, "doc"),

        // electron gsf variables. 
        PROPERTY(wbwbx::ElectronCandidateFeatures,EtaRel, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dxy, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dxyError, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dxySig, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dz, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dzError, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dzSig, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures, nbOfMissingHits, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,gsfCharge, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,   ndof, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,chi2, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,   numberOfBrems, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,fbrem, "doc"),

        // Isolation block
        PROPERTY(wbwbx::ElectronCandidateFeatures,neutralHadronIso, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,particleIso, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,photonIso, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,puChargedHadronIso, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,trackIso, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,ecalPFClusterIso, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,hcalPFClusterIso, "doc"),

        PROPERTY(wbwbx::ElectronCandidateFeatures,pfSumPhotonEt, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,pfSumChargedHadronPt, "doc"), 
        PROPERTY(wbwbx::ElectronCandidateFeatures,pfSumNeutralHadronEt, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,pfSumPUPt, "doc"),

        PROPERTY(wbwbx::ElectronCandidateFeatures,dr04TkSumPt, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dr04EcalRecHitSumEt, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dr04HcalDepth1TowerSumEt, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dr04HcalDepth1TowerSumEtBc, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dr04HcalDepth2TowerSumEt, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dr04HcalDepth2TowerSumEtBc, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dr04HcalTowerSumEt, "doc"),
        PROPERTY(wbwbx::ElectronCandidateFeatures,dr04HcalTowerSumEtBc, "doc")
    
    };

    produces<nanoaod::FlatTable>("global");
    produces<nanoaod::FlatTable>("csv");
    produces<nanoaod::FlatTable>("cpf");
    produces<nanoaod::FlatTable>("npf");
    produces<nanoaod::FlatTable>("sv");
    produces<nanoaod::FlatTable>("length");
    produces<nanoaod::FlatTable>("muon") ;
    produces<nanoaod::FlatTable>("electron") ;
}


NANOProducer::~NANOProducer()
{
}


// ------------ method called to produce the data  ------------
void
NANOProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    edm::Handle<edm::View<pat::Jet>> jets;
    iEvent.getByToken(_jet_src, jets);
    edm::Handle<std::vector<reco::JetChargeTagInfo>> tag_infos;
    iEvent.getByToken(_tag_src, tag_infos);

    unsigned int ntags = tag_infos->size();

    std::vector<int> global_jetIdx;
    std::vector<int> csv_jetIdx;
    std::vector<int> cpf_jetIdx;
    std::vector<int> npf_jetIdx;
    std::vector<int> sv_jetIdx;
    std::vector<int> mu_jetIdx;
    std::vector<int> elec_jetIdx;

    auto lengthTable = std::make_unique<nanoaod::FlatTable>(ntags, "length", false, false);
    std::vector<int> cpf_length;
    std::vector<int> npf_length;
    std::vector<int> sv_length;
    std::vector<int> elec_length;
    std::vector<int> mu_length;
    
    std::vector<int> cpf_offset;
    std::vector<int> npf_offset;
    std::vector<int> sv_offset;
    std::vector<int> elec_offset;
    std::vector<int> mu_offset;

    auto globalTable = std::make_unique<nanoaod::FlatTable>(ntags, "global", false, false);
    auto csvTable = std::make_unique<nanoaod::FlatTable>(ntags, "csv", false, false);

    

    FlatTableFillerList<wbwbx::JetFeatures> globalFillerList(globalProperties);
    FlatTableFillerList<wbwbx::ShallowTagInfoFeatures> csvFillerList(csvProperties);
    
    FlatTableFillerList<wbwbx::ChargedCandidateFeatures> cpfFillerList(cpfProperties);
    FlatTableFillerList<wbwbx::NeutralCandidateFeatures> npfFillerList(npfProperties);
    FlatTableFillerList<wbwbx::SecondaryVertexFeatures> svFillerList(svProperties);
    FlatTableFillerList<wbwbx::MuonCandidateFeatures> muonFillerList(muonProperties);
    FlatTableFillerList<wbwbx::ElectronCandidateFeatures> electronFillerList(electronProperties);

    
    unsigned int ncpf_total = 0;
    unsigned int nnpf_total = 0;
    unsigned int nsv_total = 0;
    unsigned int nmu_total = 0;
    unsigned int nelec_total = 0;

    for (unsigned int itag= 0; itag < ntags; itag++){
        const auto& features = tag_infos->at(itag).features();
        
        unsigned int ncpf = features.cpf_features.size();
        unsigned int nnpf = features.npf_features.size();
        unsigned int nsv = features.sv_features.size();
        unsigned int nmu = features.mu_features.size();
        unsigned int nelec = features.elec_features.size();
    
        cpf_offset.push_back(ncpf_total);
        npf_offset.push_back(nnpf_total);
        sv_offset.push_back(nsv_total);
        mu_offset.push_back(nmu_total);
        elec_offset.push_back(nelec_total);

        ncpf_total += ncpf;
        nnpf_total += nnpf;
        nsv_total += nsv;
        nmu_total += nmu;
        nelec_total += nelec;
        
        cpf_length.push_back(ncpf);
        npf_length.push_back(nnpf);
        sv_length.push_back(nsv);
        elec_length.push_back(nelec);
        mu_length.push_back(nmu);

        int jetIdx = -1;
        auto base_jet_ref = tag_infos->at(itag).jet();
        if (base_jet_ref.isAvailable() and base_jet_ref.isNonnull()){
            const auto& base_jet = base_jet_ref.get();
            for (std::size_t ijet = 0; ijet < jets->size(); ijet++) {
                auto jet = jets->at(ijet);
                if (reco::deltaR(base_jet->p4(),jet.p4()) < 1e-4){
                    jetIdx = ijet;
                    break;
                }
            }
        } 
        
        global_jetIdx.push_back(jetIdx);
        csv_jetIdx.push_back(jetIdx);
        
        globalFillerList.push_back(features.jet_features);
        csvFillerList.push_back(features.tag_info_features);
    }

    auto muonTable = std::make_unique<nanoaod::FlatTable>(nmu_total, "muon", false, false);
    auto electronTable = std::make_unique<nanoaod::FlatTable>(nelec_total, "electron", false, false);
    auto cpfTable = std::make_unique<nanoaod::FlatTable>(ncpf_total, "cpf", false, false);
    auto npfTable = std::make_unique<nanoaod::FlatTable>(nnpf_total, "npf", false, false);
    auto svTable = std::make_unique<nanoaod::FlatTable>(nsv_total, "sv", false, false);

    for (unsigned int itag= 0; itag < ntags; itag++)
    {
        const auto& features = tag_infos->at(itag).features();
        auto mu  = features.mu_features;
        auto elec = features.elec_features;
        auto cpf = features.cpf_features;
        auto npf = features.npf_features;
        auto sv = features.sv_features;

        unsigned int nmu = features.mu_features.size();
        unsigned int nelec = features.elec_features.size();
        unsigned int ncpf = features.cpf_features.size();
        unsigned int nnpf = features.npf_features.size();
        unsigned int nsv = features.sv_features.size();

        int jetIdx = global_jetIdx[itag];

        for (unsigned int i = 0; i < ncpf; i++)
        {
            cpf_jetIdx.push_back(jetIdx);
            cpfFillerList.push_back(cpf.at(i));
        }

        for (unsigned int i = 0; i < nnpf; i++)
        {
            npf_jetIdx.push_back(jetIdx);          
            npfFillerList.push_back(npf.at(i));
        }

        for (unsigned int i = 0; i < nsv; i++)
        {
            sv_jetIdx.push_back(jetIdx);
            svFillerList.push_back(sv.at(i));
        }

        for(unsigned int i = 0; i < nmu; i++)
        {
            mu_jetIdx.push_back(jetIdx);
            muonFillerList.push_back(mu.at(i));
        }

        for(unsigned int i = 0; i < nelec ; i++)
        {
            elec_jetIdx.push_back(jetIdx);
            electronFillerList.push_back(elec.at(i));
        }

    }

 
    lengthTable->addColumn<int>("cpf", cpf_length, "charged PF candidate track length", nanoaod::FlatTable::IntColumn);
    lengthTable->addColumn<int>("npf", npf_length, "neutral PF candidate length", nanoaod::FlatTable::IntColumn);
    lengthTable->addColumn<int>("sv", sv_length, "secondary vertex (SV) length", nanoaod::FlatTable::IntColumn);
    lengthTable->addColumn<int>("mu", mu_length, "muon length", nanoaod::FlatTable::IntColumn);
    lengthTable->addColumn<int>("ele", elec_length, "electron length", nanoaod::FlatTable::IntColumn);
    
    lengthTable->addColumn<int>("cpf_offset", cpf_offset, "charged PF candidate track offset", nanoaod::FlatTable::IntColumn);
    lengthTable->addColumn<int>("npf_offset", npf_offset, "neutral PF candidate offset", nanoaod::FlatTable::IntColumn);
    lengthTable->addColumn<int>("sv_offset", sv_offset, "secondary vertex (SV) offset", nanoaod::FlatTable::IntColumn);
    lengthTable->addColumn<int>("mu_offset", mu_offset, "muon offset", nanoaod::FlatTable::IntColumn);
    lengthTable->addColumn<int>("ele_offset", elec_offset, "electron offset", nanoaod::FlatTable::IntColumn);
    
    
    globalTable->addColumn<int>("jetIdx", global_jetIdx, "linked jet Idx", nanoaod::FlatTable::IntColumn);
    globalFillerList.fill(globalTable);
    
    csvTable->addColumn<int>("jetIdx", csv_jetIdx, "linked jet Idx", nanoaod::FlatTable::IntColumn);
    csvFillerList.fill(csvTable);
   
    cpfTable->addColumn<int>("jetIdx", cpf_jetIdx, "linked jet Idx", nanoaod::FlatTable::IntColumn);
    cpfFillerList.fill(cpfTable);

    npfTable->addColumn<int>("jetIdx", npf_jetIdx, "linked jet Idx", nanoaod::FlatTable::IntColumn);
    npfFillerList.fill(npfTable);

    svTable->addColumn<int>("jetIdx", sv_jetIdx, "linked jet Idx", nanoaod::FlatTable::IntColumn);
    svFillerList.fill(svTable);
    

    muonTable->addColumn<int>("jetIdx", mu_jetIdx, "linked jet Idx", nanoaod::FlatTable::IntColumn);
    muonFillerList.fill(muonTable);

    electronTable->addColumn<int>("jetIdx", elec_jetIdx, "linked jet Idx", nanoaod::FlatTable::IntColumn);
    electronFillerList.fill(electronTable);
    
    iEvent.put(std::move(globalTable), "global");
    iEvent.put(std::move(csvTable), "csv");
    iEvent.put(std::move(cpfTable), "cpf");
    iEvent.put(std::move(npfTable), "npf");
    iEvent.put(std::move(svTable), "sv");
    iEvent.put(std::move(lengthTable), "length");
    iEvent.put(std::move(muonTable), "muon");
    iEvent.put(std::move(electronTable), "electron");
}

void
NANOProducer::beginStream(edm::StreamID)
{
}

void
NANOProducer::endStream() {
}


void
NANOProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(NANOProducer);
