import sys
import csv
import numpy as np
import tensorflow as tf
import math
from tensorflow import keras
import os
import time

trainDataPath = sys.argv[1]
labelDataPath = sys.argv[2]
layer1 = int(sys.argv[3])
layer2 = int(sys.argv[4])
withGpu = sys.argv[5] == '1'
epochs1 = 150
epochs2 = 10000
countEpochs1 = 0
countEpochs2 = 0
batch1 = 1
batch2 = 256
time1 = 0
time2 = 0
validationSplit = 0.2
minMae = 0.001
early_stop1 = keras.callbacks.EarlyStopping(monitor='val_loss', patience = 10)
early_stop2 = keras.callbacks.EarlyStopping(monitor='val_loss', patience = 10)
earlyStopError = 0.01

class EarlyStoppingByLossVal(keras.callbacks.Callback):
    def on_epoch_end(self, epoch, logs={}):
        global earlyStopError
        current = logs.get('mean_absolute_error')
        if current < earlyStopError:
            self.model.stop_training = True

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

if (not withGpu):
    os.environ['CUDA_VISIBLE_DEVICES'] = '-1'

trainData = loadDataFromFile(trainDataPath)
labelData = loadDataFromFile(labelDataPath)

for i in range(3):
    model = build_model(layer1, layer2, trainData, labelData)
    initTime = time.time()
    history1 = model.fit(
        trainData,
        labelData,
        epochs=epochs1,
        validation_split=validationSplit,
        verbose=1,
        callbacks=[early_stop1, EarlyStoppingByLossVal()],
        batch_size=batch1
    )
    time1 += time.time() - initTime
    countEpochs1 += len(history1.history["val_mean_absolute_error"])

    model = build_model(layer1, layer2, trainData, labelData)
    initTime = time.time()
    history2 = model.fit(
        trainData,
        labelData,
        epochs=epochs2,
        validation_split=validationSplit,
        verbose=1,
        callbacks=[early_stop2, EarlyStoppingByLossVal()],
        batch_size=batch2
    )
    time2 += time.time() - initTime
    countEpochs2 += len(history2.history["val_mean_absolute_error"])

print("\nResultado final\n\n")
print("batch = " + str(batch1) + "\n")
print("épocas = " + str(countEpochs1/3) + "\n")
print("tempo =  " + str(time1/3) + "\n\n")
print("batch = " + str(batch2) + "\n")
print("épocas = " + str(countEpochs2/3) + "\n")
print("tempo =  " + str(time2/3) + "\n\n")
