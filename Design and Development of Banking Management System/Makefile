all: server client

server: server.c AllStructure/allStruct.h Modules/Customer.h Modules/Employee.h Modules/Manager.h Modules/Admin.h
	gcc server.c -o server -pthread -lcrypt -lrt

client: client.c
	gcc client.c -o client

clean:
	rm -f server client


