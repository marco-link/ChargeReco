# ChargeReco

Production of extended nanoAOD ntuples for analysis and training of b jet charge classfier.


## Checkout instructions

Using recent CMSSW release which corresponds to recommended NanoAODv9. See [PPD](https://twiki.cern.ch/twiki/bin/view/CMS/PdmVRun2LegacyAnalysis).
```
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_6_27
cd CMSSW_10_6_27/src
cmsenv
git clone git@github.com:WbWbX/ChargeReco.git -b UL ChargeReco
mkdir TopQuarkAnalysis
cd TopQuarkAnalysis
git clone https://gitlab.cern.ch/CMS-TOPPAG/BFragmentationAnalyzer.git
cd -
scram b
```


## Produce extended nanoAOD files

Configuration file for full Run 2 UL extended nanoAOD ntuple production.

```
cmsRun ChargeReco/NANOProducer/test/produceNANO.py
```

The cfg accepts the following options:
* `isData` specifies if NanoAOD for data or MC is produced. Besides others MC will contain extra truth-level information (default: `False` = MC).
* `year` controls the global tags and eras. Possible values are: `2016`, `2016preVFP`, `2017`, `2018` (default: `2016`).
* `addSignalLHE` extracts and stores weights from the generator that are used to reweight the signal sample to different top quark widths (default: `False`).


## Brief package overview

* **DataFormats**: Contains classes for all persistent information, input features (JetChargeTagFeatures) and labels (JetChargeLabel), to be saved inside edm::Event.
* **NANOProducer**: Producers to create additional FlatTables to be stored inside flat nanoAOD format.
* **JetChargeInfoProducer**: Producers to extract relevant jet features and labels.


