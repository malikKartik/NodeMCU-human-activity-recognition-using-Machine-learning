<h1>About project</h1>
In this project a model is trained to know if a person is standing or sitting or walking which is based on 18 features calculated on 100 samples taken every 2 seconds.

features calculated on nodeMCU(configered with gy87 or gy521) is then send to pc through TCP/IP and there activity is recognized using a pretrained model.

<h1>Quick start</h1>
1)Install nodejs.<br>
2)burn nodeMCUtoserver.ino code to your device enabled with wifi and gy87 or gy521 or any other suitable device.<br>
3)Turn on hotspot of your PC and connect your nodeMCU to your hotspot(you need to edit the credential in the code).<br>
4)copy IP from serial window and paste it in the tcp_ip.py<br>
5)Now open new cmd window and run the command "python tcp_ip.py".<br>
6)Now open cmd and run the command npm install(this might take some time).<br>
7)Now run the command "npm start".<br>

(once you have the IP of nodeMCU you can connect the device to just the power source)
