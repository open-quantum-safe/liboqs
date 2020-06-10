import os
import time 
from memory_profiler import memory_usage
import cProfile 
from time import sleep

def main():
    print ("choose cipher")
    print ("rsa, dilithium2, dilithium3, dilithium4, picnicl1fs, qteslapi, qteslapiii")
    count = 0

    g = input("Enter choice of cipher: ")

    if g == "rsa":
        myCmd='apps/openssl req -x509 -new -newkey rsa:3072 -keyout rsa_CA.key -out pycacert.pem -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf'
        print("Generating a rsa CA key and pycacert")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate CA key")
        print((end_time-start_time)/100)

        myCmd='apps/openssl genpkey -algorithm rsa:3072 -out ssl_key.pem'
        print("generating key.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time taken to generate key")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl req -new -newkey rsa -keyout key.pem -out rsa_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf'
        print("genetating cert signing request rsa_svr.csr")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate certificate signing request")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl x509 -req -in rsa_srv.csr -out ssl_cert.pem -CA pycacert.pem -CAkey rsa_CA.key -CAcreateserial -days 365'
        print("generating cert.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count +1 
        count = 100
        end_time = time.time()
        print ("Time taken to genetate signed certificate")
        print ((end_time-start_time)/100)

    if g == "dilithium2":
        myCmd='apps/openssl req -x509 -new -newkey dilithium2 -keyout rsa_CA.key -out pycacert.pem -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf'
        print("Generating a dilithium2 CA key and pycacert")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate CA key")
        print((end_time-start_time)/100)

        myCmd='apps/openssl genpkey -algorithm dilithium2 -out key.pem'
        print("generating key.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time taken to generate key")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl req -new -newkey dilithium2 -keyout key.pem -out rsa_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf'
        print("genetating cert signing request rsa_svr.csr")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate certificate signing request")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl x509 -req -in rsa_srv.csr -out cert.pem -CA pycacert.pem -CAkey rsa_CA.key -CAcreateserial -days 365'
        print("generating cert.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count +1 
        count = 100
        end_time = time.time()
        print ("Time taken to genetate signed certificate")
        print ((end_time-start_time)/100)


    if g == "dilithium3":
        
        myCmd='apps/openssl req -x509 -new -newkey dilithium3 -keyout rsa_CA.key -out pycacert.pem -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf'
        print("Generating a dilithium3 CA key and pycacert")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate CA key")
        print((end_time-start_time)/100)

        myCmd='apps/openssl genpkey -algorithm dilithium3 -out key.pem'
        print("generating key.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time taken to generate key")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl req -new -newkey dilithium3 -keyout key.pem -out rsa_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf'
        print("genetating cert signing request rsa_svr.csr")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate certificate signing request")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl x509 -req -in rsa_srv.csr -out cert.pem -CA pycacert.pem -CAkey rsa_CA.key -CAcreateserial -days 365'
        print("generating cert.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count +1 
        count = 100
        end_time = time.time()
        print ("Time taken to genetate signed certificate")
        print ((end_time-start_time)/100)

    if g =="dilithium4":
        
        myCmd='apps/openssl req -x509 -new -newkey dilithium4 -keyout rsa_CA.key -out pycacert.pem -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf'
        print("Generating a dilithium4 CA key and pycacert")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate CA key")
        print((end_time-start_time)/100)

        myCmd='apps/openssl genpkey -algorithm dilithium4 -out key.pem'
        print("generating key.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time taken to generate key")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl req -new -newkey dilithium4 -keyout key.pem -out rsa_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf'
        print("genetating cert signing request rsa_svr.csr")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate certificate signing request")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl x509 -req -in rsa_srv.csr -out cert.pem -CA pycacert.pem -CAkey rsa_CA.key -CAcreateserial -days 365'
        print("generating cert.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count +1 
        count = 100
        end_time = time.time()
        print ("Time taken to genetate signed certificate")
        print ((end_time-start_time)/100)

    if g == "picnicl1fs":
        
        myCmd='apps/openssl req -x509 -new -newkey picnicl1fs -keyout rsa_CA.key -out pycacert.pem -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf'
        print("Generating a picnicl1fs CA key and pycacert")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate CA key")
        print((end_time-start_time)/100)

        myCmd='apps/openssl genpkey -algorithm picnicl1fs -out key.pem'
        print("generating key.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time taken to generate key")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl req -new -newkey picnicl1fs -keyout key.pem -out rsa_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf'
        print("genetating cert signing request rsa_svr.csr")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate certificate signing request")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl x509 -req -in rsa_srv.csr -out cert.pem -CA pycacert.pem -CAkey rsa_CA.key -CAcreateserial -days 365'
        print("generating cert.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count +1 
        count = 100
        end_time = time.time()
        print ("Time taken to genetate signed certificate")
        print ((end_time-start_time)/100)

    if g == "qteslapi":
        
        myCmd='apps/openssl req -x509 -new -newkey qteslapi -keyout rsa_CA.key -out pycacert.pem -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf'
        print("Generating a qteslapi CA key and pycacert")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate CA key")
        print((end_time-start_time)/100)

        myCmd='apps/openssl genpkey -algorithm qteslapi -out key.pem'
        print("generating key.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time taken to generate key")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl req -new -newkey qteslapi -keyout key.pem -out rsa_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf'
        print("genetating cert signing request rsa_svr.csr")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate certificate signing request")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl x509 -req -in rsa_srv.csr -out cert.pem -CA pycacert.pem -CAkey rsa_CA.key -CAcreateserial -days 365'
        print("generating cert.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count +1 
        count = 100
        end_time = time.time()
        print ("Time taken to genetate signed certificate")
        print ((end_time-start_time)/100)

    if g == "qteslapiii":
        
        myCmd='apps/openssl req -x509 -new -newkey qteslapiii -keyout rsa_CA.key -out pycacert.pem -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf'
        print("Generating a qteslapiii CA key and pycacert")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate CA key")
        print((end_time-start_time)/100)

        myCmd='apps/openssl genpkey -algorithm qteslapiii -out key.pem'
        print("generating key.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time taken to generate key")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl req -new -newkey qteslapiii -keyout key.pem -out rsa_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf'
        print("genetating cert signing request rsa_svr.csr")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count + 1
        count = 100
        end_time = time.time()
        print ("Time to generate certificate signing request")
        print ((end_time-start_time)/100)

        myCmd='apps/openssl x509 -req -in rsa_srv.csr -out cert.pem -CA pycacert.pem -CAkey rsa_CA.key -CAcreateserial -days 365'
        print("generating cert.pem")
        start_time = time.time()
        while count <= 100:
            os.system(myCmd)
            count = count +1 
        end_time = time.time()
        print ("Time taken to genetate signed certificate")
        print ((end_time-start_time)/100)
    
if __name__ == '__main__':
    mem_usage = memory_usage(main)
    print('Memory usage (in chunks of .1 seconds): %s' % mem_usage)
    print('Maximum memory usage: %s' % max(mem_usage))
    