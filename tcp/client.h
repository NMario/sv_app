/* TCP Client */
#ifndef __CLIENT_H__
#define __CLIENT_H__

int tcp_conn(char *, int);

int tcp_send(char *);

int tcp_recv(char **);

void tcp_close();

#endif