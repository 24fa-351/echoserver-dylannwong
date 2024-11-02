echo_server: echo_server.c
	gcc -o echo_server echo_server.c

clean:
	rm echo_server
