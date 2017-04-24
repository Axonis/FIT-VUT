fibb(0,0). % ak je 0 vrat 0
fibb(1,1). % ak je 1 vrat 1 
fibb(X,Y) :- X>1, X2 is X-2, fibb(X2,Y2), X1 is X-1, fibb(X1,Y1), Y is Y1+Y2. % rekurzivne scitaj x-2 a x-1 a vrat ich do Y

max([],0). % ak je list prazdny vrat 0
max(L,L). % ak ma 1 prvok vrat ho ako max
max([K1,K2|M], L) :- K1 >= K2, max([K1|M], L). % ak je prve porovnavane cislo vacsie ako druhe vrat prve a pokracuj s tailom
max([K1,K2|M], L) :- K1 < K2, max([K2|M], L). % ak je druhe porovnavane cislo mensie vrat prve a pokracuj s tailom
