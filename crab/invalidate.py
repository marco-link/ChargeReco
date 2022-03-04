#!/usr/bin/python3
# -*- coding: utf-8 -*-

import os

with open('invalidate.txt') as datasets:
    for dataset in datasets:
        print(dataset)
        #os.system('python2 $DBS3_CLIENT_ROOT/examples/DataOpsScripts/DBS3SetDatasetStatus.py -v --status="INVALID" --dataset="{}" --url="https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter" --recursive=False'.format(dataset))
        command='python2 $DBS3_CLIENT_ROOT/examples/DataOpsScripts/DBS3SetDatasetStatus.py -v --dataset={} --url=https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter --status=INVALID --recursive=False'.format(dataset.rstrip())
        #print(command)
        os.system(command)
