#!/bin/bash

echo ""
echo "To enable grid proxy, have to first run this from the command line:"
echo "export X509_USER_PROXY=/tmp/x509up_u$(id -u); voms-proxy-init -voms cms -valid 192:00; cp $X509_USER_PROXY x509up"
echo "Type 'Y' if you have already done this:"
read -r yes
echo ""

if [ "$yes" != "Y" ] && [ "$yes" != "y" ] && [ "$yes" != "Yes" ] && [ "$yes" != "yes" ]
then
    echo "You typed ${yes}. Quitting."
    exit 0
fi

echo "Proceeding to submission:"
echo ""

# ## Gluon fusion H-->aa-->4b with same mass(a), 5 jobs per mass point, done in < 1 hour
# for mass in 8.5 9.0 9.5 10.0 10.5 11.0 11.5 12 12.5 13.0 13.5 14.0 14.0 15 16.0 17.0 18.5 20 21.5 23.0 25 27.5 30 32.5 35 37.5 40 42.5 45 47.5 50 52.5 55 57.5 60 62.5
# do
#     nlines=`wc --lines < test/lists/2018/kodiak/HToAATo4B_GluGluH_01J_Pt150_M-${mass}.txt`
#     nparts=5
#     for (( part=0; part<$nparts; part++ ))
#     do
#     	echo "qsub -N HToAATo4B_GluGluH_01J_Pt150_M-${mass} -v NEVT=-1,PARTNUM=${part},NPARTS=${nparts} scripts/runDeepNtuplizerPart.pbs"
#     	qsub -N HToAATo4B_GluGluH_01J_Pt150_M-${mass} -v NEVT=-1,PARTNUM=${part},NPARTS=${nparts} scripts/runDeepNtuplizerPart.pbs
#     done
# done


# ## Gluon fusion H-->aa-->4b with wide mass(H) and mass(a), 5 jobs per pT / mass point, done in < 1 hour
# for Pt in 150 250 350
# do
#     for wA in 70 10
#     do
# 	nlines=`wc --lines < test/lists/2018/kodiak/HToAATo4B_GluGluH_01J_Pt${Pt}_mH-70_mA-12_wH-70_wA-${wA}.txt`
# 	nparts=5
# 	for (( part=0; part<$nparts; part++ ))
# 	do
#     	    echo "qsub -N HToAATo4B_GluGluH_01J_Pt${Pt}_mH-70_mA-12_wH-70_wA-${wA} -v NEVT=-1,PARTNUM=${part},NPARTS=${nparts} scripts/runDeepNtuplizerPart.pbs"
#     	    qsub -N HToAATo4B_GluGluH_01J_Pt${Pt}_mH-70_mA-12_wH-70_wA-${wA} -v NEVT=-1,PARTNUM=${part},NPARTS=${nparts} scripts/runDeepNtuplizerPart.pbs
# 	done
#     done
# done


# ## QCD with GEN level b hadrons, split by HT, 40 jobs per HT range
# for qcd in "BGenFilter" "bEnriched"
# do
#     for ht in "300to500" "500to700" "700to1000" "1000to1500" "1500to2000" "2000toInf"
#     do
# 	nlines=`wc --lines < test/lists/2018/kodiak/QCD_${qcd}_HT${ht}.txt`
# 	nparts=40
# 	if [ $nlines -lt $nparts ]
# 	then
# 	    nparts=$nlines
# 	fi
# 	for (( part=0; part<$nparts; part++ ))
# 	do
# 	    echo "qsub -N QCD_${qcd}_HT${ht} -v NEVT=-1,PARTNUM=${part},NPARTS=${nparts} scripts/runDeepNtuplizerPart.pbs"
# 	    qsub -N QCD_${qcd}_HT${ht} -v NEVT=-1,PARTNUM=${part},NPARTS=${nparts} scripts/runDeepNtuplizerPart.pbs
# 	done
#     done
# done


# ## ttbar Powheg (high stats), 1000 jobs per decay mode
# for decay in Hadronic SemiLep 2L2Nu
# do
#     nlines=`wc --lines < test/lists/2018/xrootd/TTTo${decay}.txt`
#     nparts=1000
#     for (( part=0; part<$nparts; part++ ))
#     do
# 	echo "qsub -N TTTo${decay} -v NEVT=-1,PARTNUM=${part},NPARTS=${nparts} scripts/runDeepNtuplizerPart.pbs"
# 	qsub -N TTTo${decay} -v NEVT=-1,PARTNUM=${part},NPARTS=${nparts} scripts/runDeepNtuplizerPart.pbs
#     done
# done


## ttbar Powheg recovery jobs by file
for decay in Hadronic
do
    idx=139
    nlines=`wc --lines < test/lists/2018/xrootd/TTTo${decay}.txt`
    nparts=200
    echo $idx
    echo $nlines
    echo $nparts
    echo $((nlines / nparts))
    for (( line=0; line<=$((nlines / nparts)); line++ ))
    do
	echo "qsub -N TTTo${decay} -v NEVT=-1,IFILE=$((idx + (line * nparts)))  scripts/runDeepNtuplizerFile.pbs"
	qsub -N TTTo${decay} -v NEVT=-1,IFILE=$((idx + (line * nparts)))  scripts/runDeepNtuplizerFile.pbs
    done
done


# for ewk in "ZJetsToQQ"
# do
#     for ht in "800toInf"
#     do
# 	nlines=`wc --lines < test/lists/${ewk}_HT-${ht}.txt`
# 	for (( line=0; line<$nlines; line++ ))
# 	do
# 	    echo "qsub -N ${ewk}_HT-${ht} -v NEVT=-1,IFILE=${line} scripts/runDeepNtuplizerFile.pbs"
# 	    qsub -N ${ewk}_HT-${ht} -v NEVT=-1,IFILE=${line} scripts/runDeepNtuplizerFile.pbs
# 	done
#     done
# done


# # ## Gluon fusion H-->aa-->4b with same mass(a), 1 job per MiniAOD file (old approach)
# # for mass in 12 15 20 25 30 35 40 45 50 55 60
# # do
# #     nlines=`wc --lines < test/lists/2018/kodiak/HToAATo4B_GluGluH_01J_Pt150_M-${mass}.txt`
# #     for (( line=0; line<$nlines; line++ ))
# #     do
# # 	echo "qsub -N HToAATo4B_GluGluH_01J_Pt150_M-${mass} -v NEVT=-1,IFILE=${line} scripts/runDeepNtuplizerFile.pbs"
# # 	qsub -N HToAATo4B_GluGluH_01J_Pt150_M-${mass} -v NEVT=-1,IFILE=${line} scripts/runDeepNtuplizerFile.pbs
# #     done
# # done
