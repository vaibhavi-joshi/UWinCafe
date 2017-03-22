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
  "Original Blend Coffee", 1.59,1.79,1.99,
  "Dark Roast Coffee", 1.59,1.79,1.99,
  "French Vanila Coffee", 1.99,2.49,2.99,
  "Decaf Coffee",1.59,1.79,1.99,
  "Cafe Mocha",1.90,2.10,2.34,
  "Iced Tea",1.49,1.79,1.99,
};

struct cart{
  char *item;
  char *size;
   int  count;
   float total;
};


int size,num=0;
float totalVal = 0.0;
struct cart c[255];

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

// Our Application's code will start from here

void menu(int sd){

    //fprintf(stderr, "Function Menu called:");
    int max = sizeof(m)/sizeof(struct food), i, number;
    char *msgString = NULL; 
    char inputMsg[2048], coffeName[255];
    
   snprintf(inputMsg,sizeof(inputMsg),"\t%-31s%-11s%-11s%s\n", "Item", "Small", "Medium", "Large");
   int structSize = (max * (sizeof(m)+1)) + strlen(inputMsg);
   msgString = (char *) malloc(structSize);
   msgString = inputMsg;
  
     for (i = 0; i < max; i++) {
    
    snprintf(msgString  + strlen(msgString),structSize - strlen(msgString),
             "%d.\t%-30s %-10.2f %-10.2f %.2f \n",i+1, m[i].item, m[i].smallPrice, m[i].mediumPrice,m[i].largePrice);
  } 
   snprintf(msgString  + strlen(msgString),strlen(msgString)+1, "\nPlease Select Your Option from Above Menu : \n");
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

   char msgString1[255];
   sprintf(msgString1,"Please Select Size of the cup : 1. Small  2. Medium 3. Large \n");
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
  char msgString2[] = "Please Enter Quantity in form of integer i.e 1 or 2 \n";
		
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
    case 1: c[num].total = qaunt*m[menuNumber-1].smallPrice;
            c[num].size  = "Small";
            totalVal     +=  c[num].total;
            break;
     
     //CASE 2 MEDIUM SIZE
    case 2: c[num].total  = qaunt*m[menuNumber-1].mediumPrice;
            c[num].size   = "Medium";
            totalVal     += c[num].total;
            break;
 
   //CASE 3 LARGE SIZE
    case 3: c[num].total = qaunt*m[menuNumber-1].largePrice;
            c[num].size  = "Large";
            totalVal    += c[num].total;
            break;
      
    default: printf("Invalid Error!\n");
              
    }
 
   c[num].item = m[menuNumber-1].item;
   c[num].count = qaunt;
   num++;
     
   char msgString3[] = "Do you want to continue? 1-Yes, 2-No \n";
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
   
    char msgString5[255];
    char addr[255];
    
     
    sprintf(msgString5,"Your total is %.2f \nPlease Enter your E-mail address to get invoice:\n", totalVal);
    
    write(sd, msgString5, sizeof(msgString5));
		if(!read(sd, addr, 255))
		{
			close(sd);
			fprintf(stderr,"Bye, client dead, wait for a new client\n");
			exit(0);
		}   
   
   fprintf(stderr, "Client email address: %s\n", addr);
   
   
   /* ********************************** InVoicec Design ********************************************************* */
  
    int i;
    char *msgString = NULL; 
    char inputMsg[2048];
    
    
   snprintf(inputMsg,sizeof(inputMsg),"UWinCafe Invoice:\n\n\t%-41s\t%-13s\t%-11s%s\n\n", "Item", "Size", "Count", "Total");
   int structSize = ((sizeof(c)+1)) + strlen(inputMsg);
   msgString = (char *) malloc(structSize);
   msgString = inputMsg;
  
     for (i = 0; i < num; i++) {
     
             snprintf(msgString+strlen(msgString),structSize-strlen(msgString),
                   "\t%-40s\t%-12s\t%-9d%.2f\n",c[i].item,c[i].size,c[i].count,c[i].total);
        
  } 

   snprintf(msgString  + strlen(msgString),strlen(msgString)+1, "\n ****************** Total is: %.2f $ ***********************",totalVal);
     fprintf(stderr,"%s",msgString);
    char sub[255] = "Invoice";
    
    
    int check = sendmail(addr, sub, msgString);
    
    if (check != -1){
        char msg[255] = "Check your inbox for Detailed Invoice !!";
         sleep(3);
         fprintf(stderr,"Email Sent!");
         write(sd, msg, sizeof(msg));
         close(sd);
         exit(0);
    }
    
}

int sendmail(const char *to, const char *subject, const char *message)
{
    int retval = -1;
    fprintf(stderr,"sendmail:::::");
    FILE *mailpipe = popen("/usr/lib/sendmail -t", "w");
    if (mailpipe != NULL) {
        fprintf(mailpipe, "To: %s\n", to);
      //  fprintf(mailpipe, "Subject: %s\n", subject);
        fwrite(message, 1, strlen(message), mailpipe);
        fwrite(".\n", 1, 2, mailpipe);
        pclose(mailpipe);
        retval = 0;
     }
     else {
         perror("Failed to invoke sendmail");
     }
     return retval;
}
