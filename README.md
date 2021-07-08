# ChargeReco
* Production of extended nanoAOD ntuples for analysis and training
* On-the-fly tagger evaluation in miniAOD

## Checkout instructions

Using recent CMSSW release which corresponds to recommended NanoAODv8 (labelled 'NanoAODv2' in DAS). See [PPD](https://twiki.cern.ch/twiki/bin/view/CMS/PdmVRun2LegacyAnalysis).
```
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_6_19_patch2
cd CMSSW_10_6_19_patch2/src
cmsenv
git clone https://github.com/WbWbX/ChargeReco.git -b UL ChargeReco
scram b
```

## Produce extended nanoAOD files

Configuration file for full Run 2 extended nanoAOD ntuple production
```
cmsRun ChargeReco/NANOProducer/test/produceNANO.py 
```

## Brief package overview

* **DataFormats**: Contains classes for all persistent information, input features (JetChargeTagFeatures) and labels (JetChargeLabel), to be saved inside edm::Event.
* **NANOProducer**: Producers to create additional FlatTables to be stored inside flat nanoAOD format
* **JetChargeInfoProducer**: Producers to extract relevant jet features and labels


