/*
 * sender
 * This is the client in the penny mail distribution system.
 * Sender works in tandem with recvr (the server) to implement a framework
 * for DIME mail transfer.
 *
 * Currently, the TODO list is as follows:
 * 5. Lookup recvr host based on DIME management record (TXT version).
 */

#include "penny_common.h"
#include "penny_defs.h"
#include "simple_log.h"


static char msg[4096];      // arbitrary for now

/*
 * brief: create a toy mail message
 * notes: The format of the toy message is three lines
 * To: <email address>
 * From: <email address>
 * <body on one line>
 */
char *create_msg(void)
{
    char to_addr[256];      // arbitrary for now
    char from_addr[256];    // ""
    char body[4096];        // ""


    printf("To: ");
    scanf(" %s", to_addr);
    printf("From: ");
    scanf(" %s", from_addr);
    printf("Body: ");
    scanf(" %[^\n]", body);

    sprintf(msg, "To: %s\nFrom: %s\n%s\n", to_addr, from_addr, body);
    log_debug("\n\n%s\n\n", msg);

    return msg;
}

/*
 * brief: find the destination organization's domain from the To: address
 * msg: a toy message
 * ddestination: a buffer for the destination
 * returns: No error return for now
 */
 void get_destination(char *msg, char *destination)
 {
     char *d = destination;

    msg += 4;               // skip the To:
    while (*msg++ != '@')   // skip mamilbox
        ;

    while (*msg != '\n') {  // copy domain
        *d++ = *msg++;
    }

    *d = '\0';

    log_debug("destination organization is %s, %lu", destination, strlen(destination));

    return;
 }

/*
 * brief: lookup the IP address of the destination organization's DMTP server.
 * destination: the destination domain
 * returns a string with the IP address, or NULL if the domain isn't DIME enabled.
 */
char *dime_destination_ip(char *destination)
{
    // dom.ain is a special domain whose dx is always 127.0.0.1
    if (strcmp(destination, "dom.ain") == 0) {
        return "127.0.0.1";
    }

    // fetch the DIME management record for the domain

    // Parse out the dx field

    // Resolve the dx hostname and return the IP address

    return NULL;
}


 int main (int argc, char **argv)
 {
    int sock;
    struct sockaddr_in server_addr; // DIME server address
    char *server_ip;                // Server IP address (dotted quad)
    char *msg;
    char dest_domain[1024];         // arbitrary length

    // Init
    log_debug("Client coming up!");

    // Generate a message
    msg = create_msg();
    get_destination(msg, dest_domain);
    log_debug("dest_domain is %s", dest_domain);

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        log_err("Cound not initialize network");
        return -1;
    }

    if ((server_ip = dime_destination_ip(dest_domain)) == NULL) {
        log_warn("No DX for domain %s", dest_domain);
        return -1;
    }

    log_debug("Connection to %s", server_ip);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port        = htons(DIME_PORT);

    // Establish the connection to the appropriate DIME server
    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        log_err("count not connect");
        return -1;
    }

    log_debug("Connected, ready to write");

    // Send the mail message
    if (send(sock, msg, strlen(msg), 0) != strlen(msg)) {
        log_err("message not properly sent");
    }

     // Cleanup

     return 0;
 }
