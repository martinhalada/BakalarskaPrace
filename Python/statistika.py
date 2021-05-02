from collections import Counter
import pandas as pd
import csv


def vypisStatistikuVektory():
    with open("trenovaciSoubor.txt", "r", encoding = "utf-8") as soubor:
        data = soubor.read()
        #vety = data.split("0")
        #print(len(vety))
        
        #slova = data.split(" ")
        #unikatni_slova = set(slova)
        #print(len(unikatni_slova))

        slova = data.split()
        counter = Counter(slova)
        nejcastejsi_slova = counter.most_common(10)
        print(nejcastejsi_slova)

def vypisStatistikuSentiment():
    # initializing the titles and rows list
    fields = []
    rows = []
    
    # reading csv file
    with open("komentare.csv", 'r', encoding = "utf-8") as csvfile:
        # creating a csv reader object
        csvreader = csv.reader(csvfile)
        
        # extracting field names through first row
        fields = next(csvreader)
    
        # extracting each data row one by one
        for row in csvreader:
            rows.append(row)
    
        # get total number of rows
        print("Total no. of rows: %d"%(csvreader.line_num))
    
    # printing the field names
    print('Field names are:' + ', '.join(field for field in fields))
    
    data = ""
    for row in rows[:100000]:
        # parsing each column of a row
        for col in row:
            data += col
    
    vety = data.split(".")
    print(len(vety))
       
    slova = data.split(" ")
    print(len(slova))

    unikatni_slova = set(slova)
    print(len(unikatni_slova))

    slova = data.split()
    counter = Counter(slova)
    nejcastejsi_slova = counter.most_common(10)
    print(nejcastejsi_slova)
    


def vypisStatistikuKategorie():
    # initializing the titles and rows list
    fields = []
    rows = []
    
    # reading csv file
    with open("clanky_kategorie_200K.csv", 'r', encoding = "utf-8") as csvfile:
        # creating a csv reader object
        csvreader = csv.reader(csvfile)
        
        # extracting field names through first row
        fields = next(csvreader)
    
        # extracting each data row one by one
        for row in csvreader:
            rows.append(row)
    
        # get total number of rows
        print("Total no. of rows: %d"%(csvreader.line_num))
    
    # printing the field names
    print('Field names are:' + ', '.join(field for field in fields))
    
    data = ""
    for row in rows[:600000]:
        # parsing each column of a row
        for col in row:
            data += col
    
    vety = data.split(".")
    print(len(vety))
       
    slova = data.split(" ")
    print(len(slova))

    unikatni_slova = set(slova)
    print(len(unikatni_slova))

    slova = data.split()
    counter = Counter(slova)
    nejcastejsi_slova = counter.most_common(10)
    print(nejcastejsi_slova)

if __name__ == "__main__":
    #vypisStatistikuVektory()
    #vypisStatistikuSentiment()
    vypisStatistikuKategorie()