import numpy as np
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE
import pandas as pd
from sklearn.decomposition import PCA

from matplotlib import colors as mcolors 
import math 

import seaborn as sns

def nacti_vektory():
    print("Načítám vektory")
    slova={}
    vektor = []
    with open("vektory/vektory_200K_100dim_25win.txt","r", encoding="utf-8") as infile:
        data = infile.readlines()
        for i in range(1, len(data)):
            radek = data[i].split(' ')
            slovo = radek[0]

            velikost = 0
            for j in range(1,101):
                velikost+=pow(float(radek[j]),2)
            velikost = math.sqrt(velikost)
            for j in range(1,101):
                vektor.append(float(radek[j])/velikost)

            slova[slovo]=vektor
            vektor=[]

    print("dokončeno načítání")
    return slova

def opacna_slova(slova, zadane_slovo):
    nejblizsi_vzdalenost = [0]*50
    nejblizsi_slova = [""]*50
    vector = [0]*100

    #zjistit, jestli existuje slovo
    if zadane_slovo not in slova:
        print("Zadané slovo neexistuje ve slovníku, zkuste zadat jiné slovo.")
        return -1

    #výpočet vzdálenosti k ostatním slovům
    for i in range(100):
        vector[i] = ((slova["sever"][i]-slova["jih"][i]) + (slova["východ"][i]-slova["západ"][i]) + (slova["velký"][i]-slova["malý"][i]))/3 + slova[zadane_slovo][i]
    vzdalenost = 0
    for i in range(100):
        vzdalenost+=pow(vector[i],2)
    vzdalenost=math.sqrt(vzdalenost)
    for i in range(100):
        vector[i]/=vzdalenost
    # počet ke všem slovům
    #pocet_vsech = len(slova)
    for i in slova: #range(pocet_vsech):
        if i == zadane_slovo:
            continue
        dist = 0
        for j in range(100):
            dist +=vector[j]*slova[i][j]
        for j in range(50):
            if dist > nejblizsi_vzdalenost[j]:
                k=49
                while k>j:
                    nejblizsi_vzdalenost[k] = nejblizsi_vzdalenost[k-1]
                    nejblizsi_slova[k] = nejblizsi_slova[k-1]
                    k-=1
                nejblizsi_vzdalenost[j]=dist
                nejblizsi_slova[j]=i
                break
    for i in range(50):
        print(nejblizsi_slova[i], nejblizsi_vzdalenost[i])  
    return nejblizsi_slova


def nacti_testovaci_slova():
    slovnik = {}
    with open("opacna_slova_test_data.txt", "r", encoding="utf-8") as in_file:
        for slovo in in_file:
            slova = slovo.strip().split(" ")
            slovnik[slova[0]]=slova[1]
    return slovnik

def zjistit_uspesnost(slova, test):
    pocetVsech = len(test)
    pocetSpravnych=0
    for key in enumerate(test):
        slovo = key[1]
        nejblizsi_slova = opacna_slova(slova, slovo)
        if nejblizsi_slova == -1: continue
        if test[slovo] in nejblizsi_slova:
            pocetSpravnych+=1
    print("Úspěšnost je: " + str((pocetSpravnych/pocetVsech)*100) + " %")    


if __name__ == "__main__":
    slova = {}
    slova = nacti_vektory()
    test = {}
    test = nacti_testovaci_slova()
    zjistit_uspesnost(slova,test)

    while True:
        val = input("Zadej slovo (ukonči slovem: exit): ")
        val = val.lower()
        if "exit" in val:
            break
        else:
            opacna_slova(slova, val)