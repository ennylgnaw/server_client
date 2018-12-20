#include "pipe_networking.h"


static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("WKP");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  
  int to_client;
  int from_client;
  
  while (1) { 
    from_client = server_handshake( &to_client );
    char input[BUFFER_SIZE];
    while(read(from_client, input, BUFFER_SIZE)) {
      printf("Data received from client: '%s'\n", input);
      char processed[strlen(input)];
      for(int i = 0; i < strlen(input); i++)
	processed[i] = input[i] + 1;
      processed[strlen(input)] = 0;
      write(to_client, processed, BUFFER_SIZE);
      printf("Data sent by server: '%s'\n", processed);
    }
  }
}
