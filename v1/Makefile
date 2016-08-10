all:
	g++ -std=gnu++0x -c main.cpp
	g++ -std=gnu++0x -c TcpInfo.cpp -fpermissive
	gcc main.o TcpInfo.o /usr/lib/gcc/x86_64-linux-gnu/4.6/libstdc++.so -otcpinfo
clean:
	rm -rf tcpinfo
	rm -rf *.o
