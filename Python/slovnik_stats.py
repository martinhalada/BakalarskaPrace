import operator

if __name__ == "__main__":
    with open("slovnik.txt", "r", encoding="utf-8") as soubor:
        data = soubor.read().split(" ")
    
    slovnik = {}
    it = iter(data)
    for x, y, z in zip(it, it, it):
        slovnik[y] = int(z)
    
    sorted_d = dict( sorted(slovnik.items(), key=operator.itemgetter(1),reverse=True))
    print('Dictionary in descending order by value : ',sorted_d)
    print(slovnik["a"])

    pocet=0
    for item in sorted_d:
        if (len(item)) > 6:
            print(item, sorted_d[item])
            pocet+=1
            if pocet > 100:
                break