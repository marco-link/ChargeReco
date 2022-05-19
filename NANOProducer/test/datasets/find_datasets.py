
import os
import subprocess

RED   = "\033[1;31m"
BLUE  = "\033[1;34m"
GREEN = "\033[0;32m"
RESET = "\033[0;0m"


years = {
        '2016preVFP': 'RunIISummer20UL16MiniAODAPVv2-106X*-v*/MINIAODSIM',
        '2016postVFP':'RunIISummer20UL16MiniAODv2-106X*-v*/MINIAODSIM',
        '2017': 'RunIISummer20UL17MiniAODv2-106X*-v*/MINIAODSIM',
        '2018': 'RunIISummer20UL18MiniAODv2-106X*-v*/MINIAODSIM'
        }


def verifySample(sample):
    result = subprocess.run('dasgoclient --query="dataset status=* dataset={} | grep dataset.status"'.format(sample), shell=True, stdout=subprocess.PIPE).stdout.strip().decode("utf-8")

    if len(result)>0:
        sample = subprocess.run('dasgoclient --query="dataset status=* dataset={}"'.format(sample), shell=True, stdout=subprocess.PIPE).stdout.strip().decode("utf-8")



    if result == "VALID":
        print(GREEN + sample + RESET)
    elif result == "PRODUCTION":
        print(BLUE + sample + RESET)
    else:
        print(RED + sample + ' status is ' + result + RESET)
        result = 'UNDEFINED'

    return result, sample




for year in years.keys():
    with open('mc_{}.txt'.format(year), 'w') as datasetfile:
        print('\n\n' + year)
        datasetfile.write('# this file contains the dataset names for {}\n\n'.format(year))

        with open('pd_mc.txt', 'r') as datasetnames:
            for dataset in datasetnames:
                dataset = dataset.strip()

                if not dataset == '' and dataset[0] == '/':
                    dataset = dataset + '/' + years[year]

                    status, dataset = verifySample(dataset)

                    if not status == 'VALID':
                        datasetfile.write('\n# {}\n# '.format(status))

                    datasetfile.write(dataset)
                    datasetfile.write('\n')
