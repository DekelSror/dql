

module_name="$1"

mkdir "$module_name"

echo "#ifndef dql_"$module_name"_h
#define dql_"$module_name"_h

typedef void* "$module_name"_t;




typedef struct 
{
    
    
} "$module_name"_api_t;


extern const "$module_name"_api_t "${module_name^}";



#endif // dql_"$module_name"_h" > "$module_name"/"$module_name".h

echo "#include \""$module_name".h\"

#define "$module_name"_thisify _"$module_name"_t* this = _this;

typedef struct
{

} _"$module_name"_t;

const "$module_name"_api_t "${module_name^}" = {}; " > "$module_name"/"$module_name".c


echo "#include \""$module_name".h\"


int main(void)
{

    return 0;
} " > "$module_name"/"$module_name"_test.c
