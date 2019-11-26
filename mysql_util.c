/*
 ============================================================================
 Name        : mysql_util.c
 Author      : ljf
 Version     :
 Copyright   : Your copyright notice
 Description : Connect Mysql utils
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql_util.h"

MYSQL *g_conn; // mysql 连接
MYSQL_RES *g_res; // mysql 记录集
MYSQL_ROW g_row; // 字符串数组，mysql 记录行

#define MAX_BUF_SIZE 1024 // 缓冲区最大字节数

const char *g_host_name = "localhost";
const char *g_user_name = "root";
const char *g_password = "123456";
const char *g_db_name = "fastadmin";
const unsigned int g_db_port = 3306;

void print_mysql_error(const char *msg) { // 打印最后一次错误
    if (msg)
        printf("%s: %s\n", msg, mysql_error(g_conn));
    else
        puts(mysql_error(g_conn));
}

int executesql(const char * sql) {
    /*query the database according the sql*/
    if (mysql_real_query(g_conn, sql, strlen(sql))) // 如果失败
        return -1; // 表示失败

    return 0; // 成功执行
}

int list_thinfo()
{
	char sql[MAX_BUF_SIZE];
    if (executesql("select * from fa_datamanagement_thinfo")) // 句末没有分号
        print_mysql_error(NULL);

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	int iNum_rows = mysql_num_rows(g_res); // 得到记录的行数
    int iNum_fields = mysql_num_fields(g_res); // 得到记录的列数

    printf("共%d个记录，每个记录%d字段\n", iNum_rows, iNum_fields);

    printf("id\t编号\t温度\t湿度\t更新时间\n");

    while ((g_row=mysql_fetch_row(g_res))) // 打印结果集
        printf("%s\t%s\t%s\t%s\t%s\n", g_row[0], g_row[1],g_row[2],g_row[3],g_row[4]); // 第一，第二字段

    mysql_free_result(g_res); // 释放结果集

}

int add_thinfo(char *devId, char *temp, char *humi, char *timeStamp)
{
	int ret = 0;
	char sql[MAX_BUF_SIZE] = {0};
	int len = sprintf(sql, 
						"INSERT INTO `fa_datamanagement_thinfo` (`devId`, `temp`, `humi`, `timeStamp`, `admin_id`) VALUES ('%s','%s','%s','%s',1)",
						devId,
						temp,
						humi,
						timeStamp
					);
    if (executesql(sql))
    {
		print_mysql_error(NULL);
		ret = -1;
	}
    return ret;
}

/*

初始化mysql连接

*/ 
int init_mysql()
{ 
	char passwd[32] = {0};
	printf("Please input the mysql's password:\n");
	scanf("%s",passwd);
	//printf("passwd:%s\n", passwd);
    // init the database connection
    g_conn = mysql_init(NULL);
    /*设置字符编码,可能会乱码*/
    mysql_query(g_conn,"set names utf-8");
    /* connect the database */
    if(!mysql_real_connect(g_conn, g_host_name, g_user_name, passwd, g_db_name, g_db_port, NULL, 0)) // 如果失败
    {
    	printf("connect failed!!\n");
        return -1;
    }
    // 是否连接已经可用
    //if (executesql("set names utf8")) // 如果失败
       // return -1;
    return 0; // 返回成功
}

int deinit_mysql()
{
	mysql_close(g_conn); // 关闭链接
}

