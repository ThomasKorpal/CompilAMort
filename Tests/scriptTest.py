import os 
import subprocess
from colorama import Fore, Back, Style

directs = os.listdir(os.curdir)
list_dir_to_test = []

def createTestPathsList():
    for dir in directs:
        if dir != "scriptTest.py":
            for type_dir in os.listdir(os.curdir+"/"+dir):
                path = os.curdir+"/"+dir+"/"+type_dir
                list_dir_to_test.append(path)

def TestInDirectory(directory):
    print(Style.BRIGHT+directory+Style.RESET_ALL)
    cpt = 0
    flagSyn = False
    errTemp = ""
    bufferFichier = []
    bufferErreur = []
    for filename in os.listdir(directory): #Problème sur le directory, il prend tout le dossier Tests
        f = os.path.join(directory,filename)
        if os.path.isfile(f) and f.endswith(".c"):
            if directory.endswith("Syntaxe/OK") or directory.endswith("Syntaxe/KO"):
                result = subprocess.run([".././minicc",f,"-s"],capture_output=True)
            if directory.endswith("Verif/OK") or directory.endswith("Verif/KO"):
                result = subprocess.run([".././minicc",f,"-v"],capture_output=True)
            else:
                result = subprocess.run([".././minicc",f],capture_output=True)
            #print(filename+": "+str(result.stderr))
            if str(result.stderr) != str(b''):
                cpt += 1
                bufferFichier.append(filename)
                bufferErreur.append(result.stderr)
                if directory.endswith("KO"):
                    if directory.endswith("Syntaxe/KO"):
                        if str(result.stderr).split(": ")[1].split(" ")[0] != "syntax":
                            print(Fore.RED + filename+" : "+str(result.stderr).split("'")[1]+Style.RESET_ALL)
                        else:
                            print(Fore.GREEN + filename+" : "+str(result.stderr).split("'")[1]+Style.RESET_ALL)
                    else:
                        print(Fore.GREEN + filename+" : "+str(result.stderr).split("'")[1]+Style.RESET_ALL)
                else:
                    print(Fore.RED + filename+" : "+str(result.stderr).split("'")[1]+Style.RESET_ALL)
            else:
                print(Fore.GREEN + filename+" : No error at execution"+Style.RESET_ALL)

    if directory.endswith("OK"):
        if cpt != 0:
            print(Back.RED+"Pour le repertoire "+directory+", le test à échoué à cause du (ou des) fichier(s) suivant(s) : "+Style.RESET_ALL)
            i = 0
            for f in bufferFichier:
                print(Fore.RED+f+": "+str(bufferErreur[i]).split("'")[1]+Style.RESET_ALL)
                i+=1
            print("\n")
        else:
            print(Back.GREEN+"Les tests ont été correctement passés dans le repertoire "+directory+Style.RESET_ALL)
            print("")

    if directory.endswith("KO"):
        if directory.endswith("Syntaxe/KO"):
            for err in bufferErreur:
                if b"syntax" not in err:
                    flagSyn = True
                    errTemp = err
        if cpt == 0:
            if directory.endswith("Gencode/KO"):
                print("Il n'y a pas eu d'erreur dans le repertoire "+directory+" lors de la compilation, il faut vérifier sur Mars la sortie des fichiers assembleurs\n"+Style.RESET_ALL)
            else:
                print(Back.RED+"Il n'y a pas eu d'erreur dans le repertoire "+directory+", ce qui n'est pas normal je crois ...\n"+Style.RESET_ALL)
        elif cpt < len(os.listdir(directory)):
            print(Back.RED+"Il y a eu moins d'erreurs que prévu dans le repertoire "+directory+Style.RESET_ALL)
            print("")
        else:
            if flagSyn:
                print(Back.RED+"Il n'y a pas eu que des erreurs de syntaxe lors des tests dans "+directory+Style.RESET_ALL)
                print(str(errTemp).split("'")[1])
                print("")
            else:
                print(Back.GREEN+"Les tests ont été correctement passés dans le repertoire "+directory+Style.RESET_ALL)
                print("")

def removeGarbage():
    if "apres_passe_1.dot" in os.listdir(os.curdir):
        os.remove("apres_passe_1.dot")
    if "apres_syntaxe.dot" in os.listdir(os.curdir):
        os.remove("apres_syntaxe.dot")
    if "out.s" in os.listdir(os.curdir):
        os.remove("out.s")

if __name__ == '__main__':

    createTestPathsList()
    print("***  Programme d'automatisation des tests écrits pour le projet compilation  ***")
    print("***              Programme écrit par Thomas Korpal & Hugo Vouaux             ***\n")
    print("Le programme va passer dans tous les repertoires de test et executer chaque fichier")
    print("Un compte rendu des tests sera affiché à la fin des tests.\n")
    print("Debut des tests ...\n")
    for path in list_dir_to_test:
        TestInDirectory(path)
    removeGarbage()
