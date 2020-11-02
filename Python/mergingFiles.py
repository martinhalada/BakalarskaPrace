import os

path = os.path.dirname(os.path.realpath(__file__))
files = [f for f in os.listdir(path) if f.endswith('.txt')]

with open('trenovaciSoubor.txt', 'w', encoding='utf-8') as outFile:
    for file in files:
        with open(file, 'r', encoding='utf-8') as inFile:
            outFile.write(inFile.read())
        outFile.write("\n")