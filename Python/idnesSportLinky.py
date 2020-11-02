# requests for fetching html of website
import requests
from bs4 import BeautifulSoup

links=[]
txt = "{} / 1000"

for i in range(5000, 4002, -1):
  url = 'https://www.idnes.cz/sport/archiv/'+str(i)
  # Make the request to a url
  r = requests.get(url)
  # Create soup from content of request
  c = r.text
  soup = BeautifulSoup(c,'html.parser')

  # Find the element on the webpage
  main_content = soup.find('div', attrs = {'id': 'list-art-count'})
  a_content = main_content.findAll('a', attrs = {'class': 'w230'})

  if(i%10 == 0): 
    print(txt.format(i))

  for x in a_content:
    links.append(x['href'])

data = links

with open("linky1000strSport1.txt", "w") as txt_file:
    for line in data:
        txt_file.write("".join(line) + "\n")
