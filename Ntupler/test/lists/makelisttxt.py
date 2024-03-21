import os

dirn = '/cms/data/store/user/chosila/SUSY_GluGluH_01J_HToAATo4B_Pt150_mH-70_mA-15_wH-70_wA-50_TuneCP5_13TeV_madgraph_pythia8' #'/cms/data/store/user/chosila/SUSY_GluGluH_01J_HToAATo4B_Pt150_mH-70_mA-12_wH-70_wA-70_TuneCP5_13TeV_madgraph_pythia8'

#dirn = '/cms/data/store/user/abrinke1/MiniAOD/Run2_UL/2018/MC/HToAATo4B/*wH-70*'

dirns = [ '/cms/data/store/user/abrinke1/MiniAOD/Run2_UL/2018/MC/HToAATo4B/GluGluH_01J_Pt150_mH-70_mA-12_wH-70_wA-70',
       '/cms/data/store/user/abrinke1/MiniAOD/Run2_UL/2018/MC/HToAATo4B/GluGluH_01J_Pt250_mH-70_mA-12_wH-70_wA-70',
       '/cms/data/store/user/abrinke1/MiniAOD/Run2_UL/2018/MC/HToAATo4B/GluGluH_01J_Pt350_mH-70_mA-12_wH-70_wA-70',
   ]

rootf = []

#with open('tmp.txt', 'w+') as f:
for dirn in dirns:
    fn = dirn.split('/')[-1]
    f = open(f'{fn}.txt', 'w+')
    for root, dirs, files in os.walk(dirn):#, topdown=False):
        for name in files:
            line = os.path.join(root, name)
            print(line)
            f.write(f'file:{line}\n')
            

