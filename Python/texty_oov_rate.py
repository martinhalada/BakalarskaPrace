

if __name__ == "__main__":
    """
    with open("slovnik.txt", "r", encoding="utf-8") as slovnik_soubor:
        data = slovnik_soubor.read().split(" ")

    slovnik = {}
    it = iter(data)
    for x, y, z in zip(it, it, it):
        slovnik[y] = int(z)
    """

    with open("trenovaciSoubor.txt", "r", encoding="utf-8") as trenovaci_soubor:
        data = trenovaci_soubor.read()

    pocitadlo=0
    for slovo in data.split(" "):
        print(slovo)
        pocitadlo+=1
        if pocitadlo>10:
            break
            