#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int connect_smtp(const char* host, int port);
void send_smtp(int sock, const char* msg, char* resp, size_t len);



/*
  Use the provided 'connect_smtp' and 'send_smtp' functions
  to connect to the "lunar.open.sice.indian.edu" smtp relay
  and send the commands to write emails as described in the
  assignment wiki.
 */
int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Invalid arguments - %s <email-to> <email-filepath>", argv[0]);
    return -1;
  }

  char* rcpt = argv[1];
  char* filepath = argv[2];
  FILE* ptr = fopen(filepath, "r");
  if (NULL == ptr)
  {
    printf("Invalid file");
    return -1;
  }
  fseek(ptr, 0, SEEK_END);
  size_t size = ftell(ptr);
  fseek(ptr, 0, SEEK_SET);
  if(size > 4096)
  {
    printf("file too big\n");
    return -1;
  }
  int socket = connect_smtp("lunar.open.sice.indiana.edu", 25);
  char request[6000];
  char response[4096];
  char *line = NULL;
  size_t line_size = 0;
  memset(request, '\0', sizeof(request));
  strcat(request,"HELO silo.luddy.indiana.edu\r\n");
  send_smtp(socket, request, response, 4096);
  printf("%s\n", response);
  strcpy(request,"MAIL FROM:<");
  strcat(request,rcpt);
  strcat(request,">\r\n");
  send_smtp(socket, request, response, 4096);
  printf("%s\n", response);
  strcpy(request,"RCPT TO:<");
  strcat(request,rcpt);
  strcat(request,">\r\n");
  send_smtp(socket, request, response, 4096);
  printf("%s\n", response);
  strcpy(request,"DATA\r\n");
  send_smtp(socket, request, response, 4096);
  printf("%s\n", response);
  strcpy(request, "From: <");
  strcat(request,rcpt);
  strcat(request,">\r\n");
  strcat(request, "To: <");
  strcat(request,rcpt);
  strcat(request,">\r\n\r\n");
  while(getline(&line, &line_size, ptr)>=0 )
  {
    strcat(request,line);
    free(line);
    line = NULL;
  }
  fclose(ptr);
  strcat(request, "\r\n.\r\n");
 // printf("%s\n",request);
  send_smtp(socket, request, response, 4096);
  printf("%s\n", response);
  return 0;
}
