while true
do 
    ps -p 14815 -o %cpu,%mem,cmd
    sleep 0.5
done