/* -----------------------------------------------------------------------------
*
*  npstr.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2015 by Shane Saxon - saxon@openantz.com
*
*  Please see main.c for a complete list of additional code contributors.
*
*  To the extent possible under law, the author(s) have dedicated all copyright 
*  and related and neighboring rights to this software to the public domain
*  worldwide. This software is distributed without any warranty.
*
*  Released under the CC0 license, which is GPL compatible.
*
*  You should have received a copy of the CC0 Public Domain Dedication along
*  with this software (license file named LICENSE.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

#include "npstr.h"


// some of the np...() string methods are SAFE, some are NOT!
// for agood discussion on null '\0' string safety issues see:
// http://stackoverflow.com/questions/1258550/why-should-you-use-strncpy-instead-of-strcpy


//string to 32bit float conversion
//------------------------------------------------------------------------------
float npatof (const char *p)
{
	char* curs = (char*)p;

	//currently we just wrap our npstrtof...
	//a small amount of time could be saved by copying the function here...
	return npstrtof (&curs);
}

//string to 64bit float conversion
//------------------------------------------------------------------------------
double npatod (const char *p)
{
	char* curs = (char*)p;

	//currently we just wrap our npstrtof...
	//a small amount of time could be saved by copying the function here...
	return (double)npstrtof (&curs);
}

//string to 32bit int conversion, optimized for common case of zero or one
//------------------------------------------------------------------------------
int npatoi(const char* buffer)
{
	int value = 0;

	//CSV optimized, and handles MySQL c string null termiator '\0'
	//optimized for the common zero cases, 0,0.0,0.000000...
	if (*buffer == '0')
	{
		buffer++;
		if (*buffer < '0')	//if not a numeral, then its just zero
			return 0;

		while (*buffer == '0') buffer++;
		
		if (*buffer == '0')	//if not a numeral, then all zeros
			return 0;
	}

	//optimized for the common 1.0 cases, 1,1.0,1.000000...
	if (*buffer == '1')
	{
		buffer++;
		if (*buffer < '0')		//if not a numeral
			return 1;			//then it is just '1'
		
		buffer--;				//restore position for further processing
	}

    //skip over any white space
    while (*buffer == ' ' || *buffer == '\t') buffer++;

    //check for sign, modified to optimize for most common case of no sign
	if (*buffer < '0')
	{
		if (*buffer == '-')
		{
			//process the digits
			while (*buffer >= '0' && *buffer <= '9')
			{
				value = value * 10 + (*buffer - '0');
				buffer++;
			}

			return -value;
		}
		buffer++;
	}

    //process the digits
    while (*buffer >= '0' && *buffer <= '9')
	{
        value = value * 10 + (*buffer - '0');
        buffer++;
    }
	
	return value;
}

//------------------------------------------------------------------------------
int npstrtoi(char** buffer)
{
	int value = 0;
	char* curs = *buffer;

	
	//process zero or any leading zeros, optimized for CSV
	if (*curs == '0')
	{
		curs++;
		if (*curs < '0')	//if not a numeral, then its just zero
			goto endPoint;

		while (*curs == '0') curs++;	//iterate past any more zeros
		if (*curs < '0')				//if not a numeral, then value is zero
			goto endPoint;
	}						//exits having incremented past all leading zeros

	//optimization for the common case of just '1'
	if (*curs == '1')
	{
		curs++;
		if (*curs < '0')		//if not a numeral
		{
			value = 1;			//then it is just '1'
			goto endPoint;
		}
		curs--;				//restore position for re-processing below
	}

    //skip over any white space
    while (*curs == ' ' || *curs == '\t') curs++;

    //check for sign, optimized for common case of no sign
	if (*curs < '0')
	{
		if (*curs == '-')
		{
			//process the digits for negative number
			while (*curs >= '0' && *curs <= '9')
			{
				value = value * 10 + (*curs - '0');
				curs++;
			}
			return -value;	//return negative signed value
		}
		curs++;				//otherwise skip over positive sign
	}

    //process the digits
    while (*curs >= '0' && *curs <= '9')
	{
        value = value * 10 + (*curs - '0');
        curs++;
    }

endPoint:

//	while (*curs != ',' && *curs != '\r' && *curs != '\n') curs++; //debug needed?

	if (*curs == ',')
		*curs++;		//if comma field separator then increment past

	*buffer = curs;

	return value;
}

//------------------------------------------------------------------------------
long npstrtol(char** buffer)
{
	long i;
	char* endPtr;

	i = strtol(*buffer, &endPtr, 0); 

	if (*endPtr != ',')
		*buffer = endPtr;	//handles errs and end of line
	else
		*buffer = ++endPtr;	//increments past the field separator

	return i;
}

//custom string to float conversion, optimized for our tables
//much faster then conventional libraries (approx 5-10x)
//------------------------------------------------------------------------------
float npstrtof(char** buffer)
{
	
	int expSign = 0;
	float sign = 1.0f, value = 0.0f, scale = 1.0f;

	const char* curs = *buffer;


	//optimized for the common zero cases, 0,0.0,0.000000...
	//CSV optimized, might consider adding MySQL '\0' and line return handling
	if (*curs == '0')
	{
		const char* temp = curs;	//leave pointer at start in case not zero

		temp++;
		if (*temp == '.')
		{
			temp++;
			while (*temp == '0') temp++;
			if (*temp == ',')
			{
				curs = temp;
				goto endPoint;
			}
		}
		else if (*temp == ',')
		{
			curs = temp;
			goto endPoint;
		}
	}

	//optimized for the common 1.0 cases, 1,1.0,1.000000...
	if (*curs == '1')
	{
		const char* temp = curs;	//leave pointer at start in case not zero

		temp++;
		if (*temp == '.')
		{
			temp++;
			while (*temp == '0') temp++;
			if (*temp == ',')
			{
				value = 1.0f;
				curs = temp;
				goto endPoint;
			}
		}
		else if (*temp == ',')
		{
			value = 1.0f;
			curs = temp;
			goto endPoint;
		}
	}

    //skip any white space
    while (*curs == ' ' || *curs == '\t') curs++;

    //get sign if exists
	if (*curs < '0') //changed from zero
	{
		if (*curs == '-')
		{
			sign = -1.0f;	//set negative sign value
			curs++;			//increment past sign to the first digit
		}
		else if (*curs == '+')	//default sign value is positive 1.0
			curs++;			//increment past sign to the first digit
	}

    //get any digits before decimal place
    while (*curs >= '0' && *curs <= '9')
	{
        value = value * 10.0f + (*curs - '0');
        curs++;
    }

    //get any digits after the decimal place
    if (*curs == '.') 
	{
        float powTen = 10.0f;
        curs++;

        while (*curs >= '0' && *curs <= '9') 
		{
            value += (*curs - '0') / powTen;
            powTen *= 10.0f;
            curs++;
        }
    }

    //process optional exponent
    if ((*curs == 'e') || (*curs == 'E'))
	{
        unsigned int exp = 0;
        curs++;

        //get exponent sign, default expSign = 0
        if (*curs == '-') 
		{
            expSign = 1;
            curs++;
        }
		else if (*curs == '+')
            curs++;

        //get exponent digits
        while (*curs >= '0' && *curs <= '9') 
		{
            exp = exp * 10 + (*curs - '0');
            curs++;
        }

		//cap value at min max decimal for a float, does not set to infinity
		if (exp >= 38)
		{							//max exp for 64bit double is 308
			exp = 38;				//max exp for 32bit float is 38
			if (sign < 0.0f)
			{
				
				if (expSign)//neg limits differ from positive, wrong limts //zz debug
				{
					if (value > 1.1754944f)	//32bit float min is 2^(-126)
						value = 1.1754944f;	//approx 1.17*10^38
				}
				else
				{
					if (value > 3.4028234f)	//32bit float max is (2-2^(-23))*2^127
						value = 3.4028234f;	//approx 3.4*10^38
				}
			}
			else
			{
				if (expSign)	//need to double check these, may be correct //zz debug
				{
					if (value > 1.1754944f)	//32bit float min is 2^(-126)
						value = 1.1754944f;	//approx 1.17*10^38
				}
				else
				{
					if (value > 3.4028234f)	//32bit float max is (2-2^(-23))*2^127
						value = 3.4028234f;	//approx 3.4*10^38
				}
			}
		}

        //calculate scale factor, could optimize with lookup table		//zz debug
        while (exp >= 8) {scale *= 1.0e8f; exp -= 8;}
        while (exp >  0) {scale *= 10.0f;  exp -= 1;}	//could optimize with table
    }

    //calculate the final scaled value based on the exponent sign
    if (expSign)
		value = sign * (value / scale);
	else
		value = sign * value * scale;

endPoint:

//	while (*curs != ',' && *curs != '\r' && *curs != '\n') curs++;

	//zz debug, this method is specific to a CSV format, improve clarity, err handling...
	if (*curs != ',')
		*buffer = (char*)curs;		//handles errs and end of line
	else
		*buffer = (char*)++curs;	//if comma field separator then increment past

	return value;
}

//------------------------------------------------------------------------------
double npstrtod(char** buffer)
{
	int i = 0, j = 0;
	double decimal = 0.0;
	double value = 0.5;
	char* endPtr;

	value = npatod(*buffer);

	endPtr = *buffer;
	while (*endPtr != ',') endPtr++;

	if (*endPtr != ',')
		*buffer = endPtr;	//handles errs and end of line
	else
		*buffer = ++endPtr;	//increments past the field separator

	return value;
}

//zz modified to generate a single null terminated string if str == NULL
//JJ----------------------------------------------------------------------------
char* npNewStrcpy (const char* str, void* dataRef) 
{
	char* result = NULL;

	if (str == NULL)
	{
		result = malloc (1);
		*result = '\0';
		return result;
	}

	result = (char*) malloc (strlen(str) + 1);
	if (result == NULL)
	{
		npPostMsg ("err 4299 - malloc failed, npNewStrcpy\n", kNPmsgErr, dataRef);
		return NULL;
	}

	strcpy (result, str);

	return result;
}
char* npstrdup( const char* str, void* dataRef)
{
	return npNewStrcpy( str, dataRef );
}

//------------------------------------------------------------------------------
int npNextField(const char* buffer)	//does NOT Check Max
{
	char ch = 0;
	int curs = 0;

	//either Carraige Return or New Line will end loop
	while( ch != ',')
	{
		ch = buffer[curs];
		curs++;
	}

	return curs;
}

//MB//
// seeks to next line, accepts cr and or nl to be compatible with Mac, Unix, MS
//------------------------------------------------------------------------------
int npNextLine(const char* buffer )	//does NOT Check Max
{
	char ch = 0;
	int curs = 0;

	//either Carraige Return or New Line will end loop
	while( ch != '\r' && ch != '\n' )
	{
		ch = buffer[curs++];
	}

	// handles carriage return followed by line feed by incrementing forward
	if ( ch == '\r' && buffer[curs] == '\n' )
		curs++;

	return curs;	//the index of the first char of the next line
}


//------------------------------------------------------------------------------
char* npNextWhiteSpace (const char* buffer, int size)
{
	char* curs = (char*)buffer;
	char* endPtr = curs + size;

	//skip white space in case of blank lines at top of file
    while( *curs != ' ' || *curs != '\t' || *curs != '\r' || *curs != '\n' )
	{
		if (curs >= endPtr)
			return NULL;		//return NULL if we reach the end of buffer
		else
			curs++;
	}

	return curs;
}


//------------------------------------------------------------------------------
char* npSkipWhiteSpace (const char* buffer, int size)
{
	char* curs = (char*)buffer;
	char* endPtr = curs + size;

	//skip white space in case of blank lines at top of file
    while( *curs == ' ' || *curs == '\t' || *curs == '\r' || *curs == '\n' )
	{
		if (curs >= endPtr)
			return NULL;		//return NULL if we reach the end of buffer
		else
			curs++;
	}

	return curs;
}

//------------------------------------------------------------------------------
int npNextLineLimit(const char* buffer, int size )	//Checks Max
{
	char ch = 0;
	int curs = 0;

	//either Carraige Return or New Line will end loop
	while( ch != '\r' && ch != '\n' && curs < size)
	{
		ch = buffer[curs++];
	}

	//handles carriage return followed by line feed by incrementing forward
	//checks curs < size - 1 to make sure it does not access past end of buffer
	if ( ch == '\r' && curs < size)		//(size - 1))	//zzt debug fixes issues with CRLF files (seems) should be -1 though?
	{
		if ( buffer[curs] == '\n')
			curs++;
	}
	//while( (ch == '\r' || ch == '\n') && curs < size) ch = buffer[curs++]; //zz debug
	
	return curs;	//the index of the first char of the next line
}

//search from end of buffer and return index of the last line end, '\r' or '\n'
//return 0 if not found
//------------------------------------------------------------------------------
int npLastEOL(const char* buffer, int size)
{
	//pre-decrement index since last array index is size - 1
	while (--size >= 0)
	{
		if (buffer[size] == '\r' || buffer[size] == '\n')
			return size;
	}

	//printf("err 4984 - EOL not found in buffer\n");
	
	return 0;	//not found
}

// skip white space and look for >= 0 && <= 9
// return 1 for true if next non-whitespace is a digit, else return 0
int npStrDigitIsNext( const char* str, int size )
{
	int i = 0;
//	char* curs = str;	//zz may not need this

	printf("\nstr test: %s\n", str);
	//skip any whitespace
	while ( i < size && ( str[i] == ' ' || str[i] == '\t' ) ) i++;

	str += i;

	if ( *str >= '0' && *str <= '9' )
		return 1;

	return 0;
}


//------------------------------------------------------------------------------
int npStrToRange( int* lower, int* upper, const char* str, int size )
{
	int curs = 0;

	//convert first number string to int
	*lower = npatoi( str );

	//search for separator
	while( !( str[curs] == ':' || str[curs] == '-' ) && curs < size ) curs++;	//zz debug add better exception handling

	if (curs < (size - 1) )
		*upper = npatoi( str + curs + 1 );
	else
		*upper = *lower;	//test

	return (*upper - *lower + 1);	
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//passed in ptr to the CSV buffer and converts it to a string struct that is
//compatible with MySQL query struct... 
//conversion overwrites the comma deliminators with null terminators '\0'
//which turns the CSV file into a series of C strings... ta da!
//pads all deliminators with '\0' such as quotes, commas and line returns
//and creates separate ptr array to the start of each string with additional
//array for string length
//buffer with str map struct is returned to npmapstr which converts the strings 
//to the target data structure member types, primarily int, float and string
//then the buffer clear flag is set to allow re-use for next file chunk
//-----------------------------------------------------------------------------
void npCSVtoStr (char** str, const char* csv, int type, int size, void* dataRef)
{
	int i = 0;
//	pNProwFormat row = NULL;	//based on type

//	for (i=0; i < row->count; i++)
	{

	}
}

//hard coded optimization for our native data structs
//npStrToNode

//converts any table structure to a dynamically defined generic C container
//low-level on a per table type or per record bases
//separate JSON compatible shema can be used to define the data tree structure
//supporst plugins to define and work with any arbitrary data structure
//efficiency is almost as good as the structure specific hard coded routines
//some light processing overhead to for the loop and pointer math, etc...
//-----------------------------------------------------------------------------
void npMapStrToC (void** dest, const char* str, int type, int size, void* dataRef)
{
	int i = 0, j = 0;
/*
	int elementSize = npGetElementSize(type);
	int rowColumnCount = npGetColumnCount(type);

	//convert rows till we reach the defined endpoint size
	for (i < size)
	{
		//convert a row (strings) to C data type
		for (j=0; j < rowColumnCount; j++)
		{
			switch (typeElement[j])
			{
				case kNPint : dest[i] = npatoi(str[j]); break;
				case kNPfloat : dest[i] = npatof(str[j]); break;
				case kNPcstr : dest[i] = npstrncpy(str[j], strLen[j]); break;
			}
		}
		dest[i]
		i += elementSize
	}
*/
}

