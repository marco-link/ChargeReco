# ChargeReco
* Production of extended nanoAOD ntuples for analysis and training
* On-the-fly tagger evaluation in miniAOD

## Checkout instructions

Using recent CMSSW release which is shipped with all the required software (especially tensorflow C++ v1.6).
```
cmsrel CMSSW_10_2_18
cd CMSSW_10_2_18/src
cmsenv
git clone https://github.com/WbWbX/ChargeReco.git ChargeReco
scram b
```

## Produce extended nanoAOD files

Configuration file for full Run 2 extended nanoAOD ntuple production
```
cmsRun ChargeReco/NANOProducer/test/produceNANO.py 
```

## Brief package overview

* **DataFormats**: Contains classes for all persistent information, input features (XTagFeatures) and labels (JetChargeLabel), to be saved inside edm::Event.
* **NANOProducer**: Producers to create additional FlatTables to be stored inside flat nanoAOD format
* **XTagInfoProducer**: Producers to extract relevant jet features


