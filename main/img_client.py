from pylab import *;ion()
from socket import *


s=socket(AF_INET,SOCK_STREAM)

W,H,C=(360,640,3)

#s.connect(('192.168.1.42',4265));C=1
s.connect(('192.168.1.2',4265));C=1
#s.connect(('localhost',4265))

canvas=imshow(zeros((W,H,C)).squeeze(),vmax=255. ,cmap=cm.gray)

try:
	buf=''
	while 1:
		while len(buf)<(W*H*C):
			buf+=s.recv(2048)
		img=fromstring(buf[:W*H*C],'uint8').reshape(W,H,C).squeeze()
		buf=buf[W*H*C:]
		canvas.set_data(img)
		draw()
finally:
	s.close()







