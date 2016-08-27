.PHONY: all clean
CPP_OPTS   =-std=c++11 -Wall -O2 -Ithrift_generated
LIBS       =-lthrift -lsqlite3

SRC    	  = thrift_generated/Sf_service.cpp  \
             thrift_generated/sf_services_constants.cpp  \
             thrift_generated/sf_services_types.cpp \
				 sf_services_server.cpp

OBJS=$(patsubst %.cpp, %.o, $(SRC))

%.o: %.cpp
	g++ $(CPP_OPTS) -o $@ -c $< 

all: sf_server

sf_server: ${OBJS}
	g++ ${CPP_OPTS} ${OBJS} -o $@ ${LIBS}


clean:
	rm -rf *.o thrift_generated/*.o sf_server
