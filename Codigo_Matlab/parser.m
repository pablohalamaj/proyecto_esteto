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
latido_in_t = '';		 	% aqui guardo los valores de mi señal de entrada
latido_in_f = ''; 			% aqui guardo el analisis en F de la señal
latido_out_t = ''; 			% aqui guardo los valores de mi señal de salida
latido_out_f = ''; 			% aqui guardo el analisis en F de la señal


% variables de FFT
Fs = 22500;				% Sampling frequency -> Cantidad de Muestras por Segundo que tomé
t_total = 0.08;				% Duración del input en Segundos -> Cuanta señal grabé
					%
					% Fs y t_total son 2 parametros Muy importantes, afectan el resultado final!!!
					%
T = t_total/Fs;				% Sample time -> Cada cuanto tiempo saque una muestra
L = 0;					% Largo de la Señal -> se define mas abajo
t = 0;					% Vector de tiempo -> se define mas abajo


% - Fin Variables


% - Inicio Importar entrada
% - INPUT: Recibe los valores de una señal temporal en texto, muestra su espectro
% - OUTPUT: ploteo en t y f de la entrada

% guardo mis latidos en una variable temporal
var_temporal = dlmread('input.txt');	% Cambiar input.txt por el archivo donde tengo mis muestras
					% las muestras deben ir todas en una misma linea separadas por un espacio

% Le resto el valor medio asi le saco la continua :D 
latido_in_t = var_temporal - mean(var_temporal);

L = length(latido_in_t);		% Cantidad total de muestras que tengo!
t = (0:L-1)*T;				% Mi vector tiempo para los graficos
%t  = (0:L-1);

% Ploteo la señal en el Tiempo
figure(1);
subplot(2,1,1);
LIN_t =plot(t,latido_in_t,'-r');	% ploteo las muestras en funcion del tiempo
set(LIN_t,'LineWidth',3); 
title('Señal temporal de entrada');
xlabel('tiempo en segundos');
hold on;

% le hago la FFT y muestro sus componentes en F, ejemplo tomado del Matlab
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


% Para validar la entrada sin ejecutar el resto del codigo descomentar siguiente linea
% return

% Si la linea anterior esta descomentada de acá en mas no se ejecuta nada

%
% SECCION DE FILTRADO %
%

% Aqui va el codigo de Filtrado!!!
% el valor de entrada es latido_in_t
% no usarlo directamente copiarlo a alguna variable temporal
% por ejemplo: mi_var = latido_in_t;
% porque al final vamos a plotear latido_in y latido_out para compararlos

mi_var = latido_in_t;

% mi filtro no hace nada!!
mi_var_filtrada = mi_var - 0 ;

% mi filtro resta una señal de 500 hz!!
%x = 300*sin(500*pi*1*t);
%mi_var_filtrada = mi_var - x ;


% Aqui termina el codigo, saca tu señal filtrada en latido_out_t
% haciendo por ejemplo: latido_out_t = mi_var_filtrada!
latido_out_t = mi_var_filtrada;

%
% FIN SECCION DE FILTRADO
%

% - Inicio el ploteo de la salida y la comparación con la entrada
% - INPUT: señal en tiempo filtrada dentro de la variable latido_out_t
% - OUTPUT: ploteo en t y f de la entrada y la salida para compararlas.
% Ploteo la señal en el Tiempo

% Guardo la señal filtrada en el archivo de salida
dlmwrite('output.txt',latido_out_t,'delimiter','\t','precision',3);


figure(2);
subplot(2,1,1);
LON_t =plot(t,latido_out_t);
set(LON_t,'LineWidth',3);
title('Señal temporal de salida');
xlabel('tiempo en segundos');
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
xlabel('tiempo en segundos');
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

