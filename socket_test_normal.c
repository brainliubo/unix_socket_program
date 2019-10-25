/** @file socket_test.c
 *
 *  @brief
 *  @author bo.liu
 *  @date   2019/10/25
 *  COPYRIGHT NOTICE: (c) smartlogictech.
 *  All rights reserved.
 */
#if 0
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>

#define	SA	      (struct sockaddr *)
#define  MAX_BUFFER_LENGTH     2048
char *vm_addr1 = "192.168.1.168";
char *vm_addr2= "192.168.84.129";
char *pc_recv_addr = "192.168.84.1";

char pc_addr_ip[32] = {0};
int main_noselect(void)
{
	int sockfd_1;
	int sockfd_2;
	struct sockaddr_in sver_addr, cli_addr;
	struct sockaddr_in PC_addr1, PC_addr2;
	int  recv_length;
        int  PC_addr1_length;
 	unsigned int  recv_cnt = 0;
        unsigned int  send_cnt = 0;
        char msg_buffer[MAX_BUFFER_LENGTH] = {0};
	sockfd_1 = socket(AF_INET,SOCK_DGRAM,0);
	sockfd_2 = socket(AF_INET,SOCK_DGRAM,0);

	bzero(&sver_addr,sizeof(sver_addr));
	bzero(&cli_addr,sizeof(cli_addr));
	bzero(&PC_addr1,sizeof(PC_addr1));
	bzero(&PC_addr2,sizeof(PC_addr2));

	sver_addr.sin_family = AF_INET;
	inet_pton(AF_INET, vm_addr1,&(sver_addr.sin_addr.s_addr));
	sver_addr.sin_port = htons(1234);

	cli_addr.sin_family = AF_INET;
	inet_pton(AF_INET, vm_addr2,&(cli_addr.sin_addr.s_addr));
	cli_addr.sin_port = htons(5678);

	PC_addr2.sin_family = AF_INET;
	inet_pton(AF_INET, pc_recv_addr,&(PC_addr2.sin_addr));
	PC_addr2.sin_port = htons(5678);

	bind(sockfd_1,SA&sver_addr,sizeof(sver_addr));
	bind(sockfd_2,SA&cli_addr,sizeof(cli_addr));

    //sver_addr 用来接收主机的IP1 传输的数据，然后发送给vm_addr2,
       // printf(" ip = 0x%x, port = 0x%x\n",sver_addr.sin_addr.s_addr, sver_addr.sin_port);

	for (;;)
	{
               // printf("msg buffer = %s\n",msg_buffer);
		//printf("fd = %d,start receive data in 0x%x,length = %d \n",sockfd_1,msg_buffer,sizeof(msg_buffer));
		//printf("start receive ip in 0x%x,length = %d \n",&PC_addr1,sizeof(PC_addr1));
		recv_length = recvfrom(sockfd_1,msg_buffer,MAX_BUFFER_LENGTH,0,SA&PC_addr1,&PC_addr1_length); //!获取PC 发送的地址
		inet_ntop(AF_INET,&PC_addr1.sin_addr.s_addr, pc_addr_ip,sizeof(pc_addr_ip));
		if (0 == errno)
 		{
                        printf("receive data from ip:%s, port: %d, length:%d ! \n",pc_addr_ip,ntohs(PC_addr1.sin_port),recv_length);
			printf("receive data no.: %d \n",recv_cnt++);
			sendto(sockfd_2,msg_buffer,recv_length,0,SA&PC_addr2,sizeof(PC_addr2));
 			printf("send data no.: %d \n",send_cnt++);
		}else {
 			printf("errno = %d\n", errno);
      		}

	}
}
#endif
