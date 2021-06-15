import sys
import random
import math

#da li ce program ispisivati sta trenutno radi
pisi_korake = False
#da li ce program ispisivati simpleks tabele
pisi_tabele = False
#da li ce program ispisati broj iteracija za simpleks
pisi_iteracije = False
#da li ce se koristiti blendovo pravilo
koristi_blenda = False
#da li ce ispis biti kao pajton liste (nekad je preglednije tako)
pisi_pajton_liste = False

maks_iteracija =math.inf

class LinProb:
    def __init__(self,type,A,Z,b):
        self.type = type
        self.A = A
        self.Z = Z
        self.b = b

    def ispisi_problem(self):
        print(self.type+' f = ',end='')
        if not pisi_pajton_liste:
            for i in range(len(self.Z)):
                if self.Z[i]!=0:
                    print('{}*X{} '.format(round(self.Z[i],2),i),end='')
        else:
            print([round(i,2) for i in self.Z])

        print()

        for i in range(len(self.A)):

            if not pisi_pajton_liste:
                for j in range(len(self.A[i])):
                    if self.A[i][j]!=0:
                        print('{}*X{} '.format(round(self.A[i][j],2),j),end='')
            else:
                print([round(j,2) for j in self.A[i]],end='')
            print(' {} {}'.format(self.b[i][0],round(self.b[i][1],2)))
        print()

    def standardizuj(self):
        #prebacujemo linearni problem u standardni oblik
        #ako postoje nejednacine dodajemo promenljive tako da postanu jednacine
        #ako je desna strana nejednacine <0 mnozimo red sa -1
        A = self.A.copy()
        b = self.b.copy()
        Z = self.Z.copy()
        type = 'min'
        if self.type=='max':
            Z=[-i for i in Z]
        for i in range(len(b)):
            if b[i][1]<0:
                A[i] = [-j for j in A[i]]
                b[i][1] *= -1
                if b[i][0]=='<=':
                    b[i][0]='>='
                elif b[i][0]=='>=':
                    b[i][0]='<='

        #ako negde imamo <= dodajemo +s tj izravnajucu promenljivu tako da postane =
        #ako negde imamo >= dodajemo -s
        #prvo brojimo koliko imamo nejednacina da bi odmah prosirili sve sto treba
        br_ned = 0
        for i in b:
            if i[0]!='=':
                br_ned+=1

        Z += [0] * br_ned

        n = len(A[0])
        for i in range(len(b)):
            A[i] += [0] * br_ned
            if b[i][0]=='<=':
                b[i][0]='='
                A[i][n+i]=1
            elif b[i][0]=='>=':
                b[i][0]='='
                A[i][n+i]=-1
        return LinProb(type,A,Z,b)

    def pronadji_bazisne(self):
        #trazimo bazisne kolone da li postoje
        B = [-1] * len(self.b)

        for i in range(len(self.b)):
        #prolazimo kroz i-ti red i gledamo da li postoji jedinica
            for j in reversed(range(len(self.A[i]))):
                if self.A[i][j] == 1:
                    # ako postoji gledamo gore i dole da vidimo da li su sve 0
                    #pravimo listu indeksa gore i dole sem i-tog reda
                    indeksi = [l for l in range(len(self.b)) if l!=i]
                    bazisna = all(self.A[l][j]==0 for l in indeksi)

                    if bazisna and B[i]==-1:
                        B[i]=j
                        break

        return B

    def pomocni_problem(self,B):
        #tu gde je -1 tu dodajemo vestacke promenljive sa 1
        #pravimo pomocni problem resavamo po w
        Z = self.Z.copy()
        A = self.A.copy()
        type = self.type
        b = self.b.copy()

        br_prom = len([i for i in B if i==-1])
        Z = [0 for i in Z]
        Z += [1]*br_prom

        n = len(A[0])
        for i in range(len(b)):
            A[i] += [0] * br_prom

        #postavljamo 1 tamo gde je baza bila -1 jer tu dodajemo vestacke
        j = 0
        #pamtimo koje su vestacke
        W= []
        for i in range(len(B)):
            if B[i]==-1:
                A[i][n+j] = 1
                W.append(n+j)
                j+=1
        return LinProb(type,A,Z,b),W

    def simpleks_metod(self):
        #radimo simpleks metod
        #prvo trazimo bazisne kolone
        #ako se bazisne kolone nalaze u funkciji cilja moramo da je ocistimo od njih
        #to radimo tako sto pravimo 0 tamo gde je bazisna kolona u funkciji cilja sa transformacijama
        #nakon toga konstruisemo simpleks tabelu i radimo algoritam
        B = self.pronadji_bazisne()

        tabela = self.A.copy()
        for i in range(len(self.b)):
            tabela[i].append(self.b[i][1])
        tabela.append(self.Z.copy()+[0])

        if pisi_korake and pisi_tabele:
            print('Pocetna tabela za simpleks:')
            for r in tabela:
                print([round(i,2) for i in r])
            print()

        #radimo ciscenje tabele ako neka bazicna promenljiva u funkciji cilja ima koeficijent razlicit od 0
        #dodajemo red gde je ta bazicna sa -koeficijentom kako bi napravili 0 u funkciji cilja

        for i in range(len(B)):
            #u poslednjem redu se nalazi funkcija cilja zato -1
            if tabela[-1][B[i]]!=0:
                koef = -tabela[-1][B[i]]
                tabela[-1] = [i+koef*j for i,j in zip(tabela[-1],tabela[i])]

        if pisi_korake and pisi_tabele:
            print('Tabela za ociscenu ciljnu funkciju od bazicnih izgleda ovako:')
            for r in tabela:
                print([round(i, 2) for i in r])
            print()
        #radimo algoritam sada kada smo ocistili funkciju cilja

        iteracija =0
        while True:
            #ako ne postoji j takvo da je Zj <0 nasli smo optimalno resenje i stajemo
            if not any(i<0 and i not in B for i in tabela[-1][:-1]):
                if pisi_iteracije:
                    print('Simpleks je zavrsio br iteracija = {}'.format(iteracija))
                return B,tabela
            if iteracija>=maks_iteracija:
                break
            #trazimo j takvo da Zj je manje od 0 i j nije u bazi
            #ako koristimo blendovo pravilo uvek uzimamo od kandidata najmanji indeks
            #enumerate vraca (i,j) gde je i indeks a j vrednost u listi
            #poslednji element ne gledamo to je vrednost funkcije
            kandidati = [i for i,j in enumerate(tabela[-1][:-1]) if j<0 and j not in B]
            ind = kandidati[0]
            if not koristi_blenda:
                ind = random.choice(kandidati)
            #pravimo listu kandidata za kljucni red po izabranoj pivot koloni
            #racunamo po formuli bi/A[i][pivot] pri cemu A[i][pivot] > 0
            red = -1
            min = math.inf
            for i in range(len(tabela)-1):
                if tabela[i][ind] >0 and tabela[i][-1]/tabela[i][ind]<min:
                    min = tabela[i][-1]/tabela[i][ind]
                    red = i

            if red==-1:
                print('Greska u simpleksu u koloni vrv nema Aij>0')
            #sada kada imamo pivot element A[red][ind] pravimo ga bazicnim tako sto iznad i ispod pravimo 0
            #al od njega pravimo jedinicu ceo red delimo sa njim
            tabela[red] = [i/tabela[red][ind] for i in tabela[red]]
            for i in range(len(tabela)):
                if i!=red:
                    mult = -tabela[i][ind]
                    tabela[i] = [i+mult*j for i,j in zip(tabela[i],tabela[red])]
            if pisi_korake and pisi_tabele:
                print('Iteracija {} , pivotirano po A[{}][{}]'.format(iteracija,red,ind))
                for r in tabela:
                    print([round(i, 2) for i in r])
                print()
            B[red] = ind
            iteracija +=1

        if pisi_iteracije:
            print('Upozorenje! Simpleks je dosao do maks iteracija ({})'.format(maks_iteracija))
        return B,tabela

def izbrisi_vestacke_promenljive(tabela,B,W):
    while len(W)!=0:
        #brisemo vestacke promenljive tako sto radimo sledece:
        #ako nisu bazisne odmah ih samo precrtamo
        nebazisne = [i for i in W if i not in B]
        if pisi_korake:
            print()
            print('Tabela pre uklanjanja nebazisnih')
            for r in tabela:
                print([round(i, 2) for i in r])
            print()
            print('Uklanjamo vestacke koje su nebazisne')
            print('nebazisne vestacke = {}'.format(nebazisne))
        for i in range(len(tabela)):
            tabela[i] = [vrednost for index,vrednost in enumerate(tabela[i]) if index not in nebazisne ]
        #ovde azuriramo vestacke indekse ako smo uklonili neku koja je manja od w moramo da mu smanjimo indeks
        #da bi odgovarao dobijenoj tabeli
        W = [i for i in W if i not in nebazisne]
        for i in range(len(W)):
            br_manjih = len([j for j in nebazisne if j <W[i]])
            W[i] -=br_manjih

        for i in range(len(B)):
            br_manjih = len([j for j in nebazisne if j <B[i]])
            B[i] -=br_manjih

        if pisi_korake:
            print('Tabela nakon uklanjanja nebazisnih')
            for r in tabela:
                print([round(i, 2) for i in r])
            print()
            print('Uklanjamo vestacke koje su bazisne')
            print('bazisne vestacke = {}'.format(W))
        #sada uklanjamo vestacke koje su bazisne sve
        #ako su u jednom redu sve 0 osim 1 za tu bazisnu vestacku onda uklanjamo taj red i tu kolonu
        #inace ako postoji neki element koji je razlicit od 0 , uzmemo neki takav i napravimo ga bazisnim
        #nakon toga posto ta vestacka vise nije bazisna brise se isto kao malopre
        obrisane = []
        for w in W:
            red = B.index(w)
            indexi = [i for i,j in enumerate(tabela[red][:-1]) if i!=w]
            nenula = any(tabela[red][i]!=0 for i in indexi)

            if nenula:
                #ako postoji neki koji nije nula uzimamo neki od njih i pravimo ga bazom sa pivotiranjem
                kandidati = [i for i in indexi if tabela[red][i]!=0]
                ind = kandidati[0]
                if not koristi_blenda:
                    ind = random.choice(kandidati)

                tabela[red] = [i/tabela[red][ind] for i in tabela[red]]

                for i in range(len(tabela)):
                    if i!=red:
                        mult = -tabela[i][ind]
                        tabela[i] = [i+mult*j for i,j in zip(tabela[i],tabela[red])]
                B[red] = ind
                if pisi_korake:
                    print('Posto vestacka x{} u redu ima el!=0 pivotiramo oko A[{}][{}]'.format(w, red, ind))
                    print('Nova baza je {}'.format(B))
                    print()
            else:
                #ako je 0 sve onda brisemo red i kolonu
                tabela.pop(red)
                for i in range(len(tabela)):
                    tabela[i].pop(w)

                el = B.pop(red)
                B = [i if el>i else i-1 for i in B]
                W.remove(w)
                W = [i if el > i else i - 1 for i in W]
                if pisi_korake:
                    print('Posto vestacka x{} u redu gde je bazisna su sve 0 brisemo red i kolonu'.format(w))
            if pisi_korake:
                print()
                print('Tabela nakon uklanjanja bazisnih')
                for r in tabela:
                    print([round(i, 2) for i in r])

    return tabela,B


def ucitaj_file(putanja):
    #ucitavanje problema iz fajla
    file = open(putanja)
    linije = file.readlines()
    prva = linije[0].split()
    type = prva[0]
    Z = []
    b = []
    A = []

    for i in range(1,len(prva)):
        Z.append(float(prva[i]))

    for i in range(1,len(linije)):
        linija = linije[i].split()
        red = []

        for j in range(len(linija)-2):
            red.append(float(linija[j]))

        A.append(red)
        b.append([linija[-2],float(linija[-1])])

    return LinProb(type,A,Z,b)

def prob_izTabele(tabela):
    type = 'min'
    A = []
    b = []
    Z = [i for i in tabela[-1][:-1]]

    for i in range(len(tabela)-1):
        A.append([i for i in tabela[i][:-1]])
        b.append(['=',tabela[i][-1]])

    return LinProb(type,A,Z,b)

if __name__ == '__main__':

    #ako se prosledi jos jedan argument on sluzi za parametre programa
    if len(sys.argv)>=3:
        pisi_korake = 'k' in sys.argv[2]
        pisi_tabele = 't' in sys.argv[2]
        pisi_iteracije = 'i' in sys.argv[2]
        koristi_blenda = 'b' in sys.argv[2]
        pisi_pajton_liste = 'l' in sys.argv[2]

    #ako se proslede 4 argumenta cetvrti mora da bude max broj iteracija za simpleks
    if len(sys.argv) == 4:
        maks_iteracija = int(sys.argv[3])

    #ispisujemo stanja programa tj zastavica kako ce ispisivati
    print('Trenutna konfiguracija programa:')
    print('Koristi se blendovo pravilo: {}'.format(koristi_blenda))
    print('Maximalan broj iteracija u simpleksu: {}'.format(maks_iteracija))
    print('Format ispisa kao pajton liste: {}'.format(pisi_pajton_liste))
    print('Ipisuju se:')
    if pisi_korake:
        print('-koraci pri radu')
    if pisi_korake and pisi_tabele:
        print('-tabele pri iteracijama u simpleksu')
    if pisi_iteracije:
        print('-nakon koliko iteracija je zavrsen simpleks')
    print('-finalno resenje')
    print()

    #vrsimo ucitavanje pocetnog linearnog problema iz tekstualnog fajla
    Lp = ucitaj_file(sys.argv[1])

    if pisi_korake:
        print('Resavamo problem:')
        Lp.ispisi_problem()
        print('Prebacujemo u standardni oblik')
        print()

    #vrsimo standardizaciju pocetnog linearnog problema
    #prebacujemo sve u znak =
    #sve b koji su negativni mnozimo sa -1
    standard_Lp = Lp.standardizuj()

    if pisi_korake:
        standard_Lp.ispisi_problem()

    #trazimo da li problem u standardnoj formi sadrzi bazisne kolone
    #koje su lako uocljive tj odmah su u formi 0,0,..,1,...,0
    B = standard_Lp.pronadji_bazisne()

    if pisi_korake:
        print('Trazimo da li postoji podjedinicna matrica')
        print('Indeksi koji mogu da cine bazisne kolone su:')
        print([round(i,2) for i in B if i != -1])

    #racunamo koliko vestackih treba da se doda
    #da bi imali bazu max ranka
    broj_vestackih = len(standard_Lp.b) - len([i for i in B if i!=-1])
    pomocni_prob = None

    if broj_vestackih==0:
        #ako postoji podjedinicna matrica mozemo odmah simpleks
        #tj nema potrebe za dve faze
        print('Posto bazicna matrica moze da se napravi iz standardne forme')
        print('Dvofazni nije neophodan mozemo obicnim resiti')

        #radimo simpleks metod nad standardnoj formi problema
        B3, tabela3 = standard_Lp.simpleks_metod()
        func = tabela3[-1][-1]

        if Lp.type == 'min' and standard_Lp.type == 'min':
            func *= -1

        print('optimalna vrednost funkcije je {}'.format(round(func,2)))
        x = [0] * (len(tabela3[0]) - 1)
        for i in range(len(standard_Lp.b)):
            x[B3[i]] = tabela3[i][-1]

        print('x = [', end='')
        for i in x:
            print('{} '.format(round(i, 2)), end='')
        print(']')
    else:
        #ako ne postoji pravimo pomocni problem
        pomocni_prob,W = standard_Lp.pomocni_problem(B)

        if pisi_korake:
            print('Posto ne postoji podjedinicna matrica da je odmah uocljiva dodajemo vestacke promenljive')
            print('Broj vestackih promenljivih koje dodajemo je {}'.format(broj_vestackih))
            print('Pomocni problem izgleda ovako:')
            print()
            pomocni_prob.ispisi_problem()
            print('Pomocne promenljive koje smo dodali su: {}'.format(W))
            print('Krecemo simpleks metodu za pomocni problem')

        #nakon sto smo napravili pomocni problem krecemo simpleks metod nad njim
        Bn,tabela = pomocni_prob.simpleks_metod()

        #ako je rezultat 0 to znaci da pocetni problem ima resenje i prelazimo u drugu fazu
        if tabela[-1][-1] == 0:
            if pisi_korake:
                print()
                print('Resenje funkcije pomocnog problema je 0 sto znaci da pocetni problem ima resenje')
                print('Krecemo da izbacujemo vestacke promenljive koje smo dodali')

            #nakon sto smo ustanovili da pocetni problem ima resenje
            #uklanjamo vestacke promenljive iz njega
            tabela2,B2=izbrisi_vestacke_promenljive(tabela,Bn,W)

            #menjamo poslednji red da bi dodali funkciju cilja od prvobitnog problema
            tabela2[-1] = standard_Lp.Z + [0]

            if pisi_korake:
                print()
                print('Ociscena tabela izgleda ovako:')
                for r in tabela2:
                    print([round(i,2) for i in r])

            #iz nove tabele dobijamo sekundarni problem
            sekundum = prob_izTabele(tabela2)
            if pisi_korake:
                print()
                print('Sekundum problem izgleda ovako:')
                sekundum.ispisi_problem()
                print()
                print('Krecemo da primenjujemo simpleks')
                print()

            #radimo simpleks nad sekundardnim problemom
            B3,tabela3= sekundum.simpleks_metod()
            func = tabela3[-1][-1]
            if Lp.type == 'min' and sekundum.type=='min':
                func *= -1

            print('optimalna vrednost funkcije je {}'.format(round(func,2)))
            x = [0] * (len(tabela3[0])-1)
            for i in range(len(sekundum.b)):
                x[B3[i]] = tabela3[i][-1]
            print('x = [',end='')
            for i in x:
                print('{} '.format(round(i,2)),end='')
            print(']')
        else:
            #ako rezultat nije 0 to ispisujemo i zaustavljamo program
            print('Resenje funkcije pomocnog problema nije 0 -> pocetni problem nema resenje')
            quit()