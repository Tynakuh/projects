import re
GEN = 0

for x in range(GEN,40):
    f = open('network' + str(x) + '.txt','r')
    txt = f.read()
    txt = txt+"["
    result = re.findall(r"\](.*?)\[",txt)
    fwrite = open('networkhardwareResults.txt','a')
    fwrite.write('\n' + ', '.join(result))
    fwrite.close()
    f.close()
