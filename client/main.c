// The client that will recieve the audio streams and playback

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

typedef enum
{
    SENDER,
    RECEIVER,
    CLIENT_ERROR
} Client_type;

// for now, use the local ip of the PI for testing
// before figuring out a way to import it to a client/host
#define LOCAL_IP "10.42.0.21"

// for now, use this function to determine if a client is requesting to connect
// to the server as a sender or a receiver
Client_type getInfo()
{
    char buf[1024];
    memset (&buf, '0', sizeof(buf));
    printf ("Enter client info [sender/receiver]:");
    fgets (buf, sizeof(buf), stdin);
    if (strcasestr(buf, "send"))
    {
        return SENDER;
    } else if (strcasestr(buf, "receive")) 
    {
        return RECEIVER;
    } else 
    {
        return CLIENT_ERROR;
    }
}

int main(int argc, char **argv)
{
    int sockFd = 0;
    struct sockaddr_in serv_addr;

    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n ERROR: Could not create socket \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if (inet_pton(AF_INET, LOCAL_IP, &serv_addr.sin_addr) <= 0)
    {
        printf("\n ERROR: net_pton error occured \n");
        return -1;
    }

    if (connect(sockFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n ERROR: Connect failed \n");
        return -1;
    }

    // get what kind of client this is (sender or receiver) and send that info
    // to the host so it can set up recieving/sending threads
    Client_type clientType = getInfo();
    if (clientType == SENDER)
    {
        printf("Got sender\n");
    } else if (clientType == RECEIVER)
    {
        printf("Got receiver\n");
    } else {
        printf("error\n");
    }


    int n = 0;
    // while ((n = read(sockFd, buf, sizeof(buf) - 1)) > 0)
    // {
    //     buf[n] = 0;
    //     if (fputs(buf, stdout) == EOF)
    //     {
    //         printf("\n ERROR: Fputs error \n");
    //     }
    // }

    // if (n < 0)
    // {
    //     printf("\n Error: read error \n");
    // }

    return 1;
}