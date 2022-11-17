#! /usr/bin/env python

import os
import ROOT as R

print '\nInside read_files.py'

list_file_name = os.getcwd()+'/test/lists/HToAATo4B_GluGluH_01J_Pt150_M-55.txt'

print '\nTrying to read %s' % list_file_name

with open(list_file_name) as list_file:
    # in_files = [f for f in list_file.read().splitlines() if not f.startswith('#')]
    in_files = [f for f in list_file.read().splitlines() if f.endswith('.root')]

print '\nSuccess! %d files to read' % len(in_files)

chains = []
for in_file in in_files:
    print '\nOpening file %s' % in_file
    ch = R.TChain('Events')
    try:
        ch.Add(in_file)
        print 'Has %d events - success!' % ch.GetEntries()
    except:
        print '\n*** BAD FILE! SKIP! ***\n\n'
    del ch

print '\nAll done!'

