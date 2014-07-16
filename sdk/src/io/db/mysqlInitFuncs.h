struct mysqlObject* npInitMySQLDatabase(char* name);
struct mysqlObject* npNewInitMySQLTable(char *name);
struct mysqlObject* npInitMysqlQuery();
void npInitDbConnectMySQL (struct dbConnectMysql *connection, char* host, char* user, char* pass, char* db);