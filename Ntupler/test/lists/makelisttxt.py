import os

dirn = '/cms/data/store/user/chosila/SUSY_GluGluH_01J_HToAATo4B_Pt150_mH-70_mA-12_wH-70_wA-70_TuneCP5_13TeV_madgraph_pythia8'
rootf = []

with open('tmp.txt', 'w+') as f:
    for root, dirs, files in os.walk(dirn, topdown=False):
        for name in files:
            line = os.path.join(root, name)
            print(line)
            f.write(f'file:{line}\n')
            
    #for name in dirs:
   #   print(os.path.join(root, name))

