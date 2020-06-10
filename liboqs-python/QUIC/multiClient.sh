for i in  {1..20}; do python3 examples/http3_client.py --ca-certs tests/pycacert.pem https://10.0.0.142:4433/ sleep 0.1; done
