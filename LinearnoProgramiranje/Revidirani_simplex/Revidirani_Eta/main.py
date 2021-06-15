import sys
import math
import numpy

pisi_korake = False

eta = {}

#funkcija za ucitavanje problema
def ucitaj_problem(putanja):
    file = open(putanja,"r")
    linije = file.readlines()

    #ucitavamo tip funkcije i vrednosti Z
    type = linije[0].split()[0]
    Z = [float(i) for i in linije[0].split()[1:]]
    b = []
    A = []

    #ucitavamo matricu A i vektor b i znak u redu
    for i in range(1,len(linije)):
        red = linije[i].split()
        r = [float(j) for j in red[:-2]]
        A.append(r)
        b.append([red[-2],float(red[-1])])

    if pisi_korake:
        print('Ucitali smo problem:')
        print(type + ' {}'.format([round(j,2) for j in Z]))
        for i in range(len(b)):
            print([round(j,2) for j in A[i]],end='')
            print(' '+b[i][0]+' {}'.format(round(b[i][1],2)))
        print()

    #prebacujemo u kanonski oblik ako je max mnozimo sa -1
    #nejednacine prebacujemo u jednacine dodavanjem izravnavajucih promenljvih
    #ako je neko b<0 mnozimo sa -1

    if type=='max':
        Z = [-i for i in Z]

    #racunamo koliko izravnavajucih treba da dodamo
    #takodje ako je b<0 mnozimo red sa -1
    br_izravnajucih = 0
    for i in range(len(b)):
        if b[i][1]<0:
            A[i] = [-j for j in A[i]]
            if b[i][0] == '<=':
                b[i][0] = '>='
            elif b[i][0] =='>=':
                b[i][0] = '<='
        if b[i][0] != '=':
            br_izravnajucih += 1

    #prebacujemo sve u jednacine
    n = len(A[0])
    j = 0
    for i in range(len(b)):
        A[i] += [0]*br_izravnajucih
        if b[i][0] =='<=':
            A[i][n+j] = 1
            j+=1
        elif b[i][0] =='>=':
            A[i][n+j] = -1
            j+=1
        b[i][0] = '='

    #sada trazimo bazisne kolone
    B = [-1] * len(b)
    for i in range(len(A)):
        for j in reversed(range(len(A[i]))):
            if A[i][j] == 1:
                indexi = [r for r in range(len(A)) if r!=i]
                if all(A[r][j]==0 for r in indexi):
                    B[i] = j
                    break
    #dodajemo nebazisne
    N = []
    for i in range(len(A[0])):
        if i not in B:
            N.append(i)

    #ako ne postoji bazisna matrica to prijavimo i zaustavljamo program
    #uslov za simplex je da postoji
    if any(i==-1 for i in B):
        print('Ne postoji podjedinicna matrica za problem')
        quit()

    Z += [0]*br_izravnajucih

    if pisi_korake:
        print('Kanonski problem:')
        print('min {}'.format([round(j, 2) for j in Z]))
        for i in range(len(b)):
            print([round(j, 2) for j in A[i]], end='')
            print(' ' + b[i][0] + ' {}'.format(round(b[i][1], 2)))
        print('Baze = {}'.format(B))
        print()

    b = [i[1] for i in b ]

    file.close()
    return A,b,Z,B,N

def korak1(A,b,Z,B,N,B0,iteracija):
    #cistimo funkciju cilja od bazisnih promenljivih

    #pravimo cp
    cp = []
    for baza in B:
        cp.append([Z[baza]])
    #resavamo sistem u*B=cp
    if iteracija>2:
        if pisi_korake:
            print('u*B{} = {}'.format(iteracija-1,[round(j[0],2) for j in cp]))
        it = iteracija - 1
        tek_cp = [j[0] for j in cp]
        #iterativno resavanje sistema
        if pisi_korake:
            print('Iterativno resavanje sistema:')
        while it>0:
            if pisi_korake:
                print('-> u{}*E{} = {}'.format(it,it,[round(j,2) for j in tek_cp]))
            I = []
            for i in range(len(b)):
                red = [0] *len(b)
                red[i] = 1
                I.append(red)

            et = eta[it]

            for i in range(len(et[0])):
                I[i][et[1]] = et[0][i]

            tek_cp = numpy.linalg.solve(numpy.transpose(I),tek_cp)
            if pisi_korake:
                print('u{} = {}'.format(it,[round(j,2) for j in tek_cp]))
            it-=1
            u = tek_cp
    else:
        u = numpy.linalg.solve(numpy.transpose(B0),cp)
        u = [j[0] for j in u]

    if pisi_korake:
        print('Cistimo funkciju cilja od bazicnih promenljihi')
        print('B = ')
        for r in B0:
            print([round(j,2) for j in r])
        print('cp = {}'.format([round(j[0],2) for j in cp]))
        print('Resenje u = {}'.format([round(j,2) for j in u]))

    return [j for j in u]

def korak2(A,b,Z,B,N,u):
    #trazimo prvo ri < 0
    #tako da je i nebazisna
    for n in N:
        kolona = [A[i][n] for i in range(len(b))]
        rn = Z[n] - numpy.dot(u,kolona)
        if rn<0:
            return n
    return -1
def korak3(A, b, Z, B, N,B0,x):
    #resavamo sistem B*y=kx
    kx = []
    for i in range(len(b)):
        kx.append(A[i][x])
    y = numpy.linalg.solve(B0,kx)

    if pisi_korake:
        print('Resavamo B*y = {}'.format([round(j,2) for j in kx]))
        print('resenje za y = {}'.format([round(j,2) for j in y]))
        print()

    return y

def korak4(x0,y,B,nova_b):
    #pravimo parametarsko novo x
    x1 = [0] * len(x0)
    for i in range(len(B)):
        x1[B[i]] = -y[i]
    x1[nova_b] = 1
    return x1

def korak5(x0,x1,B):
    #nalazimo maksimalno t za koje je novo x >=0
    t = math.inf
    kandidati = [-x0[i]/x1[i] for i in range(len(x0)) if i in B if x1[i]!=0]

    if len(kandidati)==0:
        return t

    #pravimo listu elemenata t iz kandidata ako vazi da ce x(t) biti >=0
    zadovoljivi_kandidati = [i for i in kandidati if all(j+i*r>=0 for j,r in zip(x0,x1))]
    return max(zadovoljivi_kandidati)

def korak6(B,x0,x1,t):
    #uzimamo indeks prve kolone za prosledjeno t gde vazi da je x0-x1*t=0 za x1>0
    for baza in B:
        if x0[baza] +x1[baza]*t == 0 and x1[baza]<0:
            return baza

def revidirani_Eta(A,b,Z,B,N):

    #konstruisemo pocetno optimalno resenje
    x0 = [0]*len(A[0])
    for i in range(len(B)):
        x0[B[i]] = b[i]
    #konstruisemo B0 matricu prvo je jedinicna B0 = I
    B0 = []
    for i in range(len(b)):
        red = [0] * len(b)
        red[i] = 1
        B0.append(red)

    if pisi_korake:
        print('Pocetno B0 je')
        for r in B0:
            print(r)

    if pisi_korake:
        print('Pocetno resenje je {}'.format([round(j,2) for j in x0]))

    iteracija = 1

    while True:
        if pisi_korake:
            print('Iteracija {}'.format(iteracija))
            print('____________________')

        u = korak1(A, b, Z, B, N,B0,iteracija)
        nova_b = korak2(A,b,Z,B,N,u)

        if nova_b==-1:
            print()
            print('Resenje:')
            print('x = {}'.format([round(i,2) for i in x0]))
            f = 0
            for i in range(len(x0)):
                f+= Z[i]*x0[i]
            print('func = {}'.format(round(f,2)))
            break

        if pisi_korake:
            print('U bazu ulazi x{}'.format(nova_b))
        y=korak3(A, b, Z, B, N,B0,nova_b)
        x1=korak4(x0,y,B,nova_b)

        if pisi_korake:
            print('Parametarsko resenje je:')
            print([str(round(i,2)) +' + ( ' + str(round(j,2)) + 't )' for i,j in zip(x0,x1) ])
        t=korak5(x0,x1,B)

        if t==math.inf:
            print('Neograniceno je odozgo')
            quit()
        if pisi_korake:
            print('Makimsalno t = {}'.format(round(t,2)))

        stara_b = korak6(B,x0,x1,t)
        x0 = [i+t*j for i,j in zip(x0,x1)]
        #pravimo jedinicnu matricu
        I = []
        for i in range(len(B0)):
            red = [0] * len(B0)
            red[i] = 1
            I.append(red)
        ind = B.index(stara_b)

        for i in range(len(y)):
            I[i][ind] = y[i]
        #pamtimo samo ono sto smo zamenili i u kojoj koloni je
        eta[iteracija] = [y,ind]

        print()
        print('Matrica E{}'.format(iteracija))
        for r in I:
            print([round(j,2) for j in r])
        print()
        B0 = numpy.dot(B0,I)
        if pisi_korake:
            print('B{} = '.format(iteracija))
            for r in B0:
                print([round(j,2) for j in r])
            print()

        if pisi_korake:
            print('Nebazisna postaje promenljiva x{}'.format(stara_b))
            print('Novo x = {}'.format([round(j,2) for j in x0]))
        B[ind] = nova_b

        N.remove(nova_b)
        N.append(stara_b)
        N.sort()

        iteracija += 1

if __name__ == '__main__':

    if len(sys.argv)==3:
        pisi_korake = 'k' in sys.argv[2]

    A,b,Z,B,N = ucitaj_problem(sys.argv[1])
    revidirani_Eta(A,b,Z,B,N)