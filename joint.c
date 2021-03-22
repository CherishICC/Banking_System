/*a program to open a file, store normal user data*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdbool.h>
typedef struct jointuser{

	int userID;
	char username1[30];
	char username2[30];
	char password[30];
	int accountNo;
	float balance;
	bool status;
}joint;
int main(void){
int fd;
int n = 1;
fd=open("joint_users",O_CREAT|O_RDWR,0744);
joint joint;
while(n){
	printf("Please enter the userID :");
	scanf("%d",&joint.userID);
	printf("Please enter the username:");
	scanf(" %[^\n]",joint.username1);
	printf("Please enter other username:");
	scanf(" %[^\n]",joint.username2);
	printf("Please enter the password :");
	scanf(" %[^\n]",joint.password);
	printf("Please enter the account number:");
	scanf("%d",&joint.accountNo);
	printf("Balance has been set to zero\n");
	// scanf("%f",&joint.balance);
	joint.balance = 0;
	joint.status=true;
	printf("Your account has been created\n");
	write(fd,&joint,sizeof(joint));
	printf("Would you like to create more users\nEnter 1 for yes and 0 for no : ");
	scanf("%d",&n);
}
close(fd);
}
