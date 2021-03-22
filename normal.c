/*a program to open a file, store normal user data*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdbool.h>
typedef struct normaluser{

	int userID;
	char username[30];
	char password[30];
	int accountNo;
	float balance;
	bool status;
}normal;
int main(void){
int fd;
int n = 1;
fd=open("normal_users",O_CREAT|O_RDWR,0744);
normal normal;
while(n){
	printf("Please enter the userID :");
	scanf("%d",&normal.userID);
	printf("Please enter the username:");
	scanf(" %[^\n]",normal.username);
	printf("Please enter the password :");
	scanf(" %[^\n]",normal.password);
	printf("Please enter the account number:");
	scanf("%d",&normal.accountNo);
	printf("Balance has been set to zero\n");
	normal.balance = 0;
	normal.status=true;
	printf("Your account has been created\n");
	write(fd,&normal,sizeof(normal));
	printf("Would you like to create more users\nEnter 1 for yes and 0 for no : ");
	scanf("%d",&n);
}
close(fd);
}
