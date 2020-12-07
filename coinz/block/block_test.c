#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>


#include "block.h"



const unsigned char* smallakWallakSha = "2b030942f2d8b127600a9f32ab5fbe704dd925e38f4ddad747f57b6e39b81a29ab774d09bbd9e426d845ac76c0f55de1b7e5256357768f4ae845ec561ec9a3a3";
const unsigned char* shaOfSecondBlock = "bb355046ed43bcf12b1dfaa42a1c73a577acd47aa08d0157224d09341da18b635102e41cd40904d34a15e174419b1fa1eb0b682ae21ee947a23a1009601fa5c6";

void dodasha(Sha512 s)
{
    for (size_t i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        const unsigned char c1 = (s[i] & 0xf0) >> 4;
        const unsigned char c2 = s[i] & 0x0f;
        printf("%x%x", c1, c2);        
    }    
    printf("\n");
}

int main(void)
{
    unsigned char genesis[SHA512_DIGEST_LENGTH];
    unsigned char* sha = SHA512("smallak wallak", 14, genesis);



    //     printf("char is %c, as int is %d, uchar %u, uint %u, hex %x\n"
    //     , second->_sha[i]
    //     , (int)(second->_sha[i])
    //     , (unsigned char)(second->_sha[i])
    //     , (unsigned int)(second->_sha[i])
    //     , (second->_sha[i])
    //     );
    

    return 0;
} 
