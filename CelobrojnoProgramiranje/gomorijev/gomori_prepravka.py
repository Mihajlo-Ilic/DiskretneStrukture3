import math
import sys
from termcolor import colored

br_decimala = 2
koristi_leks = False

def pisi_promenljivu(max_duzina,S,W,prom):
    if prom in S:
        ind = S.index(prom)
        duzina = max_duzina - len('s'+str(ind))
        levo = duzina // 2
        desno = duzina - levo
        print(' '*(levo) + 's'+str(ind) + ' '*desno,end='')
    elif prom in W:
        ind = W.index(prom)
        duzina = max_duzina - len('w'+str(ind))
        levo = duzina // 2
        desno = duzina - levo
        print(' '*(levo) + 'w'+str(ind) + ' '*desno,end='')
    else:
        duzina =max_duzina- len('x'+str(prom))
        levo = duzina // 2 
        desno = duzina - levo
        print(' '*(levo) + 'x'+str(prom) + ' '*desno,end='')

    
def pisi_broj(max_duzina,broj):

    if broj.is_integer():
        duzina = max_duzina - len(str(int(broj)))
        levo = duzina//2
        desno = duzina - levo
        s = ' '*levo + str(int(broj)) + ' '*desno
        print(s,end='')
    else:
        duzina = max_duzina - len(str(round(broj,br_decimala)))
        levo = duzina//2
        desno = duzina - levo
        s = ' '*levo + str(round(broj,br_decimala)) + ' '*desno
        print(s,end='')


def pisi_simplex_tabelu(tabela,B,S,W):
        max_duzina = 0
        for red in tabela:
            for el in red:
                if el.is_integer():
                    max_duzina = max(max_duzina,len(str(int(el)))+3)
                else:
                    max_duzina = max(max_duzina,len(str(round(el,br_decimala)))+3)

        for i in range(len(tabela[0])-1):
            max_duzina = max(max_duzina,len('x'+str(i))+2)

        print(' '*(max_duzina+2),end='')
        for i in range(len(tabela[0])-1):
            pisi_promenljivu(max_duzina, S, W, i)
        print()
        print('-'*max_duzina*(len(tabela[0])+1))

        for i in range(len(tabela)-1):
            pisi_promenljivu(max_duzina, S, W, B[i])
            print('|',end='')
            for j in range(len(tabela[i])-1):
                pisi_broj(max_duzina, tabela[i][j])
            print('|',end='')
            pisi_broj(max_duzina, tabela[i][-1])
            print()
        
        print('-'*max_duzina*(len(tabela[0])+1))
        print(' '*(max_duzina+1),end='')
        for i in range(len(tabela[-1])-1):
            pisi_broj(max_duzina, tabela[-1][i])
        print('|',end='')
        pisi_broj(max_duzina,tabela[-1][-1])
        print()

def ocisti_od_vestackih(tabela,B,W):
    while len(W):
        nebazicne = [w for w in W if w not in B]
        for i in range(len(nebazicne)):
            for j in range(len(tabela)):
                tabela[j].pop(nebazicne[i])
            stara = nebazicne[i]
            nebazicne = [n if n<stara else n-1 for n in nebazicne]

            W.remove(stara)
            W = [w if w<stara else w-1 for w in W]
            B = [w if w<stara else w-1 for w in B]

        bazicne = [w for w in W if w in B]


        for b in bazicne:
            ind = B.index(b)


            if any(tabela[ind][i]!=0 for i in range(len(tabela[ind])-1) if i!=ind):
                kandidati = [i for i in range(len(tabela[ind])-1) if i!=ind and tabela[ind][i]!=0]
                col = kandidati[0]

                st = tabela[ind][col]
                tabela[ind] = [i/st for i in tabela[ind]]

                for i in range(len(tabela)):
                    if i!=ind:
                        mult = tabela[i][col]
                        tabela[i] = [j-mult*k for j,k in zip(tabela[i],tabela[ind])]
                B[ind] = col
            else:
                tabela.pop(ind)
                B.pop(ind)
                for i in range(len(tabela)):
                    tabela[i].pop(b)
                bazicne.remove(b)
                W.remove(b)
                bazicne = [n if n<b else n-1 for n in bazicne]
                W = [w if w<b else w-1 for w in W]
                B = [w if w<stara else w-1 for w in B]
    return tabela,B

class LinearniProblem:
    #konstruktor za klasu linearni problem
    def __init__(self,type,A,b,Z):
        self.type = type
        self.A = A.copy()
        for i in range(len(A)):
            self.A[i] = A[i].copy()
        self.b = b.copy()
        self.Z = Z.copy()

    def pisi_problem(self):
        print(self.type,end=' ')
        print(self.Z)
        print()
        for i in range(len(self.b)):
            print(self.A[i],end=' ')
            print(self.b[i][0],end=' ')
            print(self.b[i][1])
    
    #standardizuj problem
    def standardizuj(self):
        if self.type == 'max':
            self.Z = [-i for i in self.Z]
    #ako je neko b<0 mnozimo red sa -1
    #i obrcemo znak
        for i in range(len(self.b)):
            if self.b[i][1]<0:
                self.A[i] = [-j for j in self.A[i] ]
                self.b[i][1] = -self.b[i][1]
                if self.b[i][0] =='<=':
                    self.b[i][0] = '>='
                elif self.b[i][0] =='>=':
                    self.b[i][0] = '<='
        
    #dodajemo izravnajuce promenljive
    #ako je <= dodajemo +s
    #ako je >= dodajemo -s
    #ako je >= ali b je 0 mnozimo sa -1 pa dodamo +s
    #pamtimo listu gde cuvamo sta dodajemo gde
        S =[]
        for i in range(len(self.b)):
            #za sve ostale koji nisu ovaj red dodajemo 0
            if self.b[i][0] != '=':
                for j in range(len(self.b)):
                    if j!=i:
                        self.A[j].append(0.0)
            if self.b[i][0] == '<=':
                self.A[i].append(1.0)
                S.append(len(self.A[i])-1)
                self.Z.append(0.0)
            elif self.b[i][0] == '>=' and self.b[i][1]==0 :
                self.A[i] = [-a for a in self.A[i]]
                self.A[i].append(1.0)
                S.append(len(self.A[i])-1)
                self.Z.append(0.0)
            elif self.b[i][0] == '>=':
                self.A[i].append(-1.0)
                S.append(len(self.A[i])-1)
                self.Z.append(0.0)
            self.b[i][0] = '='
        return S


    def nadji_bazisne(self):
        #idemo unazad i gledamo da li ima 0..1..0 kolona da bude baza za red
        B = [-1] * len(self.b)
        for i in reversed(range(len(self.A[0]))):
            for j in range(len(self.b)):
                if self.A[j][i] == 1 and all(self.A[k][i]==0 for k in range(len(self.b)) if k!=j ) and B[j]==-1:
                    B[j] = i
        return B

    def dodaj_vestacke(self,B):
        W = []
        for i in range(len(B)):
            if B[i] == -1:
                for j in range(len(self.b)):
                    if j!=i:
                        self.A[j].append(0.0)
                self.A[i].append(1.0)
                B[i] = len(self.A[i]) - 1
                W.append(len(self.A[i]) - 1)
                self.Z.append(0.0)
        return B,W

    def konstruisi_simplex_tabelu(self):
        tabela = []
        for i in range(len(self.b)):
            tabela.append(self.A[i].copy())
            tabela[i].append(self.b[i][1])
        tabela.append(self.Z.copy())
        tabela[-1].append(0.0)
        return tabela
                

    def dvofazni_simpleks(self):
        print('Standardizujemo problem:')
        S = self.standardizuj()
        self.pisi_problem()
        print()
        print()
        print('Trazimo bazisne kolone :')
        B = self.nadji_bazisne()
        print(B)
        print()
        staro_Z = self.Z.copy()
        if any(i==-1 for i in B):
            print('Nema dovoljno bazisnih moramo da dodamo vestacke:')
            B,W = self.dodaj_vestacke(B)
            print()
            print('Pomocni problem je:')
            self.Z = [0.0 for i in self.Z]
            for w in W:
                self.Z[w] = 1.0
            self.pisi_problem()
            print()
            print('Krecemo simpleks tablicni za fazu 1:')
            print()
            tabela , B = tablicni_simpleks(self.konstruisi_simplex_tabelu(),B,S,W)
            if tabela!= None:
                if tabela[-1][-1] == 0:
                    print('Resenje je 0 pa postoji resenje i pocetnog problema')
                    tabela,B=ocisti_od_vestackih(tabela,B,W)
                    print('Ociscena tabela od vestackih promenljivih je:')
                    tabela[-1] = staro_Z.copy()
                    tabela[-1].append(0.0)
                    print()
                    tabela , B = tablicni_simpleks(tabela,B,S,W)
                    f = 0.0
                    x = [0.0] * (len(tabela[0])-1)
                    for b in B:
                        ind = B.index(b)
                        x[b] = tabela[ind][-1]
                        f += x[b] * staro_Z[b]
                    print('optimalno x je : {}'.format([round(j,br_decimala) if not round(j,br_decimala).is_integer() else int(j) for j in x]))
                    if self.type == 'max':
                        f = -f
                    if round(f,br_decimala).is_integer():
                        print('f {} = {}'.format(self.type,int(round(f,br_decimala))))
                    else:
                        print('f {} = {}'.format(self.type,round(f,br_decimala)))
                    return tabela,x,f,B
                else:
                    print('Resenje nije 0 pa ne postoji resenje pocetnog problema')
                    return None , [] , 0 , B
            else:
                return None , [] , 0 , B

        else:
            print('Ima dovoljno bazisnih resavamo obicnim simpleksom:')
            print()
            tabela , B = tablicni_simpleks(self.konstruisi_simplex_tabelu(),B,S,[])
            if tabela!= None:
                    f = 0.0
                    x = [0.0] * (len(tabela[0])-1)
                    for b in B:
                        ind = B.index(b)
                        x[b] = tabela[ind][-1]
                        f += x[b] * staro_Z[b]
                    print('optimalno x je : {}'.format([round(j,br_decimala) if not j.is_integer() else int(j) for j in x]))
                    if self.type == 'max':
                        f = -f
                    if round(f,br_decimala).is_integer():
                        print('f {} = {}'.format(self.type,int(round(f,br_decimala))))
                    else:
                        print('f {} = {}'.format(self.type,round(f,br_decimala)))
                    return tabela,x,f,B
            else:
                return None , [] , 0 , B

    def dodaj_rez(self,tabela,B):
        ind = -1

        m = -math.inf

        for i in range(len(tabela)):
            if razlomljeni_deo(tabela[i][-1]) > m:
                ind = i
                m = razlomljeni_deo(tabela[i][-1])
            
        if ind == -1:
            print('Greska u trazenju ne celobrojnog reda')
            quit()

        print(colored('{} red nema celobrojno resenje pa tu pravimo rez:'.format(ind),'red'))
        novi_red = [razlomljeni_deo(a) for a in tabela[ind][:-1]]
        nova_desna_strana = razlomljeni_deo(tabela[ind][-1])
        print('\nnova nejednacina')
        print([ int(round(a,br_decimala)) if round(a,br_decimala).is_integer() else round(a,br_decimala) for a in novi_red ],end=' >= ')
        print(round(nova_desna_strana,br_decimala))
        
        novi_red = [-a for a in novi_red]
        novi_red.append(1.0)
        nova_desna_strana = -nova_desna_strana
        novi_red.append(nova_desna_strana)

        for i in range(len(tabela)):
            tabela[i].insert(-1,0.0)

        tabela.insert(-1,novi_red)
        B.append(len(tabela[0])-2)


    def gomorijev(self):
        print('Krecemo gomorijev algoritam:')
        print('Prvo radimo dvofazni za inicijalni problem:')
        print()
        staro_Z = [i for i in self.Z]
        tabela , x , f , B = self.dvofazni_simpleks()
        br_secica = 0
        print()

        while not all( round(red[-1],br_decimala).is_integer() for red in tabela ):
            print(colored('+++++++++++++++++++++++++++++++++++++++++++++++++','blue'))
            print(colored('Resenje nije celobrojno moramo raditi gomorijev rez:','red'))
            self.dodaj_rez(tabela,B)
            br_secica +=1

            print()
            print(colored('Nakon dodavanja nejednacine imamo tabelu:','yellow',attrs=['underline']))
            pisi_simplex_tabelu(tabela, B, [], [])
            print()
            print('Krecemo dualni simpleks:')
            tabela,B = dualni_simpleks(tabela,B)


        print()
        print(colored('+++++++++++++++++++++++++++++++++++++++++++++++++','blue'))
        print(colored('Resenje jeste celobrojno -> stajemo sa ravnima odsecanja !!','green'))
        print(colored('Broj rezova uradjenih je : {}'.format(br_secica),'magenta'))
        print()
        f = 0.0
        x = [0.0] * (len(staro_Z))
        
        for b in B:
            if b<len(staro_Z):
                ind = B.index(b)
                x[b] = tabela[ind][-1]
                f += x[b] * staro_Z[b]

        print('optimalno x je : {}'.format([round(j,br_decimala) if not j.is_integer() else int(j) for j in x]))
        if round(f,br_decimala).is_integer():
            print('f {} = {}'.format(self.type,int(round(f,br_decimala))))
        else:
            print('f {} = {}'.format(self.type,round(f,br_decimala)))



def nadji_pivot(tabela,B,nb):
    pomocna = []
    ind = -1
    print('Kandidati za leksikografsko poredjenje:')
    for i in range(len(tabela[nb])-1):
        if tabela[nb][i] < 0 and i not in B:

            red = [tabela[j][i] for j in range(len(tabela)-1) if j!=nb]
            red.insert(0, tabela[-1][i])
            f = tabela[nb][i]

            if round(f,br_decimala).is_integer():
                    print('{} / {}'.format([ int(round(a,br_decimala)) if round(a,br_decimala).is_integer() else round(a,br_decimala) for a in red ],
                    int(round(-tabela[nb][i],br_decimala))))
            else:
                    print('{} / {}'.format([ int(round(a,br_decimala)) if round(a,br_decimala).is_integer() else round(a,br_decimala) for a in red ],
                    round(-tabela[nb][i],br_decimala) ))

            pomocna.append([a/(-f) for a in red])
        else :
            pomocna.append(None)
        
    if all(i==None for i in pomocna):
        print('nema negativnih u tekucem redu! -> nema dopustivih resenja')
        quit()
    
    for i in range(len(pomocna)):
        if pomocna[i]!=None:
            ind = i
            break

    for i in range(len(pomocna)):
        if pomocna[i]!=None: 
            indexi = [j for j in range(len(pomocna)) if j!=i and pomocna[j]!=None]
            if all(pomocna[i] <= pomocna[j] for j in indexi ):
                ind = i

                break

    return ind

def nadji_pivot_staro(tabela,B,n_b):
    m = -math.inf
    ind = -1

    for i in range(len(tabela[n_b])-1):
        if tabela[n_b][i]<0 and tabela[-1][i]/tabela[n_b][i] > m:
            m = tabela[-1][i]/tabela[n_b][i]
            ind = i

    if ind==-1:
        print('nema negativnih u tekucem redu! -> nema dopustivih resenja')
        quit()

    return ind

def dualni_simpleks(tabela,B):
    print()
    #dok nisu sve tacke celobrojne radimo dualni
    iteracija = 0
    while not all(a[-1] >= 0 for a in tabela[:-1]):

        n_b = -1
        for i in range(len(tabela)-1):
            if tabela[i][-1] < 0:
                n_b = i
                break
        #trazimo indeks po kom pivotiramo
        ind = -1
        if koristi_leks:
            ind = nadji_pivot(tabela,B,n_b)
        else :
            ind = nadji_pivot_staro(tabela,B,n_b)
        print()
        print('Pivotiramo oko elementa A[{}][{}]:{}'.format(n_b,ind,round(tabela[n_b][ind],br_decimala)))
        stari = tabela[n_b][ind]
        tabela[n_b] = [a/stari for a in tabela[n_b]]

        for j in range(len(tabela)):
            if j!=n_b:
                mult = tabela[j][ind]
                tabela[j] = [a-mult*k for a,k in zip(tabela[j],tabela[n_b])]
        B[n_b] = ind
        print()
        print('Nakon pivotiranja:')
        print('Iteracija {}'.format(iteracija))
        print('---------------')
        pisi_simplex_tabelu(tabela, B, [], [])
        iteracija += 1
    print()
    print(colored('Sva b>0 kraj','yellow'))
    return tabela,B


def tablicni_simpleks(tabela,B,S,W):
    print('Krecemo tablicni simpleks:')
    print()
    pisi_simplex_tabelu(tabela, B,S,W)
    print()
    if any(i!=0 for i in tabela[-1][:-1]):
            print('Cistimo f-ju cilja od bazicnih:')
            for b in B:
                if tabela[-1][b]!=0:
                    mult = tabela[-1][b]
                    ind = B.index(b)
                    tabela[-1] = [i-mult*j for i,j in zip(tabela[-1],tabela[ind])]

            pisi_simplex_tabelu(tabela, B,S,W)

    iteracija = 0

    while any(i<0 for i in tabela[-1][:-1]):
        print()
        print('iteracija : {}'.format(iteracija))
        iteracija += 1
        print('------------------------')

        kandidati = [i for i,j in enumerate(tabela[-1][:-1]) if j<0 and i not in B]
        col = kandidati[0]
        kandidati = [ tabela[i][-1]/tabela[i][col] if tabela[i][col]>0 else math.inf for i in range(len(tabela[:-1]))]

        if all(i == math.inf for i in kandidati):
            print('Gornje je ograniceno -> nema resenje')
            return None , B

        ind = kandidati.index(min(kandidati))

        print()
        print('Pivotiramo oko A[{}][{}]'.format(ind,col))
        staro = tabela[ind][col]
        tabela[ind] = [i/staro for i in tabela[ind]]

        for i in range(len(tabela)):
            if i!=ind:
                mult = tabela[i][col]
                tabela[i] = [ j-mult*k for j,k in zip(tabela[i],tabela[ind])  ]

        B[ind] = col
        pisi_simplex_tabelu(tabela, B,S,W)
        print()
    
    return tabela,B
          

def razlomljeni_deo(a):
    if a>=0:
        return abs(a) - math.floor(abs(a))
    else :
        b = abs(a)
        b = math.ceil(b)
        return b - abs(a)
    
def ucitaj_problem():
    file = open(sys.argv[1])
    lines = file.readlines()
    type = lines[0].split()[0]
    Z = [ float(j) for j in lines[0].split()[1:]]
    A = []
    b = []
    for i in range(1,len(lines)):
        red = [float(j) for j in lines[i].split()[:-2]]
        A.append(red)
        b.append([ lines[i].split()[-2] ,float(lines[i].split()[-1]) ])
    return LinearniProblem(type, A, b, Z)
 
if __name__ == "__main__":

    koristi_leks = any(i=='leks' for i in sys.argv)
    print('Ucitavamo problem iz fajla : {}'.format(sys.argv[1]))
    LP = ucitaj_problem()
    LP.pisi_problem()
    print()
    LP.gomorijev()