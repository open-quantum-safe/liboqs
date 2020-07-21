#g++ -o Signatures.o Signatures.cpp ../build/lib/liboqs.a
g++ -o Signatures.o Signatures.cpp -L../build/lib -loqs
./Signatures.o