all: syn-scan-network

syn-scan-network: syn-scan-network.cpp
	g++ -g -Wall syn-scan-network.cpp -o syn-scan-network -lpthread -ltins -lresolv
mysynscan: 
	g++ -g -Wall mysynscan.cpp -lpthread -ltins -lresolv
