#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

struct food {
  char *item;
  double smallPrice;
  double mediumPrice;
  double largePrice;
  
} m[] = {
  "Mocha", 2.25,2.55,3.0,
  "Latte", 1.00,1.55,1.75,
  "French Vanilla", 2.50,2.95,3.75,
};

int size;
int opt;
float totalVal = 0.0;

void child(int sd);
void menu(int sd);
void sizeOfCup(int sd,int menuNumber);
void quantity(int sd,int menuNumber);
void total(int sd,int quant, int menuNumber);
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

	  menu(sd);
}

// Our Application's code will start from here0

void menu(int sd){

    //fprintf(stderr, "Function Menu called:");
    int max = sizeof(m)/sizeof(struct food), i, number;
    char *msgString = NULL; 
    char inputMsg[1024], coffeName[255];
    
   snprintf(inputMsg,sizeof(inputMsg),"\t%-20s%-10s%-10s%s\n", "Item", "Small", "Medium", "Large");
   int structSize = (max * (sizeof(m)+1)) + strlen(inputMsg);
   msgString = (char *) malloc(structSize);
   msgString = inputMsg;


    for (i = 0; i < max; i++) {
    
    snprintf(msgString  + strlen(msgString),
             structSize - strlen(msgString),
             "%d.\t%-20s %-10.2f %-10.2f %.2f \n",i+1, m[i].item, m[i].smallPrice, m[i].mediumPrice,m[i].largePrice);
  }
 
   snprintf(msgString  + strlen(msgString),strlen(msgString)+1, "\nPlease Select Your Option from Above Menu : ");
   	write(sd, msgString, strlen(msgString)+1);
		if(!read(sd, coffeName, 255))
		{
			close(sd);
			fprintf(stderr,"Bye, client dead, wait for a new client\n");
			exit(0);
		}
    	fprintf(stderr, "Client selected coffee: %s\n", coffeName);
      
      sscanf(coffeName,"%d",&number);
    
      sizeOfCup(sd,number);
    
}

void sizeOfCup(int sd,int menuNumber){

//	fprintf(stderr, "Function Sizeof cup called:");

   char msgString1[] = "Please Select Size of the cup : \n 1. Small \n 2. Medium \n 3. Large";
   char cupSize[255];

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
    quantity(sd,menuNumber);
}

void quantity(int sd, int menuNumber){

  int qaunt;
  char quan[255];
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
   
   total(sd,qaunt,menuNumber);
}

void total(int sd, int qaunt, int menuNumber){

//fprintf(stderr, "Function   total called:");


switch(size)
    {
    //CASE 1 SMALL SIZE
    case 1: totalVal += qaunt*m[menuNumber-1].smallPrice;
            break;
     
     //CASE 2 MEDIUM SIZE
    case 2:  totalVal += qaunt*m[menuNumber-1].mediumPrice;
            break;
 
   //CASE 3 LARGE SIZE
    case 3:   totalVal += qaunt*m[menuNumber-1].largePrice;;
            break;
      
    default: printf("Invalid Error!\n");
              
    }
   
   char msgString3[] = "Do you want to continue? 1-Yes, 2-No";
   char opti[255];
   int option;

		write(sd, msgString3, sizeof(msgString3));
		if(!read(sd, opti, 255))
		{
			close(sd);
			fprintf(stderr,"Bye, client dead, wait for a new client\n");
			exit(0);
		}
   
   sscanf(opti, " %d", &option);
   fprintf(stderr, "Client selected option: %d\n", option);
   	   
   switch(option)
    {
       //CASE 1 Yes option
        case 1: menu(sd);
                break;
     
       //CASE 2 No option
       case 2:  invoice(sd);
                break;
             
       default: printf("Invalid Error!\n");
    }
}

void invoice(int sd){
    //fprintf(stderr, "Function   invoice called:");
    char msg[255], ans[255];
    int ansVal;
    
    sprintf(msg,"Your total is %.2f", totalVal);
    write(sd, msg, sizeof(ans));
   	close(sd);
    exit(0);
}
