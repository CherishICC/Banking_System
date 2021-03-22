
#include<unistd.h>
#include <stdio.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<stdbool.h>

typedef struct normaluser{

	int userID;
	char username[30];
	char password[30];
	int accountNo;
	float balance;
	bool status;
}normal;

typedef struct jointuser{

	int userID;
	char username1[30];
	char username2[30];
	char password[30];
	int accountNo;
	float balance;
	bool status;
}joint;

typedef struct admin{

	int adminID;
	char username[30];
	char password[30];
}admin;


int tocheck(int nsd,int userID,int type,char pwd[30]){
	int check=0;
	if(type==1){
		normal a1;
		int fd = open("normal_users",O_RDONLY);
		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(normal));
		if((strcmp(a1.password,pwd)==0) && (a1.status==true)){
			printf("Valid user\n");
			check=1;}
		else
			printf("Invalid user\n");

	close(fd);
	}
	else if(type==2){
		joint a1;
		int fd = open("joint_users",O_RDONLY);
		lseek(fd,(userID-2000)*(sizeof(joint)),SEEK_SET);
		read(fd,&a1,sizeof(joint));
		if((strcmp(a1.password,pwd)==0) && (a1.status==true)){
			printf("Valid user\n");
			check=1;}
		else
			printf("Invalid user\n");

	close(fd);
	}
	else if(type==3){
		admin a1;
		int fd = open("admins_list",O_RDONLY);
		lseek(fd,(userID-1)*(sizeof(admin)),SEEK_SET);
		read(fd,&a1,sizeof(admin));
		if(strcmp(a1.password,pwd)==0){
			printf("Valid user\n");
			check=1;}
		else
			printf("Invalid user\n");


	close(fd);
	}
		write(nsd,&check,sizeof(check));
		return check;

}
void deposit(int nsd,int userID,int type){
	float amt;int check=0,done;
	read(nsd,&amt,sizeof(amt));
	if(type==1){
		normal a1;
		int fd = open("normal_users",O_RDWR);

		struct flock lock;
		lock.l_type =F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(userID-1000)*sizeof(normal);
		lock.l_len=sizeof(normal);
		lock.l_pid=getpid();
		int i=fcntl(fd,F_SETLKW,&lock);
		printf("Locked\n");

		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(normal));
		printf("balance = %f\n",a1.balance);
		printf("amount = %f\n",amt);
		a1.balance=a1.balance+amt;
		printf("balance = %f\n",a1.balance);

		lseek(fd,-(sizeof(normal)),SEEK_CUR);
		read(nsd,&done,sizeof(done));
		write(fd,&a1,sizeof(normal));

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		printf("Unlocked\n");

		check=1;
		close(fd);

		write(nsd,&check,sizeof(check));
	}
	else if(type==2){
		joint a1;
		int fd = open("joint_users",O_RDWR);

		struct flock lock;
		lock.l_type =F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(userID-2000)*sizeof(joint);
		lock.l_len=sizeof(joint);
		lock.l_pid=getpid();
		int i=fcntl(fd,F_SETLKW,&lock);
		printf("Locked\n");

		lseek(fd,(userID-2000)*(sizeof(joint)),SEEK_SET);
		read(fd,&a1,sizeof(joint));
		printf("balance = %f\n",a1.balance);
		printf("amount = %f\n",amt);
		a1.balance=a1.balance+amt;
		printf("balance = %f\n",a1.balance);

		lseek(fd,-(sizeof(joint)),SEEK_CUR);
		read(nsd,&done,sizeof(done));
		write(fd,&a1,sizeof(joint));

		lock.l_type=F_UNLCK;
		int j=fcntl(fd,F_SETLK,&lock);
		printf("Unlocked\n");
		check=1;
		close(fd);

		write(nsd,&check,sizeof(check));
	}
}

int withdraw(int nsd,int userID,int type){
	float amt;int check=0;
	read(nsd,&amt,sizeof(amt));
	if(type==1){
		normal a1;
		int fd = open("normal_users",O_RDWR);
		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(normal));
		printf("balance = %f\n",a1.balance);
		if(a1.balance>=amt)
		{
			struct flock lock;
			lock.l_type =F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(userID-1000)*sizeof(normal);
			lock.l_len=sizeof(normal);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			printf("Locking\n");
			// sleep(15);
			// getchar();
			a1.balance-=amt;
			printf("Amount withdrawn\n");
			lseek(fd,-(sizeof(normal)),SEEK_CUR);
			write(fd,&a1,sizeof(normal));
			printf("balance = %f\n",a1.balance);
			lock.l_type=F_UNLCK;
			i=fcntl(fd,F_SETLK,&lock);
			printf("Unlocked\n");
			check=1;
		}
		else
			{printf("Low balance\n");check=0;}

		write(nsd,&check,sizeof(check));
	close(fd);
	}
	else if(type==2){
		joint a1;
		int fd = open("joint_users",O_RDWR);
		lseek(fd,(userID-2000)*(sizeof(joint)),SEEK_SET);
		read(fd,&a1,sizeof(joint));
		printf("balance = %f",a1.balance);
		if(a1.balance>=amt){
			struct flock lock;
			lock.l_type =F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(userID-2000)*sizeof(joint);
			lock.l_len=sizeof(joint);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			printf("Locking\n");

			a1.balance-=amt;
			printf("Amount withdrawn\n");
			lseek(fd,-(sizeof(joint)),SEEK_CUR);
			write(fd,&a1,sizeof(joint));
			printf("balance = %f\n",a1.balance);
			getchar();
			lock.l_type=F_UNLCK;
			i =fcntl(fd,F_SETLK,&lock);
			printf("Unlocked\n");
			check=1;
		}
		else
			{printf("Low balance\n");check=0;}

		write(nsd,&check,sizeof(check));
		close(fd);
	}
		return check;
}
void balanceenquiry(int nsd,int userID,int type){
	if(type==1){
		normal a1;
		int fd = open("normal_users",O_RDONLY);
		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(normal));
		printf("balance = %f",a1.balance);
		write(nsd,&a1.balance,sizeof(a1.balance));
	close(fd);
	}
	else if(type==2){
		joint a1;
		int fd = open("joint_users",O_RDONLY);
		lseek(fd,(userID-2000)*(sizeof(joint)),SEEK_SET);
		read(fd,&a1,sizeof(joint));
		printf("balance = %f",a1.balance);
		write(nsd,&a1.balance,sizeof(a1.balance));
	        close(fd);
	}
	}
void passwordchange(int nsd,int userID,int type){
	int check=0;char npwd[30],pwd[30];
	printf("i have entered to check");
	printf("type %d\n",type);
	if(type==1){
		normal a1;
		printf("i have entered to check");
		int fd = open("normal_users",O_RDWR);
		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(normal));
		read(nsd,&pwd,sizeof(pwd));
		printf("password%s",a1.password);
		printf("pwd%s",pwd);
		if(strcmp(a1.password,pwd)==0){
			check=1;
			write(nsd,&check,sizeof(check));
			read(nsd,&npwd,sizeof(npwd));
			strcpy(a1.password,npwd);


			//locking
			struct flock lock;
			lock.l_type =F_WRLCK; // write flock
			lock.l_whence=SEEK_SET;
			lock.l_start=(userID-1000)*sizeof(normal);
			lock.l_len=sizeof(normal);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			printf("Entered the critical section\n");

			lseek(fd,-(sizeof(normal)),SEEK_CUR);
			write(fd,&a1,sizeof(normal));

			lock.l_type=F_UNLCK;
			int j=fcntl(fd,F_SETLK,&lock);//unlocking

			printf("Valid password\n");
			check=1;}
		else
			printf("Invalid password\n");

	close(fd);
	}
	else if(type==2){
		joint a1;
		int fd = open("joint_users",O_RDWR);
		lseek(fd,(userID-2000)*(sizeof(joint)),SEEK_SET);
		read(fd,&a1,sizeof(joint));
		read(nsd,&pwd,sizeof(pwd));
		printf("password%s",a1.password);
		printf("pwd%s",pwd);
		if(strcmp(a1.password,pwd)==0){
			check=1;
			write(nsd,&check,sizeof(check));
			read(nsd,&npwd,sizeof(npwd));
			strcpy(a1.password,npwd);


			//locking
			struct flock lock;
			lock.l_type =F_WRLCK; // write flock
			lock.l_whence=SEEK_SET;
			lock.l_start=(userID-2000)*sizeof(joint);
			lock.l_len=sizeof(joint);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			printf("Entered the critical section\n");

			lseek(fd,-(sizeof(joint)),SEEK_CUR);
			write(fd,&a1,sizeof(joint));

			lock.l_type=F_UNLCK;
			int j=fcntl(fd,F_SETLK,&lock);//unlocking
			check=1;

			printf("Valid password\n");
			check=1;}
		else
			printf("Invalid password\n");

	close(fd);
	}
	else if(type==3){
		admin a1;
		int fd = open("admins_list",O_WRONLY);
		lseek(fd,userID*(sizeof(admin)),SEEK_SET);
		read(fd,&a1,sizeof(admin));
		if(strcmp(a1.password,pwd)==0){
			read(nsd,&npwd,sizeof(npwd));
			strcpy(a1.password,npwd);


			//locking
			struct flock lock;
			lock.l_type =F_WRLCK; // write flock
			lock.l_whence=SEEK_SET;
			lock.l_start=(userID-1000)*sizeof(admin);
			lock.l_len=sizeof(admin);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			printf("Entered the critical section\n");

			lseek(fd,-(sizeof(admin)),SEEK_CUR);
			write(fd,&a1,sizeof(admin));

			lock.l_type=F_UNLCK;
			int j=fcntl(fd,F_SETLK,&lock);//unlocking
			check=1;

			printf("Valid password\n");
			check=1;}
		else
			printf("Invalid password\n");

	close(fd);
	}
		printf("check==%d\n",check);
		return;
}
void viewdetails(int nsd,int userID,int type){
	int check=0;
	if(type==1){
		normal a1;
		int fd = open("normal_users",O_RDONLY);
		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		write(nsd,&a1,sizeof(normal));
		check=1;
		close(fd);
	}
	else if(type==2){
		joint a1;
		int fd = open("joint_users",O_RDONLY);
		lseek(fd,(userID-2000)*(sizeof(joint)),SEEK_SET);
		write(nsd,&a1,sizeof(joint));
		check=1;
		close(fd);
	}
	write(nsd,&check,sizeof(check));
	return ;
	}

void Exit(int nsd){
		write(1,"Client disconnected\n",sizeof("Client disconnected\n"));
		close(nsd);
		exit(0);
		}

void adduser(int nsd,int adminID){
	int type,check=0;
	read(nsd,&type,sizeof(type));
	if(type==1)
	{
		normal a1,a2;
		int id=1000,accno = 12345;
		read(nsd,&a2,sizeof(normal));
		int fd = open("normal_users",O_CREAT|O_RDWR,0744);
		lseek(fd,-(sizeof(normal)),SEEK_END);
		read(fd,&a1,sizeof(normal));
		id = a1.userID;
		a2.userID = id+1;
		accno = a1.accountNo;
		a2.accountNo = accno+1;
		a2.balance = 0;
		a2.status = true;
		struct flock lock;
		lock.l_type =F_WRLCK;
		lock.l_whence=SEEK_END;
		lock.l_start=-sizeof(normal);
		lock.l_len=sizeof(normal);
		lock.l_pid=getpid();
		int i=fcntl(fd,F_SETLKW,&lock);
		printf("Locked\n");
		lseek(fd,0,SEEK_END);
		write(fd,&a2,sizeof(normal));

		lock.l_type=F_UNLCK;
		i=fcntl(fd,F_SETLKW,&lock);
		printf("Unlocked\n");
		if(i!=-1)check=1;
		close(fd);
		write(nsd,&a2,sizeof(normal));
	}
	else if(type==2){
		joint a1,a2;
		int id = 2000,accno = 23456;
		int fd = open("joint_users",O_RDWR);
		lseek(fd,-(sizeof(joint)),SEEK_END);
		read(fd,&a1,sizeof(joint));
		id = a1.userID;
		a2.userID = id+1;
		accno = a1.accountNo;
		a2.accountNo = accno+1;
		a2.balance = 0;
		a2.status = true;
		struct flock lock;
		lock.l_type =F_WRLCK;
		lock.l_whence=SEEK_END;
		lock.l_start=-sizeof(joint);
		lock.l_len=sizeof(joint);
		lock.l_pid=getpid();
		int i=fcntl(fd,F_SETLKW,&lock);
		printf("Locked\n");

		write(fd,&a2,sizeof(joint));

		lock.l_type=F_UNLCK;
		i=fcntl(fd,F_SETLKW,&lock);
		printf("Unlocked\n");
		if(i!=-1)check=1;
		close(fd);
		write(nsd,&a2,sizeof(joint));
	}
	write(nsd,&check,sizeof(check));
	return ;
}

void deleteuser(int nsd,int adminID){
	int type,check=0;int userID;
	read(nsd,&type,sizeof(type));
	if(type==1){
		normal a1;
		read(nsd,&userID,sizeof(userID));
		int fd = open("normal_users",O_RDWR);
		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(normal));
		if(a1.status==true)
		{
			a1.status=false;
			struct flock lock;
			lock.l_type =F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(userID-1000)*sizeof(normal);
			lock.l_len=sizeof(normal);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			printf("Entered the critical section\n");
			lseek(fd,-(sizeof(normal)),SEEK_CUR);
			write(fd,&a1,sizeof(normal));
			lock.l_type=F_UNLCK;
			int j=fcntl(fd,F_SETLK,&lock);
			printf("User deleted\n");
			check=1;
		}
		else
			printf("Invalid user\n");

		close(fd);
	}
	else if(type==2){
		joint a1,a2;
		read(nsd,&userID,sizeof(userID));
		int fd = open("joint_users",O_RDWR);
		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(joint));
		if(a1.status==true){
			a1.status=false;
			struct flock lock;
			lock.l_type =F_WRLCK;
			lock.l_whence=SEEK_SET;
			lock.l_start=(userID-1000)*sizeof(joint);
			lock.l_len=sizeof(joint);
			lock.l_pid=getpid();
			int i=fcntl(fd,F_SETLKW,&lock);
			lseek(fd,-(sizeof(joint)),SEEK_CUR);
			write(fd,&a1,sizeof(joint));
			lock.l_type=F_UNLCK;
			int j=fcntl(fd,F_SETLK,&lock);
			printf("Account deleted\n");
			check=1;
		}
		else
			printf("Invalid user\n");

		close(fd);
	}
	write(nsd,&check,sizeof(check));
	return;
}

void modifyuser(int nsd,int adminID){
	int type,check=0;
	read(nsd,&type,sizeof(type));
	if(type==1){
		normal a1,a2;
		read(nsd,&a2,sizeof(a2));
		int fd = open("normal_users",O_RDWR);
		lseek(fd,(a2.userID-1000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(normal));
		strcpy(a1.username,a2.username);
		strcpy(a1.password,a2.password);
		a1.accountNo=a2.accountNo;
		a1.balance=a2.balance;
		struct flock lock;
		lock.l_type =F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(a2.userID-1000)*sizeof(normal);
		lock.l_len=sizeof(normal);
		lock.l_pid=getpid();
		int i=fcntl(fd,F_SETLKW,&lock);
		lseek(fd,-(sizeof(normal)),SEEK_CUR);
		write(fd,&a1,sizeof(normal));
		lock.l_type=F_UNLCK;
		i=fcntl(fd,F_SETLK,&lock);
		check=1;
		close(fd);
	}
	else if(type==2){
		joint a1,a2;
		int fd = open("joint_users",O_RDWR);
		read(nsd,&a2,sizeof(a2));
		lseek(fd,(a2.userID-2000)*(sizeof(normal)),SEEK_SET);
		read(fd,&a1,sizeof(joint));
		strcpy(a1.username1,a2.username2);
		strcpy(a1.username2,a2.username2);
		strcpy(a1.password,a2.password);
		a1.accountNo=a2.accountNo;
		a1.balance=a2.balance;
		struct flock lock;
		lock.l_type =F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(a2.userID-1000)*sizeof(joint);
		lock.l_len=sizeof(joint);
		lock.l_pid=getpid();
		int i=fcntl(fd,F_SETLKW,&lock);
		lseek(fd,-(sizeof(joint)),SEEK_CUR);
		write(fd,&a1,sizeof(joint));
		lock.l_type=F_UNLCK;
		i=fcntl(fd,F_SETLK,&lock);
		check=1;
		close(fd);
	}
	write(nsd,&check,sizeof(check));
	return ;
	}

void searchuser(int nsd,int adminID){
	int type,check=0;int userID;
	read(nsd,&type,sizeof(type));
	if(type==1){
		normal a1;
		read(nsd,&userID,sizeof(userID));
		int fd = open("normal_users",O_RDONLY);
		lseek(fd,(userID-1000)*(sizeof(normal)),SEEK_SET);
		write(nsd,&a1,sizeof(normal));
		check=1;
		close(fd);
	}
	else if(type==2){
		joint a1;
		read(nsd,&userID,sizeof(userID));
		int fd = open("joint_users",O_RDONLY);
		lseek(fd,(userID-2000)*(sizeof(joint)),SEEK_SET);
		write(nsd,&a1,sizeof(joint));
		check=1;
		close(fd);
	}
	write(nsd,&check,sizeof(check));
	return ;
	}

int main(){

	int clilen,sd,nsd,len;
	char buff[25];
	struct sockaddr_in serv,cli;
   	 sd = socket(AF_INET,SOCK_STREAM,0);

    	serv.sin_family = AF_INET;
    	serv.sin_addr.s_addr = INADDR_ANY;
    	serv.sin_port = htons(5555);
    	bind(sd,(struct sockaddr*)&serv,sizeof(serv));

    	listen(sd,5);
    	write(1,"Waiting for clients\n",sizeof("Waiting for clients\n"));

	while(1){
		clilen = sizeof(cli);
		nsd=accept(sd,(struct sockaddr *)&cli,&clilen);
		if(!fork()){
		int type=0,userID,option=0,i=0,choice=0;
		char password[30];
		write(1,"Successfully entered server\n",sizeof("Successfully entered server"));
		read(nsd,&type,sizeof(type));
		while(1){

		if(type==1){
			normal nu;
			read(nsd,&userID,sizeof(nu.userID));
			read(nsd,&password,sizeof(nu.password));
			printf("userID = %d\n",userID);
			i=tocheck(nsd,userID,type,password);
			break;
		}
		else if(type==2){
			joint ju;
			read(nsd,&userID,sizeof(ju.userID));
			read(nsd,&password,sizeof(ju.password));
			printf("userID = %d\n",userID);
			i=tocheck(nsd,userID,type,password);
			break;
		}
		else if(type==3){
			admin ad;
			read(nsd,&userID,sizeof(ad.adminID));
			read(nsd,&password,sizeof(ad.password));
			printf("userID = %d\n",userID);
			i=tocheck(nsd,userID,type,password);
			break;
		}
		}
		while(1){
		  if(type==1 || type==2)
			{
			read(nsd,&option,sizeof(option));
			if(option==1)
				{deposit(nsd,userID,type);}
			else if(option==2)
				{withdraw(nsd,userID,type);}
			else if(option==3)
				{balanceenquiry(nsd,userID,type);}
			else if(option==4)
				{passwordchange(nsd,userID,type);}
			else if(option==5)
				{viewdetails(nsd,userID,type);}
			else if(option==6)
				Exit(nsd);
			}
	    else if(type==3)
			{
			read(nsd,&choice,sizeof(choice));
			if(choice==1)
				{adduser(nsd,userID);}
			else if(choice==2)
				deleteuser(nsd,userID);
			else if(choice==3)
				modifyuser(nsd,userID);
			else if(choice==4)
				searchuser(nsd,userID);
			else if(choice==5)
				passwordchange(nsd,userID,type);
			else if(choice==6)
				Exit(nsd);
			}
			}
		}
	else close(nsd);
	}
}
