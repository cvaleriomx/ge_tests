import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
#import pandas as pd
import matplotlib.mlab as ml
from mpl_toolkits import mplot3d
X=0
Y=0
Z=0
Doset=0
for i in range(0,83):
	archivo_nombre = "archivo_" + str(i) + ".txt"
	print("Leyendo archivo ", archivo_nombre)
	BA = np.loadtxt(archivo_nombre)
	#BA = np.loadtxt('totDose3.txt')
	X=1*np.array(BA[:,0])
	Y=1*np.array(BA[:,1])
	Z=1*np.array(BA[:,2])
	Dose=1*np.array(BA[:,3])
	if(i==0):
		Doset=Dose
	else:
		Doset=Doset+Dose
		print("La suma es ", archivo_nombre)


with open('total2.txt', 'w') as f:
        for i in range(len(X)):
            # Escribe cada elemento de los arreglos como una columna en el archivo

		f.write(str(X[i]) + "\t" + str(Y[i]) + "\t" + str(Z[i]) + "\t" + str(Doset[i]) + "\n")
	
