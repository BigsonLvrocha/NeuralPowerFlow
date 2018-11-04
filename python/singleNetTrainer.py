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

if (len(sys.argv) < 7):
    print("USAGE: python program.py trainDataFile labelDataFile FirstLayer SecondLayer modelOutput historyOutput")
    sys.exit(1)

trainDataPath = sys.argv[1]
labelDataPath = sys.argv[2]
layer1 = int(sys.argv[3])
layer2 = int(sys.argv[4])
modelOutputPath = sys.argv[5]
historyOutputPath = sys.argv[6]
epochs = 300

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
    return data

def build_model(layer1, layer2, inputData, labelData):
    model = keras.Sequential([
        keras.layers.Dense(layer1, activation=tf.nn.sigmoid,
                        input_shape=(inputData.shape[1],)),
        keras.layers.Dense(layer2, activation=tf.nn.sigmoid),
        keras.layers.Dense(labelData.shape[1])
    ])
    optimizer = tf.keras.optimizers.SGD(lr=0.1)
    model.compile(loss='mse',
                optimizer=optimizer,
                metrics=['mae'])
    return model

trainData = loadDataFromFile(trainDataPath)
labelData = loadDataFromFile(labelDataPath)
model = build_model(layer1, layer2, trainData, labelData)
early_stop = keras.callbacks.EarlyStopping(monitor='val_loss', patience = 10)
history = model.fit(
        trainData,
        labelData,
        epochs=epochs,
        validation_split=0.2,
        verbose=1,
        callbacks=[early_stop],
        batch_size=1)
with open(historyOutputPath, 'wb') as historyFile:
    pickle.dump(history.history, historyFile)
model.save(modelOutputPath)