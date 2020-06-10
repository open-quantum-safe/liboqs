import os
import time 

myCmd = 'apps/openssl s_client -CAfile pycacert.pem -connect 10.0.0.155:44330'
sTime = time.time()
os.system(myCmd)
eTime = time.time()
print ("Time taken")
print (eTime-sTime)