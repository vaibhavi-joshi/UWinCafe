#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>


char coffeName[255], quan[255],ans[255], cupSize[255];
int size, qaunt;
float totalVal = 0;


void child(int sd);
void menu(int sd);
void sizeOfCup(int sd);
void quantity(int sd);
void total(int sd);
void invoice(int sd);


int main(int argc, char *argv[])
{
	int sd, client, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;

	if(argc != 2)
	{
		printf("Call model: %s <Port #>\n", argv[0]);
		exit(0);
	}

	if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
	listen(sd, 5);

	while(1)
	{
		client=accept(sd,(struct sockaddr*)NULL,NULL);
		printf("Got a client\n");
		if(!fork())
			child(client);
		close(client);
	}
}

void child(int sd)
{
	while(1)
	{	
    // printmenu();
	  menu(sd);
	}
}

// Our Application's code will start from here

void menu(int sd){

    char msgString[] = "Please Select Your Option from below Menu : \n 1. Mocha \n 2. Latte \n 3. Espresso";
	
  	write(sd, msgString, sizeof(msgString));
		if(!read(sd, coffeName, 255))
		{
			close(sd);
			fprintf(stderr,"Bye, client dead, wait for a new client\n");
			exit(0);
		}
    	fprintf(stderr, "Client sent back: %s\n", coffeName);
   
    //Add a switch case to map number with coffeeName
    
    sizeOfCup(sd);
    
}

void sizeOfCup(int sd){

 char msgString1[] = "Please Select Size of the cup : \n 1. Small \n 2. Medium \n 3. Large";
		write(sd, msgString1, sizeof(msgString1));
		if(!read(sd, cupSize, 255))
		{
			close(sd);
			fprintf(stderr,"Bye, client dead, wait for a new client\n");
			exit(0);
		}
   
   sscanf(cupSize, " %d", &size);
   fprintf(stderr, "Client selected cup size: %d\n", size);
   
   //Add a switch case to map number with Cup size



    quantity(sd);
}

void quantity(int sd){

  char msgString2[] = "Please Enter Quantity in form of integer i.e 1 or 2";
		write(sd, msgString2, sizeof(msgString2));
		if(!read(sd, quan, 255))
		{
			close(sd);
			fprintf(stderr,"Bye, client dead, wait for a new client\n");
			exit(0);
		}

    sscanf(quan, "%d", &qaunt); 
   fprintf(stderr, "Client sent back: %d\n", qaunt);
   
   total(sd);
}

void total(int sd){

switch(size)
    {
    //CASE 1 SMALL SIZE
    case 1: totalVal = qaunt*1.59;
            break;
     
     //CASE 2 MEDIUM SIZE
    case 2: totalVal = qaunt*1.79;
            break;
 
   //CASE 3 LARGE SIZE
    case 3:  totalVal = qaunt*1.99 ;
            break;
    //CASE 4 X LARGE SIZE        
    case 4:  totalVal = qaunt*2.19;
            break;
      
    default: printf("Invalid Error!\n");
              
    }
    
  invoice(sd);
}

void invoice(int sd){

    sprintf(ans,"Your total is %f", totalVal);
     write(sd, ans, sizeof(ans));
  
}
