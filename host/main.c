// The main server that will send the audio to the clients

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

// for now, use the local ip of the PI for testing
// before figuring out a way to import it to a client/host
#define LOCAL_IP "10.42.0.21"

void getClientInfo(char *buf, int bufSize)
{
	if (strcasestr(buf, "send", bufSize))
	{

	}
}

int main (int argc, char **argv)
{
	int listenFd = 0;
	int connFd = 0;
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));

	char buf[1024];
	memset(&buf, '0', sizeof(buf));

	listenFd = socket(AF_INET, SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(LOCAL_IP);
	serv_addr.sin_port = htons(5000);

	bind (listenFd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenFd, 10);
	connFd = accept(listenFd, (struct sockaddr*)NULL, NULL);

	// read the status of the client and from there determine if the host needs 
	// to receive or send the streaming data.
	// TODO once this works, impliment threading to handle sending/recieving 
	// data
	int status = read(connFd, buf, sizeof(buf));
	if (status > 0)
	{
		getClientInfo(&buf);
	} else {
		goto error_cleanup;
	}

	while (1)
	{

		printf ("Enter message: ");
		memset (&buf, '0', sizeof(buf));
		fgets (buf, sizeof(buf), stdin);

		write(connFd, buf, strlen(buf));

	}


error_cleanup:
	close(listenFd);
	return 1;
}