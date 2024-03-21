import os 

dirn = '/home/chosila/Projects/CMSSW_10_6_32/src/DeepNTuples/Ntupler/test/lists'
for i, fn in enumerate(os.listdir(dirn)):
    num = i%2
    name, extention = fn.split('.')
    if 'txt' != extention:
        continue
    print(name)
    with open(f'{name}.sh', 'w+') as txtf:
        txt = f'''#conda init bash
# exec bash
source ~cmssoft/shrc
source /home/chosila/.bashrc
source /home/chosila/.bash_profile
# cmssoft
cd /home/chosila/Projects/CMSSW_10_6_32/src/DeepNTuples/Ntupler
cmsenv
scram b 
export X509_USER_PROXY=/cms/data/$USER/.x509_user_proxy
export CUDA_VISIBLE_DEVICES={num}
cmsRun test/Hto4b_DeepNtuplizerAK8.py inputDataset={name} maxEvents=-1'''
        txtf.write(txt)


with open('submitall.sh', 'w+') as f:
    for fn in os.listdir('.'):
        print(fn)
        if ('.sh' in fn) and ('submitall' not in fn) and ('.e' not in fn) and ('.o' not in fn): 
            f.write(f'qsub -q hep -l nodes=1:ppn=18 {fn}\n')
