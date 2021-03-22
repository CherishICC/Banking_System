/*a program to open a file, store normal user data*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

typedef struct admin{
	int adminID;
	char username[30];
	char password[30];
}admin;

int main(void){
int fd;
int n = 1;
fd = open("admins_list",O_CREAT|O_RDWR,0744);
admin admin;
while(n)
{
	printf("Please enter an ID: ");
	scanf("%d",&admin.adminID);
	printf("This is your ID\n");
	printf("Please enter the username: ");
	scanf(" %[^\n]",admin.username);
	printf("Please enter the password: ");
	scanf(" %[^\n]",admin.password);
	printf("These are your login credentials\n");
	write(fd,&admin,sizeof(admin));
	printf("Would you like to create more users\nEnter 1 for yes and 0 for no : ");
	scanf("%d",&n);
}
close(fd);
}
