%%% test drive -- Aca me documento como usar las funciones
%% Asi se escribe una matriz a un archivo delimitado por tabs
M = sin(x);
dlmwrite('output.txt',M,'delimiter','\t','precision',3);

%% Asi se carga una archivo de puntos en una variable
M = dlmread("input.txt");


%%% como hacer una fft
Fs = 1000;                    % Sampling frequency
T = 1/Fs;                     % Sample time
L = 10000;                     % Length of signal
t = (0:L-1)*T;                % Time vector
% Sum of a 50 Hz sinusoid and a 120 Hz sinusoid
x = 0.7*sin(2*pi*50*t) + sin(2*pi*120*t);
y = x + 2*randn(size(t));     % Sinusoids plus noise
plot(Fs*t(1:50),y(1:50))
title('Signal Corrupted with Zero-Mean Random Noise')
xlabel('time (milliseconds)')

%%% It is difficult to identify the frequency components by looking at the original signal. Converting to the frequency domain, the discrete Fourier transform of the noisy signal y is found by taking the fast Fourier transform (FFT):
NFFT = 2^nextpow2(L); % Next power of 2 from length of y
Y = fft(y,NFFT)/L;
f = Fs/2*linspace(0,1,NFFT/2+1);

% Plot single-sided amplitude spectrum.
plot(f,2*abs(Y(1:NFFT/2+1)))
title('Single-Sided Amplitude Spectrum of y(t)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')

% Reproducir un audio a 16 Bits
load handel.mat;
nBits = 16;
sound(y,Fs,nBits);
