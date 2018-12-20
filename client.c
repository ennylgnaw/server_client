#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while (1) {

    //prompt user for input
    printf("Enter an input:\n");
    char input[BUFFER_SIZE];
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = 0;

    //send input to server
    write(to_server, input, BUFFER_SIZE);
    printf("Input to server: '%s'\n", input);

    //get response from server, display to user
    char response[BUFFER_SIZE];
    read(from_server, response, BUFFER_SIZE);
    printf("Response received by client: '%s'\n", response);
  }
}
