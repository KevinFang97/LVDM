# LVDM
Keras Implementation of the paper "Latent Variable Dialogue Models and their Diversity"

Preprocess the data using dp.cpp which indexing the words with high frequency and others as 0(<UNK>), also padding with -1(<EOS>) and printing the word amount and sentence length.

# FAR FROM FINISHED
Model is implemented in process.py where input is the indexed data and output is also indexed(int value).

Use translate.cpp to translate indexed data to normal sentences.
