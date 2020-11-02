with open("trenovaciSoubor.txt", 'r', encoding="utf8") as infile, open("SouborFinal.txt", 'w', encoding="utf8") as outfile:
    for line in infile:
        line = line.lower()
        line = ''.join([c for c in line if c not in "1234567890[](){}<>*/\|,_--'¨%#`^&$¬!@’"])
        line = line.replace(u"\u00A0", " ")

        znak = " 0 "
        
        if "." in line:
            line = line.replace(".", znak)
        if "?" in line:
            line = line.replace("?", znak)
        if "!" in line:
            line = line.replace("!", znak)
        if ":" in line:
            line = line.replace(":", znak)
        if ";" in line:
            line = line.replace(";", znak)


        line = bytes(line, 'utf-8').decode('utf-8','ignore')
        outfile.write(line)
