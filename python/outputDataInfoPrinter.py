import sys
import csv
import numpy as np
import tensorflow as tf
import math
import pickle
from tensorflow import keras
import matplotlib.pyplot as plt
import os
os.environ['CUDA_VISIBLE_DEVICES'] = '-1'

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

inputDataFilePath = sys.argv[1]
outputDataFilePath = sys.argv[2]
modelFilePath = sys.argv[3]
historyFilePath = sys.argv[4]
outputFilePath = sys.argv[5]

inputData, inputHeader, inputMean, inputStd = loadDataFromFile(inputDataFilePath)
outputData, outputHeader, outputMean, outputStd = loadDataFromFile(outputDataFilePath)
model = keras.models.load_model(modelFilePath)

with open(historyFilePath, 'rb') as historyFile:
    history = pickle.load(historyFile)

predicted = model.predict(inputData)
errors = predicted - outputData
errorsAbs = np.abs(errors)
errorsMean = errors.mean(axis=0)
errorsScaled = np.multiply(errorsMean, outputStd, out=np.zeros_like(errorsMean), where=outputStd!=0)
weights = model.get_weights()
print(errors)

with open(outputFilePath, 'wt', newline='') as outFile:
    writer = csv.writer(outFile, delimiter=';')
    for i in range(len(outputHeader) - 1):
        writer.writerow([outputHeader[i], errorsMean[i], errorsScaled[i]])

def plot_history(history):
    plt.figure()
    plt.xlabel('Epoca')
    plt.ylabel('Erro absoluto médio')
    plt.plot(history['epoch'], np.array(history['mean_absolute_error']),
            label='Treino')
    plt.plot(history['epoch'], np.array(history['val_mean_absolute_error']),
            label = 'Validação')
    plt.legend()
    plt.show()

with open(historyFilePath, 'rb') as file:
    history = pickle.load(file)
    history['epoch'] = range(len(history['mean_absolute_error']))
    plot_history(history)