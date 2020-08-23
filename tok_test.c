#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char* the_tokens = malloc(4096);    
    
    sprintf(the_tokens, "this/is/not/a/test-this-is-reality");

    char* tok = strtok(the_tokens, "/");
    printf("%s, %s\n", the_tokens, tok);

    tok = strtok(NULL, "/");
    printf("%s, %s\n", the_tokens, tok);

    tok = strtok(NULL, "/");
    printf("%s, %s\n", the_tokens, tok);

    tok = strtok(NULL, "/");
    printf("%s, %s\n", the_tokens, tok);
    

    tok = strtok(NULL, "-");
    printf("%s, %s\n", the_tokens, tok);
    
    tok = strtok(NULL, "-");
    printf("%s, %s\n", the_tokens, tok);

    tok = strtok(NULL, "-");
    printf("%s, %s\n", the_tokens, tok);

    tok = strtok(NULL, "-");
    printf("%s, %s\n", the_tokens, tok);

    return 0;
}
