import sys
import math
import numpy

pisi_korake = False

def korak1(Z,b,A,B,N):
    #cistimo funkciju cilja od bazisnih promenljivih
    sadrzi_bazisne = False

    for baza in B:
        if Z[baza]!=0:
            sadrzi_bazisne = True
    #ako ne sadrzi bazisne nista ne radimo funkciji cilja je f=0 + Z
    if not sadrzi_bazisne:
        return 0,Z
    #ako sadrzi neku bazisnu promenljivu prvo pronalazimo vektor u za bazisne promenljive
    #nalazimo ga tako sto resavamo u * kb = cb , kb su kolone baza a cb su koef baza u f-ji cilja
    kb = []
    for red in A:
        r = []
        for baza in B:
            r.append(red[baza])
        kb.append(r)
    cb = []
    for baza in B:
        cb.append(Z[baza])

    u = numpy.linalg.solve(numpy.transpose(kb), cb)
    if pisi_korake:
        print('Cistimo funkciju cilja od bazisnih')
        print('kb =')
        for r in kb:
            print([round(j,2) for j in r])
        print('cb = {}'.format([round(j,2) for j in cb]))
        print('resenje = {}'.format([round(j,2) for j in u]))
        print()
    #kada pronadjemo vektor u treba da nastelujemo funkciju cilja u oblik
    #f=f0 + sum n pripada N ( rn*xn )   kod N se misli na nebazisne
    #rn racunamo kao cn - u*kn
    #f0 racunamo kao u * b
    f0 = numpy.dot(u,b)
    Znovo = [0] * len(Z)
    for n in N:
        cn = Z[n]
        kn = []
        for red in A:
            kn.append(red[n])
        rn = cn - numpy.dot(u,kn)
        Znovo[n] = rn
    #vracamo novu funkciju u obliku f=f0 + Znovo gde Znovo predstavlja indekse uz promenljive
    return f0,Znovo

def korak2(Z,b,A,B,N):
    #proveravamo da li je uz koeficijente u Z neki negativan ako nije nasli smo optimalno resenje
    #inace vracamo prvi negativan
    for n in N:
        if Z[n]<0:
            return n
    #ako nema negativnih koristimo specijalnu vrednost -1 da oznacimo to
    return -1

def korak3(Z,b,A,B,N,i,x0):
    #za izabrano i tako da mu je ri<0
    #treba da resimo sistem jednacina
    #Kp*yp = Ki
    #gde Kp predstavlja koefiicjente bazisnih kolona ,  Ki predstavlja kolonu od i
    #ovime dobijamo yp kada resimo
    Kp = []
    for red in A:
        r = []
        for baza in B:
            r.append(red[baza])
        Kp.append(r)

    Ki = []
    for red in A:
        Ki.append([red[i]])

    Yp = numpy.linalg.solve(Kp,Ki)

    if pisi_korake:
        print('Resavamo sistem')
        print('Kp')
        for r in Kp:
            print([round(j, 2) for j in r])
        print('Ki')
        print([round(j[0],2) for j in Ki])
        print('Resenje je {}'.format([round(j[0],2) for j in Yp]))
        print()
    #sada konstruisemo resenje x u parametarskom obliku x(t)
    # X od j je jednako x0 od j - t*yj za indeks koji je bazisni tj j pripada u B
    # X od j je jednako t za indeks gde j = i , i je kolona koju smo izabrali u prethodnom koraku i prosledili ovde
    # X od j je jednako 0 za sve ostalo
    x = [0] * len(x0)
    for j in range(0,len(B)):
        #uzimamo --yp[j][0] posto numpy ce napraviti array sa jednim clanom :P
        x[B[j]] = -Yp[j][0]
    x[i] = 1
    return x


def korak4(Z,b,A,B,N,x1,x0):
    #imamo niz oblika c0-y0*t,c1-y1*t,c2-y2*t...
    #pronalazimo najvece nenegativno t tako da su svi >=0
    #brojevi su oblika ci-yi*t , ci se nalazi u x0 , yi se nalazi u x1
    #primer ako imamo 55-5t to znaci da t moze da ide od [0,11] tj [0,55/5]
    #ako imamo -55+5t ista je situacija
    #ako nemamo resenje za t to znaci da je funkcija cilja neogranicena odozgo i to ispisujemo kasnije
    #inace kada npr odredimo da je gornji 11 za svako t proveravamo da li zadovoljava da su ostali clanovi >=0
    #ako zadovoljava stavljamo ga u niz dobrih resenja i kasnije uzmemo najveci iz tog niza
    t_niz = []
    for baza in B:
        #ako su razlicitih znakova postoji opseg za t
        #tipa ako imamo 1+2t onda t nema gornji opseg za nenegativne vec je [0,inf)
        if (x0[baza]<0 and x1[baza]>0) or (x0[baza]>0 and x1[baza]<0):
            gornji = abs(x0[baza]/(x1[baza]))
            dozvoljivo = all( x+y*gornji>=0 for x,y in zip(x0,x1) )
            if dozvoljivo:
                t_niz.append(gornji)
    #vracamo najvece t koje zadovoljava
    #ako je niz prazan vracamo inf da oznacimo da je neogranicen odozgo
    if len(t_niz)==0:
        return math.inf
    return max(t_niz)

def korak5(Z,b,A,B,N,x1,x0,tn):
    #za izabrano tn iz koraka 4
    #uzimamo indeks prve bazicne kolone za koju vazi da je ci-yi*tn=0 i yi>0
    #posto cuvamo y kao negativne gledacemo da je yi<0 takodje onda je i ci+yi*tn formula
    #vracamo novo x0 za izracunato t i indeks koji smo nasli za bazicnu
    for baza in B:
        if x0[baza] + x1[baza] * tn ==0 and x1[baza]<0:
            return [c+y*tn for c,y in zip(x0,x1)],baza

def revidiran_simpleks(Z,b,A,B,N):
    #pravimo x0 resenje stavljamo 0 za pocetne promenljive i za dodate promenljive stavljamo vektor b
    x0 = [0]*len(A[0])
    for i in range(len(B)):
        x0[B[i]] = b[i]
    if pisi_korake:
        print('pocetno resenje je {}'.format([round(i,2) for i in x0]))
        print()
    iteracija = 1
    while True:

        if pisi_korake:
            print('iteracija = {}'.format(iteracija))
            iteracija+=1
            print('___________________')

        f0,Znovo = korak1(Z,b,A,B,N)
        i = korak2(Znovo,b,A,B,N)
        if i==-1:
            print('x = {}'.format([round(j,2) for j in x0]))
            f = f0
            for i in range(0,len(Znovo)):
                f+= Znovo[i]*x0[i]
            print('vrednost f-je je {}'.format(round(f,2)))
            break

        x1=korak3(Znovo,b,A,B,N,i,x0)

        if pisi_korake:
            print('Izabrali smo da u bazu ulazi'
                  ' {}'.format(i))
            print('novo x = {}'.format([str(round(j,2))+' + ( ' + str(round(i,2))+'t )' for j,i in zip(x0,x1)]))

        t_n = korak4(Znovo,b,A,B,N,x1,x0)
        if t_n==math.inf:
            print([str(round(x,2))+'+('+str(round(y,2))+'*t)' for x,y in zip(x0,x1)])
            print('neogranicen je odozgo prekidamo sa radom')
            break

        if pisi_korake:
            print('najvece t je {}'.format(round(t_n,2)))
            print()

        x0,j = korak5(Z, b, A, B, N, x1, x0, t_n)

        if pisi_korake:
            print('Novo optimalno resenje je {}'.format([round(i,2) for i in x0]))
            print('Bazu napusta kolona {}'.format(j))
            print()
        #u bazu ulazi kolona i a izlazi kolona j
        #sortiramo da bi odrzali lep raspored
        ind = B.index(j)
        B[ind] = i

        N.remove(i)
        N.append(j)
        N.sort()

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

if __name__ == '__main__':
    if len(sys.argv)==3:
        if 'k' in sys.argv[2]:
            pisi_korake = True
    A,b,Z,B,N = ucitaj_problem(sys.argv[1])
    revidiran_simpleks(Z,b,A,B,N)

