# Reference implementation for qTESLA-I

To compile:

make

This generates three executables "test\_qtesla", "PQCtestKAT\_sign" and "PQCgenKAT\_sign".

To get cycle counts for key generation, signing and verification, execute:

./test\_qtesla

To test against known answer values in the KAT folder, execute:

./PQCtestKAT\_sign

To generate new KAT files, execute:

./PQCgenKAT\_sign



