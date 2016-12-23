#!/bin/bash
./makesk_evp > sk_xmss.pem
./makepk_evp sk_xmss.pem > pk_xmss.pem
./makecert sk_xmss.pem pk_xmss.pem  > cert_xmss.pem
