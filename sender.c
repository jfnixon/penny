/*
 * sender
 * This is the client in the penny mail distribution system.
 * Sender works in tandem with recvr (the server) to implement a framework
 * for DIME mail transfer.
 *
 * Currently, the TODO list is as follows:
 * 1. Establish network connection with recvr
 * 2. Transfer data to recvr
 * 3. Transfer a simple mail format (To:, From:, and body)
 * 4. Read a mailbox
 * 5. Lookup recvr host based on DIME management record (TXT version).
 */

 #include <stdio.h>
 #include <errno.h>
 #include <string.h>
 #include <sys/socket.h>
 #include <arpa/inet.h>

 #include "simple_log.h"
 #include "penny_defs.h"




 int main (int argc, char **argv)
 {
    int sock;
    struct sockaddr_in server_addr; // DIME server address
    char *server_ip;                // Server IP address (dotted quad)
    char *msg;

    // Init
    log_debug("Client coming up!");

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        log_err("Cound not initialize network");
        return -1;
    }

    log_debug("Connection to %s", argv[1]);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port        = htons(DIME_PORT);

    // Establish the connection to the appropriate DIME server
    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        log_err("count not connect");
        return -1;
    }

    log_debug("Connected, ready to write");

    // Here's the first message
    msg = "Hello DIME";

    // Send the mail message
    if (send(sock, msg, strlen(msg), 0) != strlen(msg)) {
        log_err("message not properly sent");
    }

     // Cleanup

     return 0;
 }
