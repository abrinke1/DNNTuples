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

for mass in 12 15 20 25 30 35 40 45 50 55 60
do
    nlines==`wc --lines < test/lists/HToAATo4B_GluGluH_01J_Pt150_M-${mass}.txt`
    for (( line=0; line<$nlines-1; line++ ))
    do
	echo "qsub -N HToAATo4B_GluGluH_01J_Pt150_M-${mass} -v NEVT=-1,IFILE=${line} scripts/runDeepNtuplizer.pbs"
	qsub -N HToAATo4B_GluGluH_01J_Pt150_M-${mass} -v NEVT=-1,IFILE=${line} scripts/runDeepNtuplizer.pbs
    done
done

for qcd in "BGenFilter" "bEnriched"
do
    for ht in "500to700" "700to1000" "1000to1500" "1500to2000"
    do
	nlines==`wc --lines < test/lists/QCD_${qcd}_HT${ht}.txt`
	for (( line=0; line<$nlines-1; line++ ))
	do
	    echo "qsub -N QCD_${qcd}_HT${ht} -v NEVT=-1,IFILE=${line} scripts/runDeepNtuplizer.pbs"
	    qsub -N QCD_${qcd}_HT${ht} -v NEVT=-1,IFILE=${line} scripts/runDeepNtuplizer.pbs
	done
    done
done
