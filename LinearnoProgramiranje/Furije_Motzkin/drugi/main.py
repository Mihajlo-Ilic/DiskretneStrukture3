import sys


def proveri_pripadnost(A,tacka):
    for nejednacina in A:
        s = 0
        for i in range(0,len(nejednacina)-1):
            s+= nejednacina[i] * tacka[i]
        if s< nejednacina[-1]:
            return False

    return True

def main():

    args = sys.argv

    if(len(args)!=2):
        print('nije prosledjen lepo fajl')
        quit()
    f = open(args[1],"r")

    A =[]
    tacka = []
    linije = f.readlines()
    tacka = [float(x) for x in linije[0].split()]
    for i in range(1,len(linije)):
        A.append([ float(x) for x in linije[i].split() ])

    if proveri_pripadnost(A,tacka) == False:
        print("ne pripada")
    else:
        print("pripada")

if __name__ == "__main__":
    main()
