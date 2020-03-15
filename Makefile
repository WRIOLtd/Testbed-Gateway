APPS = Gateways
LIB_SRCS = tools-utils.c mqtt.cpp 
DEPEND = tools-utils.h
INC = 
LIBS = -l mosquittopp -pthread -Bstatic
all: $(APPS)

CC=g++
CFLAGS += -g

$(APPS) : % : %.cpp $(LIB_SRCS) $(DEPEND)
	$(CC) $(INC) $(CFLAGS) $< $(LIB_SRCS) -o $@ $(LIBS)

clean:
	rm -f $(APPS)
