import os

myCmd='apps/openssl req -x509 -new -newkey rsa:4096 -keyout rsa_CA.key -out pycacert.pem -nodes -subj "/CN=oqstest CA" -days 365 -config apps/openssl.cnf'
print("Generating a rsa CA key and pycacert")

myCmd='apps/openssl genpkey -algorithm rsa:4096 -out ssl_key.pem'
print("generating key.pem")

myCmd='apps/openssl req -new -newkey rsa -keyout key.pem -out rsa_srv.csr -nodes -subj "/CN=oqstest server" -config apps/openssl.cnf'
print("genetating cert signing request rsa_svr.csr")

myCmd='apps/openssl x509 -req -in rsa_srv.csr -out ssl_cert.pem -CA pycacert.pem -CAkey rsa_CA.key -CAcreateserial -days 365'
print("generating cert.pem")