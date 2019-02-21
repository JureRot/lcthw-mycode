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
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    //int DATA;
    //int ROWS;
    struct Address rows[MAX_ROWS];
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
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);

    if (rc != 1)
        die(conn, "Failed to load database");
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
        if (conn->file)
            fclose(conn->file);
        if (conn->db)
            free(conn->db);
        free(conn);
    }
}

void Database_create(struct Connection *conn) {
    for (int i=0; i<MAX_ROWS; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0 };
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_write(struct Connection *conn) {
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        die(conn, "Failed to read database");

    rc = fflush(conn->file);
    if (rc == -1)
        die(conn, "Cannot flush database");
}

void Database_get(struct Connection *conn, int id) {
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die(conn, "ID not set");
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    struct Address *addr = &conn->db->rows[id];

    if (addr->set)
        die(conn, "Already set, delete it first");

    addr->set = 1;
    // WARNING: bug, read the "How to Break It" and fix this
    char *res = strncpy(addr->name, name, MAX_DATA);
    res[511] = '\0';
    // demonstrate the strncpy bug
    if (!res)
        die(conn, "Name copy failed");

    res = strncpy(addr->email, email, MAX_DATA);
    if (!res)
        die(conn, "Email copy failed");
}

void Database_delete(struct Connection *conn, int id) {
    struct Address addr = { .id = id, .set = 0 };
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
    struct Database *db = conn->db;

    for (int i=0; i<MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];

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
    if (id >= MAX_ROWS)
        die(conn, "There is not that many records");

    switch (action) {
        case 'c':
            if (argc != 5)
                die(conn, "Need MAX_DATA and MAX_ROWS to create");
            Database_create(conn);
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
     */
}
