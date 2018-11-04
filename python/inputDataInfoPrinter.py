import sys
import csv
import numpy as np
import tensorflow as tf
import math
import pickle
from tensorflow import keras
import matplotlib.pyplot as plt


import sys
import csv
import numpy as np
import tensorflow as tf
import math
import pickle
from tensorflow import keras
import matplotlib.pyplot as plt

def loadDataFromFile(datasetPath):
    with open(datasetPath, 'r', newline='') as csvFile:
        num_lines = sum(1 for line in csvFile)

    with open(datasetPath, 'r', newline='') as csvFile:
        csvReader = csv.reader(csvFile, delimiter=",", quotechar='"')
        header = next(csvReader)
        columnCount = len(header) - 1

    data = np.zeros((num_lines - 1, columnCount))
    with open(datasetPath, 'r', newline='') as csvFile:
        csvReader = csv.reader(csvFile, delimiter=",", quotechar='"')
        header = next(csvReader)
        for i in range(num_lines - 1):
            row = next(csvReader)
            for j in range(columnCount):
                data[i][j] = float(row[j])
    mean = data.mean(axis=0)
    std = data.std(axis=0)
    data = data - mean
    data = np.divide(data, std, out=np.zeros_like(data), where=std!=0)
    return (data, header, mean, std)

if len(sys.argv) < 4:
    print("USAGE: python program.py trainingDataFile labelDataFile outputFile")
inputDataFilePath = sys.argv[1]
outputDataFilePath = sys.argv[2]
outputFilePath = sys.argv[3]

inputData, inputHeader, inputMean, inputStd = loadDataFromFile(inputDataFilePath)
outputData, outputHeader, outputMean, outputStd = loadDataFromFile(outputDataFilePath)

with open(outputFilePath, 'wt') as outFile:
    writer = csv.writer(outFile, delimiter=';')
    for i in range(len(inputHeader) - 1):
        writer.writerow([inputHeader[i], inputMean[i], inputStd[i]])
    writer.writerow([])
    for i in range(len(outputHeader) - 1):
        writer.writerow([outputHeader[i], outputMean[i], outputStd[i]])

