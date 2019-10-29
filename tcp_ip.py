# Import socket module
import socket               
from sklearn.externals import joblib
mj = joblib.load('model_joblib2')
# Create a socket object
s = socket.socket()         

# Define the port on which you want to connect
port = 8888                

# connect to the server on local computer
s.connect(('192.168.137.223', port))
chunks = []
while True:
	# receive data from the server
	data = s.recv(1024).decode()
	chunks.append(data)
	if(len(chunks) == 2):
		finalData = ''.join(chunks).split(' ')
		finalData[17] = 0.000
		# print(finalData)
		for i in range(0, len(finalData)-1): 
			finalData[i] = float(finalData[i])
		print(finalData[0:18])
		# print(data)
		fh = open("text.txt","w")
		fh.write(mj.predict([finalData[0:18]])[0])
		fh.close()
		print(mj.predict([finalData[0:18]])[0])
		fh = open("chart.txt","w")
		if (mj.predict([finalData[0:18]])[0]=="sitting"):
			fh.write("1")
		if (mj.predict([finalData[0:18]])[0]=="standing"):
			fh.write("2")
		if (mj.predict([finalData[0:18]])[0]=="walking"):
			fh.write("3")		
		fh.close()
		chunks=[]
 