import requests
from bs4 import BeautifulSoup

links=[]
text_file = open("linky1000strSport1.txt", "r")
lines = text_file.read().split('\n')
links=lines
links.remove("")
#print(links)
text_file.close()

i=0
txt = "{} / 35879"
for link in links:
  i=i+1
  if (i % 10 == 0):
    print(txt.format(i))
  url = link
  # Make the request to a url
  r = requests.get(url)
  # Create soup from content of request
  c = r.content
  soup = BeautifulSoup(c,'html.parser')

  # Find the element on the webpage
  try:
    articleScript = soup.find('script')
    a=str(articleScript)
    if "free" in a:
      # Download texts
      # Find the element on the webpage
      main_content = soup.find('div', attrs = {'id': 'art-text'})
      p = main_content.findAll("p")
  except:
    print("Chyba")
  else:
      f = open("Clanky1000strSport1.txt", "a", encoding="utf-8")
      for each in p: 
        f.write(str(each.get_text()))
      f.close()
