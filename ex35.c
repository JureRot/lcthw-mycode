#include <stdio.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
} VariantType;

struct Variant {
    VariantType type;
    union {
        int as_integer;
        float as_float;
        char *as_string;
    } data;
};
//as_integer, as_float and as_string are all on the same location in memory and that location is only as big as the biggest element
//so when you write to as_integer you also writer over as_float and as_string (all cells contain same sequence of zeros and ones)
//if you than read is as_integer the compiler knows what to expect and you get a meaningful result
//if you would try to read as_float the compiler would expect different format of memory (mantisa and exponent) and you would not get a meaningful result

typedef struct Variant Variant; //don't know what this is  //don't know what this is

void Variant_print(Variant *var) {
    switch (var->type) {
        case TYPE_INT:
            printf("INT: %d\n", var->data.as_integer);
            break;
        case TYPE_FLOAT:
            printf("FLOAT: %f\n", var->data.as_float);
            break;
        case TYPE_STRING:
            printf("STRING: %s\n", var->data.as_string);
            break;
        default:
            printf("UNKNOWN TYPE: %d", var->type);
    }
}

int main(int argc, char *argv[]) {
    Variant a_int = { .type = TYPE_INT, .data.as_integer = 100 };
    Variant a_float = { .type = TYPE_FLOAT, .data.as_float = 100.34 };
    Variant a_string = { .type = TYPE_STRING, .data.as_string = "YO DUDE!" };

    Variant_print(&a_int);
    Variant_print(&a_float);
    Variant_print(&a_string);

    // here's how you acces them
    a_int.data.as_integer = 200;
    a_float.data.as_float = 2.345;
    a_string.data.as_string = "Hi there";

    Variant_print(&a_int);
    Variant_print(&a_float);
    Variant_print(&a_string);

    return 0;
}
