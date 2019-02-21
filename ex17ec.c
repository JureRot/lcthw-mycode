#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100


struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_data;
    int max_rows;
    struct Address **rows; //this is an array of pointers
};

struct Connection {
    FILE *file;
    struct Database *db;
};


//forward delcaration
void Database_close(struct Connection *conn);


void die(struct Connection *conn, const char *message) {
    Database_close(conn);

    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    
    exit(1);
}

void Address_print(struct Address *addr) {
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
    if (!(conn->db && conn->file))
        die(conn, "Invalid connection during loading database");
    
    //first we read max_data and max_rows (two ints) (again using compact form)
    if (fread(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file) != 1)
        //we read 1 element of size max_data (same as size int) to the location (&) (because fread wants pointers) of db->max_data
        die(conn, "Failed reading max_data");
    //the same for max_rows
    if (fread(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file) != 1)
        die(conn, "Failed reading max_rows");

    //we reserve the space fro all the row (Address) pointers
    conn->db->rows = malloc(sizeof(struct Address*) * conn->db->max_rows); //Address* because rows is a array of pointers to Address-es
    if (!conn->db->rows)
        die(conn, "Failed reserving max_rows-number of Address structs");

    //now we load each row (each Address struct)
    for (int i=0; i<conn->db->max_rows; i++) {
        //we reserve the space for the Address struct
        conn->db->rows[i] = malloc(sizeof(struct Address)); //without a pointer

        //we get the pointer to it (for less typing)
        struct Address *row = conn->db->rows[i];

        //we load the id
        if (fread(&row->id, sizeof(row->id), 1, conn->file) != 1)
            die(conn, "Failed reading Address(id)");
        //we load the set
        if (fread(&row->set, sizeof(row->set), 1, conn->file) != 1)
            die(conn, "Failed reading Address(set)");
        //we allocate the location of the name string
        row->name = malloc(sizeof(char) * conn->db->max_data);
        if (!row->name)
            die(conn, "Failed to allocate Address(name)");
        //we load the name string
        if (fread(row->name, (sizeof(char) * conn->db->max_data), 1, conn->file) != 1)
            die(conn, "Failed to read Address(name)");
        //we do same as name for email
        row->email = malloc(sizeof(char) * conn->db->max_data);
        if (!row->email)
            die(conn, "Failed to allocate Address(email)");
        if (fread(row->email, (sizeof(char) * conn->db->max_data), 1, conn->file) != 1)
            die(conn, "Failed to read Address(email)");
    }
}

struct Connection *Database_open(const char *filename, char mode) {
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn)
        die(conn, "Memory error");

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db)
        die(conn, "Memory error");

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load(conn);
        }
    }

    if (!conn->file)
        die(conn, "Failed to open the file");

    return conn;
}

void Database_close(struct Connection *conn) {
    if (conn) {
        if (conn->file) {
            fclose(conn->file);
        }

        if (conn->db) {
            if (conn->db->rows) {
                for (int i=0; i<conn->db->max_rows; i++) {
                    struct Address *curr = conn->db->rows[i];
                    if (curr)
                        free(curr);
                }
            }

            free(conn->db);
        }

        free(conn);
    }
}

void Database_create(struct Connection *conn, int data, int rows) {
    //set the data and rows atribs in db
    conn->db->max_data = data;
    conn->db->max_rows = rows;

    //reserve space for all the rows (Address stuct pointers)
    conn->db->rows = malloc(sizeof(struct Address*) * rows); //malloc the space for rows-times of Address pointers
    //so now we have space for rows-number of pointers to Address struct and we can access it using conn->db->rows pointer
    
    if (!conn->db->rows) //and we check for error
        die(conn, "Memory error: couldn't create database");

    for (int i=0; i<rows; i++) {
        //now we reserve the space for individual row (as struct, not as pointer) and write the pointer to access it to conn->db->rows
        conn->db->rows[i] = malloc(sizeof(struct Address));

        if (!conn->db->rows[i]) //and we check for error
            die(conn, "Memory error: couldn't create row");

        //we set the id and the set argument
        conn->db->rows[i]->id = i;
        conn->db->rows[i]->set = 0;

        //we set the name
        conn->db->rows[i]->name = malloc(conn->db->max_data); //no sizeof just the value of max_data
        conn->db->rows[i]->name = memset(conn->db->rows[i]->name, '\0', conn->db->max_data);
        //memset(str, c, len) writes len bytes of value c to a string str (effectively wiping the name string in memory)
        
        //we do the same for email
        conn->db->rows[i]->email = malloc(conn->db->max_data);
        conn->db->rows[i]->email = memset(conn->db->rows[i]->email, '\0', conn->db->max_data);
    }
}

void Database_write(struct Connection *conn) {
    rewind(conn->file);

    //first we write max_data to the file
    int rc = fwrite(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file);
    //writes 1 item of size max_date (which is an int, so of size int) from location of max_data to location of file
    //remember fwrite wants a pointer from and pointer to and because max_data is not a pointer we need its address (&)
    if (rc != 1)
        die(conn, "Failed to write max_data");

    //we than write max_rows to the file (using a compact way without rc variable)
    if (fwrite(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file) != 1)
        die(conn, "Failed to write max_rows");

    //now we write all the rows
    for (int i=0; i<conn->db->max_rows; i++) {
        struct Address *row = conn->db->rows[i]; //get a pointer of a current row (just for less writing)o

        //and again using simple syntax we write all the arguments in Address struct to file
        if (fwrite(&row->id, sizeof(row->id), 1, conn->file) != 1)
            die(conn, "Failed to write Address(id)");
        if (fwrite(&row->set, sizeof(row->set), 1, conn->file) != 1)
            die(conn, "Failed to write Address(set)");
        if (fwrite(row->name, (sizeof(char) * conn->db->max_data), 1, conn->file) != 1) // we write max_data-number of sizes of char and we dont use & because name and email are pointers
            die(conn, "Failed to write Address(name)");
        if (fwrite(row->email, (sizeof(char) * conn->db->max_data), 1, conn->file) != 1)
            die(conn, "Failed to write Address(email)");
    }

    rc = fflush(conn->file);
    if (rc == -1)
        die(conn, "Cannot flush database");
}


void Database_get(struct Connection *conn, int id) {
    struct Address *addr = conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die(conn, "ID not set");
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    struct Address *addr = conn->db->rows[id];

    if (addr->set)
        die(conn, "Already set, delete it first");

    addr->set = 1;
    addr->name = malloc(sizeof(char) * conn->db->max_data);
    if (!addr->name)
        die(conn, "Failed allocating Address(name)");
    // WARNING: bug, read the "How to Break It" and fix this
    char *res = strncpy(addr->name, name, conn->db->max_data);
    // demonstrate the strncpy bug
    if (!res)
        die(conn, "Name copy failed");
    res[conn->db->max_data - 1] = '\0';

    addr->email = malloc(sizeof(char) * conn->db->max_data);
    if (!addr->email)
        die(conn, "Failed allocating Address(email)");
    res = strncpy(addr->email, email, conn->db->max_data);
    if (!res)
        die(conn, "Email copy failed");
    res[conn->db->max_data - 1] = '\0';
}

void Database_delete(struct Connection *conn, int id) {
    struct Address addr = { .id = id, .set = 0 };
    conn->db->rows[id] = &addr; //& because row is no longer actual struct, but a pointer
}
//DELETE DOESNT WORK, its not that simple, we need to actually create new and kill the other (free)
//look in db_create and in db_close

void Database_list(struct Connection *conn) {
    struct Database *db = conn->db;

    for (int i=0; i<db->max_rows; i++) {
        struct Address *cur = db->rows[i]; //here without the & because we are getting pointer (not the var)

        if (cur->set) {
            Address_print(cur);
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc < 3)
        die(NULL, "USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if (argc > 3)
        id = atoi(argv[3]);
    //if (id >= MAX_ROWS)
        //die(conn, "There is not that many records");

    switch (action) {
        case 'c':
            if (argc != 5)
                die(conn, "Need MAX_DATA and MAX_ROWS to create");
            Database_create(conn, atoi(argv[3]), atoi(argv[4]));
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4)
                die(conn, "Need an id to get");
            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6)
                die(conn, "Need id, name, email to set");
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4)
                die(conn, "Need id to delete");
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        
        default:
            die(conn, "Invalid action: c=create, g=get, s=set, d=delete, l=list");
    }

    Database_close(conn);

    return 0;

    /*
     * Idea for dyinamic sizes of data and number of rows
     *
     * rows can no longer be an array, they will have to be a pointer
     * because we can no longer know in advance how big they can be
     * and the same goes for name and email in Address
     *
     * we will have to malloc name and email as well as every addr in rows
     * this means we will have to free name, email, rows, db and conn and fclose the file
     *
     * on top of that the db wont be a solid block of memory and wont be easy to address (with &)
     *
     * conn will have a pointer to db which will have a pointer to rows
     * rows will be an array of pointers to addr OR will it be just a pointer
     * addr will have a pinter to name and a pointer to email
     *
     * [https://stackoverflow.com/questions/21269700/dynamically-sized-structs-learn-c-the-hard-way-ex17]
     * [https://pastebin.com/MvLXkDCz]
     */
}
