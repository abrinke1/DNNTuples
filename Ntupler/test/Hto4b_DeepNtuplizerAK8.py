import FWCore.ParameterSet.Config as cms
import os
import sys
import math

# ---------------------------------------------------------
from FWCore.ParameterSet.VarParsing import VarParsing

print('\nRunning Hto4b_DeepNtuplizerAK8.py')

options = VarParsing('analysis')

options.register('skipEvents', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "skip N events")
options.register('inputDataset', '', VarParsing.multiplicity.singleton, VarParsing.varType.string, "Input dataset")
options.register('isTrainSample', True, VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool, "if the sample is used for training")
options.register('fileIndex', -1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Single file index")
options.register('partNum', -1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Which partial set of files")
options.register('numParts', -1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "How many partial sets")

options.parseArguments()

list_kodiak = os.getcwd()+'/test/lists/2018/kodiak/'+options.inputDataset+'.txt'
list_xrootd = os.getcwd()+'/test/lists/2018/xrootd/'+options.inputDataset+'.txt'
if os.path.isfile(list_kodiak):
    list_file_name = list_kodiak
    print('\nRunning over files from kodiak in:')
elif os.path.isfile(list_xrootd):
    list_file_name = list_xrootd
    print('\nRunning over files from xrootd in:')
else:
    print('\n*** ERROR!!! Input file list does not exist! Quitting.')
    print(list_kodiak)
    print(list_xrootd)
    open('DoesNotExist')
print(list_file_name)

in_file_list = []
with open(list_file_name) as list_file:
    in_file_list = [f for f in list_file.read().splitlines() if not f.startswith('#')]
print('Got %d input files' % len(in_file_list))
if options.fileIndex >= 0:
    in_file_list = [in_file_list[options.fileIndex]]
    print('\nWill only run on file %d:' % options.fileIndex)
    print(in_file_list[0])
elif options.partNum >= 0 and options.numParts >= 0:
    if options.partNum >= options.numParts:
        print('\nERROR!!! partNum = %d >= numParts = %d! Quitting.' % (options.partNum, options.numParts))
        assert(options.partNum < options.numParts)
    if options.numParts > len(in_file_list):
        print('\nERROR!!! numParts = %d > # of files = %d! Quitting.' % (options.numParts, len(in_file_list)))
        assert(options.numParts <= len(in_file_list))
    part_file_list = []
    for ii in range(len(in_file_list)):
        if (ii % options.numParts) == options.partNum:
            part_file_list.append(in_file_list[ii])
    in_file_list = part_file_list
    print('\nWill only run on files %d out of %d:' % (options.partNum, options.numParts))
    print(in_file_list)

options.inputFiles = in_file_list

if options.maxEvents >= 0:
    out_dir = os.getcwd()+'/output/'
else:
    out_dir = '/cms/data/store/user/abrinke1/Trees/HtoAAto4B/ParticleNet/Ntuples/2024_04_01/'
    # out_dir = '/home/chosila/Projects/CMSSW_10_6_32/src/DeepNTuples/Ntuples/'
out_str = out_dir+'AK8_'+options.inputDataset
if options.fileIndex >= 0:
    out_str += ('_file_%03d' % options.fileIndex)
elif options.partNum >= 0 and options.numParts >= 0:
    if options.numParts <= 10:
        out_str += ('_part_%01d_%01d' % (options.partNum, options.numParts))
    elif options.numParts <= 100:
        out_str += ('_part_%02d_%02d' % (options.partNum, options.numParts))
    elif options.numParts <= 1000:
        out_str += ('_part_%03d_%03d' % (options.partNum, options.numParts))
    else:
        print('\nERROR!!! Are you crazy??? numParts = %d > 1000! Quitting.' % (options.numParts))
        assert(options.numParts <= 1000)
options.outputFile = out_str+'.root'

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
if options.maxEvents < 1:
    printEvent = 5000
else:
    printEvent = min(5000, int(math.sqrt(options.maxEvents*10)))
print('\nWill process up to %d events, report every %d' % (options.maxEvents, printEvent))
process.MessageLogger.cerr.FwkReport.reportEvery = printEvent

process.options = cms.untracked.PSet(
    allowUnscheduled=cms.untracked.bool(True),
    wantSummary=cms.untracked.bool(False)
)

print('\nUsing output file ' + options.outputFile)

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
# ## Only keep mass-decorrelated tagger values
# pfDeepBoostedJetTagsMD = [tg for tg in pfDeepBoostedJetTagsAll if 'MassDecorrelated' in tg]
# pfParticleNetJetTagsMD = [tg for tg in pfParticleNetJetTagsAll if 'MassDecorrelated' in tg]

# !!! set `useReclusteredJets = True ` if you need to recluster jets (e.g., to adopt a new Puppi tune) !!!
useReclusteredJets = False
jetR = 0.8

bTagDiscriminators = [
    'pfCombinedInclusiveSecondaryVertexV2BJetTags',
    'pfBoostedDoubleSecondaryVertexAK8BJetTags',
    'pfParticleNetMassRegressionJetTags:mass',
]

subjetBTagDiscriminators = ['None']

if useReclusteredJets:
    JETCorrLevels = ['L2Relative', 'L3Absolute']

    from DeepNTuples.Ntupler.jetToolbox_cff import jetToolbox
    jetToolbox(process, 'ak8', 'dummySeq', 'noOutput', PUMethod='Puppi', JETCorrPayload='AK8PFPuppi',
               JETCorrLevels=JETCorrLevels, Cut='pt > 170.0 && abs(rapidity()) < 2.4', runOnMC=(not 'JetHT' in options.inputDataset), addNsub=True,
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
process.ak8GenJetsWithNuMatch  = cms.EDProducer("GenJetMatcher",  # cut on deltaR; pick best by deltaR
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
process.ak8GenJetsWithNuSoftDropMatch  = cms.EDProducer("GenJetMatcher",  # cut on deltaR; pick best by deltaR
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
process.deepntuplizer.jetPtMin = 170
process.deepntuplizer.jetMassMin = 50
process.deepntuplizer.jetAvgXbbMin = 0.2

process.deepntuplizer.genJetsMatch = 'ak8GenJetsWithNuMatch'
process.deepntuplizer.genJetsSoftDropMatch = 'ak8GenJetsWithNuSoftDropMatch'

isQCD = ('/QCD_' in options.inputDataset or 'QCD_' in options.inputFiles[0])
isHto4b = ('HToAATo4B_' in options.inputDataset or 'HToAATo4B_' in options.inputFiles[0])
isData = ('JetHT' in options.inputDataset)
process.deepntuplizer.isQCDSample = isQCD
if isQCD:
    process.deepntuplizer.sampleType = ('QCD_BGen' if 'BGenFilter' in options.inputFiles[0] else \
                                        ('QCD_bEnr' if 'bEnriched' in options.inputFiles[0] else \
                                         'QCD_Incl') )
    process.deepntuplizer.minLheHT = cms.double(int(options.inputFiles[0].split('_HT')[1].split('00to')[0]+'00'))
elif isHto4b:
    process.deepntuplizer.sampleType = ('H_aa_bbbb_'+options.inputFiles[0].split('_TuneCP5')[0][-4:])
elif isData:
    process.deepntuplizer.sampleType = 'data'

print('\nisQCDSample = %s, sampleType = %s, minLheHT = %s\n' % (str(process.deepntuplizer.isQCDSample),
                                                                process.deepntuplizer.sampleType,
                                                                process.deepntuplizer.minLheHT))

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
if not isData:
    process.p.associate(process.genJetTask)
