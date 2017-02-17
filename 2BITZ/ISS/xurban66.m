%1
[x,Fs] = audioread('/homes/eva/xu/xurban66/2BIT/ISS/xurban66.wav');
seconds = length(x) / Fs;

%2
X = fft(x);
Xshow = abs(X(1:(Fs/2+1)));
k=0:Fs/2;
f = k /Fs * Fs;
plot(f,Xshow);
ylabel('Amplitúda');
xlabel('Frekvencia [Hz]'); 

%3
maximum = find(max(Xshow) == Xshow);
max_freq = f(maximum);

%4
b = [0.2324 -0.4112 0.2324];
a = [1 0.2289 0.4662];

zplane (b,a); 
p = roots(a); 
if (isempty(p) | abs(p) < 1) 
  disp('stabilný')
else
  disp('nestabilný')
end

%5
H = freqz(b,a,Fs); 
f=(0:Fs-1) / Fs * Fs / 2; 
plot (f,abs(H)); grid; xlabel('Frekvencia [Hz]'); ylabel('|H(f)|')
%horna priepust

%6
fx = filter(b,a,x);
fY = fft(fx);
Yshow = abs(fY(1:(Fs/2+1)));
k=0:Fs/2;
f = k /Fs * Fs;
plot(f,Yshow);
ylabel('Amplitúda');
xlabel('Frekvencia [Hz]'); 

%7
maximumfx = find(max(Yshow) == Yshow);
max_freq_fx = f(maximumfx);


%8
s = [1 1 1 1 -1 -1 -1 -1];
s = repmat(s,1,40);
[corsig,lag] = xcorr(x,s);
[~,I] = max(abs(corsig));
lagDiff = lag(I);
timeDiff = lagDiff/Fs;

%9
Rv = xcorr(x,'biased'); k = -50:50;
tmp = Rv(Fs-50:Fs+50);
plot(k,tmp);

%10
Rv10 = Rv(Fs+10);

%11
xmin = min(min(x)); xmax = max(max(x));
kolik = 100;
fnc = linspace(xmin,xmax,kolik);
[h,p,r] = hist2opt(x(1:length(x)-10),x(11:length(x)), fnc);
imagesc(fnc,fnc,p); axis xy; colorbar;

%12
%overene v hist2opt

%13
display(r);
display(Rv10);