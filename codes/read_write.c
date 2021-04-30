/*
 *
 *  GattLib - GATT Library
 *
 *  Copyright (C) 2016-2019  Olivier Martin <olivier@labapart.org>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gattlib.h"
#define READ 0
#define WRITE 1
//typedef enum { READ, WRITE} operation_t;
//operation_t g_operation;

static uuid_t g_uuid;
char value_data;

//static void usage(char *argv[]) {
//	printf("%s <device_address> <read|write> <uuid> [<hex-value-to-write>]\n", argv[0]);
//}

void delay(){
for(int i=0;i<1000;i++);
}

int ble_read(char* uuid[], char* macID[], int fileSelect) {
	FILE *fptr;
	int i, ret;
	size_t len;

	gatt_connection_t* connection;

	gattlib_string_to_uuid(uuid, strlen(uuid) + 1, &g_uuid);

	connection = gattlib_connect(NULL, macID, GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
	if (connection == NULL) {
		fprintf(stderr, "Fail to connect to the bluetooth device.\n");
		return 1;
	}


		uint8_t *buffer = NULL;

		ret = gattlib_read_char_by_uuid(connection, &g_uuid, (void **)&buffer, &len);
		if (ret != GATTLIB_SUCCESS) {
			char uuid_str[MAX_LEN_UUID_STR + 1];

			gattlib_uuid_to_string(&g_uuid, uuid_str, sizeof(uuid_str));

			if (ret == GATTLIB_NOT_FOUND) {
				fprintf(stderr, "Could not find GATT Characteristic with UUID %s. "
					"You might call the program with '--gatt-discovery'.\n", uuid_str);
			} else {
				fprintf(stderr, "Error while reading GATT Characteristic with UUID %s (ret:%d)\n", uuid_str, ret);
			}
			goto EXIT;
		}

		printf("Read UUID completed: ");
		for (i = 0; i < len; i++) {
			printf("%c",buffer[i]);
		}
		printf("\n");
               if(fileSelect == 1){
		fptr=fopen("/home/pi/sepServer/t1currentStat.txt","w");
		fprintf(fptr,"%s",buffer);
		fclose(fptr);
		}
		else if(fileSelect == 2){
		fptr=fopen("/home/pi/sepServer/t1currentStat.txt","w");
                fprintf(fptr,"%s",buffer);
                fclose(fptr);
		}
		free(buffer);

EXIT:
	gattlib_disconnect(connection);
	return ret;
}

int ble_write(char* uuid[], char* macID[],int fileSelect)
{
        FILE *fptr;
	char buff[2];
        int i, ret;
        size_t len;

        gatt_connection_t* connection;

        gattlib_string_to_uuid(uuid, strlen(uuid) + 1, &g_uuid);
	printf("Trying to write to %s\n",uuid);
        connection = gattlib_connect(NULL, macID, GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
        if (connection == NULL) {
                fprintf(stderr, "Fail to connect to the bluetooth device.\n");
                return 1;
        }
	if(fileSelect == 1){
		fptr=fopen("/home/pi/sepServer/t1relayStat.txt","r");
		fgets(buff,2,(FILE*)fptr);
		fclose(fptr);
	}
	else if(fileSelect == 2){
		fptr=fopen("/home/pi/sepServer/t2relayStat.txt","r");
		fgets(buff,2,(FILE*)fptr);
		fclose(fptr);
	}
		printf("Value to be written : %s",buff);
		value_data=buff[0];
                ret = gattlib_write_char_by_uuid(connection, &g_uuid, &value_data, sizeof(value_data));
                if (ret != GATTLIB_SUCCESS) {
                        char uuid_str[MAX_LEN_UUID_STR + 1];

                        gattlib_uuid_to_string(&g_uuid, uuid_str, sizeof(uuid_str));

                        if (ret == GATTLIB_NOT_FOUND) {
                                fprintf(stderr, "Could not find GATT Characteristic with UUID %s. "
                                        "You might call the program with '--gatt-discovery'.\n", uuid_str);
                        } else {
                                fprintf(stderr, "Error while writing GATT Characteristic with UUID %s (ret:%d)\n",
                                        uuid_str, ret);
                        }
			goto EXIT;
                        }
EXIT:
        gattlib_disconnect(connection);
        return ret;
}

int main()
{
char uuid1[]="88dd973c-f6c1-45e2-8e32-95bbe7cbbe9e";
char uuid2[]="88dd973c-f5c1-45e2-8e32-95bbe7cbbe9e";
char macID1[]="ED:8E:6C:A7:6D:6E";
//char macID2[]="";
while(1)
{
ble_read(uuid1,macID1,1);
delay();
ble_write(uuid2,macID1,1);
delay();
//ble_read(uuid1,macID2,2);
//ble_write(uuid2,macID2,2);
}
return 0;
}
