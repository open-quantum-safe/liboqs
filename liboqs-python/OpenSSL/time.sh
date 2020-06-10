#/bin/bash
for i in {0..100}
do 
    apps/openssl s_client -CAfile pycacert.pem -connect 10.0.0.155:44330 <<< "Q"
done

