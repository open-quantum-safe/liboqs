import sys
import oqs
sys.path.insert(1,'/home/pi/PQQUIC/modLiboqs/liboqs/liboqs-python/oqs')
import test

print ("TEST PRINTING OQS KEYS\n")
t = test.Test()
t.gen_dili2_private_key("DILITHIUM_2")


print (t.private)
print (t.public)