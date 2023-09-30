import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
#import pandas as pd
import matplotlib.mlab as ml
from mpl_toolkits import mplot3d
from matplotlib import cm
from matplotlib.ticker import LinearLocator

BA = np.loadtxt('total2.txt')
#BA = np.loadtxt('totDose3.txt')
X=1*np.array(BA[:,0])
Y=1*np.array(BA[:,1])
Z=1*np.array(BA[:,2])
Dose=1*np.array(BA[:,3])
Dose_reverted = Dose[::-1]
Dose2=Dose+Dose_reverted
#Dose=Dose2
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
ax.set_xlabel(' X')
ax.set_ylabel(' Y')
ax.set_zlabel('Z')
DNew=[]
XNew=[]
YNew=[]
ZNew=[]
DZNew=[]
Y2New=[]
Z2New=[]
D2New=[]

for i in range(len(Z)):
    valor=Z[i]
    # Aqui decidimos en que plano de profundidad de z graficaremos la dosis
    if valor==1:
        DNew.append(Dose[i])
        XNew.append(X[i])
        YNew.append(Y[i])
    valor2=X[i]
    # Aqui decidimos en que plano de profundidad de z graficaremos la dosis
    if valor2==15:
        D2New.append(Dose[i])
        Z2New.append(X[i])
        Y2New.append(Y[i])


for i in range(len(Z)):
    valor1=X[i]
    valor2=Y[i]
    if valor1==15 and valor2==15:
        DZNew.append(1e6*Dose[i])
        ZNew.append(Z[i])
        print(Dose[i])
        
	
print(len(DNew))
# Crea una malla de valores para X y Y


npts = 200
fig2 = plt.figure()
ax = fig2.add_subplot()
NCELL=50

plt.xlabel('X (m)')
plt.ylabel('Y (m)')
#fig.colorbar(surf)
##Aqui definimos los valores espaciales de X,Y del plano de boxeles para graficarlos
X1, Y1 = np.meshgrid(np.linspace(-0.15, 0.15, NCELL), np.linspace(-0.15, 0.15, NCELL))
##Despues de crear el arreglo Dnew este tiene la forma de una columna, tenemos que decirle a python 
#que su significado es el de un arreglo bidimensional de 30X30 para eso usamos reshape
arrZ = np.array(DNew)
ZE=arrZ.reshape(NCELL, NCELL)
arrZ2 = np.array(D2New)
ZE2=arrZ2.reshape(NCELL, NCELL)
#Ahora lo graficamos 
plt.title("Perfil a profundidad deseada")
plt.pcolormesh(X1,Y1,ZE)
cbar = plt.colorbar()


figt, ax = plt.subplots(subplot_kw={"projection": "3d"})
# Plot the surface.
surf = ax.plot_surface(X1, Y1, ZE2, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

# Customize the z axis.
#ax.set_zlim(-1.01, 1.01)
#ax.zaxis.set_major_locator(LinearLocator(10))
# A StrMethodFormatter is used automatically
#ax.zaxis.set_major_formatter('{x:.02f}')

# Add a color bar which maps values to colors.
figt.colorbar(surf, shrink=0.5, aspect=5)






fig3x = plt.figure()
plt.xlabel('Z (m)')
plt.ylabel('Y (m)')

plt.title("Perfil a profundidad deseada")
plt.pcolormesh(X1,Y1,ZE2)
cbar = plt.colorbar()

#ax.imshow(ZE)
fig3 = plt.figure()
# plot data points.
plt.title("Una linea de como la dosis evoluciona con la profundidad")
plt.scatter(ZNew,DZNew,marker='o', s=5, zorder=10)

plt.show()









BB = np.loadtxt('GammaDistribution.txt')
#BA = np.loadtxt('totDose3.txt')
PX=1*np.array(BB[:,0])
PY=1*np.array(BB[:,1])
PZ=1*np.array(BB[:,2])

#BB2 = np.loadtxt('buenos/GammaDistribution.txt')
#BA = np.loadtxt('totDose3.txt')
#PX2=1*np.array(BB2[:,0])
#PY2=1*np.array(BB2[:,1])
#PZ2=1*np.array(BB2[:,2])

# Creating histogram
fig, axs = plt.subplots(1, 1,
                        figsize =(10, 7),
                        tight_layout = True)
n_bins=500
axs.hist(PZ, bins = n_bins,alpha=.5, color='red',density=True)
#axs.hist(PZ2, bins = n_bins,alpha=.5, color='green',density=True)
PT=np.sqrt(PX*PX+PY*PY)
#axs.hist(PT, bins = n_bins,alpha=.7, color='yellow')


fig2, axs2 = plt.subplots(1, 1,
                        figsize =(10, 7),
                        tight_layout = True)
n_bins=300
axs2.hist(PX, bins = n_bins,alpha=.7, color='red')
axs2.hist(PY, bins = n_bins,alpha=.7, color='yellow')
plt.show()

