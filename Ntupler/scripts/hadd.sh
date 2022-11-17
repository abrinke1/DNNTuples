#!/bin/bash

DIR="/cms/data/store/user/abrinke1/Trees/HtoAAto4B/ParticleNet/Ntuples/2022_11_14/"

cd ${DIR}

for mass in 12 15 20 25 30 35 40 45 50 55 60
do
    echo "hadd -f AK8_HToAATo4B_GluGluH_01J_Pt150_M-${mass}.root AK8_HToAATo4B_GluGluH_01J_Pt150_M-${mass}_file*.root"
    hadd -f AK8_HToAATo4B_GluGluH_01J_Pt150_M-${mass}.root AK8_HToAATo4B_GluGluH_01J_Pt150_M-${mass}_file*.root
done

for qcd in "BGenFilter" "bEnriched"
do
    for ht in "500to700" "700to1000" "1000to1500" "1500to2000"
    do
	echo "hadd -f AK8_QCD_${qcd}_HT${ht}.root AK8_QCD_${qcd}_HT${ht}_file*.root"
	hadd -f AK8_QCD_${qcd}_HT${ht}.root AK8_QCD_${qcd}_HT${ht}_file*.root
    done
done

cd -
