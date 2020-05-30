#Template for a program to identify disulfide bridges on an XYZ file

#Write first any import statements that you need in your program

#Define here any functions you will be using
def parse_line(number, string):
    "Return a list of number, element symbol and coordinates"
    l=string.split()
    sym=str(l[0])
    coordx=float(l[1])
    coordy=float(l[2])
    coordz=float(l[3])
    coordinates=[coordx, coordy, coordz]
    parse=[number,sym,coordinates]
    return parse

def distance(list1, list2):
    "Give the distance between 2 sets of coordinates"
    x1=float(list1[0])
    y1=float(list1[1])
    z1=float(list1[2])
    x2=float(list2[0])
    y2=float(list2[1])
    z2=float(list2[2])
    dist = ( (x1-x2)**2 + (y1-y2)**2 + (z1-z2)**2 )**0.5
    return dist

def isBridge(list1, list2):
    "Is there a bond?"
    if distance(list1, list2) <= 2.4 and distance(list1, list2)!=0:
        bond = True
    else:
        bond = False
    return bond

def formatter(list1, list2):
    "Show bonded atoms"
    el_sym1= str(list1[1])
    num1=str(list1[0])
    el_sym2= str(list2[1])
    num2=str(list2[0])
    lin=":"+el_sym1+"("+num1+")-"+el_sym2+"("+num2+")"
    return lin




#The following if statement does not affect the working of the program,
#but will allow to test your functions even if the rest of the program
#does not work

if __name__=="__main__":
    file_in=input("Filename?")
    with open(file_in, 'r') as f:
        f1=list(f)
        f2=f1[2:]
        s_data=[]
        number=0
        for line in f2:
            number=number+1
            if "S" in line.split():
                v=parse_line(number,line)
                s_data=s_data +[v]
    bonds=[]
    for i in s_data:
        coord=i[2]
        for j in s_data:
            isBridge(coord,j[2])
            if isBridge(coord,j[2])==True:
                bonds=bonds+[formatter(i,j)]
                if formatter(j,i) not in bonds:
                    print(formatter(i,j))
    input("Press return to close the window.")