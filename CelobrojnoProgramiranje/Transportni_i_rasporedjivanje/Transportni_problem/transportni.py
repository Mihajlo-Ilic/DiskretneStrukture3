import sys
import math

#promenljiva koja sluzi kao indikator da li da se ispisuju svi koraci u algoritmu
pisi_korake = False

#funkcija za ucitavanje problema na ulazu
def ucitaj_problem():

    if len(sys.argv) < 2:
        print('nije prosledjena putanja do fajla!')
        quit()

    #ako je prosledjen jedan vise arg
    # ument i on je k ispisivacemo sve korake
    if len(sys.argv) == 3 and 'k' in sys.argv[2]:
        #ako bi stavili bez global , pisi_korake bi bila lokalna promenljiva za funkciju pa mora ovako
        global pisi_korake
        pisi_korake = True
    
    file = open(sys.argv[1],"r")
    lines = file.readlines()

    C = []
    a = []
    #kad smo ucitali sve linije idemo kroz njih i citamo vrednosti
    for line in lines[:-1]:
        values = line.split()
        #do poslednjeg je red za C
        C.append([float(i) for i in values[:-1]])
        #poslednji je element za vektor a
        a.append(float(values[-1]))

    #poslednja linija je vektor b
    b = [float(i) for i in lines[-1].split()]

    return a,b,C

def standardizuj_problem(a,b,C):
    #ovde proveravamo da li je suma a = suma b
    #i dodajemo promenljive ako treba
    #takodje vracamo matricu Q

    Q = [[-math.inf for j in red] for red in C]
    V = [[0 for j in red] for red in C]

    if sum(a)<sum(b):
        if pisi_korake:
            print('\nPosto je suma A < suma B -> Problem je nebalansiran')
            print('Dodajemo novi vestacki red')
            print()

        red = [50 for i in C[0]]
        C.append(red)
        Q.append([-math.inf for j in C[0]])
        V.append([1 for j in C[0]])
        a.append(abs(sum(a)-sum(b)))
    elif sum(a)>sum(b):
        if pisi_korake:
            print('\nPosto je suma B < suma A -> Problem je nebalansiran')
            print('Dodajemo novu vestacku kolonu')
            print()

        for r in C:
            r.append(50)
        for r in Q:
            r.append(-math.inf)
        for r in V:
            r.append(1)
        b.append(abs(sum(a)-sum(b)))
    return Q,V

def min_cena_k1(a,b,C,helper):
    #prolazimo kroz matricu C iz problema i trazimo najmanji element
    p = 0
    q = 0
    minimum = math.inf
    for i in range(len(C)):
        for j in range(len(C[i])):

            if C[i][j] < minimum and helper[i][j]==1:
                minimum = C[i][j]
                p = i
                q = j
    return p,q

def pisi_problem(a,b,C,Q):
    #zbog lepseg ispisa trazimo kolika je velicina stringa za najvecu celiju
    max_len = 0
    for i in range(len(C)):
        for j in range(len(C[i])):
            if Q[i][j]==-math.inf:
                max_len = max(max_len,2+len(str('{},'.format(round(C[i][j],2)))))
            else:
                max_len = max(max_len,2+len(str('{}[{}],'.format(round(C[i][j],2),round(Q[i][j],2)))))
 
    #ispisujemo problem
    for i in range(len(C)):
        for j in range(len(C[i])-1):
            if Q[i][j]==-math.inf:
                s = '{}'.format(round(C[i][j],2))
                duzina = max_len-len(s)
                levo = duzina//2
                desno = duzina-levo
                levi = " " * levo
                desni = " " * desno
                s = levi + s + desni
                print(s,end='|')
            else:
                s = '{}[{}]'.format(round(C[i][j],2),round(Q[i][j],2))
                duzina = max_len-len(s)
                levo = duzina//2
                desno = duzina-levo
                levi = " " * levo
                desni = " " * desno
                s = levi + s + desni
                print(s,end='|')

        if Q[i][j+1]==-math.inf:
            s = '{}'.format(round(C[i][j+1],2))
            duzina = max_len-len(s)
            levo = duzina//2
            desno = duzina-levo
            levi = " " * levo
            desni = " " * desno
            s = levi + s + desni
            print(s,end='|')
        else:
            s = '{}[{}]'.format(round(C[i][j+1],2),round(Q[i][j+1],2))
            duzina = max_len-len(s)
            levo = duzina//2
            desno = duzina-levo
            levi = " " * levo
            desni = " " * desno
            s = levi + s + desni
            print(s,end='|')
            
        print(round(a[i],2))
    for i in range(5+max_len*len(Q[0])):
        print('-',end='')
    print()
    for i in b:
        s=str(round(i,2))
        duzina = max_len-len(s)
        levo = duzina//2
        desno = duzina-levo
        levi = " " * levo
        desni = " " * desno
        s = levi + s + desni
        print(s,end=' ')

def minimalna_cena(a_c,b_c,C_c,Q_c):
    a = a_c.copy()
    b = b_c.copy()
    C = C_c.copy()
    Q = Q_c.copy()

    #pravimo pomocnu matricu gde cuvamo sta smo izbrisali
    helper = [[1 for j in red] for red in C]

    #krecemo metod minimalnih cena
    #dok postoji neki element u nekom redu koji je 1 radimo alg
    br = 0
    while any(any(i==1 for i in red) for red in helper):
        p,q = min_cena_k1(a,b,C,helper)
        #trazimo min od a[p] , b[q]
        min_el = min(a[p],b[q])
        #postavljamo Q[p][q] na taj el
        Q[p][q] = min_el
        br += 1
        #stavljamo a_novo = a_staro - min_el i tako i za b
        a[p] -= min_el
        b[q] -=min_el

        if pisi_korake:
            print('\nIzabrali smo najmanji element C[{}][{}] = {}'.format(p,q,round( C[p][q],2) ) )
            print('min(a[{}],b[{}]) = {}'.format(p,q,min_el))
            print('a_novo = {} , b_novo = {}'.format(round(a[p],2) , round(b[q],2)))
        #sada ako je b ili a jednako 0 brisemo kolonu ili red
        if a[p]==0:
            helper[p] = [-math.inf for j in helper[p]]
        if b[q]==0:
            for i in range(len(helper)):
                helper[i][q] = -math.inf

    if pisi_korake:
        print()
        print('Kraj minimalnih cena:')
        print()
        for r in Q:
            print([str(round(j,2)) if j!=-math.inf else "*" for j in r])
        print()

    #ako ne postoji m+n-1 alociranih celija znaci da imamo degeneraciju problema
    #moramo to da sredimo
    #radimo sledece biramo najmanju nealociranu celiju tako da sa njom ne moze da se formira zatvoren cikl
    if len(Q) + len(Q[0]) - 1 > br:

        if pisi_korake:
            print('Desila se degeneracija u problemu')
            print('Moramo da alociramo jos {} celije'.format( (len(Q) + len(Q[0]) - 1) -br  ))
            print()

        br_cel = (len(Q) + len(Q[0]) - 1) - br

        for i in range(br_cel):
            #uzimamo sve indekse i stavljamo u listu koju sortiramo po vrednosti celije rastuce
            #ali da prethodno nisu alocirani
            indexi = []
            for j in range(len(Q)):
                for k in range(len(Q[j])):
                    if Q[j][k] == -math.inf:
                        indexi.append([j,k])
            #sortiramo indekse po vrednosti koja im je u C
            indexi.sort(key = lambda x:C[x[0]][x[1]] )
            #proveravamo da li se pravi cikl zatvoren ako datu poziciju alociramo
            for el in indexi:
                
                helper = [[0 for el in red] for red in Q]
                path = []
                path.append([el[0],el[1]])

                l = be_ef_es(Q,helper,0,el[0],el[1],path)
                r = be_ef_es(Q,helper,1,el[0],el[1],path)
                u = be_ef_es(Q,helper,2,el[0],el[1],path)
                d = be_ef_es(Q,helper,3,el[0],el[1],path)

                if l==None and r==None and u==None and d==None:
                    if pisi_korake:
                        print('Alocirali smo [{}][{}]'.format(el[0],el[1]))
                    Q[el[0]][el[1]] = 0
                    break
                else:
                    Q[el[0]][el[1]] = -math.inf


        if pisi_korake:
            print()
            print('Kada smo popravili degeneraciju:')
            for r in Q:
                print([str(round(j,2)) if j!=-math.inf else "*" for j in r])
        print()
        
    return Q

def nadji_max_kapica(a,b,C,Q):
    #pravimo koliko koji red ima kapica
    redovi_kapice = [sum(j!=-math.inf for j in i) for i in Q]
    #koliko koja kolona ima kapica
    kolone_kapice = [sum(Q[j][i]!=-math.inf for j in range(len(Q))) for i in range(len(Q[0]))]

    maximum_redovi = max(redovi_kapice)
    maximum_kolone = max(kolone_kapice)

    #vracamo najveci od ova dva
    if maximum_redovi>=maximum_kolone:
        return redovi_kapice.index(maximum_redovi),True
    else:
        return kolone_kapice.index(maximum_kolone),False

def korak1(a,b,C,Q,ind,tip):
    v = [-math.inf for i in range(len(Q[0]))]
    u = [-math.inf for i in range(len(Q))]

    #stavljamo 0 tu gde je najvise kapica
    if tip == True:
        u[ind] = 0
    else:
        v[ind] = 0

    #dok je neki -beskonacno moramo da ga racunamo
    while any(i==-math.inf for i in v) or any(i==-math.inf for i in u):

        #prolazimo kroz redove i kolone trazimo Cij!=-besk i pokusamo da racunamo u i v za te indekse
        for i in range(len(Q)):
            for j in range(len(Q[i])):
                if Q[i][j]!=-math.inf:
                    #ako je jedan od u ili v jednak -besk racunamo drugi inace nista
                    if u[i]!=-math.inf and v[j]==-math.inf:
                        v[j] = -u[i] + C[i][j]
                    elif v[j]!=-math.inf and u[i]==-math.inf:
                        u[i] = -v[j] + C[i][j]
    return u,v

def korak2i3(u,v,C,Q):

    min = math.inf

    ii = -1
    jj = -1

    for i in range(len(Q)):
        for j in range(len(Q[i])):
            if Q[i][j]==-math.inf:
                res = C[i][j] - u[i] - v[j]
                if res<0 and res<min:
                    ii=i
                    jj=j
                    min = res

    return ii,jj

def ispisi_cikl(Q,path):
    P = [['-' for j in red] for red in Q]
    print(path)
    j = 0
    for i in path:
        P[i[0]][i[1]] = str(j)
        j+=1
    P[path[0][0]][path[0][1]] = '0'
    for red in P:
        print(red)


#direkcija 0-levo ,1-desno , 2-gore, 3-dole
def be_ef_es(Q,helper,direkcija,i,j,path):
    #ako smo obisli trenutni cvor ili smo van indeksa matrice izlazimo
    if i < 0 or j < 0:
        return None
    if i >= len(Q) or j >= len(Q[0]):
        return None

    #ako smo nasli put mora biti duzine barem 4 takodje mora da je cikl da se ponavljaju pocetni i tekuci el
    if len(path)>=4 and [i,j] == path[0]:
        res = path.copy()
        res.append([i,j])
        return res

    if helper[i][j]==1 and Q[i][j]!=-math.inf:
        return None

    helper[i][j] = 1
    #ako se nalazimo na polju koje je bazisno onda proveravamo na sve 4 strane sveta da li moze cikl da se izgradi
    if Q[i][j] != -math.inf:
        path.append([i,j])
        levo = be_ef_es(Q,helper,0,i,j-1,path)
        desno = be_ef_es(Q,helper,1,i,j+1,path)
        gore = be_ef_es(Q,helper,2,i-1,j,path)
        dole = be_ef_es(Q,helper,3,i+1,j,path)
        path.pop()

        if levo != None :
            return levo
        if desno != None:
            return desno
        if gore != None:
            return gore
        if dole != None:
            return dole
    #ako smo na polju koje nije bazisno moramo da nastavimo u direkciji u kojoj smo se setali
    else:
        pi = i
        pj = j
        if direkcija == 0:
            pj = j-1
        elif direkcija == 1:
            pj = j+1
        elif direkcija == 2:
            pi = i-1
        elif direkcija == 3:
            pi = i+1
        res = be_ef_es(Q,helper,direkcija,pi,pj,path)
        helper[i][j] = 0
        return res
    
    

def korak4(a,b,C,Q,i,j):
    #uzmemo listu svih bazisnih indeksa i onda radimo bfs da nadjemo cikl
    helper = [[0 for el in red] for red in Q]
    path = []
    path.append([i,j])
    res = []
    for dir in range(4):
        res = be_ef_es(Q,helper,dir,i,j,path)
        if res!=None:
            if pisi_korake:
                print('Cikl koji smo izabrali:')
                ispisi_cikl(Q,res)   
            break
    #cistimo putanju koju smo uzeli da bi imali najdalje tacke izmedju dve bazicne
    #tj ako smo istom direkcijom isli izmedju 2 tacke
    za_brisanje = []
    for ri in range(1,len(res)-1):
        if res[ri][0]==res[ri-1][0] and res[ri][0]==res[ri+1][0]:
            za_brisanje.append(ri)
        elif res[ri][1]==res[ri-1][1] and res[ri][1]==res[ri+1][1]:
            za_brisanje.append(ri)
    res = [k for r,k in enumerate(res) if r not in za_brisanje ]

    #sada za svaki drugi tj tu gde je - trazimo minimum
    #pravimo samo bazisne iz puta
    bazisni = [q for q in res if Q[q[0]][q[1]]!=-math.inf ]
    #pravimo samo one koji su negativni ( svaki drugi je pozitivan )
    bazisni = [q for n,q in enumerate(bazisni) if n%2==0]
    vrednosti = [Q[q[0]][q[1]] for q in bazisni]
    najmanji = min(vrednosti)
    ni = vrednosti.index(najmanji)

    if pisi_korake:
        print('\nZa teta smo uzeli: {}'.format(round(najmanji,2)))
    
    put = [q for q in res if Q[q[0]][q[1]]!=-math.inf ]
    for k in range(len(put)):
        if k%2==0:
           Q[put[k][0]][put[k][1]] -= najmanji
        else:
            Q[put[k][0]][put[k][1]] += najmanji
    Q[i][j] = najmanji
    Q[bazisni[ni][0]][bazisni[ni][1]] = -math.inf

def metod_potencijala(a,b,C,Q,V):

    iteracija = 0
    while True:

        #krecemo metod potencijala prvo uzimamo kolonu ili red sa najvise kapica
        ind,tip = nadji_max_kapica(a,b,C,Q)

        if pisi_korake:
            print('Racunamo gde je u ili v na pocetku 0 da bi izracunali ostale')
            if tip==False:
                print('Uzimamo kolonu ind = {}'.format(ind))
            else:
                print('Uzimamo red ind = {}'.format(ind))

        if pisi_korake:
            print('Iteracija = {}'.format(iteracija))
            print('-------------')
            pisi_problem(a,b,C,Q)
        #racunamo u,v tu gde je ind stavljamo 0 i rekurzivno racunamo
        u,v = korak1(a,b,C,Q,ind,tip)

        if pisi_korake:
            print('\nIzracunali smo u i v:')
            print('v = {}'.format([round(j,2) for j in v]))
            print('u = {}'.format([round(j,2) for j in u]))
        
        #proveravamo da li postoji neki nebazisni tako da je cij-ui-vj<0
        #ako postoji uzimamo najnegativniji po formuli od njih inace stop
        i,j = korak2i3(u,v,C,Q)
        if [i,j] == [-1,-1]:
            if pisi_korake:
                print('\nNe postoji nebazicno C po formuli da je <0 -> Kraj')
            #trazimo resenje idemo kroz bazisne Cij i Qij i saberemo njihov proizvod
            print()
            print('Resenje je:')

            #ispisujemo poslednju tabelu
            if pisi_korake:
                pisi_problem(a,b,C,Q)
                print()

            st = ""
            res = 0
            for ri in range(len(Q)):
                for rj in range(len(Q[ri])):
                    if Q[ri][rj]!=-math.inf and V[ri][rj]==0 and Q[ri][rj]!=0:
                        st += str(C[ri][rj]) +'*'+str(Q[ri][rj])+' + '
                        res +=C[ri][rj] * Q[ri][rj]
            st = st[:-2]
            print(st)
            print(res)
            quit()

        if pisi_korake:
            print('\nNajmanje nebazicno po formuli C je: C[{}][{}]'.format(i,j))
        
        #pravimo cikl sada
        if pisi_korake:
            print('\nUzeli smo indekse za postavku teta [{},{}]'.format(i,j))

        korak4(a,b,C,Q,i,j)
        if pisi_korake:
            print('Nakon racunanja teta:')
            for red in Q:
                print([str(round(j,2)) if j!=-math.inf else '*' for j in red])
            print()
        iteracija += 1

if __name__ == "__main__":
    a,b,C = ucitaj_problem()
    Q,V = standardizuj_problem(a,b,C)

    if pisi_korake:
        print('Ucitali smo problem:')
        pisi_problem(a,b,C,Q)
        print()
        print()
        print('Krecemo metodu minimalnih cena:')
    
    
    Q = minimalna_cena(a,b,C,Q)

    if pisi_korake:
        print('Krecemo metod potencijala:')
    metod_potencijala(a,b,C,Q,V)