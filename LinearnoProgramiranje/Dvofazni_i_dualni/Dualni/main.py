import sys
import math
import random

koristi_blenda = False
pisi_indexe = False
pisi_iteracije = False
max_iteracija = math.inf

def ucitaj_problem(putanja):
    file = open(putanja)
    linije = file.readlines()
    tip = linije[0].split()[0]
    Z = []
    b= []
    A = []

    for i in range(1,len(linije[0].split())):
        Z.append(float(linije[0].split()[i]))

    for i in range(1,len(linije)):
        linija = linije[i].split()
        red = []
        for j in range(len(linija)-2):
            red.append(float(linija[j]))
        A.append(red)
        b.append([linija[-2],float(linija[-1])])

    return tip,Z,b,A

def sredi_problem(tip,Z,A,b):
    #svodimo problem na kanonski oblik dovodimo bazicne i stavljamo znakove =
    #ako je problem tipa maximizacije svodimo ga na min mnozenjem -1
    if tip=='max':
        Z = [-i for i in Z]
        tip = 'min'
    #ako je znak >= dodajemo -bazicnu inace dodajemo +bazicnu
    # takodje ako je znak >= mnozimo sa -1 ceo red
    n = len(A[0])
    B = []
    for i in range(len(b)):
        A[i] += [0] * len(b)
        if b[i][0]=='>=':
            A[i] = [-j for j in A[i]]
            b[i][1] *=-1
        b[i][0]='='
        A[i][n+i] = 1
        B.append(n+i)
    Z += [0] * len(b)
    return tip,Z,b,A,B


if __name__ == '__main__':

    if len(sys.argv)==3:
        pisi_indexe = 'i' in sys.argv[2]
        koristi_blenda = 'b' in sys.argv[2]
        pisi_iteracije = 'l' in sys.argv[2]

    if len(sys.argv) == 4:
        max_iteracija = int(sys.argv[3])

    print('Parametri programa:')
    print('Koristimo blenda: {}'.format(koristi_blenda))
    print('Pisemo indexe: {}'.format(pisi_indexe))
    print('Pisemo br iteracija: {}'.format(pisi_iteracije))
    print()
    #ucitavamo pocetni problem
    tip,Z,b,A = ucitaj_problem(sys.argv[1])
    #dovodimo problem na kanonski oblik
    tip,Z,b,A,B = sredi_problem(tip,Z,A,b)
    #zapocinjemo algoritam uslov za stop nam je da je svako b >= 0
    func = 0
    iteracija = 0
    while True:
        neg_b = [i[1] for i in b if i[1]<0]

        if iteracija>= max_iteracija:
            print('Prekida program zbog previse iteracija: {}'.format(iteracija))
            break

        if len(neg_b)==0:
            break
            #ako nema negativnih nasli smo optimalno resenje
        else:
            #inace idemo na sledeci korak
            #ako postoji neko b <0 uzimamo taj indeks reda
            nb = next(i for i,x in enumerate(b) if x[1]<0)
            if not koristi_blenda:
                nb = random.choice([i for i,x in enumerate(b) if x[1]<0])
            if pisi_indexe:
                print(nb)
            #proveravamo da li su u tom redu svi veci od nule ako jesu onda nema dopustivo resenje
            if all(i>0 for i in A[nb]):
                print('Nema dopustivo resenje u redu su svi >0 ')
                quit()
            else:
                #trazimo maksimalni element za negativne u redu po formuli Z[j] / A[red][j]
                ind = -1
                max = -math.inf

                for i in range(len(A[nb])):
                    if A[nb][i] < 0 and Z[i] / A[nb][i] > max:
                        ind = i
                        max = Z[i] / A[nb][i]

                if ind == -1:
                    print('Greska u simpleksu')
                    quit()
                #delimo vrstu sa A[nb][ind]
                b[nb][1] /= A[nb][ind]
                A[nb] = [i/A[nb][ind] for i in A[nb]]

                #pivotiramo gore i dole pravimo 0
                for i in range(len(A)):
                    if i!=nb:
                        mult = A[i][ind]
                        A[i] = [i-mult*j for i,j in zip(A[i],A[nb])]
                        b[i][1] -= mult*b[nb][1]
                zmult = Z[ind]
                Z = [z-zmult*j for z,j in zip(Z,A[nb])]
                func += b[nb][1] * zmult
                #nova baza je
                B[nb] = ind
                iteracija +=1

    if pisi_iteracije:
        print('Algoritam je gotov , br iteracija: {}'.format(iteracija))
    resenje = [0] * len(A[0])
    for i in range(len(B)):
        resenje[B[i]] = b[i][1]
    print('optimalno resenje je [',end='')
    for i in resenje:
        print('{} '.format(round(i,2)),end='')
    print(']')
    print('Resenje funkcije je {}'.format(round(func,2)))



