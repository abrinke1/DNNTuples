import os

top_dir = '/cms/data/store/user/abrinke1/MiniAOD/2018/MC/'
# top_dir = '/cms/data/store/user/chosila/'
out_dir = 'tmp/'

if not os.path.exists(out_dir):
    os.makedirs(out_dir)

dir_names = os.walk(top_dir).next()[1]

for dir_name in dir_names:
    print('\nLooking at directory:')
    print(top_dir+dir_name)
    if not 'TuneCP5' in dir_name:
        print('\n\n*** ERROR! TuneCP5 not found in %s! Skipping.\n\n' % (dir_name))
        continue
    root_files = []
    ## Walk down to lowest directory and get ROOT files
    ## https://stackoverflow.com/questions/22919562/get-a-list-of-the-lowest-subdirectories-in-a-tree
    for root,dirs,files in os.walk(top_dir+dir_name):
        if not dirs:
            for rf in os.listdir(root):
                if rf.endswith('.root'):
                    root_files.append(root+'/'+rf)
    if len(root_files) == 0:
        continue
    out_name = out_dir+dir_name.split('_TuneCP5')[0]+'.txt'

    ## Adjust names for simplicity
    out_name = out_name.replace('SUSY_GluGluH_01J_HToAATo4B', 'HToAATo4B_GluGluH_01J')
    if out_name.endswith('_BGenFilter.txt'):
        out_name = (out_name.replace('_BGenFilter','')).replace('QCD','QCD_BGenFilter')
    elif out_name.startswith('QCD_HT'):
        if 'PSWeights' in root_files[0]:
            out_name = out_name.replace('QCD', 'QCD_Inclusive_PSWeights')
        elif 'madgraphMLM' in root_files[0]:
            out_name = out_name.replace('QCD', 'QCD_Inclusive_MLM')
    elif out_name.startswith('TTJets'):
        if 'madgraphMLM' in root_files[0]:
            out_name = out_name.replace('TTJets', 'TTJets_Inclusive_MG')

    out_list = open(out_name, 'w')
    for rf in root_files:
        out_list.write('file:'+rf+'\n')
    print('Wrote output file:')
    print(out_name)

print('\n\nALL DONE!!!\n\n')
