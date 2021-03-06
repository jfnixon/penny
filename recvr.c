/*
 * recvr
 * This is the server in the penny mail distribution system.
 * Recvr works in tandem with sender (the client) to implement a framework
 * for DIME mail transfer.
 *
 * Currently, the TODO list is in the sender header.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "simple_log.h"
#include "penny_defs.h"

#define MAXPENDING 5    /* Maximum outstanding connection requests */

int create_server_socket(void)
{
    int sock;                        // socket to create
    struct sockaddr_in local_addr;   // Local address

    // Create socket for incoming connections
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        log_err("socket() failed");
        return -1;
    }

    // Construct local address structure
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family      = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port        = htons(DIME_PORT);

    // Bind to the local address
    if (bind(sock, (struct sockaddr *) &local_addr, sizeof(local_addr)) < 0) {
        log_err("bind() failed");
        return -1;
    }

    // Mark the socket so it will listen for incoming connections
    if (listen(sock, MAXPENDING) < 0) {
        log_err("listen() failed");
        return -1;
    }

    return sock;
}

int accept_connection(int sock)
{
    int client_sock;
    struct sockaddr_in client_addr;
    unsigned int client_len;

    // Set the size of the in-out parameter
    client_len = sizeof(client_addr);

    // Wait for a client to connect
    if ((client_sock = accept(sock, (struct sockaddr *) &client_addr, &client_len)) < 0) {
        log_err("accept() failed");
        return -1;
    }

    // client_sock is connected to a client

    char tmp_addr[50];
    strncpy(tmp_addr, inet_ntoa(client_addr.sin_addr), 49);
    log_debug("Handling client %s\n", tmp_addr);

    return client_sock;
}

#define MAXBUFFERSIZE 4099

int main (int argc, char **argv)
{
    int server_sock;
    int client_sock;
    char buffer[MAXBUFFERSIZE];
    int msg_size;

    // Init
    log_debug("server coming up");
    if ((server_sock = create_server_socket()) < 0) {
        log_err("Could not initialize the network");
        return -1;
    }

    // Do arg processing

    // Start the main loop

    while (1) {
        if ((client_sock = accept_connection(server_sock)) < 0) {
            log_err("Accept failed");
            return -1;
        }

        log_debug("a connection has arrived!");

        // read the message
        if ((msg_size = recv(client_sock, buffer, MAXBUFFERSIZE, 0)) < 0) {
            log_err("recv failed");
            return -1;
        }

        // process the message
        buffer[msg_size] = '\0';
        log_debug("msg: %s", buffer);
    }

    // Cleanup

    return 0;
 }
