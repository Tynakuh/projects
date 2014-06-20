import re
GEN = 0

for x in range(GEN,121):
    f = open('networknomutelite' + str(x) + '.txt','r')
    txt = f.read()
    txt = txt+"["
    result = re.findall(r"\](.*?)\[",txt)
    fwrite = open('networknomuteliteResults.txt','a')
    fwrite.write('\n' + ', '.join(result))
    fwrite.close()
    f.close()
