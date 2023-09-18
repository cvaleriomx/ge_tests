# diferent ways to load fields
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
#import pandas as pd
import matplotlib.mlab as ml
from mpl_toolkits import mplot3d


BA = np.loadtxt('totDose1.txt')
X=1*np.array(BA[:,0])
Y=1*np.array(BA[:,1])
Z=1*np.array(BA[:,2])
Dose=1*np.array(BA[:,3])

# Crea una figura en 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Normaliza los valores T para mapearlos a colores
norm = plt.Normalize(Dose.min(), Dose.max())
# Grafica los puntos en 3D con colores basados en T
scatter = ax.scatter(X, Y, Z, c=Dose, norm=norm)

# Agrega una barra de color
cbar = fig.colorbar(scatter, ax=ax, label='T')

# Etiqueta los ejes
ax.set_xlabel('Eje X')
ax.set_ylabel('Eje Y')
ax.set_zlabel('Eje Z')
DNew=[]
XNew=[]
YNew=[]
ZNew=[]
DZNew=[]

for i in range(len(Z)):
    valor=Z[i]
    # Haces algo con cada valor de x, por ejemplo, imprimirlo
    if valor==2:
        DNew.append(Dose[i])
        XNew.append(X[i])
        YNew.append(Y[i])
for i in range(len(Z)):
    valor1=X[i]
    valor2=Y[i]
    if valor1==14 and valor2==14:
        DZNew.append(1e6*Dose[i])
        ZNew.append(Z[i])
        print(Dose[i])
        
	
print(len(DNew))
# Crea una malla de valores para X y Y
npts = 200
fig2 = plt.figure()
ax = fig2.add_subplot()

# Crear el surface plot
#surf = ax.plot_trisurf(XNew, YNew, DNew, cmap='viridis')
#surf = ax.plot_surface(XNew, YNew, DNew, cmap='viridis')

# Añadir barra de color
#fig.colorbar(surf)
#X, Y = np.meshgrid(np.linspace(0, 30, 30), np.linspace(0, 30, 30))
arrZ = np.array(DNew)
ZE=arrZ.reshape(30, 30)
ax.imshow(ZE)
fig3 = plt.figure()
# plot data points.
plt.scatter(ZNew,DZNew,marker='o', s=5, zorder=10)

plt.show()
