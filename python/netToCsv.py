import sys
import csv
import numpy as np
import tensorflow as tf
import math
from tensorflow import keras
import matplotlib.pyplot as plt

modelPath = sys.argv[1]
csvOutput = sys.argv[2]

model = keras.models.load_model(modelPath)
weights = model.get_weights()
print(weights)
print(len(weights))

with open(csvOutput, 'w', newline='') as csvFile:
    writer = csv.writer(csvFile, delimiter=';')
    for i in range(math.floor(len(weights)/2)):
        writer.writerow(['camada ' + str(i+1)])
        line = ['neuronio', 'b']
        for j in range(weights[2*i].shape[0]):
            line.append('w_' + str(j))
        writer.writerow(line)
        for j in range(weights[2*i].shape[1]):
            line = [j, weights[2*i+1][j]]
            for k in range(weights[2*i].shape[0]):
                line.append(weights[2*i][k][j])
            writer.writerow(line)

