
## Navigate to working directory
cd /cms/data/abrinke1/CMSSW/HiggsToAA/ParticleNet/CMSSW_10_6_30/src/DeepNTuples/Ntupler

## Load CMSSW environment
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch/
source $VO_CMS_SW_DIR/cmsset_default.sh
cmsenv

## To enable grid proxy, have to first run this from the command line:
## export X509_USER_PROXY=/tmp/x509up_u$(id -u); voms-proxy-init -voms cms -valid 192:00; cp $X509_USER_PROXY x509up
export X509_USER_PROXY=$PWD/x509up
voms-proxy-info

## Run the script!
echo ""
echo "cmsRun test/Hto4b_DeepNtuplizerAK8.py inputDataset=$1 maxEvents=$2 partNum=$3 numParts=$4"
echo ""
cmsRun test/Hto4b_DeepNtuplizerAK8.py inputDataset=$1 maxEvents=$2 partNum=$3 numParts=$4
