import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
#import pandas as pd
import matplotlib.mlab as ml
from mpl_toolkits import mplot3d
from matplotlib import cm
from matplotlib.ticker import LinearLocator

BA = np.loadtxt('archivo_0.txt')
X=1*np.array(BA[:,0])
DoseT = np.zeros(len(X))
    

X=[]
Y=[]
Z=[]


for var in range(10):
    nombre="archivo_"+str(var)+".txt"
    BA = np.loadtxt(nombre)
    X=1*np.array(BA[:,0])
    Y=1*np.array(BA[:,1])
    Z=1*np.array(BA[:,2])
    Dose=1*np.array(BA[:,3])
    DoseT=DoseT+Dose

archivo_final = 'archivo_final.txt'  # Nombre del archivo final
resultados = np.column_stack((BA[:, :3], DoseT))
np.savetxt(archivo_final, resultados, fmt='%s', delimiter=' ')

print(f"Archivo final '{archivo_final}' creado con éxito.")