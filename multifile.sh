#!/bin/bash

for i in 0 1 2 3 4 5 6 7 8 9 10
do
  ./ml2 batch.mac
  
  # Define el nombre archivo utilizando una variable
  archivo="archivo_${i}.txt"
  cp totDose.txt "$archivo"
  # Utiliza el comando 'touch' para crear el archivo vacío
  #touch "$archivo"
  rm totDose.txt
  # Puedes agregar contenido a los archivos si lo deseas
  # Por ejemplo, para agregar una línea de texto:
  # echo "Contenido del archivo $i" > "$archivo"
done
