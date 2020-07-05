#!/usr/bin/env python
# coding: utf-8

# In[ ]:



import matplotlib.pyplot as plt
import math
import os
import pandas as pd
#Aquí importamos las librerías

__author__ = "EdwinSB"



def Menu():

    print("                       ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»")
    print("                       º                      :: TCU-230 ::                                    º")
    print("                       ÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹")
    print("                       º                                                                       º")
    print("                       º  1.- InsertarID        2.-Nuevo ID       3.- Borr. Pant.              º")
    print("                       º                                                                       º")
    print("                       º  5.- Menú              6.- Version       7.- SALIR                    º")
    print("                       º                                                                       º")
    print("                       º                                                                       º")
    print("                       ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼")
    print("")

def mcd(num1, num2):
    a = max(num1, num2)
    b = min(num1, num2)
    while b!=0:
        mcd = b
        b = a%b
        a = mcd
    return mcd

def mcm(num1, num2):
    a = max(num1, num2)
    b = min(num1, num2)
    mcm = (a / mcd(a, b)) * b
    return mcm

def borrarPantalla():
    if os.name == "posix":
        os.system ("clear")
    elif os.name == "ce" or os.name == "nt" or os.name == "dos":
        os.system ("cls")

def Calculadora():

    Menu()

    opc = int(input("Selecione nº de Opción: "))

    while (opc >0 and opc <8):

        
        if(opc==1):
            print("")
            print("Insertar ID Productor:")
            ID = input()
            print(f"Buscando archivos de: {ID}")
            extension = '.csv'
            excel = ID + extension
            print(excel)
            
            # Se crea la variable datos que almacena los datos contenidos en el archivo lote.csv
            datos = pd.read_csv(excel)
            df=pd.DataFrame(datos)
            df.groupby('humedad')['humedad'].plot(kind='bar')
            plt.title('Productor ID :')
            plt.xlabel('Hora')
            plt.ylabel('Humedad')
            fig_temp = ID + 'humedad'
            plt.savefig(fig_temp)

            Continuar = input("¿Desea Continuar con esta Opción?(S/N): ")

            if Continuar == "s" or Continuar == "S":
                opc = 1

            elif Continuar == "n" or Continuar == "N":
                print(" ")
                opc = int(input("Selecione nº de Opción: "))

            else:
                print("")
                Continuar = input("Por Favor escoja(S/N): ")

                if Continuar == "s" or Continuar == "S":
                    opc = 1

                elif Continuar == "n" or Continuar == "N":
                    print("")
                    opc = int(input("Selecione nº de Opción: "))


        elif(opc==2):
            print("")
            num1 = int(input("Insertar ID Productor: "))
            print("")

            Continuar = input("¿Desea Continuar con esta Opción?(S/N): ")

            if Continuar == "s" or Continuar == "S":
                opc = 2

            elif Continuar == "n" or Continuar == "N":
                print("")
                opc = int(input("Selecione nº de Opción: "))

            else:
                print("")
                Continuar = input("Por Favor escoja(S/N): ")

                if Continuar == "s" or Continuar == "S":
                    opc = 2

                elif Continuar == "n" or Continuar == "N":
                    print("")
                    opc = int(input("Selecione nº de Opción: "))

        
        elif(opc==6):
            print("")
            print("    ========================     Autores   =============================")
            print("    |                                                                  |")
            print("    |   Universidad de Costa Rica                                      |")
            print("    |   Sede Rodrigo Facio                                             |")
            print("    |                                                                  |")
            print("    | Profesores a cargo:                                              |")
            print("    |                                                                  |")
            print("    |                                                                  |")
            print("    | Estudiante: Edwin Somarribas Barahona                            |")
            print("    | Carné: B16453                                                    |")
            print("    |                                                                  |")
            print("    |                                                                  |")
            print("    |                                                                  |")
            print("    | Autor: Edwin Somarribas Barahona                                 |")
            print("    | Correo: edwinsb26@gmail.com                                      |")
            print("    | github: https://github.com/Edwin-SB/TCU_Secador_Cacao            |")                         
            print("    ====================================================================")
           
            opc = int(input("Selecione nº de Opción: "))

        elif(opc==3):
            borrarPantalla()
            Menu()
            opc = int(input("Selecione nº de Opción: "))

        elif(opc==5):
            Menu()
            opc = int(input("Selecione nº de Opción: "))

        elif(opc==7):
            exit(0)

    while(opc <1 or opc >6 ):
        print("")
        print("Opción no Encontrada")
        print("")
        opc = int(input("Selecione nº de Opción: "))

Calculadora()


# In[ ]:




