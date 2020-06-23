import sys
import oqs
sys.path.insert(1,'/home/pi/PQQUIC/modLiboqs/liboqs/liboqs-python/oqs')
import test

print ("TEST PRINTING OQS KEYS\n")
print (test.pqPublic())
print (test.pqPrivate())
