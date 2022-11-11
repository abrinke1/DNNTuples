import FWCore.ParameterSet.Config as cms
import os
import math

# ---------------------------------------------------------
from FWCore.ParameterSet.VarParsing import VarParsing

print '\nRunning Hto4b_DeepNtuplizerAK8.py'

options = VarParsing('analysis')

options.register('skipEvents', 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "skip N events")
options.register('inputDataset', '', VarParsing.multiplicity.singleton, VarParsing.varType.string, "Input dataset")
options.register('isTrainSample', True, VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool, "if the sample is used for training")

options.parseArguments()

with open(os.getcwd()+'/test/lists/'+options.inputDataset+'.txt') as list_file:
    options.inputFiles = list_file.read().splitlines()
print '\nGot %d input files from %s' % (len(options.inputFiles), options.inputDataset)

options.outputFile = 'output/AK15_'+options.inputDataset+'.root'

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
    printEvent = max(100, min(5000, int(math.sqrt(options.maxEvents*10)) ) )
print '\nWill process up to %d events, report every %d' % (options.maxEvents, printEvent)
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
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
from RecoBTag.ONNXRuntime.pfParticleNet_cff import _pfParticleNetJetTagsAll as pfParticleNetJetTagsAll

useReclusteredJets = True
jetR = 1.5

bTagDiscriminators = ['None']

JETCorrLevels = ['L2Relative', 'L3Absolute']

from DeepNTuples.Ntupler.jetToolbox_cff import jetToolbox
jetToolbox(process, 'ak15', 'dummySeq', 'noOutput', PUMethod='Puppi', JETCorrPayload='AK8PFPuppi',
           JETCorrLevels=JETCorrLevels, Cut='pt > 120.0 && abs(rapidity()) < 2.4', runOnMC=True, addNsub=True, maxTau=3,
           addSoftDrop=True, addSoftDropSubjets=True, subJETCorrPayload='AK4PFPuppi', subJETCorrLevels=JETCorrLevels,
           bTagDiscriminators=['None'],
           subjetBTagDiscriminators=['None'])

updateJetCollection(
    process,
    jetSource=cms.InputTag('packedPatJetsAK15PFPuppiSoftDrop'),
    rParam=jetR,
    jetCorrections=('AK8PFPuppi', cms.vstring(['L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators=bTagDiscriminators,
    postfix='AK15WithPuppiDaughters',  # needed to tell the producers that the daughters are puppi-weighted
)

srcJets = cms.InputTag('selectedUpdatedPatJetsAK15WithPuppiDaughters')
# ---------------------------------------------------------
from PhysicsTools.PatAlgos.tools.helpers import getPatAlgosToolsTask, addToProcessAndTask
patTask = getPatAlgosToolsTask(process)

from RecoJets.JetProducers.ak8GenJets_cfi import ak8GenJets
process.ak15GenJetsWithNu = ak8GenJets.clone(
    src='packedGenParticles',
    rParam=cms.double(jetR),
    jetPtMin=25.0
)
process.ak15GenJetsWithNuSoftDrop = process.ak15GenJetsWithNu.clone(
    useSoftDrop=cms.bool(True),
    zcut=cms.double(0.1),
    beta=cms.double(0.0),
    R0=cms.double(jetR),
    useExplicitGhosts=cms.bool(True)
)
process.ak15GenJetsWithNuMatch = cms.EDProducer("GenJetMatcher",  # cut on deltaR; pick best by deltaR
                                                src=srcJets,  # RECO jets (any View<Jet> is ok)
                                                # GEN jets  (must be GenJetCollection)
                                                matched=cms.InputTag("ak15GenJetsWithNu"),
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
process.ak15GenJetsWithNuSoftDropMatch = cms.EDProducer("GenJetMatcher",  # cut on deltaR; pick best by deltaR
                                                        src=srcJets,  # RECO jets (any View<Jet> is ok)
                                                        # GEN jets  (must be GenJetCollection)
                                                        matched=cms.InputTag("ak15GenJetsWithNuSoftDrop"),
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
    process.ak15GenJetsWithNu,
    process.ak15GenJetsWithNuMatch,
    process.ak15GenJetsWithNuSoftDrop,
    process.ak15GenJetsWithNuSoftDropMatch,
)

# DeepNtuplizer
process.load("DeepNTuples.Ntupler.DeepNtuplizer_cfi")
process.deepntuplizer.jets = srcJets
process.deepntuplizer.useReclusteredJets = useReclusteredJets
process.deepntuplizer.bDiscriminators = bTagDiscriminators
process.deepntuplizer.jetR = jetR
process.deepntuplizer.jetPtMin = 100
process.deepntuplizer.jetMassMin = 50

process.deepntuplizer.genJetsMatch = 'ak15GenJetsWithNuMatch'
process.deepntuplizer.genJetsSoftDropMatch = 'ak15GenJetsWithNuSoftDropMatch'

process.deepntuplizer.isQCDSample = ('/QCD_' in options.inputDataset or 'QCD_' in options.inputFiles[0])
process.deepntuplizer.sampleType = ('QCD_BGen' if 'QCD_BGen' in options.inputFiles[0] else \
                                    ('QCD_bEnr' if 'QCD_bEnr' in options.inputFiles[0] else \
                                     ('QCD_Incl' if 'QCD_Incl' in options.inputFiles[0] else '') ) )
print '\nisQCDSample = %s, sampleType = %s\n' % (str(process.deepntuplizer.isQCDSample), process.deepntuplizer.sampleType)
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
