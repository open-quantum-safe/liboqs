import psutil
import time 

while True:
	cpu = psutil.cpu_percent()
	mem = psutil.virtual_memory().used
	print ("CPU mem")
	print (str(cpu)+" "+str(mem))
	time.sleep(0.5)

