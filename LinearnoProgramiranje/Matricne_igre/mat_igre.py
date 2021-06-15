import sys
import math


#### DVOFAZNI ALGORITAM
class LinProb:
    def __init__(self,type,A,Z,b):
        self.type = type
        self.A = A.copy()
        self.Z = Z.copy()
        self.b = b.copy()

    def ispisi_problem(self):
        print(self.type+' f = ',end='')
        print([round(i,2) for i in self.Z])

        print()

        for i in range(len(self.A)):
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
            elif b[i][1]==0 and b[i][0]=='>=':
                A[i] = [-j for j in A[i]]
                b[i][1] *= -1
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

        #radimo ciscenje tabele ako neka bazicna promenljiva u funkciji cilja ima koeficijent razlicit od 0
        #dodajemo red gde je ta bazicna sa -koeficijentom kako bi napravili 0 u funkciji cilja

        for i in range(len(B)):
            #u poslednjem redu se nalazi funkcija cilja zato -1
            if tabela[-1][B[i]]!=0:
                koef = -tabela[-1][B[i]]
                tabela[-1] = [i+koef*j for i,j in zip(tabela[-1],tabela[i])]

        #radimo algoritam sada kada smo ocistili funkciju cilja

        iteracija =0
        while True:

       
            #ako ne postoji j takvo da je Zj <0 nasli smo optimalno resenje i stajemo
            if not any(i<0 and i not in B for i in tabela[-1][:-1]):
                return B,tabela
            #trazimo j takvo da Zj je manje od 0 i j nije u bazi
            #ako koristimo blendovo pravilo uvek uzimamo od kandidata najmanji indeks
            #enumerate vraca (i,j) gde je i indeks a j vrednost u listi
            #poslednji element ne gledamo to je vrednost funkcije
            kandidati = [i for i,j in enumerate(tabela[-1][:-1]) if j<0 and j not in B]
            ind = kandidati[0]
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
                quit()
            #sada kada imamo pivot element A[red][ind] pravimo ga bazicnim tako sto iznad i ispod pravimo 0
            #al od njega pravimo jedinicu ceo red delimo sa njim
            tabela[red] = [i/tabela[red][ind] for i in tabela[red]]
            for i in range(len(tabela)):
                if i!=red:
                    mult = -tabela[i][ind]
                    tabela[i] = [i+mult*j for i,j in zip(tabela[i],tabela[red])]

            B[red] = ind
            iteracija +=1

        return B,tabela

def izbrisi_vestacke_promenljive(tabela,B,W):
    while len(W)!=0:
        #brisemo vestacke promenljive tako sto radimo sledece:
        #ako nisu bazisne odmah ih samo precrtamo
        nebazisne = [i for i in W if i not in B]

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


        obrisane = []
        for w in W:
            red = B.index(w)
            indexi = [i for i,j in enumerate(tabela[red][:-1]) if i!=w]
            nenula = any(tabela[red][i]!=0 for i in indexi)

            if nenula:
                #ako postoji neki koji nije nula uzimamo neki od njih i pravimo ga bazom sa pivotiranjem
                kandidati = [i for i in indexi if tabela[red][i]!=0]
                ind = kandidati[0]

                tabela[red] = [i/tabela[red][ind] for i in tabela[red]]

                for i in range(len(tabela)):
                    if i!=red:
                        mult = -tabela[i][ind]
                        tabela[i] = [i+mult*j for i,j in zip(tabela[i],tabela[red])]
                B[red] = ind
            else:
                #ako je 0 sve onda brisemo red i kolonu
                tabela.pop(red)
                for i in range(len(tabela)):
                    tabela[i].pop(w)

                el = B.pop(red)
                B = [i if el>i else i-1 for i in B]
                W.remove(w)
                W = [i if el > i else i - 1 for i in W]

    return tabela,B

def prob_izTabele(tabela):
    type = 'min'
    A = []
    b = []
    Z = [i for i in tabela[-1][:-1]]

    for i in range(len(tabela)-1):
        A.append([i for i in tabela[i][:-1]])
        b.append(['=',tabela[i][-1]])

    return LinProb(type,A,Z,b)

def dvofazni_simpleks(lp):
    standard_Lp = lp.standardizuj()

    if pisi_korake:
        print('Standardizovan problem:')
        standard_Lp.ispisi_problem()
        print()

    B = standard_Lp.pronadji_bazisne()

    if any(i == -1 for i in B):

        pomocni,W = standard_Lp.pomocni_problem(B)
        Bn,tabela = pomocni.simpleks_metod()

        if tabela[-1][-1]!=0:
            return None,None
        
        tabela2,B2 = izbrisi_vestacke_promenljive(tabela,Bn,W)
        tabela2[-1] = standard_Lp.Z + [0]

        sekundum = prob_izTabele(tabela2)
        B3,tabela3= sekundum.simpleks_metod()

        x = [0] * (len(tabela3[0])-1)
        for i in range(len(sekundum.b)):
            x[B3[i]] = tabela3[i][-1]

        return x,tabela3[-1][-1]
    else:
        Bn,tabela = standard_Lp.simpleks_metod()

        x = [0] * (len(tabela[0])-1)
        for i in range(len(standard_Lp.b)):
            x[Bn[i]] = tabela[i][-1]

        return x,tabela[-1][-1]


### TEORIJA IGARA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

pisi_korake = True
koristi_dominaciju = False

def ucitaj_problem():
    file = open(sys.argv[1],"r")
    linije = file.readlines()

    A = [[float(el) for el in linija.split()]  for linija in linije]
    return A

def nadji_dominacija_red(A):

    for i in range(len(A)):
        for j in range(len(A)):
            if i!=j:
                if all(A[j][k]<=A[i][k] for k in range(len(A[i]))):
                    return j
    return -1

def nadji_dominacija_kolonu(A):

    for i in range(len(A[0])):
        for j in range(len(A[0])):
            if i!=j:
                if all(A[k][i]<=A[k][j] for k in range(len(A))):
                    return j
    return -1

def dominacija(A):
    #ako postoji red j tako da je A[j][k]<=A[i][k] za proizvoljno i , ali za sve k
    #brisemo j red
    x_opt = [-math.inf for i in A]
    y_opt = [-math.inf for i in A[0]]
    while True:
        red = nadji_dominacija_red(A)
        kolona = nadji_dominacija_kolonu(A)
        if red == -1 and kolona== -1:
            break
        if red!=-1:
            if pisi_korake:
                print('Dominirajuci red je bio {}'.format(red))
            A.pop(red)
            x_opt[red] = 0

        elif kolona!=-1:
            if pisi_korake:
                print('Dominirajuca kolona je bila {}'.format(kolona))
            for r in A:
                r.pop(kolona)
            y_opt[kolona] = 0

        if pisi_korake:
            print('Nova matrica:')
            for red in A:
                print([round(j,2) for j in red])
            print()

    return x_opt,y_opt,A

def konstruisi_prvi(A):
    #konstruisemo linearni problem od problema iz matrice A
    Acp = A.copy()
    type = 'min'
    b = [['<=',0] for i in Acp]
    b.append(['=',1])
    for i in range(len(Acp)):
        Acp[i] = A[i].copy()
        #dodajemo 2 promenljive v=v1-v2 , kad se prebace na drugu stranu bice -v1+v2
        Acp[i].append(-1)
        Acp[i].append(1)

    Acp.append([1 for i in Acp[0]])
    Acp[-1][-1] = 0
    Acp[-1][-2] = 0

    Z = [0 for i in Acp[0]]
    Z[-2] = 1
    Z[-1] = -1

    return LinProb(type,Acp,Z,b)

def konstruisi_drugi(A):
    #konstruisemo linearni problem od problema iz matrice A
    Acp = []
    for i in range(len(A[0])):
            red = []
            for j in range(len(A)):
                red.append(A[j][i])
            Acp.append(red)
            
    type = 'max'
    b = [['>=',0] for i in Acp]
    b.append(['=',1])
    for i in range(len(Acp)):
        #dodajemo 2 promenljive v=v1-v2 , kad se prebace na drugu stranu bice -v1+v2
        Acp[i].append(-1)
        Acp[i].append(1)

    Acp.append([1 for i in Acp[0]])
    Acp[-1][-1] = 0
    Acp[-1][-2] = 0

    Z = [0 for i in Acp[0]]
    Z[-2] = 1
    Z[-1] = -1

    return LinProb(type,Acp,Z,b)

def popuni(x1,x):
    for i in range(len(x)):
        if all(el!=-math.inf for el in x1):
            break
        for j in range(len(x1)):
            if x1[j] == -math.inf:
                x1[j] = x[i]
                break


if __name__ == "__main__":
    A = ucitaj_problem()

    x1 = [-math.inf for i in range(len(A))]
    y1 = [-math.inf for i in range(len(A[0]))]
    
    if len(sys.argv)==3:
        if "dom" in sys.argv[2]:
            koristi_dominaciju = True

    if pisi_korake:
        print('Ucitali smo problem A:')
        for red in A:
            print([round(j,2) for j in red])
        print()
    
    if koristi_dominaciju:
        print('Krecemo metodu dominacije:')
        print('__________________________')
        print()
        x1,y1,A=dominacija(A)

        if pisi_korake:
            print('x = {}'.format(['*' if i==-math.inf else str(i) for i in x1]))
            print('y = {}'.format(['*' if i==-math.inf else str(i) for i in y1]))
            print()
            print('Gotova metoda dominacije')

    if pisi_korake:
        print()
        print('Konstruisemo linearni problem za I igraca:')
        
    prvi = konstruisi_drugi(A)

    if pisi_korake:
        prvi.ispisi_problem()
        print()
        print('Resavamo dvofaznim simpleksom problem za igraca I:')
    x,f1 = dvofazni_simpleks(prvi)

    if pisi_korake:
        print()
        print('Konstruisemo linearni problem za II igraca:')

    drugi = konstruisi_prvi(A)
    if pisi_korake:
        print()
        drugi.ispisi_problem()
        print()
        print('Resavamo dvofaznim simpleksom problem za igraca II:')
    
    y,f2 = dvofazni_simpleks(drugi)
    if pisi_korake:
            if [x,f1] != [None,None]:
                popuni(x1,x)
                print('x optimalno = {}'.format([round(j,2) for j in x1]))
                print(round(f1,2))
    if [y,f2] != [None,None]:
        if pisi_korake:
            print()
            popuni(y1,y)
            print('y optimalno = {}'.format([round(j,2) for j in y1]))
            print(round(-1*f2,2))
