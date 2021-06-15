import math
import sys

skup_Z = False
algoritam = 'unapred'

def ucitaj_problem():
    file = open(sys.argv[1])
    lines = file.readlines()
    #ako se prosledi drugi argument i to Z
    #onda resvamo tako da x sme da bude u skupu celih brojeva
    global skup_Z
    global algoritam

    algoritam = sys.argv[2]
    skup_Z = 'Z' in sys.argv[3]

    #u prvoj liniji citamo koliku vrednost ima koji objekat
    C = [int(i) for i in lines[0].split()]
    #u drugoj liniji citamo tezine objekata poslednji broj je tezina ranca
    A = [int(i) for i in lines[1].split()[:-1]]
    b = int(lines[1].split()[-1])

    return C,A,b

def pisi_element(el,max_duzina,pomocni = ''):
    duzina = max_duzina - len(el)
    levo_pad = duzina//2
    desno_pad = duzina - levo_pad

    novi_el = ' '*levo_pad + el + ' '*desno_pad
    print(novi_el,end=pomocni)

def pisi_tabelu(tabela):
    #da bi lepo ispisali tabelu pravimo padding sa leve i desne strane
    max_duzina = 0
    for red in tabela:
        for el in red:
            max_duzina = max(max_duzina,len(str(el))+4)

    podvlaka = '-'*( (len(tabela[0])) * (max_duzina+1) )

    for i in range(len(tabela)):
        for j in range(len(tabela[i])):
            pisi_element( str(tabela[i][j]) ,max_duzina,pomocni='|')
        print()
        print(podvlaka)


def f_unapred(tabela,i,j):
    if j<0:
        return -math.inf
    return tabela[i][j+1]


def rekonstruisi_unapred(tabela,indexi,resenje,A):
    broj_objekata = len(tabela) - 1
    tezina = len(tabela[-1]) - 1
    global skup_Z

    while broj_objekata>0 and (tezina-1)>0:
        print('Trenutna tezina je {} , a broj objekata je {}'.format(tezina-1,broj_objekata))
        print('Gledamo indeks I{}({})'.format(broj_objekata,tezina-1))
        x = indexi[broj_objekata][tezina]
        if x!=0:
            print('x{} povecavamo'.format(x))
        if skup_Z:
            if x!=0:
                resenje[x-1] += 1
        else:
            if x!=0:
                resenje[x-1] = 1
        tezina -= A[x-1]
        if not skup_Z:
            broj_objekata =x-1

    


def ranac_unapred(C,A,b):

    global skup_Z

    j = b+2
    n = sum(i!=0 for i in A)

    #pravimo listu gde promenljiva koja nema tezinu definisanu dobija parametar u sledecoj zavisnosti
    #ako resavamo 0-1 , ako je vrednost pozitivna dobija 1 jer je to max inace dobija 0
    #ako resavamo u Z , ako je vrednost pozitivna dobija infinity
    #ako je negativna dobija 0 jer nmzemo da stavimo manje od 0 u kutiju
    pomocna = []
    pom_A = []

    if skup_Z:
        pomocna =[ math.inf if A[i]==0 else 0 for i in range(len(A)) ]
    else:
        pomocna =[ 1 if A[i]==0 else 0 for i in range(len(A)) ]

    pom_A = [0] * len(A)
    pom_C = [0] * len(C)

    print()
    for i in range(len(pomocna)):
        if pomocna[i]!=0:
            pom_A[i] = A[i] 
            pom_C[i] = C[i]
            print('promenljiva x{} dobija vrednost {} jer tad f-ja ima max'.format(i+1,pomocna[i]))

    C =[c for c,a in zip(C,A) if a!=0 ]
    A =[a for a in A if a!=0]

    print()
    print('Resavamo ranac unapred:')
    print('Formiramo tabele:')
    print()

    #pravimo tabelu
    tabela =[[0]*j for i in range(n+1)]
    #pravimo tabelu indeksa da rekonstruisemo resenje
    indexi =[[0]*j for i in range(n+1)]

    for i in range(1,len(tabela[0])):
        tabela[0][i]= i - 1
        indexi[0][i]= i - 1
    
    for i in range(1,len(tabela)):
        tabela[i][0] = i
        indexi[i][0] = i

    #ako radimo u 0-1 onda ako je tezina prvog objekta manja od Y mozemo da ga stavimo inace 
    #ako radimo u Z stavljamo maks kolko mozemo
    for i in range(1,len(tabela[0])):
        if not skup_Z and i-1>=A[0]:
            tabela[1][i] = C[0]
        elif skup_Z:
            tabela[1][i] = C[0]*((i-1)//A[0])
        if tabela[1][i]!=0:
            indexi[1][i] = 1

    for i in range(2,len(tabela)):
        for j in range(1,len(tabela[i])):

            prethodno = tabela[i-1][j]

            k = 1
            if skup_Z:
                k = (j-1)//A[i-1]

            vrednosti = [ f_unapred(tabela,i-1,(j-1)-A[i-1]*l) + C[i-1]*l for l in range(k+1) ]

            novo = max(vrednosti)

            tabela[i][j] = max( prethodno , novo )

            if tabela[i][j]!=0:
                if prethodno>=novo:
                    indexi[i][j] = indexi[i-1][j]
                else: 
                    indexi[i][j]= i 
    
    #popunjavamo tabelu
    tabela[0][0] ='K\Y'
    indexi[0][0] ='I\Y'

    pisi_tabelu(tabela)
    print()
    pisi_tabelu(indexi)

    resenje = [0] * n
    print()
    rekonstruisi_unapred(tabela,indexi,resenje,A)

    for i in range(len(pomocna)):
        if pomocna[i]!=0 and pom_C[i]>0:
            resenje.insert(i,pomocna[i])
            C.insert(i, pom_C[i])
            A.insert(i,pom_A[i])
        elif pomocna[i]!=0:
            resenje.insert(i,0)
            C.insert(i, pom_C[i])
            A.insert(i,pom_A[i])
    print()
    print('Optimalan raspored je {}'.format(resenje))

    f = sum(i*j for i,j in zip(resenje,C) if i!=math.inf )


    print('Optimalna vrednost je {} '.format(f),end='')
    if any(i==math.inf for i in resenje):
        print('+ {}'.format(math.inf))


def formula_unazad(memo,C,A,b,i,j):

    if i<0 or j<0:
        return -math.inf
    
    if memo[i][j]!=-math.inf:
        if i==1:
             print('F{}({}) = {}'.format(i,j-1,memo[i][j]))
        return memo[i][j]

    global skup_Z

    k = 1
    if skup_Z:
        k = j//A[i-1]
    
    print('F{}({}) = max['.format(i,j-1),end='')

    for l in range(k+1):
        if (j-1)-A[i-1]*l>=0:
            print(' {} + F{}({}) ,'.format( C[i-1]*l , i-1 , (j-1)-A[i-1]*l ),end='')
    print(']\n')

    mogucnosti = [ C[i-1]*l+formula_unazad(memo,C,A,b,i-1,j-A[i-1]*l) for l in range(k+1) if (j-1)-A[i-1]*l>=0]

    najvece = max(mogucnosti)

    memo[i][j] = najvece

    return najvece

def rekonstrui_unazad(memo,i,j,A,C,resenje):
    if i<0 or j<0:
        return
    global skup_Z

    if i==1:
        if memo[i][j]!=0:
            if skup_Z:
                resenje[i-1] = j//A[i-1]
            else:
                resenje[i-1] = 1
        return

    k = 1
    if skup_Z:
        k = j//A[i-1]

    mogucnosti = [ C[i-1]*l+memo[i-1][j-A[i-1]*l] for l in range(k+1) if (j-1)-A[i-1]*l>=0]
    najvece = max(mogucnosti)
    ind = mogucnosti.index(najvece)
    resenje[i-1] += ind
    rekonstrui_unazad(memo,i-1,j-A[i-1]*ind,A,C,resenje)

def ranac_unazad(C,A,b):
    global skup_Z

    j = b
    n = sum(i!=0 for i in A)

    #pravimo listu gde promenljiva koja nema tezinu definisanu dobija parametar u sledecoj zavisnosti
    #ako resavamo 0-1 , ako je vrednost pozitivna dobija 1 jer je to max inace dobija 0
    #ako resavamo u Z , ako je vrednost pozitivna dobija infinity
    #ako je negativna dobija 0 jer nmzemo da stavimo manje od 0 u kutiju
    pomocna = []
    pom_A = [0] * len(A)
    pom_C = [0] * len(C)

    if skup_Z:
        pomocna =[ math.inf if A[i]==0 else 0 for i in range(len(A)) ]
    else:
        pomocna =[ 1 if A[i]==0 else 0 for i in range(len(A)) ]

    print()
    for i in range(len(pomocna)):
        if pomocna[i]!=0:
            pom_A[i] = A[i]
            pom_C[i] = C[i]
            print('promenljiva x{} dobija vrednost {} jer tad f-ja ima max'.format(i+1,pomocna[i]))
    print()
    print('Resavamo ranac unazad:')
    print()

    C =[c for c,a in zip(C,A) if a!=0 ]
    A =[a for a in A if a!=0]
    
    #pravimo memoizaciju da bi lakse racunali
    memo = [[-math.inf] * (j+2) for i in range(n+1)]

    resenje = [0] * n
    for i in range(1,len(memo[0])):
        memo[0][i] = i-1
    for i in range(1,len(memo)):
        memo[i][0] = i
    memo[0][0] = 'K\Y'

    #postavljamo inicijalne vrednosti
    for i in range(1,len(memo[1])):
        if skup_Z:
            memo[1][i] = C[0]*((i-1)//A[0])
        else:
            if i>A[0]:
                memo[1][i] = C[0]
            else:
                memo[1][i]=0
    print()
    #radimo ranac unazad
    f=formula_unazad(memo,C,A,b,n,j+1)

    print()
    print('Nakon racunanja memoizacije:')
    print()
    pisi_tabelu(memo)
    rekonstrui_unazad(memo,n,j+1,A,C,resenje)

    for i in range(len(pomocna)):
        if pomocna[i]!=0 and pom_C[i]>0:
            resenje.insert(i,pomocna[i])
            A.insert(i,pom_A[i])
            C.insert(i,pom_C[i])
        elif pomocna[i]!=0:
            resenje.insert(i,0)
            A.insert(i,pom_A[i])
            C.insert(i,pom_C[i])


    print()
    print('Optimalan raspored je {}'.format(resenje))

    f = sum(i*j for i,j in zip(resenje,C) if i!=math.inf )


    print('Optimalna vrednost je {} '.format(f),end='')
    if any(i==math.inf for i in resenje):
        print('+ {}'.format(math.inf))

if __name__ == "__main__":

    C,A,b = ucitaj_problem()

    print('Ucitali smo problem:\n')
    print('max {}'.format(C))
    print('    {} <= {}'.format(A,b))
    if skup_Z:
        print(' za xi u skupu Z')
    else:
        print(' za xi u skupu [0-1]')
    print()

    if algoritam =='unapred':
        ranac_unapred(C,A,b)
    else:
        ranac_unazad(C,A,b)