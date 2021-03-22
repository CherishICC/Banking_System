#include<unistd.h>
#include <stdio.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdbool.h>

void normaluserLogin(int sfd);
void jointuserLogin(int sfd);
void adminLogin(int sfd);
void deposit();
void withdraw();
void balanceenquiry();
void passwordchange();
void viewdetails();
void Exit();
void selectoptn(int sfd ,int type);
void adminoptn(int sfd);
typedef struct normaluser{

	int userID;
	char username[30];
	char password[30];
	int accountNo;
	float balance;
	bool status;

}nuser;

typedef struct jointuser{

	int userID;
	char username1[30];
	char username2[30];
	char password[30];
	int accountNo;
	float balance;
	bool status;
}juser;

typedef struct admin{

	int adminID;
	char username[30];
	char password[30];
}admin;

int userID;
int check=0;
int Login(int sfd){

	int num;
	write(1,"Connected to server\n",sizeof("Connected to server\n"));
	write(1,"1: Normal user \n2: Joint user \n3: admin \n",sizeof("1: Normal user \n2: Joint user \n3: admin \n"));
	write(1,"Enter the corresponding number: ",sizeof("Enter the corresponding number: "));
	scanf("%d",&num);
	switch(num)
	{
		case 1: write(sfd,&num,sizeof(num));
			normaluserLogin(sfd);
			break;
		case 2: write(sfd,&num,sizeof(num));
			jointuserLogin(sfd);
			break;
		case 3:	write(sfd,&num,sizeof(num));
			adminLogin(sfd);
			break;
		default: write(1,"Invalid input\n",sizeof("Invalid input"));
			 break;
	}
	return num;

}
void selectoptn(int sfd,int type){
		int option=0;
		write(1,"1: Deposit\n2: Withdraw\n3: Balance Enquiry\n4: Password Change\n5: View Details\n6: Exit\n",sizeof("1: Deposit\n2:withdraw\n3: Balance Enqiuiry\n4: Password Change\n5: View Details\n6: Exit\n"));
		write(1,"Enter the corresponding number: \n",sizeof("Enter the corresponding number: "));
		scanf("%d",&option);
		switch(option)
		{
		case 1:	write(sfd,&option,sizeof(option));deposit(sfd,type);
		        break;
		case 2:	write(sfd,&option,sizeof(option));withdraw(sfd,type);
			break;
		case 3:	write(sfd,&option,sizeof(option));balanceenquiry(sfd,type);
			break;
		case 4:	write(sfd,&option,sizeof(option));passwordchange(sfd,type);
			break;
		case 5:	write(sfd,&option,sizeof(option));viewdetails(sfd,type);
			break;
		case 6:	write(sfd,&option,sizeof(option));Exit();
			break;
		default:write(1,"Invalid option",sizeof("Invalid option"));break;
		}
	return;

	}

void normaluserLogin(int sfd){
	nuser nu;
	write(1,"Enter ID : ",sizeof("Enter ID : "));
	scanf("%d",&nu.userID);
	write(1,"Enter Password : ",17);
	scanf("%s",nu.password);
	userID=nu.userID;
	write(sfd,&nu.userID,sizeof(nu.userID));
	write(sfd,&nu.password,sizeof(nu.password));
	read(sfd,&check,sizeof(check));
	// if(check)
	// 	{check=1;
	// 	}
	return;
}

void jointuserLogin(int sfd){
	juser ju;
	write(1,"Enter UserID : ",sizeof("Enter UserID : "));
	scanf("%d",&ju.userID);
	write(1,"Enter Password : ",sizeof("Enter Password : "));
	scanf("%s",ju.password);
	userID=ju.userID;
	write(sfd,&ju.userID,sizeof(ju.userID));
	write(sfd,&ju.password,sizeof(ju.password));
	read(sfd,&check,sizeof(check));
	// if(check)
	// 	{check=1;
	// 	}
	return;
}

void adminLogin(int sfd){
	admin ad;
	write(1,"Enter your ID : ",sizeof("Enter your ID : "));
	scanf("%d",&ad.adminID);
	write(1,"Enter the password : ",sizeof("Enter the password : "));
	scanf("%s",ad.password);
	userID=ad.adminID;
	write(sfd,&ad.adminID,sizeof(ad.adminID));
	write(sfd,&ad.password,sizeof(ad.password));
	read(sfd,&check,sizeof(check));
	// if(check){check=1;}
	return;
}

void deposit(int sfd,int type){
	float amt;
	int j;
	int done;
	write(1,"Enter the amount to deposit: ",sizeof("Enter the amount to deposit: "));
	scanf("%f",&amt);
	printf("amount = %f\n",amt);
	write(sfd,&amt,sizeof(amt));
	done = 1;
	write(sfd,&done,sizeof(done));
	read(sfd,&j,sizeof(j));
	if(j){
	printf("Successfully deposited\n");
	}
	else{
	printf("Couldn't deposit crossed the limit\n");
	}
	selectoptn(sfd,type);
	return;
	}
void withdraw(int sfd,int type){
	float amt;
	int i;
	write(1,"Enter the amount to withdraw: ",sizeof("Enter the amount to withdraw: "));
	scanf("%f",&amt);
	write(sfd,&amt,sizeof(amt));
	read(sfd,&i,sizeof(i));
	if(i){
	printf("Successfully withdrawn\n");
	}
	else{
	printf("Not enough balance\n");
	}
	selectoptn(sfd,type);
	return;
	}
void balanceenquiry(int sfd,int type){
	float bal;
	int i;
	read(sfd,&bal,sizeof(bal));
	printf("Your balance is = %f\n",bal);
	selectoptn(sfd,type);
	return;
	}
void passwordchange(int sfd,int type){
	char pwd[30],npwd[30];int i=0;
	printf("Enter the old password: ");
	scanf(" %[^\n]",pwd);
	write(sfd,&pwd,sizeof(pwd));

	read(sfd,&i,sizeof(i));
	if(i){
	printf("Enter the new password: ");
	scanf(" %[^\n]",npwd);
	write(sfd,&npwd,sizeof(npwd));
	printf("Successfully changed password\n");
	}
	else{
	printf("Wrong password \n");
	}
	if(type==1 || type == 2)
		selectoptn(sfd,type);
	else if(type ==3)
		adminoptn(sfd);
	return;

	}
void viewdetails(int sfd,int type){
	int check=0;
		if(type==1){
			nuser nu;
			read(sfd,&nu,sizeof(nu));
			printf("User ID = %d\n",nu.userID);
			printf("username = %s\n",nu.username);
			printf("account number =%d\n",nu.accountNo);
			printf("Balance = %f\n",nu.balance);
			}
		else if(type==2){
			juser ju;
			read(sfd,&ju,sizeof(ju));
			printf("User ID = %d\n",ju.userID);
			printf("username = %s\n",ju.username1);
			printf("username = %s\n",ju.username2);
			printf("account number =%d\n",ju.accountNo);
			printf("Balance = %f\n",ju.balance);
			}
		if(check){
			printf("Search Successful\n");
	}
	selectoptn(sfd,type);
	}

void Exit(){
	exit(0);
	}


void adduser(int sfd){
	int type,check;
	printf("choose the type of account\n1: normal\n2: joint\n");
	scanf("%d",&type);
	write(sfd,&type,sizeof(type));
	nuser nu1,nu2;
	juser ju1,ju2;
	if(type==1)
	{
		printf("Enter the username: \n");
		scanf(" %[^\n]",nu1.username);
		printf("Enter the password: \n");
		scanf(" %[^\n]",nu1.password);
		write(sfd,&nu1,sizeof(nuser));
		read(sfd,&nu2,sizeof(nuser));
	}
	if(type==2)
	{
		printf("Enter the username\n");
		scanf(" %[^\n]",ju1.username1);
		printf("Enter the second username\n");
		scanf(" %[^\n]",ju1.username2);
		printf("Enter the password\n");
		scanf(" %[^\n]",ju1.password);
		write(sfd,&ju1,sizeof(juser));
		read(sfd,&ju2,sizeof(juser));
	}
	read(sfd,&check,sizeof(int));
	if(check)
	{
		printf("User added successfully\n");
		write(1,"Your id is ",sizeof("Your id is "));
		if(type == 1){printf("%d\n",nu2.userID);}
		else if(type == 2){printf("%d\n",ju2.userID);}
	}
	adminoptn(sfd);

	}
void deleteuser(int sfd){
	int type,check,userID;
	printf("Choose the type of account\n1: normal\n2: joint\n");
	scanf("%d",&type);
	write(sfd,&type,sizeof(type));
	printf("Enter the userID");
	scanf("%d",&userID);
	write(sfd,&userID,sizeof(userID));
	if(check){
		printf("Successfully deleted the account \n");
		}
	else if(check==0){
		printf("User doesnot exist\n");
		}
	adminoptn(sfd);
	return;
	}
void modifyuser(int sfd){
	int type,check;
	printf("choose the type of account\n1: normal\n2: joint\n");
	scanf("%d",&type);
	write(sfd,&type,sizeof(type));
	if(type==1){
		nuser nu;
		printf("Enter the userID\n");
		scanf("%d",&nu.userID);
		printf("Enter the username\n");
		scanf(" %[^\n]",nu.username);
		printf("Enter the password\n");
		scanf(" %[^\n]",nu.password);
		printf("Enter the balance\n");
		scanf("%f",&nu.balance);
		write(sfd,&nu,sizeof(nuser));
	}
	else if(type==2){
		juser ju;
		printf("Enter the userID\n");
		scanf("%d",&ju.userID);
		printf("Enter the username\n");
		scanf(" %[^\n]",ju.username1);
		printf("Enter the other username\n");
		scanf(" %[^\n]",ju.username2);
		printf("Enter the password\n");
		scanf(" %[^\n]",ju.password);
		ju.balance = 0;
		write(sfd,&ju,sizeof(juser));
	}
	read(sfd,&check,sizeof(check));
	if(check)
		printf("Successfully modified the user\n");
	adminoptn(sfd);
	return;
	}
void searchuser(int sfd){
		int ID,type;
		printf("choose the type of account\n1: normal\n2: joint\n");
		scanf("%d",&type);
		write(sfd,&type,sizeof(type));
		printf("Enter the userID\n");
		scanf("%d",&ID);
		write(sfd,&ID,sizeof(ID));
		if(type==1){
			nuser nu;
			read(sfd,&nu,sizeof(nu));
			printf("User ID = %d\n",nu.userID);
			printf("username = %s\n",nu.username);
			printf("account number = %d\n",nu.accountNo);
			printf("Balance = %f\n",nu.balance);
			}
		else if(type==2){
			juser ju;
			read(sfd,&ju,sizeof(ju));
			printf("User ID = %d\n",ju.userID);
			printf("username = %s\n",ju.username1);
			printf("username = %s\n",ju.username2);
			printf("account number = %d\n",ju.accountNo);
			printf("Balance = %f\n",ju.balance);
			}
		if(check){
			printf("Search Successful\n");

		}
		adminoptn(sfd);
		return;
}

void adminoptn(int sfd){
		int choice;
		write(1,"1: add\n2: delete\n3: modify\n4: search\n5: change password\n6: Exit\n",sizeof("1: add\n2: delete\n3: modify\n4: search\n5: change password\n6: Exit\n"));
		printf("Enter the corresponding number: ");
		scanf("%d",&choice);
		switch(choice){
			case 1:write(sfd,&choice,sizeof(choice));adduser(sfd);break;
			case 2:write(sfd,&choice,sizeof(choice));deleteuser(sfd);break;
			case 3:write(sfd,&choice,sizeof(choice));modifyuser(sfd);break;
			case 4:write(sfd,&choice,sizeof(choice));searchuser(sfd);break;
			case 5:write(sfd,&choice,sizeof(choice));passwordchange(sfd,3);break;
			case 6:write(sfd,&choice,sizeof(choice));Exit();
			default:printf("Invalid input\n");
		}
		return;
	}

int main(){
	int sd,len,type;
	struct sockaddr_in serv;
	sd = socket(AF_INET,SOCK_STREAM,0);

	char buff[25],username[25],password[25];

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv.sin_port = htons(5555);

	connect(sd,(struct sockaddr *)&serv,sizeof(serv));

	type=Login(sd);
	if(type==1 || type==2){
		if(check) {selectoptn(sd,type);}
		else{
		write(1,"wrong ID/password\n",sizeof("wrong ID/password "));
		}
	}
	else if(type==3){
		if(check) adminoptn(sd);
		else{
			write(1,"wrong ID/password\n",sizeof("wrong ID/password "));
			}
		}
	}
