from __future__ import division
import sys, os

os.environ["CUDA_VISIBLE_DEVICES"] = "1"

import numpy as np
from sklearn.utils import shuffle
import re
import string
import math
from ROOT import TFile, TTree
from ROOT import *
import ROOT
import numpy as np
from keras.models import Model, Sequential, load_model
from keras.layers import Input, Dense, Activation, Dropout, add
from keras.regularizers import l2
from keras.layers.normalization import BatchNormalization
from array import array
import pandas as pd

timer = ROOT.TStopwatch()
timer.Start()

trainInput = "/home/ljw1015/public/5f/b1M63lj_delphes_default_5f.h5"
data = pd.read_hdf(trainInput)

# pickup only interesting variables
variables = ['dR','M','pt1','pt2','eta1','eta2','e1','e2']
pd_train_out  = data.filter(items = ['signal'])
pd_train_data = data.filter(items = variables)

#covert from pandas to array
train_out = np.array( pd_train_out )
train_data = np.array( pd_train_data )

numbertr=len(train_out)

#Shuffling
order=shuffle(range(numbertr),random_state=100)
train_out=train_out[order]
train_data=train_data[order,0::]

train_out = train_out.reshape( (numbertr, 1) )
trainnb=0.9 # Fraction used for training

#Splitting between training set and cross-validation set
valid_data=train_data[int(trainnb*numbertr):numbertr,0::]
valid_data_out=train_out[int(trainnb*numbertr):numbertr]

train_data_out=train_out[0:int(trainnb*numbertr)]
train_data=train_data[0:int(trainnb*numbertr),0::]

import tensorflow as tf

model = tf.keras.models.Sequential([
  tf.keras.layers.Flatten(),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(300, activation=tf.nn.relu),
  tf.keras.layers.Dropout(0.1),
  tf.keras.layers.Dense(1, activation=tf.nn.sigmoid)
])

modelshape = "10L_300N"
batch_size = 1024
epochs = 50
model_output_name = 'model_%s_%dE' %(modelshape, epochs)

if os.path.exists('models/'+model_output_name+'/model.h5'):
    print "Model file exists already!"
    from tensorflow import keras
    model = keras.models.load_model('models/'+model_output_name+'/model.h5')
else:
    model.compile(optimizer='adam',
              loss='binary_crossentropy',
              metrics=['accuracy', 'binary_crossentropy'])
    history = model.fit(train_data, train_data_out, batch_size=batch_size, epochs=epochs, validation_data=(valid_data,valid_data_out))

    if not os.path.exists("models"):
      os.mkdir('models')
    os.mkdir('models/'+model_output_name)
    model.save('models/'+model_output_name+'/model.h5')

    #using only fraction of data
    #evaluate = model.predict( valid_data ) 

model.summary()

# check the matching efficiency with full dataset 
# filter array with only interesting variables
jetCombi = data.filter(items = variables)

# convert to array
input_data = np.array(jetCombi)

# predict from the model
pred = model.predict( input_data )

# change the format to pandas
pred = pd.DataFrame(pred, columns=['pred'])

# add prediction as an element
output_data = pd.concat([data,pred], axis=1)

#calculate total number of events 
total_selected_events = output_data.groupby('event')
nEvents = len(total_selected_events)

#calculate machable events 
matachable = output_data[ output_data['signal'] > 0 ] 
matachable_grouped = matachable.groupby('event')
nmatachable = len(matachable_grouped) 

#select the max combination per event
idx = output_data.groupby('event')['pred'].transform(max) == output_data['pred']
output_events = output_data[idx]

#select correctly predicted events
output_events_correct = output_events[ output_events['signal'] > 0 ]

#calcuate the efficiency
num = len(output_events_correct)
eff = num / nEvents 
matchable_ratio = nmatachable / nEvents 

#print
print "matched : ", num , "/", nEvents , " = ", eff 
print "matchable : ", nmatachable, "/", nEvents, " = ", matchable_ratio

timer.Stop()
rtime = timer.RealTime(); # Real time (or "wall time")
ctime = timer.CpuTime(); # CPU time
print("RealTime={0:6.2f} seconds, CpuTime={1:6.2f} seconds").format(rtime,ctime)
print("{0:4.2f} events / RealTime second .").format( nEvents/rtime)
print("{0:4.2f} events / CpuTime second .").format( nEvents/ctime)
