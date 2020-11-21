

module_name="$1"

mkdir "$module_name"

echo "#ifndef dql_"$module_name"_h
#define dql_"$module_name"_h

typedef void* "$module_name"_t;

typedef struct 
{
    "$module_name"_t(*create)();
    void(*free)("$module_name"_t);
} "$module_name"_api_t;

extern const "$module_name"_api_t "${module_name^}";

#endif // dql_"$module_name"_h" > "$module_name"/"$module_name".h

echo "#include \""$module_name".h\"

#define "$module_name"_thisify _"$module_name"_t* this = _this;

typedef struct
{

} _"$module_name"_t;

static "$module_name"_t Create();
static void Free("$module_name"_t _this);

const "$module_name"_api_t "${module_name^}" = {Create, Free}; " > "$module_name"/"$module_name".c


echo "#include \""$module_name".h\"


int main(void)
{

    return 0;
} " > "$module_name"/"$module_name"_test.c
