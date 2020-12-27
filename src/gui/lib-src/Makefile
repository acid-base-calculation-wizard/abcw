all: pHCalcFunc.o pHCalc.o
	gcc -o pHCalcFunc.dll pHCalcFunc.o pHCalc.o -shared -Wl,--out-implib

pHCalcFunc.o: pHCalcFunc.c pHCalcFunc.h
	gcc -o pHCalcFunc.o -c pHCalcFunc.c -fPIC 

pHCalc.o: pHCalc.c pHCalc.c
	gcc -o pHCalc.o -c pHCalc.c -fPIC

clean:
	rm *.o abcw
