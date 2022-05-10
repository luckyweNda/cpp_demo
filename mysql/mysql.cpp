#include <iostream>
#include <mysql/mysql.h>
#include <string>
#include <cstdio>

int main()
{
    MYSQL *mysql = NULL;
    mysql = new MYSQL;

    mysql_init(mysql);

    if(mysql == NULL)
    {
        std::cout << "Mysql object initialized error.\n";
        return 0;
    }
    else 
    {
        std::cout << "Successfully init a mysql structure\n";
    }

    if(mysql_real_connect(mysql, 
    "127.0.0.1" , 
    "root", 
    "password", 
    "", 
    3306, 
    NULL, 
    0) == NULL)
    {
        std::cout << "Mysql connect fail.\n";
        return 0;
    }
    else
    {
        std::cout << "Mysql connect success.\n";
    }

    if(mysql_set_character_set(mysql, "utf8") == 0)
    {
        std::cout << "char set successful.\n";
    }
    else
    {
        std::cout << "char set error.\n";
    }

    char query[256] = "use test;";

    if(mysql_real_query(mysql, query, sizeof(query)) != 0)
    {
        std::cout << "query failed.exit.\n";
        return 0;
    }
    else 
    {
        std::cout << "query success.\n";
    }

    sprintf(query, "SELECT * FROM family;");
    mysql_real_query(mysql, query, sizeof(query));

    MYSQL_RES *result = NULL;
    result = mysql_store_result(mysql);

    if(result == NULL)
    {
        std::cout << "result error.\n";
        return 0;
    }
    else 
    {
        std::cout << "successfully fetch result.\n";
    }

    MYSQL_ROW row;

    while((row = mysql_fetch_row(result)) != NULL)
    {
        std::cout << "field1 = " << row[0] << std::endl;
        std::cout << "field2 = " << row[1] << std::endl;
        std::cout << "field3 = " << row[2] << std::endl;
    }

    mysql_free_result(result);
    mysql_close(mysql);

    /*if you delete result, it will cause a core dumped error while running the application.*/
    //delete result;
    delete mysql;

    result = NULL;
    mysql = NULL;

    return 0;
}