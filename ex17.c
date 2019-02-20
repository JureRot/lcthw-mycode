#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512 //create a global, reliable constant
#define MAX_ROWS 100


struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    struct Address rows[MAX_ROWS]; //not a pointer, an actual array of Address structures
    //because this has the actual whole data table, this is a big struct (big chunk of memory) 100 lines of 512*2 + 8*2 bytes
    //this means the OS maybe cant give us the malloc() and we will have to check if we got it
};

struct Connection {
    FILE *file; //FILE is a struct for files, defined by C standard library
    struct Database *db;
};


void die(const char *message) {
    if (errno) { //when error returs from a function the errno (error number) is usually set
        perror(message); //we can print the error message with perror()
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
    // file_read(to_where/output, size, num_items, from_where/input); returns 1 if successful
    // fread(ptr, size, nitems, stream);
    // The function fread() reads nitems objects, each size bytes long, from the stream pointed to by stream, storing them at the location given by ptr.

    if (rc != 1)
        die("Failed to load database");
}

struct Connection *Database_open(const char *filename, char mode) {
    struct Connection *conn = malloc(sizeof(struct Connection)); //reserve space for connection, check for error
    if (!conn) //NULL is 0, so we can use it as bool, but we could do conn == NULL
        die("Memory error");

    conn->db = malloc(sizeof(struct Database)); //reserve space for database inside conn, check for error
    if (!conn->db)
        die("Memory error");

    if (mode == 'c') { //if mode=create, we write to file
        conn->file = fopen(filename, "w");
    } else { //we open existing one
        conn->file = fopen(filename, "r+");

        if (conn->file) { //if file actually existed (was found/pressent), we load db from it
            Database_load(conn);
        }
    }

    if (!conn->file) //if file wasnt found to read from, we abort
        die("Failed to open the file");

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
        //make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0 }; //thats how you init and set at the same time
        //then just assign it
        conn->db->rows[i] = addr;
    }
    //creates all rows in conn->db with unique id and sets their set argument to 0
}

void Database_write(struct Connection *conn) {
    rewind(conn->file); //set position of the file stream to the beginning

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    //fwrite(ptr, size, nitems, stream)
    //The function fwrite() writes nitems objects, each size bytes long, to the stream pointed to by stream, obtaining them from the location given by ptr.
    //care about the input parameters
    if (rc != 1)
        die("Failed to read database");

    rc = fflush(conn->file); //forces a write of all buffered date for stream (clears / flushes the buffer)
    if (rc == -1)
        die("Cannot flush database");
}

void Database_get(struct Connection *conn, int id) {
    struct Address *addr = &conn->db->rows[id];
    //we get the reference to address (line in database) by actally finding it in memory and pointing the pointer to it
    //we basically do conn.db.rows[i] and with & get the location of that. we assign this location to our addr pointer

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID not set");
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    struct Address *addr = &conn->db->rows[id];
    //again get the reference to the line in db to set by point at the raw memory address of it
    if (addr->set)
        die("Already set, delete it first");

    addr->set = 1;
    // WARNING: bug, read the "How to Break It" and fix this
    char *res = strncpy(addr->name, name, MAX_DATA);
    //strncpy(in, out, n) creates a string copy of len n even if string istn that long (similarly to strcpy())
    //strncpy will not add the \0 char at the end (if len of name too big). we need to force it
    res[511] = '\0';
    // demonstrate the strncpy bug
    if (!res)
        die("Name copy failed");

    res = strncpy(addr->email, email, MAX_DATA);
    if (!res)
        die("Email copy failed");
}

void Database_delete(struct Connection *conn, int id) {
    struct Address addr = { .id = id, .set = 0 };
    conn->db->rows[id] = addr;
    //we dont delete it, we just overwrite it with a new address (new line) with the same id which is clean and not set
    //we can do this because Address structure is not heap (malloc) but is on stack (will be cleared after this func is done)
    //it used to be different (actually copying the memory of new addr to the location in conn->db->rows[id]
    //memcpy(conn->db->rows[id], addr, sizeof(struct Address));
    //he compilers still does that but you dont need to write it like that, you can just assign one addr to the other
    //if addr was a pointer we would get its value: conn->db->rows[id] = *addr;
}

void Database_list(struct Connection *conn) {
    struct Database *db = conn->db; //get the pointer to the database struct (so we dont need to acces the conn->db every loop of for)

    for (int i=0; i<MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i]; //get the pointer to the location (in memory) of the row

        if (cur->set) {
            Address_print(cur);
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc < 3) //weird form of for (no braces)
        die("USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0]; //first letter of second arg
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    //if (argc > 3) id = atoi(argv[3]); //different syntax for if (hard to read)
    if (argc > 3)
        id = atoi(argv[3]); //ascii_to_integer (if there are more than 3 args, the third is always the id of record)
    if (id >= MAX_ROWS) //if that id is too large (wrongly passed) we abort
        die("There is not that many records");

    switch (action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4)
                die("Need an id to get");
            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6)
                die("Need id, name, email to set");
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4)
                die("Need id to delete");
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        
        default:
            die("Invalid action: c=create, g=get, s=set, d=delete, l=list");
    }

    Database_close(conn);

    return 0;

    /*
     * STACK vs HEAP
     *
     * Heap:
     * is all the remaining memory in the computer
     * we can ask the OS to give us some more memory with malloc() (which returns the poiter to that location of memory (this pointer is on the stack and will be lost when function exits))
     * OS than assigns that memory to our program so we can read and write all over it
     * at the end we use free() to say to the OS that we dont need tha mem any more and other programs can use it (otherwise we get memory leaks)
     *
     * Stack:
     * each function has its stack of memory which houses all the local values for that fuction
     * when you call the function, the arguments are pushed to that stack (copy is made (thats why sending pointers to funcs is better))
     * any other local vars inside that function are also added to the stack
     * when function exits all of the stack is rolled back (cleaned) by itself (same goes for main) so there is no memory leak
     * if you pass or return a pointer from a stack to another function, that pointer is pointing to dead space (seg fault)
     * if you put too much data on the stack you will get a stack overflow and the program will exit (in this case use the heap)
     *
     * simple way to diferenciate: if malloc is used (directly or through a function) -> its the heap, otherwise -> its the stack
     */
}
