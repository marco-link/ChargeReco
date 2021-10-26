function run_test()
{
    cd ~
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    export SCRAM_ARCH=slc7_amd64_gcc700 || return 1
    echo
    echo "==================== setup CMSSW ==============="
    scramv1 project CMSSW CMSSW_10_6_27 || return 1
    cd CMSSW_10_6_27/src || return 1
    eval `scram runtime -sh` || return 1
    mkdir -p ChargeReco || return 1
    rsync -r --stats /scripts/ ChargeReco/. || return 1
    mkdir -p TopQuarkAnalysis || return 1
    cd TopQuarkAnalysis || return 1
    git clone https://github.com/WbWbX/BFragmentationAnalyzer.git || return 1
    cd $CMSSW_BASE/src || return 1

    echo
    echo "==================== compiling ================"
    scram b || return 1
    wget -nv https://raw.githubusercontent.com/WbWbX/test-files/main/RunIISummer20UL_miniAODv2/WbjToLNu_4f_TuneCP5_13TeV-madgraph-pythia8_2016_preVFP.root  || return 1
    cmsRun ChargeReco/NANOProducer/test/produceNANO.py inputFiles=file:WbjToLNu_4f_TuneCP5_13TeV-madgraph-pythia8_2016_preVFP.root year=2016preVFP || return 1
}

run_test
