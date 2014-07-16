void npLoadMysqlNodeStateResultsIntoAntz(struct mysqlObject *object, void* dataRef);
void npLoadMysqlResultsIntoAntz(struct dbConnectMysql *connect, struct mysqlObject *object, int tableType, void* dataRef);
void npLoadNodeStateResultIntoAntz(MYSQL_RES *result, void* dataRef);