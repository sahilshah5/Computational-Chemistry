def fatty_gen(chain_length, DB_position, orientation):
    "Generate the SMILES code for a specific molecule"
    if orientation=="Z":
        SMILES= (DB_position-1)*"C" + "\C=C\\" + (chain_length-DB_position-1)*"C" + "(=0)O"
    if orientation=="E":
        SMILES=(DB_position-1)*"C" + "/C=C\\" + (chain_length-DB_position-1)*"C" + "(=0)O"
    return SMILES
chain_length=input("What is the chain length of the molecule?")
print("Chain Length:",chain_length)
DB_position=input("What is the position of the double bond?")
print("Double Bond position:", DB_position)
orientation=input("Does the molecule have E or Z geometry?")
print("Geometry:", orientation)

print(fatty_gen(chain_length, DB_position, orientation)
input("Press return to close the window.")
