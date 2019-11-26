/*

初始化mysql连接

*/ 
#include <mysql/mysql.h>

int init_mysql();
int deinit_mysql();

int add_thinfo(char *devId, char *temp, char *humi, char *timeStamp);

int list_thinfo();

void print_mysql_error(const char *msg);

