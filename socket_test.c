/** @file socket_test.c
 *
 *  @brief
 *  @author bo.liu
 *  @date   2019/10/25
 *  COPYRIGHT NOTICE: (c) smartlogictech.
 *  All rights reserved.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>

#define	SA	      (struct sockaddr *)
#define  MAX_BUFFER_LENGTH     2048
char *vm_addr1 = "192.168.1.168";
char *vm_addr2= "192.168.84.129";
char *PC_src_ip = "192.168.1.135";
char *PC_dst_ip = "192.168.84.1";
char msg_buffer[MAX_BUFFER_LENGTH] = {0};
char msg_buffer_2[MAX_BUFFER_LENGTH] = {0};
char pc_addr_ip[32] = {0};
int main(void)
{
	int sockfd_1;
	int sockfd_2;
	struct sockaddr_in sver_addr, cli_addr;
	struct sockaddr_in PC_addr_src, PC_addr_dst;
	int  recv_length;
	int recv_length_2;
        int  PC_addr_src_length;
	int  PC_addr_dst_length;
	fd_set    rset;
        fd_set    wset;
	int       max_fd;
 	unsigned int  recv_cnt[2] = {0};
        unsigned int  send_cnt[2] = {0};

	FD_ZERO(&rset);
	FD_ZERO(&wset);

	sockfd_1 = socket(AF_INET,SOCK_DGRAM,0);
	sockfd_2 = socket(AF_INET,SOCK_DGRAM,0);

	bzero(&sver_addr,sizeof(sver_addr));
	bzero(&cli_addr,sizeof(cli_addr));
	bzero(&PC_addr_src,sizeof(PC_addr_src));
	bzero(&PC_addr_dst,sizeof(PC_addr_dst));

	sver_addr.sin_family = AF_INET;
	inet_pton(AF_INET, vm_addr1,&(sver_addr.sin_addr.s_addr));
	sver_addr.sin_port = htons(1234);

	cli_addr.sin_family = AF_INET;
	inet_pton(AF_INET, vm_addr2,&(cli_addr.sin_addr.s_addr));
	cli_addr.sin_port = htons(1234);

	PC_addr_src.sin_family = AF_INET;
	inet_pton(AF_INET, PC_src_ip,&(PC_addr_src.sin_addr.s_addr));
	PC_addr_src.sin_port = htons(9999);


	PC_addr_dst.sin_family = AF_INET;
	inet_pton(AF_INET, PC_dst_ip,&(PC_addr_dst.sin_addr.s_addr));
	PC_addr_dst.sin_port = htons(8888);

	bind(sockfd_1,SA&sver_addr,sizeof(sver_addr));
	bind(sockfd_2,SA&cli_addr,sizeof(cli_addr));



    //sver_addr 用来接收主机的IP1 传输的数据，然后发送给vm_addr2,
       // printf(" ip = 0x%x, port = 0x%x\n",sver_addr.sin_addr.s_addr, sver_addr.sin_port);

	for (;;)
	{
		FD_SET(sockfd_1,&rset);
		FD_SET(sockfd_2,&rset);
		max_fd = (sockfd_1>=sockfd_2) ? sockfd_1:sockfd_2;
	    	max_fd = max_fd +1;
		select(max_fd, &rset,NULL,NULL,NULL);
		if (FD_ISSET(sockfd_1,&rset))
		{

               // printf("msg buffer = %s\n",msg_buffer);
		//printf("fd = %d,start receive data in 0x%x,length = %d \n",sockfd_1,msg_buffer,sizeof(msg_buffer));
			printf("start receive ip from %s\n",PC_src_ip);
			recv_length = recvfrom(sockfd_1,msg_buffer,MAX_BUFFER_LENGTH,0,NULL,NULL); //!获取PC 发送的地址
			inet_ntop(AF_INET,&PC_addr_src.sin_addr.s_addr, pc_addr_ip,sizeof(pc_addr_ip));
			if (0 == errno)
	 		{
		                printf("receive data from ip:%s, port: %d, length:%d ! \n",pc_addr_ip,ntohs(PC_addr_src.sin_port),recv_length);
				printf("channel:0 -- receive data no.: %d \n",recv_cnt[0]++);
				sendto(sockfd_2,msg_buffer,recv_length,0,SA&PC_addr_dst,sizeof(PC_addr_dst));
	 			printf("channel:0 -- send data no.: %d \n",send_cnt[0]++);

				if (0 != errno)
				{
					printf("channel 0--send errno = %d\n", errno);
				}
			}
		}else
		{
 			printf("channel 0: receive errno = %d\n", errno);
    		}


		if (FD_ISSET(sockfd_2,&rset))
		{

		       // printf("msg buffer = %s\n",msg_buffer);
			//printf("fd = %d,start receive data in 0x%x,length = %d \n",sockfd_1,msg_buffer,sizeof(msg_buffer));
			printf("start receive ip from %s\n",PC_dst_ip);
			recv_length_2 = recvfrom(sockfd_2,msg_buffer_2,MAX_BUFFER_LENGTH,0,NULL,NULL); //!获取PC 发送的地址
			inet_ntop(AF_INET,&PC_addr_dst.sin_addr.s_addr, pc_addr_ip,sizeof(pc_addr_ip));
 			if (0 == errno)
			{
				printf("receive data from ip:%s, port: %d, length:%d ! \n",pc_addr_ip,ntohs(PC_addr_dst.sin_port),recv_length_2);
				printf("channel:1 -- receive data no.: %d \n",recv_cnt[1]++);
				sendto(sockfd_1,msg_buffer_2,recv_length_2,0,SA&PC_addr_src,sizeof(PC_addr_src));
				printf("channel:1 -- send data no.: %d \n",send_cnt[1]++);
				if (0 != errno)
				{
					printf("channel 1: send errno = %d\n",errno);
				}
			}
			else
			{
				printf("channel 1: receive errno = %d\n",errno);
			}
		}
	}
}
