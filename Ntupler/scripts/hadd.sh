#!/bin/bash

DIR="/cms/data/store/user/abrinke1/Trees/HtoAAto4B/ParticleNet/Ntuples/2024_04_01/"

echo $PWD
cd ${DIR}
echo $PWD

# for mass in 8.5 9.0 9.5 10.0 10.5 11.0 11.5 12 12.5 13.0 13.5 14.0 14.0 15 16.0 17.0 18.5 20 21.5 23.0 25 27.5 30 32.5 35 37.5 40 42.5 45 47.5 50 52.5 55 57.5 60 62.5
# do
#     echo "hadd -f AK8_HToAATo4B_GluGluH_01J_Pt150_M-${mass}.root AK8_HToAATo4B_GluGluH_01J_Pt150_M-${mass}_file*.root"
#     hadd -f AK8_HToAATo4B_GluGluH_01J_Pt150_M-${mass}.root AK8_HToAATo4B_GluGluH_01J_Pt150_M-${mass}_part*.root
# done

# for Pt in 150 250 350
# do
#     for wA in 70 10
#     do
# 	echo "hadd -f AK8_HToAATo4B_GluGluH_01J_Pt${Pt}_mH-70_mA-12_wH-70_wA-${wA}.root AK8_HToAATo4B_GluGluH_01J_Pt${Pt}_mH-70_mA-12_wH-70_wA-${wA}_part*.root"
# 	hadd -f AK8_HToAATo4B_GluGluH_01J_Pt${Pt}_mH-70_mA-12_wH-70_wA-${wA}.root AK8_HToAATo4B_GluGluH_01J_Pt${Pt}_mH-70_mA-12_wH-70_wA-${wA}_part*.root
#     done
# done

# for qcd in "BGenFilter" "bEnriched"
# do
#     for ht in "500to700" "700to1000" "1000to1500" "1500to2000"
#     # for ht in "300to500" "2000toInf"
#     do
# 	echo "hadd -f AK8_QCD_${qcd}_HT${ht}.root AK8_QCD_${qcd}_HT${ht}_part_?_10.root"
# 	hadd -f AK8_QCD_${qcd}_HT${ht}.root AK8_QCD_${qcd}_HT${ht}_part_?_10.root
#     done
# done

# # for decay in Hadronic SemiLep 2L2Nu
# for decay in 2L2Nu
# do
#     for part in 4 6
#     do
# 	echo "hadd -f AK8_TTTo${decay}_part_${part}_10.root AK8_TTTo${decay}_part_*${part}_200.root"
# 	hadd -f AK8_TTTo${decay}_part_${part}_10.root AK8_TTTo${decay}_part_*${part}_200.root
#     done
# done

for ewk in "ZJetsToQQ"
do
    for ht in "200to400" "400to600" "600to800" "800toInf"
    do
	echo "hadd -f AK8_${ewk}_HT-${ht}.root AK8_${ewk}_HT-${ht}_part_*_100.root"
	hadd -f AK8_${ewk}_HT-${ht}.root AK8_${ewk}_HT-${ht}_part_*_100.root
    done
done

echo $PWD
cd -
echo $PWD
