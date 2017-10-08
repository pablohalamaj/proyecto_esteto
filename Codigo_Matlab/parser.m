% Autor: Pablo Halamaj
% Fecha: 2017-10-08
% Funcion: Tomar una matrix de valores de entrada procesarla con un codigo a eleccion y graficar la salida
% Version: 0.1-alpha
% Input: Archivo de texto input.txt con valores de señal de entrada en el tiempo en formato <a definir>
% Output: Archivo de texto output.txt con valores de señal de salida en el tiempo en formato <a definir>

% Limpio el workspace
clear;

% - Incio Variables

% IN y out de mi señal
latido_in_t = dlmread('input.txt'); 	% aqui guardo los valores de mi señal de entrada
latido_in_f = ''; 			% aqui guardo el analisis en F de la señal
latido_out_t = ''; 			% aqui guardo los valores de mi señal de salida
latido_out_f = ''; 			% aqui guardo el analisis en F de la señal

% variables de FFT
Fs = 100;				% Sampling frequency -> 1000 Muestras por Segundo
T = 2/Fs;				% Sample time -> 2 segundos
L = 0;					% Largo de la Señal -> se define mas abajo
t = 0;					% Vector de tiempo -> se define mas abajo


% - Fin Variables


% - Inicio Importar entrada
% - INPUT: Recibe los valores de una señal temporal en texto, muestra su espectro
% - OUTPUT: ploteo en t y f de la entrada

latido_in_t = dlmread('input.txt');
L = length(latido_in_t);
t = (0:L-1)*T;

% Ploteo la señal en el Tiempo
figure(1);
subplot(2,1,1);
LIN_t =plot(t,latido_in_t,'-r');
set(LIN_t,'LineWidth',3); 
title('Señal temporal de entrada');
xlabel('tiempo en milisegundos');
hold on;

% le hago la FFT y muestro sus componentes en F
NFFT = 2^nextpow2(L); % Next power of 2 from length of y
latido_in_f = fft(latido_in_t,NFFT)/L;
f = Fs/2*linspace(0,1,NFFT/2+1);

% Plot single-sided amplitude spectrum.
subplot(2,1,2);
LIN_F = plot(f,2*abs(latido_in_f(1:NFFT/2+1)),'-r');
set(LIN_F,'LineWidth',3); 
title('Espectro de Amplitud solo f positivas de latido_in_t');
xlabel('Frecuencia (Hz)');
ylabel('|latido_in_f|');
hold off;





% Aqui va el codigo de Filtrado!!!
% el valor de entrada es latido_in_t
% no usarlo directamente copiarlo a alguna variable temporal
% por ejemplo: mi_var = latido_in_t;
% porque al final vamos a plotear latido_in y latido_out para compararlos

mi_var = latido_in_t;

% no hago nada!
mi_var_filtrada = mi_var -  sin(2*pi*1.5*t) ;


latido_out_t = mi_var_filtrada;
% Aqui termina el codigo sacar tu señal filtrada en latido_out_t
% haciendo por ejemplo: latido_out_t = mi_var_filtrada;

% - Inicio el ploteo de la salida y la comparación con la entrada
% - INPUT: señal en tiempo filtrada dentro de la variable latido_out_t
% - OUTPUT: ploteo en t y f de la entrada y la salida para compararlas.
% Ploteo la señal en el Tiempo
figure(2);
subplot(2,1,1);
LON_t =plot(t,latido_out_t);
set(LON_t,'LineWidth',3);
title('Señal temporal de salida');
xlabel('tiempo en milisegundos');
hold on;

% le hago la FFT y muestro sus componentes en F
NFFT = 2^nextpow2(L); % Next power of 2 from length of y
latido_out_f = fft(latido_out_t,NFFT)/L;
f = Fs/2*linspace(0,1,NFFT/2+1);

% Plot single-sided amplitude spectrum.
subplot(2,1,2);
LON_F = plot(f,2*abs(latido_out_f(1:NFFT/2+1)));
set(LON_F,'LineWidth',3);
title('Espectro de Amplitud solo f positivas de latido_out_t');
xlabel('Frecuencia (Hz)');
ylabel('|latido_in_f|');
hold off;

%
% ahora grafico entrada y salida juntos!
%
figure(3);
subplot(2,1,1);
LON_t =plot(t,latido_out_t,'--b');
set(LON_t,'LineWidth',3);
title('Señal temporal de salida');
xlabel('tiempo en milisegundos');
hold on;
LIN_t =plot(t,latido_in_t,'-r');
set(LIN_t,'LineWidth',2);
legend('Señal en t de Salida','Señal en t de Entrada');
hold on;


% le hago la FFT y muestro sus componentes en F
NFFT = 2^nextpow2(L); % Next power of 2 from length of y
latido_out_f = fft(latido_out_t,NFFT)/L;
f = Fs/2*linspace(0,1,NFFT/2+1);

% Plot single-sided amplitude spectrum.
subplot(2,1,2);
LON_F = plot(f,2*abs(latido_out_f(1:NFFT/2+1)),'--b');
set(LON_F,'LineWidth',3);
title('Espectro de Amplitud solo f positivas de latido_out_t');
xlabel('Frecuencia (Hz)');
ylabel('|latido_in_f|');
hold on;
LIN_F = plot(f,2*abs(latido_in_f(1:NFFT/2+1)),'-r');
set(LIN_F,'LineWidth',2);
legend('Espectro Salida','Espectro Entrada');
hold off;

