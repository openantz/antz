/*
 *  npdbTypes.h
 *  antz_osx_mysql-lde
 *
 *  Created by Lucas Erickson on 12/3/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

/*
 #define kNPdbHostMax	512		///< max number of DB host servers
 #define kNPdbMax		4096	///< max number of databases
 #define kNPtblMax		128		///< max number of tables, new lde
 #define kNPpwdMax		256		///< max password length
 #define kNPuserNameMax	256		///< max user name length
 #define kNPdbNameMax	64		///< @todo make sure 64 is good for non-MySQL DBs
 #define kNPdbFuncSetMax 64		///< max number DB function sets
 */
#define	kNPurlMax			4096			//!< maximum length of URL accepted
enum NP_DATABASE {
	kNPdbHostMax	= 512,		///< max number of DB host servers
	kNPdbMax		= 4096,		///< max number of databases
	kNPtblMax		= 128,		///< max number of tables, new lde
	kNPpwdMax		= 256,		///< max password length
	kNPuserNameMax	= 256,		///< max user name length
	kNPdbNameMax	= 64,		///< @todo make sure 64 is good for non-MySQL DBs
	kNPdbFuncSetMax	= 64		///< max number DB function sets
};

struct NPdbFunction{
	int			type;					///< function protype index
	void*		funcPtr;				///< pointer to the function
	
	char		name[kNPdbNameMax];		///< function UI name
	char*		desc;					///< function description
};
typedef struct NPdbFunction NPdbFunction;
typedef struct NPdbFunction *pNPdbFunction;

struct NPdbFuncSet{
	int			id;						///< the function set ID
	char		hostType[kNPdbNameMax];	///< content type, antz or 3rd party
	void*		dbLib;					///< library handle
	
	//pNPdbFunction*	funcList;
	//int				funcCount;
	
	///  abstract our database server type specific methods
	
	/*
	void* (__stdcall *init)			();
	void* (__stdcall *connect)		();
	int   (__stdcall *options)		();
	void* (__stdcall *ping)			();
	void* (__stdcall *close)		    ();
	
	void* (__stdcall *show)			();
	void* (__stdcall *query)		    ();
	void* (__stdcall *store_result)	();
	void* (__stdcall *free_result)	();
	
	void* (__stdcall *use)			();
	void* (__stdcall *select)		    ();
	void* (__stdcall *alter)		    ();
	void* (__stdcall *insert)		    ();
	void* (__stdcall *fetch_row)	    ();
	void* (__stdcall *fetch_lengths)  ();
	void* (__stdcall *num_fields)	    ();
	void* (__stdcall *num_rows)		();
	void* (__stdcall *db_error)		();
	void* (__stdcall *db_errno)		();
	void* (__stdcall *conn_thread_id) ();
	*/
	
	void* (*init)			();
	void* (*connect)		();
	int   (*options)		();
	void* (*ping)			();
	void* (*close)		    ();
	
	void* (*show)			();
	void* (*query)		    ();
	void* (*store_result)	();
	void* (*free_result)	();
	
	void* (*use)			();
	void* (*select)		    ();
	void* (*alter)		    ();
	void* (*insert)		    ();
	void* (*fetch_row)	    ();
	void* (*fetch_lengths)  ();
	void* (*num_fields)	    ();
	void* (*num_rows)		();
	void* (*db_error)		();
	void* (*db_errno)		();
	void* (*conn_thread_id) ();
	
	///< error and errno use 'db_' prefix to prevent name conflict
	
	int   (*InitConnOptions)		();
	void* (*GetTableFields)			();	///creates the table fields descrisptor
	
	void* (*StatementInsert)		();
	void* (*StatementCreate)		();
	void* (*StatementCreateTable)	();
	void* (*StatementUse)			();
	void* (*StatementShow)			();
	void* (*StatementDrop)			();
	void* (*StatementSelect)		();
	void* (*StatementTruncate)		();
	void* (*StatementUpdate)		();
	
	int size;
};
typedef struct NPdbFuncSet NPdbFuncSet;
typedef struct NPdbFuncSet *pNPdbFuncSet;

/// Host id unique to the session, ip is generally more permanent.
/// Note that the list of databases is not stored with the host.
/// The name of the current database in USE is stored with the host.
struct NPdbHost{
	int			id;				///< local host ID unique to this session only
	void*		conn;			///< host connection handle
	bool		connected;
	unsigned long conn_id;		///< Host's database thread id, lde
	
	char		type[64];		///< server type 'mysql', 'postgresql', etc.
	
	int			port;					///< port address
	char		ip[kNPurlMax];			///< IP address or URL
	
	char		user[kNPuserNameMax];	///< this hosts user name
	char		password[kNPpwdMax];	///< this hosts password
	
	char		inUseDB[kNPdbNameMax];	///< name of DB currently in USE
	int			dbCount;				///< number of databases for this host
	
	//char		currentTable[];
	
	pNPdbFuncSet hostFuncSet;			///< function calls for this host type
}; //@todo: hosts need an active server, lde
typedef struct NPdbHost NPdbHost;
typedef struct NPdbHost * pNPdbHost;


struct NPdbFields{
	char*		name;		///< field name
	char*		typeStr;	///< field type as DB string format
	char*		params;		///< any additional field params, limits, etc.
	int *		type;		///< field type constant
	int			count;		///< number of field columns
};
typedef struct NPdbFields NPdbFields;
typedef struct NPdbFields * pNPdbFields;

/// holds a pair of lists that correspond an external id to local node pointer
/// nextID index is used for fast search processing when using an ordered list
struct NPmapID{
	int		nextID;		///< index used for accelerated search of ordered list
	int*	recID;		///< list of external table record id's to map
	int*	nodeID;		///< matching list of local scene node id's
	int		count;		///< number of item pairs in the lists
};
typedef struct NPmapID NPmapID;
typedef struct NPmapID * pNPmapID;	

#define kNPmaxFields 4096
struct NPdbTable{
	int			id;							///< local table ID for this database
	int			type;						///< table type: node, tag, chmap, etc.
	char*		name;
	
	int			rows;						///< number of rows in this table
	
	pNPdbFields	fields[100];				///< field descriptor
	int			fieldCount;					///< number of fields in this table, init to 0 , @todo, lde
	
	pNPmapID	mapID;						///< maps the row id to local data ptr
	
	void*		owner;						///< Database this table is attached to, lde pNPdatabase
	
	int			size;						///< size of this table in bytes
};
typedef struct NPdbTable NPdbTable;
typedef struct NPdbTable * pNPdbTable;

struct NPdbCSVwrite
{
	char*	   csvName;
	int		   running;
	pNPdbTable table;
	void*	   dataRef;
};
typedef struct NPdbCSVwrite NPdbCSVwrite;
typedef struct NPdbCSVwrite *pNPdbCSVwrite;


enum NP_DB_CONTENT_TYPES 
{
	kNPdbTypeNull = 0,
	kNPdbTypeNative,		///< native dataset with nodes, tags, etc.
	kNPdbTypePlugin,		///< DB plugin type for 3rd party support
	kNPdbTypeUnknown,		///< unknown DB content structure
	kNPdbTypeCount
};

struct NPdatabase{
	int			id;						///< this databases ID // Connection structure
	int*		idMap;
	int			format;					///< content format, antz or 3rd party
	int*		refCount;				///< memory reference counter, lde
	
	char		name[kNPdbNameMax +1];	///< database name +1 for '\0'
	
	//	pNPdbTable* tables;					///< contains row count		// old, lde
	pNPdbTable	tableList[kNPtblMax];	///< list of tables			// new, lde
	pNPdbTable	tableInUse;				///< current table in use
	int			tableCount;				///< total number of tables
	
	int			nodeCount;				///< if exists, node table row count // @todo, include nodeCount on dbMenu, lde
	
	float		saveUpdateRate;			///< auto save update rate, 0 is off
	float		loadUpdateRate;			///< auto load update rate, 0 is off
	
	pNPdbHost	host;		///< references this databases host
	
};
typedef struct NPdatabase NPdatabase;
typedef struct NPdatabase * pNPdatabase;




/// holds list of databases and there hosts
struct NPdbs {
	void* coreNode; ///< core nodes tie global structures to the scene graph
	
	bool			running;				///< true if hosts are connected
	
	pNPdbHost		hosts[kNPdbHostMax];	///< list of database hosts
	int				hostCount;				///< number of DB host servers
	
	pNPdbFuncSet	funcSetList[kNPdbFuncSetMax]; ///< host type specific
	int				funcSetCount;				  ///< function set count
	
	pNPdatabase		dbList[kNPdbMax];	///< list of databases
	int				dbCount;			///< number of databases
	
	pNPdatabase		activeDB;			///< points to active DB in list
	
	float			saveUpdateRate;		///< auto save update rate, 0 is off
	float			loadUpdateRate;		///< auto load update rate, 0 is off
	
	bool			loadUpdate;			///< flag a single update this cycle
	bool			saveUpdate;			///< flag a single update this cycle
	
	int				size;				///< size in RAM of all db elements
};
typedef struct NPdbs NPdbs;
typedef struct NPdbs *pNPdbs;

//!<struct pNPdatabases { //!<zzsql
struct NPdatabases {
	void* coreNode; ///< core nodes tie global structures to the scene graph
	//!< each global struct has a corresponding base node.
	
	char** list;		//!< list of databases by name
	int size;			//!< number of items in the list
};
typedef struct NPdatabases NPdatabases;
typedef struct NPdatabases *pNPdatabases;

/* new struct, lde */
struct NPtables {
	char** list;		//!< list of tables by name
	int size;			//!< number of items in the list
};
typedef struct NPtables NPtables;
typedef struct NPtables *pNPtables;
