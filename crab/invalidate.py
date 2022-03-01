#!/usr/bin/python3
# -*- coding: utf-8 -*-

import os

with open('invalidate.txt') as datasets:
    for dataset in datasets:
        print(dataset)
        os.system('python2 $DBS3_CLIENT_ROOT/examples/DataOpsScripts/DBS3SetDatasetStatus.py --status=INVALID --dataset="{}" --url="https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter" --recursive=False'.format(dataset))





