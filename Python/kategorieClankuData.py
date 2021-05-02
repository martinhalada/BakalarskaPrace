import re
import csv

data = []

def nacti_data(soubor_nazev, trida):
    """
    Fce načte data
    """
    with open(soubor_nazev, "r", encoding="utf-8") as soubor:
        cely_soubor = soubor.read()
        vety = re.split(r' *[\.\?!][\'"\)\]]* *',cely_soubor)
        pocitadlo=0
        for v in vety:
            if len(v) == 0: continue
            vysl = [trida, v]
            data.append(vysl)
            pocitadlo+=1
            if pocitadlo >= 200000:
                return

def uloz_do_souboru():
    pole = ['kategorie', 'veta']

    with open("clanky_kategorie_200K.csv", "w", encoding="utf-8") as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(pole)
        csvwriter.writerows(data)

if __name__ == '__main__':
    soubor_sport = "Clanky1000strSport2Preprocess.txt"
    soubor_domaci = "Clanky1000strDomaci2Preprocess.txt"
    soubor_kultura = "Clanky1000strKulturaPreprocess.txt"

    nacti_data(soubor_sport, 0)
    nacti_data(soubor_domaci, 1)
    nacti_data(soubor_kultura, 2)

    uloz_do_souboru()
