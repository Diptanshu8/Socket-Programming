#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT  12345

int main (int argc, char *argv[])
{
   int    i, len, num, rc, on = 1;
   int    listen_sd, accept_sd;
   char   buffer[80];
   struct sockaddr_in   addr;

   if (argc >= 2)
      num = atoi(argv[1]);
   else
      num = 1;

   listen_sd = socket(AF_INET, SOCK_STREAM, 0);
   if (listen_sd < 0)
   {
      perror("socket() failed");
      exit(-1);
   }

   rc = setsockopt(listen_sd,
                   SOL_SOCKET,  SO_REUSEADDR,
                   (char *)&on, sizeof(on));
   if (rc < 0)
   {
      perror("setsockopt() failed");
      close(listen_sd);
      exit(-1);
   }

   memset(&addr, 0, sizeof(addr));
   addr.sin_family      = AF_INET;
   //char* localhost = "127.0.0.1";
   addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   addr.sin_port        = htons(SERVER_PORT);
   rc = bind(listen_sd,
             (struct sockaddr *)&addr, sizeof(addr));
   if (rc < 0)
   {
      perror("bind() failed");
      close(listen_sd);
      exit(-1);
   }

   rc = listen(listen_sd, 5);
   if (rc < 0)
   {
      perror("listen() failed");
      close(listen_sd);
      exit(-1);
   }

   printf("The server is ready\n");

   for (i=0; i < num; i++)
   {
      printf("Interation: %d\n", i+1);
      printf("  waiting on accept()\n");
      accept_sd = accept(listen_sd, NULL, NULL);
      if (accept_sd < 0)
      {
         perror("accept() failed");
         close(listen_sd);
         exit(-1);
      }
      printf("  accept completed successfully\n");

      printf("  wait for client to send us a message\n");
      rc = recv(accept_sd, buffer, sizeof(buffer), 0);
      if (rc <= 0)
      {
         perror("recv() failed");
         close(listen_sd);
         close(accept_sd);
         exit(-1);
      }
      printf("%s\n", buffer);

      //printf("  echo it back\n");
      len = rc;
      rc = send(accept_sd, buffer, len, 0);
      if (rc <= 0)
      {
         perror("send() failed");
         close(listen_sd);
         close(accept_sd);
         exit(-1);
      }
      close(accept_sd);
   }
   close(listen_sd);
   return 0;
}
