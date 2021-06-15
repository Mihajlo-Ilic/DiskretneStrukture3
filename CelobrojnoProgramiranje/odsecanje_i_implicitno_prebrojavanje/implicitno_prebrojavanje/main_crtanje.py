import math
import sys
from graphviz import Graph

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

def racunaj_lp(x,lp):
    f = sum(i*j for i,j in zip(x,lp.Z))
    
    for i in range(len(lp.b)):
        l_str = sum(i*j for i,j in zip(x,lp.A[i]))
        if lp.b[i][0]=='<=':
            if l_str > lp.b[i][1]:
                return f,False,'nejednacina {} nezadovoljena\n'.format(i+1)+str(round(l_str,2))+' '+lp.b[i][0]+' '+str(lp.b[i][1])+'\n'+str(x)

    return f,True,''

def racunaj_lp2(x,lp):
    f = sum(i*j for i,j in zip(x,lp.Z))
    
    for i in range(len(lp.b)):
        l_str = sum(i*j for i,j in zip(x,lp.A[i]))
        if lp.b[i][0]=='<=':
            if l_str > lp.b[i][1]:
                return f,False,'nejednacina {} nezadovoljena\n'.format(i+1)+str(round(l_str,2))+' '+lp.b[i][0]+' '+str(lp.b[i][1])+'\n'+str(x)
        elif lp.b[i][0]=='>=':
            if l_str < lp.b[i][1]:
                return f,False,'nejednacina {} nezadovoljena\n'.format(i+1)+str(round(l_str,2))+' '+lp.b[i][0]+' '+str(lp.b[i][1])+'\n'+str(x)
        elif lp.b[i][0]=='=':
            if l_str != lp.b[i][1]:
                return f,False,'nejednacina {} nezadovoljena\n'.format(i+1)+str(round(l_str,2))+' '+lp.b[i][0]+' '+str(lp.b[i][1])+'\n'+str(x)
    return f,True,''

najbolji = -math.inf
x_opt = []
cvor = 0
mit = -1
cvorovi = []
ivice = []

def prebrojavanje(x,ind,lp,roditelj):

    global najbolji
    global x_opt
    global cvor
    global mit
    global cvorovi
    global ivice

    if ind == len(x):
        f,b,s = racunaj_lp2(x,lp)
        if b!=False:
            cvorovi.append([str(cvor),'f = '+str(round(f,2))+'\n'+str(x)])

            if lp.type == 'max':
                if f > najbolji:
                    najbolji = f
                    x_opt = x.copy()
                    mit = cvor
                    print('MIT JE {}'.format(mit))
            else:
                if f < najbolji:
                    najbolji = f
                    x_opt = x.copy()
                    mit = cvor

            print()
            print('Resenje {}'.format(x))
            print(lp.type+' f = {}'.format(round(f,2)))
            print('je jedan kandidat')
            print()
        else:
            cvorovi.append([str(cvor),None,s])
            print('Resenje {}'.format(x))
            print('Ne zadovoljava problem -> prekidamo dalje grananje')
            print()

        return
    b = True
    f = 0
    s=''
    if ind!=-1:
        f,b,s = racunaj_lp(x,lp)

    else:
        ind = 0


    if b!=False:

        cvorovi.append([str(cvor),str(cvor),s])

        
        prvi_x = x.copy()
        drugi_x = x.copy()

        prvi_x[ind] = 1
        print('stavljamo x{} da je 1'.format(ind))

        cvor +=1
        ivice.append([str(roditelj),str(cvor),str('x{} = 1'.format(ind))])

        prebrojavanje(prvi_x,ind+1,lp,cvor)
        drugi_x[ind] = 0
        print('stavljamo x{} da je 0'.format(ind))

        cvor +=1
        ivice.append([str(roditelj),str(cvor),str('x{} = 0'.format(ind))])
        prebrojavanje(drugi_x,ind+1,lp,cvor)
    
    else:
        cvorovi.append([str(cvor),None,s])
        print('Resenje {}'.format(x))
        print('Ne zadovoljava problem -> prekidamo dalje grananje')
        print()

if __name__ == "__main__":

    print('Ucitavamo problem iz fajla : {}'.format(sys.argv[1]))
    LP = ucitaj_problem()
    LP.pisi_problem()
    print()
    print('Krecemo metodu implicitnog prebrojavanja:\n')
    x = [0 for i in LP.Z]

    if LP.type == 'max':
        najbolji = -math.inf
    else:
        najbolji = math.inf

    dot = Graph(comment='Implicitno prebrojavanje',format='png')
    dot.attr(nodesep='1')

    prebrojavanje(x,-1,LP,0)

    if x_opt != []:
        print('\n\nNajbolje resenje je:\n')
        print(LP.type + ' f = {}'.format(round(najbolji,2)))
        print(x_opt)

    for c in cvorovi:
        if c[1]!=None:
            if c[0]==c[1]:
                dot.node(c[0],c[1],style='bold')
            else:
                dot.node(c[0],c[1],style='bold',shape='box')
        else:
            dot.node(c[0],c[2],style='filled,bold',fillcolor='#8c230f',fontcolor='white',shape='box')

    for e in ivice:
        dot.edge(e[0],e[1],label=e[2])

    if mit!=-1:
        dot.node(str(mit),style='filled,bold',fillcolor='#146321',fontcolor='white')

    dot.render('resenje')