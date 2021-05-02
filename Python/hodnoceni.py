import requests
import csv
from urllib.request import urlopen
from bs4 import BeautifulSoup

links = []

def najdi_filmy(_url):
    url = _url
    r = requests.get(url, headers={'User-Agent': 'Mozilla/5.0'}) 
    page = r.text
  
    soup = BeautifulSoup(page, 'html.parser') 

    main_content = soup.find('table', attrs={'class':'content ui-table-list striped'})
    a_content = main_content.findAll('a')
    
    for link in a_content:
        links.append(link['href'])

def preved_hodnoceni(hvezdicky):
    return len(hvezdicky)

data = []

def uloz_do_souboru():
    pole = ['hodnoceni', 'koment']

    with open("komentare.csv", "w", encoding="utf-8") as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(pole)
        csvwriter.writerows(data)

def najdi_hodnoceni():
    url = "https://www.csfd.cz"
    
    pom = 1

    for film in links:
        link = film
        new_url = url+link
        url_comentare = new_url + "komentare/?all=1"

        r = requests.get(url_comentare, headers={'User-Agent': 'Mozilla/5.0'}) 
        page = r.text
        soup = BeautifulSoup(page, 'html.parser')

        content = soup.find('ul', attrs={'class':'ui-posts-list'})
        li_content = content.findAll('li')

        #print(url_comentare)
        
        pocet_hodnoceni = len(li_content)
        #print(pocet_hodnoceni)
        for i in range(pocet_hodnoceni):
            #print(i)
            img = li_content[i].find('img', attrs={'class':'rating'})
            if img is None:
                continue
            #print(img)
            hodnoceni = preved_hodnoceni(img['alt'])
            coment = li_content[i].find('p', attrs={'class':'post'}).text

            vysl = [hodnoceni, coment]
            data.append(vysl)

        pom+=1
        print(pom)

def nacti_csv():
    filename = "komentare.csv"
    fields = []
    rows = []

    with open(filename, 'r', encoding = "utf-8") as csvfile: 
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
    
    #  printing first 5 rows 
    print('\nFirst 5 rows are:\n') 
    for row in rows[:5]: 
        # parsing each column of a row 
        for col in row: 
            print("%10s"%col), 
        print('\n') 

if __name__ == '__main__':
    url = "https://www.csfd.cz/zebricky/nejlepsi-filmy/?show=complete"
    najdi_filmy(url)
    url = "https://www.csfd.cz/zebricky/nejhorsi-filmy/?show=complete"
    najdi_filmy(url)
    url = "https://www.csfd.cz/zebricky/nejrozporuplnejsi-filmy/?show=complete"
    najdi_filmy(url)

    #najdi_hodnoceni()
    #uloz_do_souboru()

    nacti_csv()
