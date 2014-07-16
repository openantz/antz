int npGenMysqlFields(char* fields, int count, int type, void* dataRef);
void npGenMysqlQuery(struct mysqlObject *object, void* dataRef);
void npGenMysqlDatabaseQuery(struct mysqlObject *object);
void npGenMysqlTableQuery(struct mysqlObject *object, void* dataRef);