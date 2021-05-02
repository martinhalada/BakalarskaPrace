import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
import gensim
from gensim.models import Word2Vec, KeyedVectors, word2vec
from keras.preprocessing.text import Tokenizer
from keras.preprocessing import sequence
from keras.utils import to_categorical
from keras.utils import np_utils
from keras.layers.embeddings import Embedding
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, LSTM, Reshape
from keras.layers import Flatten
import matplotlib.pylab as plt
import pickle
import h5py

def natrenuj_sit(dataset, vektory_soubor):
    """
    Funkce načítá dataset a předtrénované vektory
    """

    # rozdělení dat do trénovacího, testovacího, validačního souboru
    X = dataset['texty_final']
    y = dataset['tridy']
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
    X_train, X_val, y_train, y_val = train_test_split(X_train, y_train, test_size=0.2, random_state=42)

    #načtení předtrénovaných vektorů
    w2v_model = gensim.models.KeyedVectors.load_word2vec_format(vektory_soubor, binary=False)
    vektory = w2v_model.wv.syn0

    #tokenizace vět
    tokenizer = Tokenizer(vektory.shape[0])
    tokenizer.fit_on_texts(X_train)
    seq_train = tokenizer.texts_to_sequences(X_train)
    seq_test = tokenizer.texts_to_sequences(X_test)
    seq_val = tokenizer.texts_to_sequences(X_val)

    index_slova = tokenizer.word_index
    X_train = sequence.pad_sequences(seq_train, maxlen=50)
    X_test = sequence.pad_sequences(seq_test, maxlen=50)
    X_val = sequence.pad_sequences(seq_val, maxlen=50)

    y_train = np_utils.to_categorical(y_train, 3)
    y_test = np_utils.to_categorical(y_test, 3)
    y_val = np_utils.to_categorical(y_val, 3)

    matice_vah = Embedding(vektory.shape[0],
                            vektory.shape[1],
                            weights=[vektory],
                            trainable=False)

    model = Sequential()

    model.add(matice_vah)
    
    model.add(LSTM(128, recurrent_dropout=0.5, dropout=0.2))
    model.add(Dense(3, activation='softmax'))
    #model.add(Activation('softmax'))

    model.summary()

    epochy = 5

    model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
    natrenovany_model = model.fit(X_train, y_train, epochs=epochy, batch_size=32, validation_data=(X_val, y_val)) #shuffle=True,
    score = model.evaluate(X_test, y_test)
    print("Test Loss: %.2f%%" % (score[0]*100))
    print("Test Accuracy: %.2f%%" % (score[1]*100))

    #model.save('model_sentiment.h5')
    #output = open('wordindexsentiment.pkl', 'wb')
    #pickle.dump(index_slova, output)

    """
    plt.figure(0)
    plt.plot(natrenovany_model.history['acc'],'r')
    plt.plot(natrenovany_model.history['val_acc'],'g')
    plt.xticks(np.arange(0, epochy+1, epochy/5))
    plt.rcParams['figure.figsize'] = (8, 6)
    plt.xlabel("Počet epoch")
    plt.ylabel("Přesnost")
    plt.title("Přesnost trénovacích a validačních dat")
    plt.legend(['trénovací', 'validační'])

    plt.figure(1)
    plt.plot(natrenovany_model.history['loss'],'r')
    plt.plot(natrenovany_model.history['val_loss'],'g')
    plt.xticks(np.arange(0, epochy+1, epochy/5))
    plt.rcParams['figure.figsize'] = (8, 6)
    plt.xlabel("Počet epoch")
    plt.ylabel("Ztrátová funkce")
    plt.ylabel("Ztrátová funkce trénovacích a validačních dat")
    plt.legend(['trénovací', 'validační'])
    plt.show()
    """
    

def preprocessing(x):
    """
    Funkce filtruje nežádoucí znaky/slova
    """

    x = x.lower().strip()
    x = ''.join([c for c in x if c not in "0123456789[](){}<>*/\|,_--'¨%#`^&$¬!?.:;@’"])
    x = x.replace(u"\u00A0"," ")
    x = bytes(x, 'utf-8').decode('utf-8','ignore')

    if "\r" in x:
        x = x.replace("\r",'')
    if "\n" in x:
        x = x.replace("\n",'')
    if "\t" in x:
        x = x.replace("\t",'')

    x = " ".join(x.split())
    x = x.strip()

    return x

def tridy_preprocess(x):
    """
    0 -> 0 - špatný film
    1 -> 0 - špatný film
    2 -> 1 - průměrný film
    3 -> 1 - průměrný film
    4 -> 2 - výborný film
    5 -> 2 - výborný film
    """
    if x <= 1:
        return 0
    elif x <= 3:
        return 1
    else:
        return 2

if __name__ == '__main__':
    
    komentare_soubor = "komentare.csv"
    vektory_soubor = "vektory/vektory_100K_100dim_25win.txt"

    komentare = pd.read_csv(komentare_soubor, delimiter = ",")
    komentare['texty_final'] = komentare['koment'].apply(preprocessing)
    komentare['tridy'] = [tridy_preprocess(x) for x in komentare['hodnoceni']]

    natrenuj_sit(komentare, vektory_soubor)
    
    """
    epochy=5
    plt.figure(0)
    plt.plot([1,2,3,4,5],[0.7898, 0.7938, 0.8027, 0.8089, 0.8112],'r')
    plt.plot([1,2,3,4,5],[0.7902, 0.8015, 0.8084, 0.8080, 0.8138],'g')
    plt.xticks(np.arange(0, epochy+1, epochy/5))
    plt.rcParams['figure.figsize'] = (8, 6)
    plt.xlabel("Počet epoch")
    plt.ylabel("Přesnost")
    plt.title("Přesnost trénovacích a validačních dat")
    plt.legend(['trénovací', 'validační'])

    plt.figure(1)
    plt.plot([1,2,3,4,5],[0.6283, 0.5826, 0.5461, 0.5224, 0.5072],'r')
    plt.plot([1,2,3,4,5],[0.5896, 0.5435, 0.5189, 0.5162, 0.4948],'g')
    plt.xticks(np.arange(0, epochy+1, epochy/5))
    plt.rcParams['figure.figsize'] = (8, 6)
    plt.xlabel("Počet epoch")
    plt.ylabel("Ztrátová funkce")
    plt.title("Ztrátová funkce trénovacích a validačních dat")
    plt.legend(['trénovací', 'validační'])
    plt.show()
    """
