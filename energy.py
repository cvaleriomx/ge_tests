import matplotlib.pyplot as plt

# Leer el archivo de texto
with open('datos.txt', 'r') as file:
    lines = file.readlines()

# Crear una lista para almacenar los valores de la última columna
columna7 = []

# Iterar sobre las líneas y extraer los valores de la última columna
for line in lines[1:]:  # Empezamos desde la segunda línea para evitar la cabecera
    columns = line.split()
    if len(columns) >= 7:
        valor = float(columns[6])  # La última columna es la columna 7
        columna7.append(valor)

# Crear un histograma
plt.hist(columna7, bins=100, color='blue', edgecolor='black', alpha=0.7, density=True)
plt.xlabel('Energia')
plt.ylabel('Frecuencia')
plt.title('Histograma de Energia')
plt.show()

