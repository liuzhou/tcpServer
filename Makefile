BIN=server
CLIENT=client
MYSQL_DEMO=mysql_demo
CFLAG = -o
all:
	${CC} -g -o ${BIN} -I/usr/include/mysql/ server.c mysql_util.c -L/usr/lib64/mysql/ -lmysqlclient -lz
	${CC} client.c ${CFLAG} ${CLIENT}
	${CC} -g -o ${MYSQL_DEMO} -I/usr/include/mysql/ mysql_demo.c mysql_util.c -L/usr/lib64/mysql/ -lmysqlclient -lz

clean:
	rm -rvf *.o ${BIN} ${CLIENT} ${MYSQL_DEMO}
