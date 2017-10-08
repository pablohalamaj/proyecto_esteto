%% Autor: Pablo Halamaj
%% Fecha: 2017-10-08
%% Funcion: Tomar una matrix de valores de entrada procesarla con un codigo a eleccion y graficar la salida
%% Version: 0.1-alpha
%% Input: Archivo de texto input.txt con valores de señal de entrada en el tiempo en formato <a definir>
%% Output: Archivo de texto output.txt con valores de señal de salida en el tiempo en formato <a definir>


%% - Incio Variables

% IN y out de mi señal
latido_in_t = dlmread('input.txt'); 	% aqui guardo los valores de mi señal de entrada
latido_in_f = ''; 			% aqui guardo el analisis en F de la señal
latido_out_t = ''; 			% aqui guardo los valores de mi señal de salida
latido_out_f = ''; 			% aqui guardo el analisis en F de la señal

% variables de FFT
Fs = 10000;				% Sampling frequency
T = 1/Fs;				% Sample time
L = 0;					% Length of signal
t = 0;					% Time vector


%% - Fin Variables


%% - Inicio Importar entrada
%% - INPUT: Recibe los valores de una señal temporal en texto, muestra su espectro
%% - OUTPUT: ploteo en t y f de la entrada

latido_in_t = dlmread("input.txt");
L = length(latido_in_t);
t = (0:L-1)*T;

% Ploteo la señal en el Tiempo
plot(t,latido_in_t)
title('Señal temporal de entrada')
xlabel('tiempo en xxxx')


% le hago la FFT y muestro sus componentes en F






%Aqui va el codigo de Filtrado!!!
% el valor de entrada es latido_in_t
% no usarlo directamente copiarlo a alguna variable temporal
% por ejemplo: mi_var = latido_in_t;
% porque al final vamos a plotear latido_in y latido_out para compararlos




% Aqui termina el codigo sacar tu señal filtrada en latido_out_t
% haciendo por ejemplo: latido_out_t = mi_var_filtrada;

%% - Inicio el ploteo de la salida y la comparación con la entrada
%% - INPUT: señal en tiempo filtrada dentro de la variable latido_out_t
%% - OUTPUT: ploteo en t y f de la entrada y la salida para compararlas.
