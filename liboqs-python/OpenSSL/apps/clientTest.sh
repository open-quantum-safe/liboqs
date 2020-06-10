start_time="$(date -u +%s)"
for i in {1..100}
do
    apps/openssl s_client -CAfile pycacert.pem -connect 10.0.0.155:44330 <<< "Q"
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Total of $elapsed seconds elapsed for process"
