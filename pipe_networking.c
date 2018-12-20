#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("Making WKP...\n");
  mkfifo("WKP", 0666);
  printf("WKP pipe made!\n");

  int to_server = open("WKP", O_RDONLY);
  printf("Pipe sends to server\n");
  
  char private_pipe[HANDSHAKE_BUFFER_SIZE];
  read(to_server, private_pipe, sizeof(private_pipe));
  remove("WKP");
  printf("Message received by server: %s\n", private_pipe);

  int from_server = open(private_pipe, O_WRONLY);
  write(from_server, ACK, HANDSHAKE_BUFFER_SIZE);
  printf("Message sent by server: %s\n", ACK);

  char nod[HANDSHAKE_BUFFER_SIZE];
  read(to_server, nod, HANDSHAKE_BUFFER_SIZE);
  printf("Message from client received by server: %s\n", nod);
  
  *to_client = from_server;
  return to_server;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("Making private pipe...\n");
  char private_pipe[HANDSHAKE_BUFFER_SIZE] = "pp";
  mkfifo(private_pipe, 0666);
  printf("Pipe made by client!\n");

  int to_serverr = open("WKP", O_WRONLY);
  printf("Client connected to pipe, can send to server\n");

  write(to_serverr, private_pipe, HANDSHAKE_BUFFER_SIZE);
  printf("Message sent by client: %s\n", private_pipe);

  int from_server = open(private_pipe, O_RDONLY);
  char nod[HANDSHAKE_BUFFER_SIZE];
  read(from_server, nod, HANDSHAKE_BUFFER_SIZE);
  remove(private_pipe);
  printf("Message from server received by client: %s\n", nod);

  printf("Sending message to server...\n");
  write(to_serverr, ACK, HANDSHAKE_BUFFER_SIZE);
  *to_server = to_serverr;
  return from_server;
}
