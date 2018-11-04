import sys
import csv
import numpy as np
import tensorflow as tf
import math
import pickle
from tensorflow import keras
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
        
def trainModel(model, inputData, labelData, split, patience = 10):
    EPOCHS = 100
    # The patience parameter is the amount of epochs to check for improvement
    early_stop = keras.callbacks.EarlyStopping(
        monitor='val_loss',
        patience = patience
    )

    # Store training stats
    history = model.fit(
        train_data,
        label_data,
        epochs=EPOCHS,
        validation_split=split,
        verbose=1,
        callbacks=[early_stop],
        batch_size=1
    )
    return history


datasetPath = sys.argv[1]
labelPath = sys.argv[2]
modelPath = sys.argv[3]
historyPath = sys.argv[4]

validationSplit = 0.2
train_data = loadDataFromFile(datasetPath)
label_data = loadDataFromFile(labelPath)

layers = [10, 10]
errorBest = 0
errorNow = 0
patience = 5
netBestMeanError = 0
netBestValMeanError = 0
netBestErrorTotal = 0
netBestEpoch = 0
errors = []

while (1 == 1):
    netBestMeanError = 0
    netBestValMeanError = 0
    netBestErrorTotal = 0

    print('layers: ')
    print(layers)

    for i in range(patience):
        model = build_model(layers[0], layers[1], train_data, label_data)
        history = trainModel(model, train_data, label_data, 0.2)
        meanError = history.history['mean_absolute_error'][-1]
        valMeanError = history.history['val_mean_absolute_error'][-1]
        errorTotal = meanError + valMeanError

        print('\ni: ' + str(i) + '\n\n')
        print('mean_absolute_error: ' + str(meanError) + '\n')
        print('val_mean_absolute_error: ' + str(valMeanError) + '\n')
        print('errorTotal: ' + str(errorTotal) + '\n')
        
        if (errorTotal < netBestErrorTotal or netBestErrorTotal == 0):
            netBestMeanError = meanError
            netBestValMeanError = valMeanError
            netBestErrorTotal = errorTotal
            net = model
            netHistory = history
            netBestEpoch = len(history.epoch)
    
    errors.append([
        netBestMeanError,
        netBestValMeanError,
        netBestErrorTotal,
        netBestEpoch
    ])

    if netBestErrorTotal < errorBest or errorBest == 0:
        errorBest = netBestErrorTotal
        bestModel = net
        bestHistory = netHistory
        histPath = historyPath + '_' +  str(layers[0]) + '_' + str(layers[1])
        modPath = modelPath + '_' + str(layers[0]) + '_' + str(layers[1])
        with open(histPath, 'wb') as historyFile:
            pickle.dump(bestHistory.history, historyFile)
        bestModel.save(modPath)
        layers[0] += 5
        layers[1] += 5
    else:
        break


print(errors)
