import FWCore.ParameterSet.Config as cms

# ---------------------------------------------------------
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

# options.outputFile = 'output/Hto4b_HToAATo4B_test.root'
# ## options.inputFiles = 'file:/cms/data/store/user/abrinke1/MiniAOD/Run2_UL/2018/MC/HToAATo4B/GluGluH_01J_Pt150/M-20/9EB2BA58-7682-194F-94AD-BDD3E05C259B.root' ## Small
# ## options.inputFiles = 'file:/cms/data/store/user/abrinke1/MiniAOD/Run2_UL/2018/MC/HToAATo4B/GluGluH_01J_Pt150/M-20/7051A354-757E-504B-BFCF-379AF8F3D6C8.root' ## Larger
# base_str = 'root://cmsxrootd.fnal.gov//store/mc/RunIISummer20UL18MiniAODv2/SUSY_GluGluH_01J_HToAATo4B_Pt150_M-20_TuneCP5_13TeV_madgraph_pythia8/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v1/'
# options.inputFiles = [
#     base_str+'2520000/0CBDE505-6EE4-B44D-97B2-7CA3AB7C9E5F.root',
#     base_str+'2520000/0E2F71F8-7E51-BC4B-A159-7CD4E2732F60.root',
#     base_str+'2520000/12F95D86-2AAC-7641-8B59-C983824D24A5.root',
#     base_str+'2520000/2442B826-9952-584B-96A3-ABB7697B6FE1.root',
#     base_str+'2520000/33A29F57-5189-C945-95D5-67D8A106EB1A.root',
#     base_str+'2520000/3B8D81E7-989E-1742-85F5-C8DCB544C23F.root',
#     base_str+'2520000/3EAECF82-3161-844E-8B6C-5BC27F7421FF.root',
#     base_str+'2520000/40298762-C1EC-A045-AE48-A8E1FB201511.root',
#     base_str+'2520000/457DA897-8EB8-5940-94D2-4B0CD6E8E363.root',
#     base_str+'2520000/4BF50F70-86D3-D049-9D2E-2A6B4737668A.root',
#     base_str+'2520000/576B5954-FA9D-734D-B3A4-ADAD935A9354.root',
#     base_str+'2520000/5DAA8F64-BD5A-8044-9A2F-D742AAE528C7.root',
#     base_str+'2520000/7FE190C6-B417-DA42-8764-B7248DA5F4B3.root',
#     base_str+'2520000/8293F4F3-A707-4243-9EF6-6284C2841E40.root',
#     base_str+'2520000/95689CF7-83AA-2D48-A3FC-B31AA44887B9.root',
#     base_str+'2520000/95810A96-A180-B74B-B6D2-7B9F3BFAC776.root',
#     base_str+'2520000/9EB2BA58-7682-194F-94AD-BDD3E05C259B.root',
#     base_str+'2520000/A564C435-AFD9-3F4B-91EE-AC66586C2EA5.root',
#     base_str+'2520000/A92D40D0-BA29-D147-8BEE-DE1A694B7995.root',
#     base_str+'2520000/AD01C5F7-08CF-314A-8644-7223E449953E.root',
#     base_str+'2520000/B212278C-E2E5-8F4F-A16D-229CFC2183E3.root',
#     base_str+'2520000/C449F0A4-0FEE-544C-B182-3D307678B035.root',
#     base_str+'2520000/C7312AFB-6768-FB4B-ABB6-38C5289D9B45.root',
#     base_str+'2520000/CCFBD779-31AB-A44C-AB64-785EBBFACF89.root',
#     base_str+'2520000/E4E63FF7-AF9E-3949-9C43-1FB4E67C55A2.root',
#     base_str+'2520000/E6115A1D-31FA-1644-B73C-AF3D4969D940.root',
#     base_str+'2520000/EF35802D-48B4-2F41-A31F-FDC25E3A549F.root',
#     base_str+'2520000/F052280B-8B61-964E-8CB7-A327668588BD.root',
#     base_str+'2520000/F67CB2A4-1A2F-574F-9E40-060698DBEDD1.root',
#     base_str+'2540000/1700A733-F57C-AC48-A029-2524614819EF.root',
#     base_str+'2540000/1BF921F0-6DCF-C249-89A3-C82923E675A4.root',
#     base_str+'260000/003A1234-0E1D-154A-9704-9406B61CB642.root',
#     base_str+'260000/0B5221FE-B9CF-A449-A523-33FFCAF65CD2.root',
#     base_str+'260000/1F414194-3EA3-6F47-B78D-C255BCB4480D.root',
#     base_str+'260000/5BD5F9BB-9E07-1745-B970-B128FAE7FE90.root',
#     base_str+'260000/6DC8F87E-2B81-804C-9D01-C5BFABBD4151.root',
#     base_str+'260000/7051A354-757E-504B-BFCF-379AF8F3D6C8.root',
#     base_str+'260000/7052697E-2785-D443-9D26-6AE432CC27C4.root',
#     base_str+'260000/827B53CD-2138-C740-BF68-1A8D0072279C.root',
#     base_str+'260000/83EF2569-DC5F-1D4C-9B28-712F9751D75D.root',
#     base_str+'260000/A449234C-53E2-FC45-A815-C35A16EED5F9.root',
#     base_str+'260000/B9AD2376-4A6F-6844-B4F1-F6360AE99401.root',
#     base_str+'260000/C24DE3D0-A270-BE44-8904-F0527552C4D9.root',
#     base_str+'260000/D6ED821B-45D3-7147-90EA-CE778BF18152.root'
# ]

options.outputFile = 'output/Hto4b_QCD_BGenFilter_HT700to10000_test.root'
## options.inputFiles = 'file:/cms/data/store/user/abrinke1/MiniAOD/Run2_UL/2018/MC/QCD_BGenFilter/HT700to1000/6AAC6361-CD31-7E49-8AC9-7325E3F2697A.root'
base_str = 'root://cmsxrootd.fnal.gov//store/mc/RunIISummer20UL18MiniAODv2/QCD_bEnriched_HT700to1000_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/106X_upgrade2018_realistic_v16_L1v1-v1/'
options.inputFiles = [
    base_str+'280000/13E96CD0-B336-4C4A-A69C-B7447AEFBC69.root',
    base_str+'280000/19EBFA22-429F-CD40-A608-C5FECC1E761E.root',
    base_str+'280000/1A023B8B-0745-E349-8CFA-93E5C6FBB17E.root',
    base_str+'280000/1C0FA638-CB27-2F48-AD0F-68EDC00EF518.root',
    base_str+'280000/26DB15A1-158E-B24A-84C7-7CD85D760D02.root',
    base_str+'280000/2789F3AB-BCBC-DE47-BA4E-9365E2C9673C.root',
    base_str+'280000/27DAB083-09A0-934D-BDE5-A4AF7F7697A0.root',
    base_str+'280000/3ECACF0D-CC47-6842-87C7-CD24B54441AB.root',
    base_str+'280000/41BEB6B4-8C47-2346-B917-9527A513FBD5.root',
    base_str+'280000/4E315CD0-D6A1-E94C-A7ED-1A7643DE0BB3.root',
    base_str+'280000/53BA3ADF-C759-F746-82AB-077124212A65.root',
    base_str+'280000/64B6464C-FC38-8845-97C5-592598E05124.root',
    base_str+'280000/6AAC6361-CD31-7E49-8AC9-7325E3F2697A.root',
    base_str+'280000/6C710484-3ADB-E142-BA5B-73FD6AB08A6C.root',
    base_str+'280000/7F6CC720-51C2-AF4E-B9A1-2A6B33E3A07B.root',
    base_str+'280000/8379C71E-A74B-9246-8B98-2F8BC7F722AE.root',
    base_str+'280000/839E5782-5602-5742-AED3-3A6B30C5AADE.root',
    base_str+'280000/8F823DB5-D2E4-CA4D-8A81-29E1FB54E561.root',
    base_str+'280000/91541677-E0A5-AD49-907D-A0D4DA7BDD9B.root',
    base_str+'280000/92333EC7-46F5-3F43-AF7B-0BB6E5FA0479.root',
    base_str+'280000/954E0FD9-4088-BA4D-B701-D7F1D192D5B5.root',
    base_str+'280000/9ABB3D83-7500-BE40-9DA3-877EBE5FBA21.root',
    base_str+'280000/9DBD56BE-6A12-6543-A501-EC7634BF9C91.root',
    base_str+'280000/AE946F6A-5E85-4D4B-A2FA-676CDEC19076.root',
    base_str+'280000/BD98B3DA-986C-DE4C-A894-055F4BEE6491.root',
    base_str+'280000/C82F86E3-5885-804E-A71F-FF1CCD552E24.root',
    base_str+'280000/C9CA58AD-5BB2-1D41-A439-2EC578C40BB4.root',
    base_str+'280000/CBD6F02C-ED33-6249-A8B6-03137E328720.root',
    base_str+'280000/D4B3102A-4D23-7B48-B202-3C826796C4E2.root',
    base_str+'280000/D56B78B9-F48F-C544-A9E4-89028641354E.root',
    base_str+'280000/DEA5565D-4318-EF4A-BC9C-DB202ACDD28A.root',
    base_str+'280000/DF57E095-33EE-2F43-87DE-2E75EB3955C4.root',
    base_str+'280000/E56B4DE5-82E4-5F43-9F35-1E479331D757.root',
    base_str+'280000/ED52A79C-2458-6B4E-885F-2382B7D00D12.root',
    base_str+'280000/F23BA159-9626-CD41-B82F-4E719A3B31EA.root',
    base_str+'280000/F3F1FC29-0714-C340-B527-15807AACB4F2.root',
    base_str+'280000/F6712F4D-DE49-2B49-8A32-603C2E39D900.root',
    base_str+'280000/F6E62A5E-0972-1546-9196-C67A8D912C9B.root'
]

options.maxEvents = -1

options.register('skipEvents', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "skip N events")
options.register('inputDataset',
                 '',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "Input dataset")
options.register('isTrainSample', True, VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool, "if the sample is used for training")

options.parseArguments()

globalTagMap = {
    'auto': 'auto:phase1_2018_realistic',
    'UL18': '106X_upgrade2018_realistic_v16_L1v1',
    'UL17': '106X_mc2017_realistic_v9',
}

era = 'auto'
for k in globalTagMap:
    if k in options.inputDataset:
        era = k
# ---------------------------------------------------------
process = cms.Process("DNNFiller")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.options = cms.untracked.PSet(
    allowUnscheduled=cms.untracked.bool(True),
    wantSummary=cms.untracked.bool(False)
)

print('Using output file ' + options.outputFile)

process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string(options.outputFile))

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))

process.source = cms.Source('PoolSource',
                            fileNames=cms.untracked.vstring(options.inputFiles),
                            skipEvents=cms.untracked.uint32(options.skipEvents)
                            )
# ---------------------------------------------------------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, globalTagMap[era], '')
print('Using global tag', process.GlobalTag.globaltag)
# ---------------------------------------------------------
# read JEC from sqlite
# if era == 'Summer19UL17':
#     import os
#     jecTag = 'Summer19UL17_V5_MC'
#     jecFile = '%s.db' % jecTag
#     if not os.path.exists(jecFile):
#         os.symlink('../data/' + jecFile, jecFile)
#     from CondCore.CondDB.CondDB_cfi import CondDB
#     CondDBJECFile = CondDB.clone(connect=cms.string('sqlite:%s' % jecFile))
#     process.jec = cms.ESSource('PoolDBESSource',
#                                CondDBJECFile,
#                                toGet=cms.VPSet(
#                                    cms.PSet(
#                                        record=cms.string('JetCorrectionsRecord'),
#                                        tag=cms.string('JetCorrectorParametersCollection_%s_AK4PFchs' % jecTag),
#                                        label=cms.untracked.string('AK4PFchs')
#                                    ),
#                                    cms.PSet(
#                                        record=cms.string('JetCorrectionsRecord'),
#                                        tag=cms.string('JetCorrectorParametersCollection_%s_AK4PFPuppi' % jecTag),
#                                        label=cms.untracked.string('AK4PFPuppi')
#                                    ),
#                                    # ...and so on for all jet types you need
#                                )
#                                )
#     print(jecTag, process.jec.toGet)
#     # Add an ESPrefer to override JEC that might be available from the global tag
#     process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')
# ---------------------------------------------------------
# Update to PuppiV14
# from CommonTools.PileupAlgos.customizePuppiTune_cff import UpdatePuppiTuneV14_MC
# UpdatePuppiTuneV14_MC(process)
# ---------------------------------------------------------
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
from RecoBTag.ONNXRuntime.pfDeepBoostedJet_cff import _pfDeepBoostedJetTagsAll as pfDeepBoostedJetTagsAll
from RecoBTag.ONNXRuntime.pfParticleNet_cff import _pfParticleNetJetTagsAll as pfParticleNetJetTagsAll

# !!! set `useReclusteredJets = True ` if you need to recluster jets (e.g., to adopt a new Puppi tune) !!!
useReclusteredJets = False
jetR = 0.8

bTagDiscriminators = [
    'pfCombinedInclusiveSecondaryVertexV2BJetTags',
    'pfBoostedDoubleSecondaryVertexAK8BJetTags',
    # 'pfDeepDoubleBvLJetTags:probHbb',
    # 'pfDeepDoubleCvLJetTags:probHcc',
    # 'pfDeepDoubleCvBJetTags:probHcc',
    # 'pfMassIndependentDeepDoubleBvLJetTags:probHbb',
    # 'pfMassIndependentDeepDoubleCvLJetTags:probHcc',
    # 'pfMassIndependentDeepDoubleCvBJetTags:probHcc',

    'pfParticleNetMassRegressionJetTags:mass',
]

subjetBTagDiscriminators = ['None']

if useReclusteredJets:
    JETCorrLevels = ['L2Relative', 'L3Absolute']

    from DeepNTuples.Ntupler.jetToolbox_cff import jetToolbox
    jetToolbox(process, 'ak8', 'dummySeq', 'noOutput', PUMethod='Puppi', JETCorrPayload='AK8PFPuppi',
               JETCorrLevels=JETCorrLevels, Cut='pt > 170.0 && abs(rapidity()) < 2.4', runOnMC=True, addNsub=True,
               maxTau=3, addSoftDrop=True, addSoftDropSubjets=True, subJETCorrPayload='AK4PFPuppi',
               subJETCorrLevels=JETCorrLevels, bTagDiscriminators=['pfCombinedInclusiveSecondaryVertexV2BJetTags'],
               subjetBTagDiscriminators=subjetBTagDiscriminators)

    updateJetCollection(
        process,
        jetSource=cms.InputTag('packedPatJetsAK8PFPuppiSoftDrop'),
        rParam=jetR,
        jetCorrections=('AK8PFPuppi', cms.vstring(['L2Relative', 'L3Absolute']), 'None'),
        btagDiscriminators=bTagDiscriminators + pfDeepBoostedJetTagsAll + pfParticleNetJetTagsAll,
        postfix='AK8WithPuppiDaughters',  # needed to tell the producers that the daughters are puppi-weighted
    )
    srcJets = cms.InputTag('selectedUpdatedPatJetsAK8WithPuppiDaughters')
else:
    updateJetCollection(
        process,
        jetSource=cms.InputTag('slimmedJetsAK8'),
        rParam=jetR,
        jetCorrections=('AK8PFPuppi', cms.vstring(['L2Relative', 'L3Absolute']), 'None'),
        btagDiscriminators=['pfParticleNetMassRegressionJetTags:mass'],
    )
    srcJets = cms.InputTag('selectedUpdatedPatJets')
# ---------------------------------------------------------
from PhysicsTools.PatAlgos.tools.helpers import getPatAlgosToolsTask, addToProcessAndTask
patTask = getPatAlgosToolsTask(process)

from RecoJets.JetProducers.ak8GenJets_cfi import ak8GenJets
process.ak8GenJetsWithNu = ak8GenJets.clone(
    src='packedGenParticles',
    rParam=cms.double(jetR),
    jetPtMin=100.0
)
process.ak8GenJetsWithNuSoftDrop = process.ak8GenJetsWithNu.clone(
    useSoftDrop=cms.bool(True),
    zcut=cms.double(0.1),
    beta=cms.double(0.0),
    R0=cms.double(jetR),
    useExplicitGhosts=cms.bool(True)
)
process.ak8GenJetsWithNuMatch = cms.EDProducer("GenJetMatcher",  # cut on deltaR; pick best by deltaR
                                               src=srcJets,  # RECO jets (any View<Jet> is ok)
                                               # GEN jets  (must be GenJetCollection)
                                               matched=cms.InputTag("ak8GenJetsWithNu"),
                                               mcPdgId=cms.vint32(),  # n/a
                                               mcStatus=cms.vint32(),  # n/a
                                               checkCharge=cms.bool(False),  # n/a
                                               maxDeltaR=cms.double(jetR),  # Minimum deltaR for the match
                                               # maxDPtRel   = cms.double(3.0),                  # Minimum deltaPt/Pt for the match (not used in GenJetMatcher)
                                               # Forbid two RECO objects to match to the same GEN object
                                               resolveAmbiguities=cms.bool(True),
                                               # False = just match input in order; True = pick lowest deltaR pair first
                                               resolveByMatchQuality=cms.bool(False),
                                               )
process.ak8GenJetsWithNuSoftDropMatch = cms.EDProducer("GenJetMatcher",  # cut on deltaR; pick best by deltaR
                                                       src=srcJets,  # RECO jets (any View<Jet> is ok)
                                                       # GEN jets  (must be GenJetCollection)
                                                       matched=cms.InputTag("ak8GenJetsWithNuSoftDrop"),
                                                       mcPdgId=cms.vint32(),  # n/a
                                                       mcStatus=cms.vint32(),  # n/a
                                                       checkCharge=cms.bool(False),  # n/a
                                                       maxDeltaR=cms.double(jetR),  # Minimum deltaR for the match
                                                       # maxDPtRel   = cms.double(3.0),                  # Minimum deltaPt/Pt for the match (not used in GenJetMatcher)
                                                       # Forbid two RECO objects to match to the same GEN object
                                                       resolveAmbiguities=cms.bool(True),
                                                       # False = just match input in order; True = pick lowest deltaR pair first
                                                       resolveByMatchQuality=cms.bool(False),
                                                       )
process.genJetTask = cms.Task(
    process.ak8GenJetsWithNu,
    process.ak8GenJetsWithNuMatch,
    process.ak8GenJetsWithNuSoftDrop,
    process.ak8GenJetsWithNuSoftDropMatch,
)

# DeepNtuplizer
process.load("DeepNTuples.Ntupler.DeepNtuplizer_cfi")
process.deepntuplizer.jets = srcJets
process.deepntuplizer.useReclusteredJets = useReclusteredJets
process.deepntuplizer.bDiscriminators = bTagDiscriminators + pfDeepBoostedJetTagsAll + pfParticleNetJetTagsAll

process.deepntuplizer.genJetsMatch = 'ak8GenJetsWithNuMatch'
process.deepntuplizer.genJetsSoftDropMatch = 'ak8GenJetsWithNuSoftDropMatch'

process.deepntuplizer.isQCDSample = ('/QCD_' in options.inputDataset or 'QCD_' in options.inputFiles[0])
process.deepntuplizer.sampleType = ('QCD_BGen' if 'QCD_BGen' in options.inputFiles[0] else \
                                    ('QCD_bEnr' if 'QCD_bEnr' in options.inputFiles[0] else \
                                     ('QCD_Incl' if 'QCD_Incl' in options.inputFiles[0] else '') ) )
process.deepntuplizer.isPythia = 'pythia' in options.inputDataset.lower()
process.deepntuplizer.isHerwig = 'herwig' in options.inputDataset.lower()
# note: MG can be interfaced w/ either pythia or herwig
process.deepntuplizer.isMadGraph = 'madgraph' in options.inputDataset.lower()

process.deepntuplizer.isTrainSample = options.isTrainSample
if not options.inputDataset:
    # interactive running
    process.deepntuplizer.isTrainSample = False
#==============================================================================================================================#
process.p = cms.Path(process.deepntuplizer)
process.p.associate(patTask)
process.p.associate(process.genJetTask)
