#include <stdio.h>
#include <stdlib.h>


#include "strings_man.h"
#include "table.h"

int main(void)
{
    strings_t sm = Strings.create();

// create
    table_t this = Table.create(Strings.get(sm, "test_table"), 
        3,
        Strings.get(sm, "id"), number, 
        Strings.get(sm, "description"), string, 
        Strings.get(sm, "amount"), number
    );

// add_rows
    string_t desc_1 = Strings.get(sm, "this is an item with amount and the text is honestly quite long and pointless");
    string_t desc_2 = Strings.get(sm, "you are a pokemon\n");
    string_t desc_3 = Strings.get(sm, "call me maybe");
    string_t desc_4 = Strings.get(sm, "9");

    string_t descs[4] = {desc_1, desc_2, desc_3, desc_4};

    test_row_t* rows = malloc(sizeof(*rows) * 4);

    for (size_t i = 0; i < 4; i++)
    {
        test_row_t* row = rows + i;
        row->amount = (i + 6) << 10;
        row->description = descs[i];
        row->id = (i + 13) << 2 | (i + 300);
        Table.add_row(this, row);
    }

    for (size_t i = 0; i < 4; i++)
    {
        // printf("%lf\n", (double)rows[i].id);
        // string_t d = rows[i].description;
        // printf("%s\n", String.chars(d));
        // printf("%lf\n", (double)rows[i].amount);
        Table.print_row(this, i, 2);
    }
    


// query
    {};

    Strings.free(sm);
    Table.free(this);
    free(rows);


    return 0;
} 
