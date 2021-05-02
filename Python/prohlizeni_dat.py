import pandas as pd
import matplotlib.pyplot as plt 

def sentiment():
    """
    Metoda vizualizuje rozdělení dat
    """

    data = pd.read_csv("komentare.csv", delimiter = ",") #clanky_kategorie.csv
    print("Columns in the original dataset:\n")
    print(data.columns)

    print("Number of rows per star rating:")
    print(data['hodnoceni'].value_counts())

    # Function to map stars to sentiment
    def map_sentiment(stars_received):
        """
        if stars_received <= 2:
            return -1
        elif stars_received <= 4:
            return 0
        else:
            return 1
        """
        return stars_received
        
    # Mapping stars to sentiment into three categories
    data['sentiment'] = [ map_sentiment(x) for x in data['hodnoceni']]
    # Plotting the sentiment distribution
    plt.figure()
    pd.value_counts(data['sentiment']).plot.bar(title="Rozdělení sentimentu v datech")
    plt.xlabel("Sentiment")
    plt.ylabel("Počet řádků")
    plt.show()

def kategorie():
    """
    Metoda vizualizuje rozdělení dat
    """
    
    data = pd.read_csv("clanky_kategorie_200K.csv", delimiter = ",")
    print("Columns in the original dataset:\n")
    print(data.columns)

    print("Number of rows per star rating:")
    print(data['kategorie'].value_counts())

    # Function to map stars to sentiment
    def map_kategory(stars_received):
        if stars_received == 0:
            return 0
        elif stars_received == 1:
            return 1
        else:
            return 2
    # Mapping stars to sentiment into three categories
    data['kat'] = [ map_kategory(x) for x in data['kategorie']]
    # Plotting the sentiment distribution
    plt.figure()
    pd.value_counts(data['kat']).plot.bar(title="Rozdělení kategorií v datech")
    plt.xlabel("Kategorie")
    plt.ylabel("Počet řádků")
    plt.show()

if __name__ == '__main__':
    sentiment()
    kategorie()
