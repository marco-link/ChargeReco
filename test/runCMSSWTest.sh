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
    mkdir -p ChargeReco
    rsync -r --stats /scripts/ ChargeReco/. || return 1
    echo
    echo "==================== compiling ================"
    scram b || return 1
    wget -nv https://raw.githubusercontent.com/WbWbX/test-files/main/RunIISummer20UL_miniAODv2/WbjToLNu_4f_TuneCP5_13TeV-madgraph-pythia8_2016_preVFP.root 
    cmsRun ChargeReco/NANOProducer/test/produceNANO.py inputFiles=file:WbjToLNu_4f_TuneCP5_13TeV-madgraph-pythia8_2016_preVFP.root year=2016preVFP
}

run_test
