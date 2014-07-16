void npSetMysqlQueryObject(struct dbConnectMysql *connect, struct mysqlObject* query, struct mysqlObject *dbOrTable, void* dataRef);
int npSetMysqlQueryCommand(struct dbConnectMysql *connect, struct mysqlObject *query, int command, void* dataRef);
void npSetMysqlQueryFields(struct dbConnectMysql *connect, struct mysqlObject *query, char *fields);
void npSetMysqlQueryValues(struct dbConnectMysql *connect, struct mysqlObject *object, struct chunksObj *valueChunks);