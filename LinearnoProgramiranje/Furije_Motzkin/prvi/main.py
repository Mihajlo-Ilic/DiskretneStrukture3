import random
import math
import sys

def eliminisi_promenljivu(A):
    Vece = []
    Manje = []
    Nule = []

    for nejednacina in A:

        #ako je koeficijent uz x jednak 0 prepisujemo u Nule
        if nejednacina[0] == 0:
            Nule.append( [nejednacina[i] for i in range(1,len(nejednacina))]  )
        #ako je koef uz x veci od 0 prebacujemo sve na drugu stranu i delimo sa koefom
        elif nejednacina[0]>0:
            tek_vece = []
            for i in range(1,len(nejednacina)-1):
                tek_vece.append(-nejednacina[i]/nejednacina[0])
            tek_vece.append(nejednacina[-1] / nejednacina[0])
            Vece.append(tek_vece)
        #ako je manji prebacujemo i delimo sve sa neg koefom
        else :
            tek_manje = []
            for i in range(1, len(nejednacina)-1):
                tek_manje.append(nejednacina[i] / -nejednacina[0])
            tek_manje.append(nejednacina[-1] / nejednacina[0])
            Manje.append(tek_manje)

    novo_A = []
    #nejednacine gde je uz x bio 0 koeficijent prepisujemo jer je vec eliminisan x
    for nejednacina in Nule:
        novo_A.append(nejednacina)
    #za svaku nejednacinu >= i svaku <= kombinujemo i dodajemo u novu matricu
    for vece in Vece:
        for manje in Manje:

            nej = [ manje[i]-vece[i] for i in range(0,len(vece)-1)]
            nej.append(vece[-1]-manje[-1])
            novo_A.append(nej)
    return novo_A

def nadji_interval(A):
    vece = []
    manje = []
    for nejednacina in A:
        if nejednacina[0]<0:
            manje.append(nejednacina)
        elif nejednacina[0]>0:
            vece.append(nejednacina)
    donji = -math.inf
    gornji = math.inf
    if len(vece)>0:
        for v in vece:
            s = 0
            for l in range(1,len(v)-1):
                s -= v[l]
            v[-1] +=s
        donji =  max(vece, key=lambda x: x[-1]/x[0])
        donji = donji[-1]/donji[0]
    if len(manje) > 0:
        for v in manje:
            s = 0
            for l in range(1, len(v) - 1):
                s -= v[l]
            v[-1] += s
        gornji = min(manje, key=lambda x: -x[-1]/(-x[0]) )
        gornji = -gornji[-1]/(-gornji[0])

        if donji>gornji:
            print('donji ={}  , gornji ={} '.format(donji,gornji))
            print('Greska donji>gornji nema resenja')
            quit()
    return donji,gornji

def nalazi_resenja(istorija,n):
    if n ==0:
        return
    donji,gornji = nadji_interval(istorija.pop())
    x = random.uniform(donji,gornji)

    if donji ==-math.inf and gornji!=math.inf:
        x = gornji

    if donji !=math.inf and gornji==math.inf:
        x = donji

    if donji ==-math.inf and gornji==math.inf:
        x = 0

    for ist in istorija:
        for k in ist:
            k[n-1] = k[n-1] *x
    print("X{} pripada u [{d:.2f},{g:.2f}] uzimamo broj {br:.2f}".format(n,d=donji,g=gornji,br=x))
    nalazi_resenja(istorija,n-1)


def main():

    args = sys.argv

    if(len(args)!=2):
        print('nije prosledjen lepo fajl')
        quit()
    f = open(args[1],"r")

    A = []

    for line in f:
        red = [float(x) for x in line.split()]
        A.append(red)



    prethodni = []
    novo_A = A
    prethodni.append(novo_A)
    while True:

        novo_A = eliminisi_promenljivu(novo_A)
        prethodni.append(novo_A)
        if len(novo_A[0]) <= 2:

            nalazi_resenja(prethodni,len(A[0])-1)
            break
    f.close()


if __name__ == "__main__":
    main()

