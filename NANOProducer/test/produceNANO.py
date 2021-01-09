import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
#from PhysicsTools.NanoAOD.common_cff import *
from Configuration.StandardSequences.Eras import eras
from RecoBTag.Configuration.RecoBTag_cff import *
from Configuration.AlCa.GlobalTag import GlobalTag


options = VarParsing ('analysis')

options.register(
    'isData',
    False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "is data"
)

options.register(
    'addSignalLHE',
    True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "adds LHE weights of signal samples"
)

options.register(
    'year',
    '2016',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "add year file"
)
options.parseArguments() 

if options.year == '2016':
    process = cms.Process('NANO',eras.Run2_2016,eras.run2_nanoAOD_94X2016)
elif options.year == '2017':
    process = cms.Process('NANO',eras.Run2_2017,eras.run2_nanoAOD_94XMiniAODv2)
elif options.year == '2018' or options.year == '2018D':
    process = cms.Process('NANO',eras.Run2_2018,eras.run2_nanoAOD_102Xv1)
else:
    process = cms.Process('NANO',eras.Run2_2016,eras.run2_nanoAOD_94X2016)

print "Selected year: ",options.year

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load('PhysicsTools.NanoAOD.nano_cff')

process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load('TrackingTools/TransientTrack/TransientTrackBuilder_cfi')


if options.isData:
    process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
    dataTier = cms.untracked.string('NANOAOD')
else:
    process.load('SimGeneral.MixingModule.mixNoPU_cfi')
    process.load('Configuration.StandardSequences.MagneticField_cff')
    dataTier = cms.untracked.string('NANOAODSIM')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.options = cms.untracked.PSet()

#2016 dataset: /ST_4f_w_lo/mkomm-miniaod16v3_201024-53f8667ba4b240d5eafd36e71bf34742/USER

files = {
    '2016': {
        "mc": "root://gfe02.grid.hep.ph.ic.ac.uk/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/mkomm/ST/miniaod16v3_201024/ST_4f_w_lo/miniaod16v3_201024/201024_150323/0000/ST_1.root",
        "data": "/store/data/Run2016H/SingleMuon/MINIAOD/17Jul2018-v1/00000/16924A85-4D8C-E811-A51C-A4BF01013F29.root",
    },
    '2017': {
        "data": "/store/data/Run2017E/SingleMuon/MINIAOD/31Mar2018-v1/00000/A6325FCE-1C39-E811-BB22-0CC47A745298.root"
    },
    '2018': {
        "data": "/store/data/Run2018B/SingleMuon/MINIAOD/17Sep2018-v1/60000/FF47BB90-FC1A-CC44-A635-2B8B8C64AA39.root"
    },
    '2018D': {
        "data": "/store/data/Run2018B/SingleMuon/MINIAOD/17Sep2018-v1/60000/FF47BB90-FC1A-CC44-A635-2B8B8C64AA39.root"
    }
}

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(files[options.year]['data'] if options.isData else files[options.year]['mc'])

)
# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('test102X nevts:10000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)


#Output definition
process.NANOAODSIMoutput = cms.OutputModule("NanoAODOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    saveProvenance = cms.untracked.bool(True),
    fakeNameForCrab = cms.untracked.bool(True),
    dataset = cms.untracked.PSet(
        dataTier = dataTier,
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring(
            ['wbwbxnanoAOD_step_mu','wbwbxnanoAOD_step_ele'] if options.isData else ['wbwbxnanoAOD_step']
        ) #only events passing this path will be saved
    ),
    fileName = cms.untracked.string('nano.root'),
    #outputCommands = process.NANOAODSIMEventContent.outputCommands+cms.untracked.vstring(
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep nanoaodFlatTable_*Table_*_*',
        'keep edmTriggerResults_*_*_*',
        'keep nanoaodMergeableCounterTable_*Table_*_*',
        'keep nanoaodUniqueString_nanoMetadata_*_*',
        
        'drop *_caloMetTable_*_*',
        
        'drop *_fatJetTable_*_*',
        'drop *_genJetAK8FlavourTable_*_*',
        'drop *_genJetAK8Table_*_*',
        'drop *_genVisTauTable_*_*',
        'drop *_subJetTable_*_*',
        'drop *_tkMetTable_*_*',
        'drop *_puppiMetTable_*_*',
        'drop *_ttbarCategoryTable_*_*',
        
        'drop *_saJetTable_*_*',
        'drop *_FatJetTable_*_*',
        'drop *_saTable_*_*',
        
        'drop *_rivetMetTable_*_*',
        'drop *_rivetProducerHTXS_*_*',
        
        #'drop *_rivetMetTable_*_*',
    )
)
'''
## Output file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.OUT = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('test.root'),
    outputCommands = cms.untracked.vstring(['keep *'])
)
'''

if options.year == "test":
    options.year = "2016"

if options.isData:
    if options.year == '2016':
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_v12', '')
    if options.year == '2017':
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_v12', '')
    if options.year == '2018':
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_v12', '')
    if options.year == '2018D':
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_Prompt_v15', '')
    jetCorrectionsAK4PFchs = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute','L2L3Residual'], 'None')
else:
    if options.year == '2016':
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_mcRun2_asymptotic_v7', '')
    if options.year == '2017':
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_mc2017_realistic_v7', '')
    if options.year == '2018' or options.year == '2018D':
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_upgrade2018_realistic_v20', '')
    jetCorrectionsAK4PFchs = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None')


from PhysicsTools.NanoAOD.common_cff import *
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection


#produces ShallowTagInfo pfDeepCSVTagInfosXTag
updateJetCollection(
    process,
    labelName = "XTag",
    jetSource = cms.InputTag('updatedJets'),
    jetCorrections = jetCorrectionsAK4PFchs,
    pfCandidates = cms.InputTag('packedPFCandidates'),
    pvSource = cms.InputTag("offlineSlimmedPrimaryVertices"),
    svSource = cms.InputTag('slimmedSecondaryVertices'),
    muSource = cms.InputTag('slimmedMuons'),
    elSource = cms.InputTag('slimmedElectrons'),
    btagInfos = [
        'pfImpactParameterTagInfos',
        'pfInclusiveSecondaryVertexFinderTagInfos',
        'pfDeepCSVTagInfos',
    ],
    btagDiscriminators = ['pfCombinedInclusiveSecondaryVertexV2BJetTags'],
    explicitJTA = False,
)

process.jetChargeTagInfos = cms.EDProducer("JetChargeTagInfoProducer",
    jets = cms.InputTag("updatedJets"),
    muonSrc  = cms.InputTag("slimmedMuons"),
    electronSrc = cms.InputTag("slimmedElectrons"),
    shallow_tag_infos = cms.InputTag('pfDeepCSVTagInfosXTag'),
    vertices = cms.InputTag('offlineSlimmedPrimaryVertices'),
    secondary_vertices = cms.InputTag("slimmedSecondaryVertices")
)

process.nanoTable = cms.EDProducer("NANOProducer",
    srcJets = cms.InputTag("finalJets"),
    srcTags = cms.InputTag("jetChargeTagInfos"),
)

process.nanoGenTable = cms.EDProducer("NANOGenProducer",
    srcJets = cms.InputTag("finalJets"),
    srcLabels = cms.InputTag("jetChargeLabels")
)

#redo ghost clustering since linked gen particles have been discarded
process.patJetPartons = cms.EDProducer('HadronAndPartonSelector',
     src = cms.InputTag("generator"),
     particles = cms.InputTag("prunedGenParticles"),
     partonMode = cms.string("Auto"),
     fullChainPhysPartons = cms.bool(True)
)
process.jetFlavourAssociation = cms.EDProducer("JetFlavourClustering",
     jets = cms.InputTag("updatedJets"),
     bHadrons = cms.InputTag("patJetPartons","bHadrons"),
     cHadrons = cms.InputTag("patJetPartons","cHadrons"),
     partons = cms.InputTag("patJetPartons","physicsPartons"),
     leptons = cms.InputTag("patJetPartons","leptons"),
     jetAlgorithm = cms.string("AntiKt"),
     rParam = cms.double(0.4),
     ghostRescaling = cms.double(1e-18),
     relPtTolerance = cms.double(9999999999), #supress warning since jets are corrected; thus pt match fails
     hadronFlavourHasPriority = cms.bool(False)
)

process.jetChargeLabels = cms.EDProducer(
    "JetChargeLabelProducer",
    srcJets = cms.InputTag("updatedJets"), #need to be same input as for 'jetFlavourAssociation'
    customFlavourAssociation = cms.InputTag("jetFlavourAssociation")
)



process.lheWeightsTable = cms.EDProducer(
    "LHEWeightsProducer",
    lheInfo = cms.VInputTag(cms.InputTag("externalLHEProducer"), cms.InputTag("source")),
    weightGroups = cms.PSet()
)

#mass/width reweighting
process.lheWeightsTable.weightGroups.width = cms.vstring()
for i in range(1,106):
    process.lheWeightsTable.weightGroups.width.append("rwgt_%i"%(i))
    
#PDF NNPDF3.1 NNLO hessian
process.lheWeightsTable.weightGroups.nnpdfhessian = cms.vstring()
for i in range(1048,1149):
    process.lheWeightsTable.weightGroups.nnpdfhessian.append("%i"%(i))
    
#PDF NNPDF3.1 NNLO replicas
process.lheWeightsTable.weightGroups.nnpdfreplica = cms.vstring()
for i in range(1149,1250):
    process.lheWeightsTable.weightGroups.nnpdfreplica.append("%i"%(i))
    
#scale weights
for scaleSet in [
    ['murNominal_mufNominal',range(1001,1006)],
    ['murUp_mufNominal',range(1006,1011)],
    ['murDown_mufNominal',range(1011,1016)],
    ['murNominal_mufUp',range(1016,1021)],
    ['murUp_mufUp',range(1021,1026)],
    ['murDown_mufUp',range(1026,1031)],
    ['murNominal_mufDown',range(1031,1036)],
    ['murUp_mufDown',range(1036,1041)],
    ['murDown_mufDown',range(1041,1046)],
    ['emission',range(1046,1048)],
]:
    
    setattr(process.lheWeightsTable.weightGroups,scaleSet[0],cms.vstring())
    for i in scaleSet[1]:
        getattr(process.lheWeightsTable.weightGroups,scaleSet[0]).append("%i"%(i))
        


# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeData,nanoAOD_customizeMC

#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
if options.isData:
    process = nanoAOD_customizeData(process)
else:
    process = nanoAOD_customizeMC(process)
    


if options.isData:
    process.selectedMuonsForFilter = cms.EDFilter("CandViewSelector",
        src = cms.InputTag("slimmedMuons"),
        cut = cms.string("pt>25 && isGlobalMuon()")
    )
    process.selectedMuonsMinFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag("selectedMuonsForFilter"),
        minNumber = cms.uint32(1)
    )
        
    process.muonFilterSequence = cms.Sequence(
        process.selectedMuonsForFilter+process.selectedMuonsMinFilter
    )


    process.selectedElectronsForFilter = cms.EDFilter("CandViewSelector",
        src = cms.InputTag("slimmedElectrons"),
        cut = cms.string("pt>25")
    )
    process.selectedElectronsMinFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag("selectedElectronsForFilter"),
        minNumber = cms.uint32(1)
    )
        
    process.electronFilterSequence = cms.Sequence(
        process.selectedElectronsForFilter+process.selectedElectronsMinFilter
    )

    process.wbwbxnanoAOD_step_mu = cms.Path(
        process.muonFilterSequence+
        process.nanoSequence+
        process.jetChargeTagInfos+
        process.nanoTable
    )
    process.wbwbxnanoAOD_step_ele = cms.Path(
        process.electronFilterSequence+
        process.nanoSequence+
        process.jetChargeTagInfos+
        process.nanoTable
    )
else:
    process.wbwbxnanoAOD_step = cms.Path(
        process.nanoSequenceMC+
        process.patJetPartons+
        process.jetFlavourAssociation+
        process.jetChargeTagInfos+
        process.jetChargeLabels+
        process.nanoTable+
        process.nanoGenTable
    )
    
    if options.addSignalLHE:
        process.wbwbxnanoAOD_step += process.lheWeightsTable
    
process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODSIMoutput_step = cms.EndPath(process.NANOAODSIMoutput)




if options.isData:
    process.schedule = cms.Schedule(process.wbwbxnanoAOD_step_mu,process.wbwbxnanoAOD_step_ele,process.endjob_step,process.NANOAODSIMoutput_step)
else:
    process.schedule = cms.Schedule(process.wbwbxnanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)
    
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


modulesToRemove = [
    'jetCorrFactorsAK8',
    'updatedJetsAK8',
    'tightJetIdAK8',
    'looseJetIdAK8',
    'tightJetIdLepVetoAK8',
    'updatedJetsAK8WithUserData',
    'chsForSATkJets',
    'softActivityJets',
    'softActivityJets2',
    'softActivityJets5',
    'softActivityJets10',
    'finalJetsAK8',
    'fatJetTable',
    'subJetTable',
    'saJetTable',
    'saTable',


    "genJetAK8Table",
    "genJetAK8FlavourAssociation",
    "genJetAK8FlavourTable",
   
    "HTXSCategoryTable",
    "rivetProducerHTXS",
    "genSubJetAK8Table",
    
    "l1bits",
]

#override final jets

#process.finalJets.addBTagInfo=cms.bool(True)
#process.finalJets.addDiscriminators = cms.bool(True)
#process.finalJets.addTagInfos=cms.bool(True)

#remove unneeded modules
for moduleName in modulesToRemove:
    if hasattr(process,moduleName):
        print "removing module: ",moduleName
        if options.isData:
            process.nanoSequence.remove(getattr(process,moduleName))
        else:
            process.nanoSequenceMC.remove(getattr(process,moduleName))
    else:
        print "module for removal not found: ",moduleName

#override final photons (required by object linker) so that ID evaluation is not needed
#process.finalPhotons.cut = cms.string("pt > 5")
#process.finalPhotons.src = cms.InputTag("slimmedPhotons")

    
process.add_(cms.Service('InitRootHandlers', EnableIMT = cms.untracked.bool(False)))
# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
#print process.dumpPython()
# End adding early deletion

#process.endpath= cms.EndPath(process.OUT)
