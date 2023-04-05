import os 

def iterInDirectory(directory):
    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        if os.path.isfile(f):
            print(f)

def TestInDirectory(directory):
    for filename in os.listdir(directory): #Problème sur le directory, il prend tout le dossier Tests
        f = os.path.join(directory,filename)
        if os.path.isfile(f) and f.endswith(".c"):
            print(filename)
            

if __name__ == '__main__':

    print("Entrez le code du repertoire à tester : \n")
    print("Codes : \n")
    print(" - 1 = Gencode/OK\n")
    print(" - 2 = Gencode/KO\n")
    print(" - 3 = Syntaxe/OK\n")
    print(" - 4 = Syntaxe/KO\n")
    print(" - 5 = Verif/OK\n")
    print(" - 6 = Verif/KO\n")
    print(" - 7 = Tout\n")
    code = input("Code : ")
    if code == 1:
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Gencode/OK")
    elif code == 2:
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Gencode/KO")
    elif code == 3:
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Syntaxe/OK")
    elif code == 4:
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Syntaxe/KO")
    elif code == 5:
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Verif/OK")
    elif code == 6:
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Verif/KO")
    else:
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Gencode/OK")
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Gencode/KO")
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Syntaxe/OK")
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Syntaxe/KO")
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Verif/OK")
        TestInDirectory("/home/thomas/Documents/projet_compilation_src/Tests/Verif/KO")
