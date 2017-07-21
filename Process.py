from keras.layers import Input, Embedding, GRU, Dense, concatenate, Dropout
import tensorflow as tf
import collections
import numpy as np
import random
import re
import math

#training datas are sentences with words embedded as a 512 dim vector

train_size = 10000 #10000 pairs of x,y
embedding_dim = 512
word_amount = 5000; #word amount of int data(we have this in the command line after compiling and running dp.cpp)
sentence_length = 20; #sentence length of int data(we have this in the command line after compiling and running dp.cpp)
#size of train_x, train_y : [train_size,?(words_in_a_sentence),embedding_dim]




x = Input(shape=(sentence_length), name='x')
embedded_x = Embedding(output_dim=512, input_dim=word_amount, input_length=100)(x)
hx_cat = Bidirectional(GRU(embedding_dim))(embedded_x)
hx = Dense(64, activation='sinh')(hx_cat)

y = Input(shape=(sentence_length), name='y')
embedded_y = Embedding(output_dim=512, input_dim=word_amount, input_length=100)(y)
hy_cat = Bidirectional(GRU(embedding_dim))(embedded_y)
hy = Dense(64, activation='sinh')(hy_cat)

hxhy = concatenate([hx, hy], axis = 0)

z_dim = 64
Q_u = Dense(z_dim)(hxhy) #what's the activation func here?
Q_sigma = Dense(z_dim)(hxhy)
sample = tf.random_normal([z_dim]) #sample layer from standard normal multivariate distribution
Q_dot = dot([sample,Q_sigma], axes = 0)
Q = add([Q_dot,Q_u])

hxz_cat = concatenate([hx, z], axis = 0)
hxz = Dropout(0.5)(hxz_cat)
#annealing?

prediction_raw = GRU(word_amount)(hxz)
prediction = softmax(prediction_raw)

#some loss_func calculation supporting tensor
Q_sigma_square = dot([Q_sigma,Q_sigma], axes = 0)
Q_u_square = dot([Q_u,Q_u], axes = 0)
kl_loss = 0.5 * tf.reduce_mean(1 + tf.log(Q_sigma_square) - Q_u_square - Q_sigma_square)

#define loss function with KL term annealing
run_time = 0; 
anneal_parameter = 0.0001
def kl_weight()
	temp = run_time*anneal_parameter
	run_time += 1
	if (temp > 1) :
		temp = 1.0
	return temp

def total_loss()
	return kl_loss*kl_weight() + categorical_crossentropy(y_raw, prediction)
	

model = Model(inputs=x, outputs = predictions)
model.compile(optimizer = 'adadelta', loss =  'total_loss')





