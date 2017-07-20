import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

import os,sys


options = VarParsing.VarParsing('standard')

####################################
# Options for reading in the data
options.register('fileDirectory',
                '/eos/user/t/tquast/data/Testbeam/July2017/DWC/',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 'Path to the file from which the DWCs are read.'
                )

options.register('reportEvery',
                50000,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 'Path to the file from which the DWCs are read.'
                )

options.parseArguments()
input_data = []

#100 GeV pions
input_data.append((1159, "", 0, "1"))
input_data.append((1161, "", 0, "1"))
input_data.append((1162, "", 0, "1"))
input_data.append((1163, "", 0, "1"))
input_data.append((1165, "", 0, "1"))
input_data.append((1166, "", 0, "1"))   #check synchronisation of trigger
input_data.append((1171, "HexaData_Run1171_TIMING_RDOUT_ORM0.txt", 0, "1"))   #Number of events below 2ms difference:  622  /  659
input_data.append((1172, "HexaData_Run1172_TIMING_RDOUT_ORM0.txt", 0, "1"))   #Number of events below 2ms difference:  208  /  219                     
input_data.append((1173, "HexaData_Run1173_TIMING_RDOUT_ORM0.txt", 0, "1"))   #Number of events below 2ms difference:  400  /  424                                  
input_data.append((1193, "HexaData_Run1193_TIMING_RDOUT_ORM0.txt", 0, "1"))   #Number of events below 2ms difference:  8658  /  8714


#150 GeV pions:
input_data.append((1051, "RUN_1051_60717_1425_TIMING_RDOUT2.txt 60717_1425", 1, "2"))
input_data.append((1067, "RUN_1067_70717_0015_TIMING_RDOUT2.txt 70717_0015", 0, "2"))
input_data.append((1069, "RUN_1069_70717_0115_TIMING_RDOUT2.txt 70717_0115", 0, "2"))
input_data.append((1070, "RUN_1070_70717_0205_TIMING_RDOUT2.txt 70717_0205", 0, "2"))
input_data.append((1071, "RUN_1071_70717_0251_TIMING_RDOUT2.txt 70717_0251", 0, "2"))
input_data.append((1072, "RUN_1072_70717_0336_TIMING_RDOUT2.txt 70717_0336", 0, "2"))
input_data.append((1073, "RUN_1073_70717_0421_TIMING_RDOUT2.txt 70717_0421", 0, "2"))
input_data.append((1074, "RUN_1074_70717_0506_TIMING_RDOUT2.txt 70717_0506", 0, "2"))
input_data.append((1076, "RUN_1076_70717_0558_TIMING_RDOUT2.txt 70717_0558", 0, "2"))
input_data.append((1079, "RUN_1079_70717_0654_TIMING_RDOUT2.txt 70717_0654", 0, "2"))
input_data.append((1082, "RUN_1082_70717_0901_TIMING_RDOUT2.txt 70717_0901", 1, "2"))
input_data.append((1088, "RUN_1088_70717_1048_TIMING_RDOUT2.txt 70717_1048", 1, "2"))
input_data.append((1090, "RUN_1090_70717_1203_TIMING_RDOUT2.txt 70717_1203", 0, "2"))
input_data.append((1096, "RUN_1096_70717_1625_TIMING_RDOUT2.txt 70717_1625", 0, "2"))

input_data.append((1134, "", 1, "2"))   #possibly remove last: careful check is necessary
input_data.append((1135, "", 0, "2"))   #check synchronisation of trigger
input_data.append((1136, "", 0, "2"))   #check synchronisation of trigger
input_data.append((1185, "HexaData_Run1185_TIMING_RDOUT_ORM0.txt", 0, "2"))   #Number of events below 2ms difference:  4981  /  5000                     
input_data.append((1192, "HexaData_Run1192_TIMING_RDOUT_ORM0.txt", 0, "2"))   #Number of events below 2ms difference:  29927  /  30000

#200 GeV pions
input_data.append((1132, "", 0, "3"))
input_data.append((1133, "", 0, "3"))   #check synchronisation of trigger
input_data.append((1190, "HexaData_Run1190_TIMING_RDOUT_ORM0.txt", 2, "3"))   #Number of events below 2ms difference:  29438  /  30000

#250 GeV pions
input_data.append((1189, "HexaData_Run1189_TIMING_RDOUT_ORM0.txt", 0, "4"))   #Number of events below 2ms difference:  27915  /  30000

#300 GeV pions
input_data.append((993, "RUN_993_150717_0800_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((994, "RUN_994_150717_0838_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((996, "RUN_996_150717_0924_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((997, "RUN_997_150717_1009_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((998, "RUN_998_150717_1100_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((999, "RUN_999_150717_1145_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1000, "RUN_1000_150717_1343_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1001, "RUN_1001_150717_1455_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1002, "RUN_1002_150717_1532_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1003, "RUN_1003_150717_1619_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1004, "RUN_1004_150717_1701_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1012, "RUN_1012_150717_1851_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1013, "RUN_1013_150717_1929_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1025, "RUN_1025_150717_2331_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1027, "RUN_1027_160717_0117_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1028, "RUN_1028_160717_0153_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1029, "RUN_1029_160717_0229_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1030, "RUN_1030_160717_0305_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1032, "RUN_1032_160717_0347_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1033, "RUN_1033_160717_0422_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1034, "RUN_1034_160717_0459_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1035, "RUN_1035_160717_0546_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1036, "RUN_1036_160717_0710_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1039, "RUN_1039_160717_0950_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1040, "RUN_1040_160717_1010_TIMING_RDOUT2.txt", 1, "5"))
input_data.append((1045, "RUN_1045_160717_1154_TIMING_RDOUT2.txt", 0, "5"))
input_data.append((1050, "RUN_1050_160717_1342_TIMING_RDOUT2.txt", 1, "5"))

#150 GeV muons
input_data.append((1140, "", 0, "12"))
input_data.append((1181, "HexaData_Run1181_TIMING_RDOUT_ORM0.txt", 0, "12"))  #Number of events below 2ms difference:  1436  /  1640

#150 GeV electrons
input_data.append((1194, "HexaData_Run1194_TIMING_RDOUT_ORM0.txt", 0, "22"))  #Number of events below 2ms difference:  831  /  1019

#250 GeV electrons
input_data.append((1038, "RUN_1038_160717_0758_TIMING_RDOUT2.txt", 0, "24"))

            
files = ["dwc_run_%s.root" % data[0] for data in input_data]
timingFileNames = [data[1] for data in input_data]


################################
# Setting an upper limit for the events to be processed, e.g. for debugging
options.maxEvents = -1
process = cms.Process("unpack")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

####################################
# Reduces the frequency of event count couts
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = options.reportEvery

####################################
# Load the standard sequences
process.load('HGCal.StandardSequences.LocalReco_cff')
####################################


####################################
# Initialize the data read-in plugins
process.source = cms.Source("HGCalTBWireChamberSource",
    OutputCollectionName = cms.string("WireChambers"), 
    fileNames = cms.untracked.vstring(["file:%s/%s"%(options.fileDirectory, file) for file in files]),
    timingFileNames = cms.vstring(["/eos/user/t/tquast/data/Testbeam/July2017/Timing/%s"%file for file in timingFileNames]),
    skipFirstEventInDWCProducer = cms.vint32([data[2] for data in input_data]),
    runType = cms.vstring([data[3] for data in input_data]),
    performAlignment = cms.untracked.bool(True),
    alignmentParamaterFile = cms.untracked.string("/tmp/millepede.res") 

)


process.millepede_binarywriter.binaryFile = cms.string('/afs/cern.ch/user/t/tquast/millepede.bin')
process.millepede_binarywriter.nLayers = cms.int32(4)
process.millepede_binarywriter.MWCQualityCut = cms.bool(True)
process.millepede_binarywriter.makeTree = cms.untracked.bool(True)
process.millepede_binarywriter.MWCHAMBERS = cms.InputTag("source","WireChambers","unpack")
process.millepede_binarywriter.RUNDATA = cms.InputTag("source","RunData","unpack")
process.millepede_binarywriter.fittingMethod = cms.string("lineAnalytical")
process.millepede_binarywriter.binaryFile = cms.string("/tmp/millepede.bin")
                              


#tree file:
process.TFileService = cms.Service("TFileService", fileName = cms.string("outfile_DWCs.root"))

####################################
#add skip event exception which might occur for simulated samples because the last event is not properly passed forward
process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.p = cms.Path(process.millepede_binarywriter*process.dwc_ntupelizer)

