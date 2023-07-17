

#include <stdio.h>
#include "mysql.h"
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "libmysql.lib")
#define TODAY_YEAR 2022
#define TODAY_MONTH 06
#define TODAY_DAY 18

typedef struct {
	char payment_ID[20];
	char store_ID[20];
	char product_ID[20];
	char customer_ID[20];
	char payment_year[20];
	char payment_month[20];
	char payment_day[20];
}payment_info;
typedef struct {
	char payment_ID[20];
	char tracking_number[20];
	char due_year[20];
	char due_month[20];
	char due_day[20];
	char delivered_year[20];
	char delivered_month[20];
	char delivered_day[20];
}delivery_info;
typedef struct {
	char package_ID[20];
	char component_ID[20];
	char ammount[10];
};
const char* host = "localhost";
const char* user = "root";
const char* pw = "1q2w3e4r";
const char* db = "project";
MYSQL* connection = NULL;
MYSQL conn;
MYSQL_RES* sql_result;
MYSQL_ROW sql_row;


void initialization();
void read_input_file();
void package_decomposite();
void execute();
void type1();
void type2();
void type3();
void type_3_1(int );
void type_3_2(int );
void type4();
void type_4_1(int);
void type_4_2(int);
void type5();
void type6();
void type7();
void quit();
void read_drop_file();

int main() {
	initialization();
	execute();
	quit();
}
void initialization() {
	
	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		exit(0);
	}
	else
	{
		printf("Connection Succeed\n");
		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			exit(0);
		}

		read_input_file();

	}
	printf("������ %d�� %d�� %d���Դϴ�.\n", TODAY_YEAR, TODAY_MONTH, TODAY_DAY);
	package_decomposite();
}
void read_input_file() {
	FILE* file_pointer = NULL;
	file_pointer = fopen("20170024.txt", "r");
	if (file_pointer != NULL) {
		char buffer[500];
		char* ptr;
		while (!feof(file_pointer)) {
			ptr = fgets(buffer, sizeof(buffer), file_pointer);
			mysql_query(connection, ptr);
		}
		fclose(file_pointer);
	}
	else {
		printf("file reading error\n");
		exit(0);
	}
}
void package_decomposite() {
	int state;
	char order_for_decomposite_payment[300];
	char order_for_decomposite_delivery[300];
	payment_info saved_payment_info[10];
	delivery_info saved_delivery_info[10];
	sprintf(order_for_decomposite_delivery, "select * from delivery where payment_ID in (select payment_ID from payment where product_ID like '%%package') order by payment_ID;");
	sprintf(order_for_decomposite_payment, "select * from payment where product_ID like '%%package' order by payment_ID;");
	
	state = mysql_query(connection, order_for_decomposite_payment);

	int payment_cnt = 0;
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			strcpy(saved_payment_info[payment_cnt].payment_ID, sql_row[0]);
			strcpy(saved_payment_info[payment_cnt].store_ID, sql_row[1]);
			strcpy(saved_payment_info[payment_cnt].product_ID, sql_row[2]);
			strcpy(saved_payment_info[payment_cnt].customer_ID, sql_row[3]);
			strcpy(saved_payment_info[payment_cnt].payment_year, sql_row[4]);
			strcpy(saved_payment_info[payment_cnt].payment_month, sql_row[5]);
			strcpy(saved_payment_info[payment_cnt].payment_day, sql_row[6]);
			payment_cnt++;
		}
		mysql_free_result(sql_result);
	}
	int delivery_cnt = 0;
	state = mysql_query(connection, order_for_decomposite_delivery);
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			strcpy(saved_delivery_info[delivery_cnt].payment_ID, sql_row[0]);
			strcpy(saved_delivery_info[delivery_cnt].tracking_number, sql_row[1]);
			strcpy(saved_delivery_info[delivery_cnt].due_year, sql_row[2]);
			strcpy(saved_delivery_info[delivery_cnt].due_month, sql_row[3]);
			strcpy(saved_delivery_info[delivery_cnt].due_day, sql_row[4]);
			strcpy(saved_delivery_info[delivery_cnt].delivered_year, sql_row[5]);
			strcpy(saved_delivery_info[delivery_cnt].delivered_month, sql_row[6]);
			strcpy(saved_delivery_info[delivery_cnt].delivered_day, sql_row[7]);
			delivery_cnt++;
		}
		mysql_free_result(sql_result);
	}
	int check = 0;
	int d_cnt = 0;
	for (int i = 0; i < payment_cnt; i++) {
		check = 0;
		char find_payment_in_delivery[500];
		sprintf(find_payment_in_delivery, "select* from delivery where payment_ID in(select payment_ID from payment where payment_ID = '%s');", saved_payment_info[i].payment_ID);
		state = mysql_query(connection, find_payment_in_delivery);
		if (state == 0) {
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				check = 1;
			}
			mysql_free_result(sql_result);
		}

		if (check == 1) {//���̸�Ʈ�� ������������ �ִٸ�
			/*
			   ���������� �������ְ�
			   ���̸�Ʈ�� �������ְ�

			   ���̸�Ʈ�� �߰����ְ� 
			   ���������� �߰����ְ�
			   �̰� �߰��ϴ� �������� ���̸�Ʈid�� ���ݾ� �ٲ㰡�鼭 �߰����ֱ� ����. �������������� �� �Ȱ������ְ�.
			*/
			char order[500];
			sprintf(order, "delete from delivery where payment_ID='%s';", saved_delivery_info[d_cnt].payment_ID);
			mysql_query(connection, order);
			sprintf(order, "delete from payment where payment_ID='%s';", saved_payment_info[i].payment_ID);
			mysql_query(connection, order);

			int ammount;
			//payment �߰�
			sprintf(order, "select component_ID,ammount from package where product_ID='%s';", saved_payment_info[i].product_ID);
			state = mysql_query(connection, order);
			if (state == 0) {
				int ID_increase_cnt = 0;
				
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
					/// <summary>
					/// ////////////////////////////////////�������ϰ�, �Ʒ����� �߰��ϴ��������� tissue ��Ű���� �ѹ��� �߰��� �̰� 10���߰��ǰ� �ٲٱ�
					/// </summary>
					char in_order[500];
					int ammount_increase_cnt = 0;
					char ammount_str[10];
					sprintf(ammount_str, "%s", sql_row[1]);
					int ammount = atoi(ammount_str);
					for (int j = 0; j < ammount; j++) {
						sprintf(in_order, "insert into payment values('%s^%d*%d','%s','%s','%s','%s','%s','%s');", saved_payment_info[i].payment_ID, ID_increase_cnt,ammount_increase_cnt, saved_payment_info[i].store_ID, sql_row[0], saved_payment_info[i].customer_ID, saved_payment_info[i].payment_year, saved_payment_info[i].payment_month, saved_payment_info[i].payment_day);
						mysql_query(connection, in_order);
						sprintf(in_order, "insert into delivery values('%s^%d*%d','%s','%s','%s','%s','%s','%s','%s');", saved_payment_info[i].payment_ID, ID_increase_cnt++,ammount_increase_cnt++, saved_delivery_info[d_cnt].tracking_number, saved_delivery_info[d_cnt].due_year, saved_delivery_info[d_cnt].due_month, saved_delivery_info[d_cnt].due_day, saved_delivery_info[d_cnt].delivered_year, saved_delivery_info[d_cnt].delivered_month, saved_delivery_info[d_cnt].delivered_day);
						mysql_query(connection, in_order);
					}
					
				}
				mysql_free_result(sql_result);
			}

			//delivery �߰�


			d_cnt++;
		}
		else {
			/*
				���̸�Ʈ �������ְ�
				���̸�Ʈ �߰����ְ�
				�̰� �߰��ϴ� �������� ���̸�Ʈ id�� ���ݾ� �ٲ㰡�� �߰����ֱ�. 
			*/
			char order[500];
			sprintf(order, "delete from payment where payment_ID='%s';", saved_payment_info[i].payment_ID);
			mysql_query(connection, order);

			sprintf(order, "select component_ID,ammount from package where product_ID='%s';", saved_payment_info[i].product_ID);

			state = mysql_query(connection, order);
			if (state == 0) {
				int ID_increase_cnt = 0;
				
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
					int ammount_increase_cnt = 0;
					char ammount_str[10];
					sprintf(ammount_str, "%s", sql_row[1]);
					int ammount = atoi(ammount_str);
					char in_order[500];
					for (int j = 0; j < ammount; j++) {
						sprintf(in_order, "insert into payment values('%s^%d*%d','%s','%s','%s','%s','%s','%s');", saved_payment_info[i].payment_ID, ID_increase_cnt++, ammount_increase_cnt++, saved_payment_info[i].store_ID, sql_row[0], saved_payment_info[i].customer_ID, saved_payment_info[i].payment_year, saved_payment_info[i].payment_month, saved_payment_info[i].payment_day);
						mysql_query(connection, in_order);
					}
					
				}
				mysql_free_result(sql_result);
			}
		}
		
	}

}
void execute() {
	int order;
	while (1) {
		printf("------- SELECT QUERY TYPES -------\n\n");
		printf("\t1. TYPE 1\n");
		printf("\t2. TYPE 2\n");
		printf("\t3. TYPE 3\n");
		printf("\t4. TYPE 4\n");
		printf("\t5. TYPE 5\n");
		printf("\t6. TYPE 6\n");
		printf("\t7. TYPE 7\n");
		printf("\t0. QUIT\n");
		printf("----------------------------------\n");
		scanf("%d", &order);
		printf("\n\n");
		printf("---- TYPE %d ----\n\n",order);
		switch (order) {
		case 0:
			return;
		case 1: 
			type1();
			break;
		case 2:
			type2();
			break;
		case 3:
			type3();
			break;
		case 4:
			type4();
			break;
		case 5:
			type5();
			break;
		case 6:
			type6();
			break;
		case 7:
			type7();
			break;
		default: 
			printf("�߸��� �Է�\n");
			break;
		}
	}
}
void type1() {
	const char* query = "select distinct tracking_number from delivery where delivered_month is null;";
	int state = 0;
	state = mysql_query(connection, query);
	char onging_tracking_number[10][10];
	int cnt = 0;
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			strcpy(onging_tracking_number[cnt], sql_row[0]);
			cnt++;
		}
		mysql_free_result(sql_result);
	}
	for (int j = 0; j < cnt; j++) {
		printf("���� ����� %d: %s\n", j+1,onging_tracking_number[j]);
	}
	printf("� ����� ��ĥ ������ �����Ͻÿ�: ");
	int choose;
	scanf("%d", &choose);
	if (choose <= 0 || choose > cnt) {
		printf("�ش� ����� �����ϴ�.\n");
		return;
	}
	printf("��۹�ȣ %s�� ��� ���� ��� �����ϴ�.\n", onging_tracking_number[choose - 1]);
	
	char type1_order[500];
	sprintf(type1_order, "select customer_ID, phone_number from customer where customer_ID in (select distinct customer_ID from payment where payment_ID in (select payment_ID from delivery where tracking_number=%s));", onging_tracking_number[choose - 1]);
	state = mysql_query(connection, type1_order);
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("�� %s�� ����ó: %s\n", sql_row[0], sql_row[1]);
		}
		mysql_free_result(sql_result);
	}


	//type1-1 ����
	
	char type1_1_order[500];
	payment_info saved_payment_info[5];
	cnt = 0;
	sprintf(type1_1_order, "select * from payment where payment_ID in (select payment_ID from delivery where tracking_number=%s);", onging_tracking_number[choose - 1]);
	state = mysql_query(connection, type1_1_order);
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			strcpy(saved_payment_info[cnt].payment_ID, sql_row[0]);
			strcpy(saved_payment_info[cnt].store_ID, sql_row[1]);
			strcpy(saved_payment_info[cnt].product_ID, sql_row[2]);
			strcpy(saved_payment_info[cnt].customer_ID, sql_row[3]);
			cnt++;
			//printf("%s %s %s %s %s %s %s \n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6]);
		}
		mysql_free_result(sql_result);
	}
	/*
		payment�� ������ �� delivery�� �����Ǿ��ֱ� ������
		delete from delivery where payment_ID='d';
		delete from payment where payment_ID = 'd';
		�̷��� delivery���� ���� ������ �� payment�� ����������Ѵ�.
	*/
	for (int i = 0; i < cnt; i++) {
		char temp[500];
		sprintf(temp, "delete from delivery where payment_ID='%s';", saved_payment_info[i].payment_ID);
		mysql_query(connection, temp);
		sprintf(temp, "delete from payment where payment_ID = '%s';", saved_payment_info[i].payment_ID);
		mysql_query(connection, temp);
	}
	int random = rand() % 9998 + 1;
	for (int i = 0; i < cnt; i++) {
		strcat(saved_payment_info[i].payment_ID, "_re");
		char temp[500];
		sprintf(temp, "insert into payment values('%s','%s','%s','%s',%d,%d,%d);", saved_payment_info[i].payment_ID, saved_payment_info[i].store_ID, saved_payment_info[i].product_ID, saved_payment_info[i].customer_ID, TODAY_YEAR, TODAY_MONTH, TODAY_DAY );
		mysql_query(connection, temp);
		sprintf(temp, "insert into delivery values('%s',%d, % d, % d, % d, % s, % s, % s);", saved_payment_info[i].payment_ID, random, TODAY_YEAR, TODAY_MONTH+1, TODAY_DAY, "null","null","null");
		mysql_query(connection, temp);
	}
	printf("�ֹ��� ���� �־����ϴ�. �Ѵ� �� ��� �� �����Դϴ�\n");

	for (int i = 0; i < cnt; i++) {
		char temp[500];
		//sprintf(temp, "select * from payment where payment_ID='%s';", saved_payment_info[i].payment_ID);
		

		sprintf(temp, "select * from delivery where payment_ID='%s';", saved_payment_info[i].payment_ID);
		state = mysql_query(connection, temp);
		if (state == 0) {
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
				printf("���ο� ������� - payment_id: '%s' tracking number: %s ��ۿ����� %s/%s/%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
			}
			mysql_free_result(sql_result);
		}
	}



	//���� �� Payment�����߿��� �ʿ��Ѱ� �߷��� ���� �ֹ��ϸ�ɵ�

}
void type2() {
	int state;
	char type2_order[500];
	char vip_id[100];
	
	sprintf(type2_order, "select customer_ID,sum(price) from payment as p,product as t where (p.product_ID=t.product_ID and p.payment_year=%d)group by customer_ID order by sum(price) desc limit 1;", TODAY_YEAR-1);
	state = mysql_query(connection, type2_order);	
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			strcpy(vip_id, sql_row[0]);
			printf("�� %s�� %d�⿡ %s������ ���� ������ ���̿÷�����ϴ�.\n",vip_id,TODAY_YEAR-1,sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
	char type2_1_order[500];
	state = 0;
	sprintf(type2_1_order, "select customer_ID,p.product_ID,sum(price) from payment as p,product as t where t.product_ID=p.product_ID and customer_id='%s' and payment_year=%d group by t.product_ID order by sum(price) desc limit 1 ;", vip_id, TODAY_YEAR - 1);

	state = mysql_query(connection, type2_1_order);
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("�� %s�� %d�⿡ ���� ���� �ݾ��� ������ ��ǰ�� %s�� %s���� �����߽��ϴ�.\n", sql_row[0], TODAY_YEAR - 1, sql_row[1], sql_row[2]);
		}

		mysql_free_result(sql_result);
	}
	return;
}
void type3() {
	int state;
	char type3_order[500];
	char vip_id[100];

	sprintf(type3_order, "select pro.product_id, sum(price) from payment as pay, product as pro where (pro.product_ID=pay.product_id and payment_year=%d) group by pro.product_id order by sum(price) desc;", TODAY_YEAR - 1);
	printf("�۳⿡ �� ��� ��ǰ���� ���\n");
	int cnt = 0;
	state = mysql_query(connection, type3_order);
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("%s\n", sql_row[0]);
			cnt++;
		}
		mysql_free_result(sql_result);
	}

	printf("---------subtypes in TYPE 3 ---------\n");
	printf("\t 1. type 3-1\n");
	printf("\t 2. type 3-2\n");
	int choose;
	scanf("%d", &choose);
	if (choose == 1) type_3_1(cnt);
	else if (choose == 2) type_3_2(cnt);
	else return;

}
void type_3_1(int range) {
	printf("--------type 3-1---------\n");
	printf("** Then, find the top k products by won ammount\n");
	int k;
	printf("Which K? ( in range 0 to %d the number of product we sell in %d ): ",range,TODAY_YEAR-1);
	scanf("%d", &k);
	if (k > range || k <= 0) {
		printf("wrong input\n");
		return;
	}

	int state;
	char type3_order[500];
	char vip_id[100];

	sprintf(type3_order, "select pro.product_id, sum(price) from payment as pay, product as pro where (pro.product_ID=pay.product_id and payment_year=%d) group by pro.product_id order by sum(price) desc limit %d", TODAY_YEAR - 1,k);
	state = mysql_query(connection, type3_order);
	int cnt = 0;
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			cnt++;
			printf("%d��: %s, %s����!\n",cnt, sql_row[0],sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
	
}
void type_3_2(int range) {
	printf("--------type 3-2---------\n");
	int state;
	char type3_order[500];
	char vip_id[100];
	int target = range / 10;
	printf("�۳⿡ �� �Ǹŵ� ��ǰ�� ������ %d������, ���� 10�ۼ�Ʈ �� ���� %d���� ��ǰ���!!\n", range, target);
	sprintf(type3_order, "select pro.product_id, sum(price) from payment as pay, product as pro where (pro.product_ID=pay.product_id and payment_year=%d) group by pro.product_id order by sum(price) desc limit %d;", TODAY_YEAR - 1,target);
	state = mysql_query(connection, type3_order);
	int cnt = 0;
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			cnt++;
			printf("%d��: %s, %s����!\n", cnt,sql_row[0],sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
}
void type4() {
	int state;
	char type4_order[500];

	sprintf(type4_order, "select product_ID, count(payment_ID) from payment where payment_ID in (select payment_ID from payment where payment_year=2021) group by product_ID;");
	printf("�۳⿡ �� ��� ��ǰ���� ���\n");
	int cnt = 0;
	state = mysql_query(connection, type4_order);
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("%s: %s��\n", sql_row[0], sql_row[1]);
			cnt++;
		}
		mysql_free_result(sql_result);
	}


	printf("---------subtypes in TYPE 4 ---------\n");
	printf("\t 1. type 4-1\n");
	printf("\t 2. type 4-2\n");
	int choose;
	scanf("%d", &choose);
	if (choose == 1) type_4_1(cnt);
	else if (choose == 2) type_4_2(cnt);
	else return;
}
void type_4_1(int range) {
	printf("--------type 4-1---------\n");
	printf("** Then, find the top k products by unit sales **\n");
	int k;
	printf("Which K? ( in range 0 to %d the number of product we sell in %d ): ", range, TODAY_YEAR - 1);
	scanf("%d", &k);
	if (k > range || k <= 0) {
		printf("wrong input\n");
		return;
	}
	
	int state;
	char type3_order[500];
	char vip_id[100];

	sprintf(type3_order, "select product_ID, count(payment_ID) from payment where payment_ID in(select payment_ID from payment where payment_year = %d) group by product_ID order by count(payment_ID) desc limit %d;", TODAY_YEAR - 1, k);
	state = mysql_query(connection, type3_order);
	int cnt = 0;
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			cnt++;
			printf("%d��: %s, %s��!\n", cnt, sql_row[0], sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
}
void type_4_2(int range) {
	printf("--------type 3-2---------\n");
	int state;
	char type3_order[500];
	int target = range / 10;
	printf("�۳⿡ �� �Ǹŵ� ��ǰ�� ������ %d������, ���� 10�ۼ�Ʈ �� ���� %d���� ��ǰ���!!\n", range, target);
	sprintf(type3_order, "select product_ID, count(payment_ID) from payment where payment_ID in(select payment_ID from payment where payment_year = %d) group by product_ID order by count(payment_ID) desc limit %d;", TODAY_YEAR - 1, target);
	state = mysql_query(connection, type3_order);
	int cnt = 0;
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			cnt++;
			printf("%d��: %s, %s��!\n", cnt, sql_row[0], sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
}
void type5() {
	int state;
	char type5_order[500];

	sprintf(type5_order, "select product_ID from product where product_ID not like '%%package' and product_ID not in (select product_ID from product_stock where stock_amount>0 and store_ID in (select store_ID from store where address='california'));");
	state = mysql_query(connection, type5_order);
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("%s\n", sql_row[0]);
		}
		mysql_free_result(sql_result);
	}
	printf("�� ��ǰ�� Ķ�����Ͼ������� �����ϴ�.\n");
}
void type6() {
	int state;
	char type5_order[500];

	sprintf(type5_order, "select dil.payment_ID,tracking_number,due_year,due_month,due_day,delivered_year,delivered_month,delivered_day,product_ID from delivery as dil, payment as pay where delivered_year is not null and pay.payment_ID=dil.payment_ID;");
	state = mysql_query(connection, type5_order);
	/*
		due_year=sql_row[2]		delivered_year=sql_row[5]
		due_month=sql_row[3]	delivered_month=sql_row[6]
		due_day=sql_row[4]		delivered_day=sql_row[7]
	*/
	int rate_check = 0;
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			rate_check = 0;
			if (atoi(sql_row[2]) < atoi(sql_row[5]))
				rate_check = 1;
			else if (atoi(sql_row[3]) < atoi(sql_row[6]))
				rate_check = 1;
			else if (atoi(sql_row[4] )< atoi(sql_row[7]))
				rate_check = 1;
			else rate_check = 0;
			if (rate_check == 1) {
				printf("payment_ID: %s, tracking_number: %s, product_ID: %s \t�� ����� �����Ϻ��� �ʰ� ��۵ƽ��ϴ� ������: %s/%s/%s �����: %s/%s/%s.\n", sql_row[0], sql_row[1], sql_row[8], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7]);
			}
		}
		mysql_free_result(sql_result);
	}
}
void type7() {
	int state;
	char type5_order[500];

	sprintf(type5_order, "select customer_ID, sum(price) from payment as pay,product as pro where pay.product_ID=pro.product_ID and payment_year=%d and payment_month=%d and customer_ID in (select customer_ID from customer where account is not null) group by customer_ID;",TODAY_YEAR,TODAY_MONTH-1);
	state = mysql_query(connection, type5_order);
	if (state == 0) {
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("customer_ID: %s %d�� %s�� ����\n", sql_row[0],TODAY_MONTH-1,sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
}
void quit() {
	read_drop_file();
	mysql_close(connection);
	exit(0);
}
void read_drop_file() {
	/*FILE* file_pointer = NULL;
	file_pointer = fopen("drop.txt", "r");
	if (file_pointer != NULL) {
		char strTemp[500];
		char* pStr;
		while (!feof(file_pointer)) {
			pStr = fgets(strTemp, sizeof(strTemp), file_pointer);
			mysql_query(connection, pStr);
		}
		fclose(file_pointer);
	}
	else {
		printf("file reading error\n");
		exit(0);
	}*/
	int state;
	char order[500];
	sprintf(order, "drop table delivery;");
	mysql_query(connection, order);
	sprintf(order, "drop table product_stock;");
	mysql_query(connection, order);
	sprintf(order, "drop table payment;");
	mysql_query(connection, order);
	sprintf(order, "drop table package;");
	mysql_query(connection, order);
	sprintf(order, "drop table customer;");
	mysql_query(connection, order);
	sprintf(order, "drop table product;");
	mysql_query(connection, order);
	sprintf(order, "drop table store;");
	mysql_query(connection, order);

}