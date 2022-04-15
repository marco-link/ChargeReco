
import os
import subprocess
import argparse


# this will use CRAB client API
from CRABAPI.RawCommand import crabCommand

from WMCore.Configuration import Configuration
from CRABClient.UserUtilities import config, getUsernameFromCRIC


RED   = "\033[1;31m"
BLUE  = "\033[1;34m"
GREEN = "\033[0;32m"
RESET = "\033[0;0m"
BOLD    = "\033[;1m"
REVERSE = "\033[;7m"

years = ['2016preVFP', '2016', '2017', '2018']


def verifySample(sample):
    result = subprocess.check_output('dasgoclient --query="dataset dataset={} | grep dataset.status"'.format(sample), shell=True).replace('\n', '').replace(' ', '')

    if result == "VALIDVALID":
        print(GREEN + sample  + RESET)
        return True
    elif result == "PRODUCTION":
        print(BLUE + sample  + RESET)
        return True
    else:
        print(RED + sample + ' is ' + result + RESET)
        return False



parser = argparse.ArgumentParser()

parser.add_argument('--input', '-i', type=str, required=True,
                    help='input txt file containing the sample names (one line each)')

parser.add_argument('--version', '-v', type=str, required=True,
                    help='version tag')

parser.add_argument('--dry', action='store_true',
                    help='only generate configs, don\'t submit the jobs')

parser.add_argument('--year', '-y', type=str, required=True,
                    help='year tag')

parser.add_argument('--data', action='store_true',
                    help='for data processing')


args = parser.parse_args()
print(args)

tag = os.path.splitext(os.path.basename(args.input))[0]


if args.year not in years:
    raise Exception('year "{}" not found, available options: {}'.format(args.year, ', '.join(years)))


# Configuration

config = Configuration()

config.section_("General")
config.General.workArea = 'crab_' + tag
config.General.transferOutputs = True


config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../NANOProducer/test/produceNANO.py'
config.JobType.outputFiles = ['nanox.root']
#config.JobType.inputFiles = ['']
config.JobType.maxMemoryMB = 2000
#config.JobType.numCores = 1
#config.JobType.sendExternalFolder = True


config.section_("Data")
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = True
config.Data.outLFNDirBase = '/store/user/mlink/WbChargeReco/{}/{}/'.format(args.version, tag)
##config.Data.totalUnits = 2
config.Data.outputDatasetTag = 'WbChargeReco_{}_{}'.format(tag, args.version)
config.Data.allowNonValidInputDataset = False

# lumi maksk from https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiLUM
config.Data.lumiMask = ''
if args.data:
    if '2016' in args.year:
        config.Data.lumiMask = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions16/13TeV/Legacy_2016/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt'
    elif '2017' in args.year:
        config.Data.lumiMask = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions17/13TeV/Legacy_2017/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt'
    elif '2018' in args.year:
        config.Data.lumiMask = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions18/13TeV/Legacy_2018/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt'

config.section_("Site")
config.Site.storageSite = 'T1_DE_KIT_Disk'
#config.Site.blacklist = ['T2_US_*']


config.section_("User")
config.User.voGroup = 'dcms'






with open(args.input, 'r') as samplefile:
    for sample in samplefile:
        sample = sample.strip()

        if not '#' in sample and len(sample.split('/')) == 4:
            # check dataset valid
            if not verifySample(sample):
                continue

            requestName = sample.split('/')[1]
            if args.data:
                requestName = sample.split('-')[0].replace('/', '_')[1:]

            # LHE container not available for diboson samples
            addSignalLHE = 'addSignalLHE=1'
            if args.data or 'WW_' in requestName or 'WZ_' in requestName or 'ZZ_' in requestName:
                addSignalLHE = 'addSignalLHE=0'

            config.JobType.pyCfgParams = ['year={}'.format(args.year), 'isData={}'.format(int(args.data)), addSignalLHE]


            # LHE container not available for diboson samples
            addSignalLHE = 'addSignalLHE=1'
            if 'WW_' in requestName or 'WZ_' in requestName or 'ZZ_' in requestName:
                addSignalLHE = 'addSignalLHE=0'

            config.JobType.scriptArgs = ['year={}'.format(args.year), 'isData={}'.format(int(args.data)), addSignalLHE]


            if 'USER' in sample:
                config.Data.inputDBS = 'phys03'
            else:
                config.Data.inputDBS = 'global'

            if 'ext' in sample:
                requestName = requestName + '_' + sample.split('/')[2].split('_')[-1]

            config.General.requestName = requestName
            config.Data.inputDataset = sample

            # save config
            with open('{}_{}.py'.format(args.year, requestName), 'w') as f:
                print >> f, config


            if not args.dry:
                logpath = 'crab_{}/crab_{}'.format(tag, requestName)
                if os.path.isdir(logpath):
                    print('Already submitted (see "{}"). Skipping!'.format(logpath))
                else:
                    print('submitting {} ...'.format(requestName))

                    result = crabCommand('submit', config = config)
                    print (result)
