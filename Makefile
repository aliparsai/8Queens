CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJBF =		8queens-bruteforce.o 
OBJGA =		8queens-genetic.o

LIBS =

BF =	8queens-bruteforce 
GA = 	8queens-genetic


all:	$(BF) $(GA)

clean:
	rm -f $(OBJBF) $(OBJGA) $(BF) $(GA) ga.log

$(BF):	$(OBJBF)
	$(CXX) -o $(BF) $(OBJBF) $(LIBS)

$(GA):	$(OBJGA)
	$(CXX) -o $(GA) $(OBJGA) $(LIBS)

	
