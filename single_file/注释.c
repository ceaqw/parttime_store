#ifdef __GNUC__		//定义头文件
#pragma GCC visibility push(default)
#endif
#include <string.h>			//头文件导入
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
#include <locale.h>
#ifdef __GNUC__
#pragma GCC visibility pop
#endif
#include "cJSON.h"	//头文件定义功能
#define true ((cJSON_bool)1)		time
#define false ((cJSON_bool)0)
typedef struct {
    const unsigned char *json;
    size_t position;
} error;
static error global_error = { NULL, 0 };
CJSON_PUBLIC(const char *) cJSON_GetErrorPtr(void)
{
    return (const char*) (global_error.json + global_error.position);
}
#if (CJSON_VERSION_MAJOR != 1) || (CJSON_VERSION_MINOR != 5) || (CJSON_VERSION_PATCH != 7)
    #error cJSON.h and cJSON.c have different versions. Make sure that both have the same.
#endif
CJSON_PUBLIC(const char*) cJSON_Version(void)
{
    static char version[15];
    sprintf(version, "%i.%i.%i", CJSON_VERSION_MAJOR, CJSON_VERSION_MINOR, CJSON_VERSION_PATCH);
    return version;
}
static int case_insensitive_strcmp(const unsigned char *string1, const unsigned char *string2)//对比字符串
{
    if ((string1 == NULL) || (string2 == NULL))//有空返回1
    {
        return 1;
    }
    if (string1 == string2)	//相等返回0
    {
        return 0;
    }
    for(; tolower(*string1) == tolower(*string2); (void)string1++, string2++)//遍历两个字符串，比较内容
    {
        if (*string1 == '\0')		//字符串1结束返回0
        {
            return 0;
        }
    }
    return tolower(*string1) - tolower(*string2);		//返回两个字符串的长度差
}
typedef struct internal_hooks			//链表信息储存结构体
{
    void *(*allocate)(size_t size);			//分配空间
    void (*deallocate)(void *pointer);		//删除分配的空间
    void *(*reallocate)(void *pointer, size_t size);		//重新分配空间
} internal_hooks;

static internal_hooks global_hooks = { malloc, free, realloc };

/*复制字符串函数*/
static unsigned char* cJSON_strdup(const unsigned char* string, const internal_hooks * const hooks)
{
    size_t length = 0;
    unsigned char *copy = NULL;
    if (string == NULL)			//空字符串，返回0
    {
        return NULL;
    }
    length = strlen((const char*)string) + sizeof("");		//得该字符串长度+1
    if (!(copy = (unsigned char*)hooks->allocate(length)))		//copy得到分配得空间为0时返回0
    {
        return NULL;
    }
    memcpy(copy, string, length);		//复制静态字符给copy
    return copy;		//返回copy
}
/*实现界面进入功能*/
CJSON_PUBLIC(void) cJSON_InitHooks(cJSON_Hooks* hooks)		//结构体内存管理函数
{
    if (hooks == NULL)		//不存在hooks，对hooks结构体进行分配
    { 
        global_hooks.allocate = malloc;
        global_hooks.deallocate = free;
        global_hooks.reallocate = realloc;
        return;
    }
    global_hooks.allocate = malloc;		//global_hooks.allocate分配空间
    if (hooks->malloc_fn != NULL)		//如果hooks->malloc_fn非空
    {
        global_hooks.allocate = hooks->malloc_fn;		//将值传给global_hooks.allocate
    }
    global_hooks.deallocate = free;		//释放global_hooks.deallocate空间
    if (hooks->free_fn != NULL)//同上
    {
        global_hooks.deallocate = hooks->free_fn;
    }
    global_hooks.reallocate = NULL;		//释放空间
    if ((global_hooks.allocate == malloc) && (global_hooks.deallocate == free))//满足global_hooks.allocate有空间global_hooks.deallocate被释放
    {
        global_hooks.reallocate = realloc;		//重新分配空间
    }
}
static cJSON *cJSON_New_Item(const internal_hooks * const hooks)		//传入数据分析
{
    cJSON* node = (cJSON*)hooks->allocate(sizeof(cJSON));//将传入得参数信息赋给node结构体
    if (node)		//在node数据结尾加上结束字符\0
    {
        memset(node, '\0', sizeof(cJSON));
    }
    return node;		//返回该字符信息
}//对话框功能

CJSON_PUBLIC(void) cJSON_Delete(cJSON *item)		//数据删除
{
    cJSON *next = NULL;		//创建临时数据储存结构体
    while (item != NULL)		//有信息存在
    {
        next = item->next;		
        if (!(item->type & cJSON_IsReference) && (item->child != NULL))		//以下都是判断传入的结构体对应的位置有无数据进行对应的处理
        {
            cJSON_Delete(item->child);		//删除对应得结构体信息
        }
        if (!(item->type & cJSON_IsReference) && (item->valuestring != NULL))
        {
            global_hooks.deallocate(item->valuestring);
        }
        if (!(item->type & cJSON_StringIsConst) && (item->string != NULL))
        {
            global_hooks.deallocate(item->string);
        }
        global_hooks.deallocate(item);
        item = next;
    }
}//登录界面

static unsigned char get_decimal_point(void)		//得到小数点得位置
{
    struct lconv *lconv = localeconv();				//创建结构体储存localeconv返回的数值，即数据格式得查询结构	
    return (unsigned char) lconv->decimal_point[0];		//返回检测到的小数点的位置
}

typedef struct		//定义解析缓冲区数据结构体，储存解析数据得不同信息
{
    const unsigned char *content;
    size_t length;	
    size_t offset;		
    size_t depth; 
    internal_hooks hooks;
} parse_buffer;	
/*此处定义得快捷判断宏代表对应的功能*/		
#define can_read(buffer, size) ((buffer != NULL) && (((buffer)->offset + size) <= (buffer)->length))		//信息可以读取
#define cannot_read(buffer, size) (!can_read(buffer, size))		//信息不可读取
#define can_access_at_index(buffer, index) ((buffer != NULL) && (((buffer)->offset + index) < (buffer)->length))//允许使用
#define cannot_access_at_index(buffer, index) (!can_access_at_index(buffer, index))		//禁止使用
#define buffer_at_offset(buffer) ((buffer)->content + (buffer)->offset)		//缓冲区清楚，关闭
//解析缓冲区数据长度
static cJSON_bool parse_number(cJSON * const item, parse_buffer * const input_buffer)		
{
    double number = 0;
    unsigned char *after_end = NULL;
    unsigned char number_c_string[64];
    unsigned char decimal_point = get_decimal_point();		//得到小数点得位置
    size_t i = 0;
    if ((input_buffer == NULL) || (input_buffer->content == NULL))//如果数据为空
    {
        return false;		//返回错误
    }
    for (i = 0; (i < (sizeof(number_c_string) - 1)) && can_access_at_index(input_buffer, i); i++)//循环运算
    {
        switch (buffer_at_offset(input_buffer)[i])
        {
            case '0':
            case '1':
            case '2':
            case '+':
            case '-':
            case 'e':
            case 'E':
                number_c_string[i] = buffer_at_offset(input_buffer)[i];
                break;
            case '.':
                number_c_string[i] = decimal_point;
                break;
            default:
                goto loop_end;
        }
    }//输入命令
loop_end:		//循环结束
    number_c_string[i] = '\0';		//字符串末尾加结束符
    number = strtod((const char*)number_c_string, (char**)&after_end);		//将字符串转换为浮点数
    if (number_c_string == after_end)		//如果为空返回错误
    {
        return false; 
    }
    item->valuedouble = number;		//将浮点数值储存在结构体中
    if (number >= INT_MAX)			//如果浮点数大于最大数值INT_MAX
    {
        item->valueint = INT_MAX;		//将INT_MAX存入信息结构体中
    }
    else if (number <= INT_MIN)		//除非将INT_MIN存入结构体，这里是有效长度储存
    {
        item->valueint = INT_MIN;
    }
    else		//除非有效长度为数据长度
    {
        item->valueint = (int)number;
    }
    item->type = cJSON_Number;
    input_buffer->offset += (size_t)(after_end - number_c_string);		//缓冲区信息清除
    return true;		//返回1
}
CJSON_PUBLIC(double) cJSON_SetNumberHelper(cJSON *object, double number)
{
    if (number >= INT_MAX)		//如果浮点数长度大于最大长度，将最大有效长度存入object结构体中
    {
        object->valueint = INT_MAX;
    }
    else if (number <= INT_MIN)	//如果浮点数长度小于最大长度，将最大有效长度存入object结构体中
    {
        object->valueint = INT_MIN;
    }
    else		
    {
        object->valueint = (int)number;		//有效长度为数据长度
    }
    return object->valuedouble = number;		//返回浮点数值
}//利用算法进行数据处理
typedef struct		//创建打印缓冲区信息结构体
{
    unsigned char *buffer;
    size_t length;
    size_t offset;
    size_t depth; 
    cJSON_bool noalloc;
    cJSON_bool format; 
    internal_hooks hooks;
} printbuffer;

static unsigned char* ensure(printbuffer * const p, size_t needed)		//打印缓冲区结构体信息处理
{
    unsigned char *newbuffer = NULL;
    size_t newsize = 0;
    if ((p == NULL) || (p->buffer == NULL))		//如果为空返回
    {
        return NULL;
    }
    if ((p->length > 0) && (p->offset >= p->length))		//缓冲区存在有效数据返回
    {
        return NULL;
    }
    if (needed > INT_MAX)			//数据长度大于最大有效值返回
    {
        return NULL;
    }
    needed += p->offset + 1;		//得到链表下一个位置得数据补偿长度
    if (needed <= p->length)		//小于数据长度
    {
        return p->buffer + p->offset;		//返回缓冲+补偿的数据大小
    }
    if (p->noalloc) {		//如果该结构体无信息，即结束，返回0
        return NULL;
    }
    if (needed > (INT_MAX / 2))		//数据长度大于最大有效值得一半
    {
        if (needed <= INT_MAX)		//小于最大有效值
        {
            newsize = INT_MAX;		//新的储存空间大小等于最大有效值
        }
        else		//除非但会0
        {
            return NULL;
        }
    }
    else		//除非新的储存空间大小等于needed的两倍
    {
        newsize = needed * 2;
    }
    if (p->hooks.reallocate != NULL)		//如果结构体重新分配的结构空间存在
    {
        newbuffer = (unsigned char*)p->hooks.reallocate(p->buffer, newsize);		//数据缓冲区重新分配大小
        if (newbuffer == NULL)		//如果未分配到，清除该结构体储存的数据
        {
            p->hooks.deallocate(p->buffer);
            p->length = 0;
            p->buffer = NULL;
            return NULL;
        }
    }
    else
    {
        newbuffer = (unsigned char*)p->hooks.allocate(newsize);////数据缓冲区重新分配大小
        if (!newbuffer)		//分配空间为0，始化该节点
        {
            p->hooks.deallocate(p->buffer);
            p->length = 0;
            p->buffer = NULL;
            return NULL;
        }
        if (newbuffer)
        {
            memcpy(newbuffer, p->buffer, p->offset + 1);//复制数据给newbuffer
        }
        p->hooks.deallocate(p->buffer);		//清楚缓冲区
    }
    p->length = newsize;		//该信息节点长度获取
    p->buffer = newbuffer;		//数据获取
    return newbuffer + p->offset;		//返回数据长度
}//调用采集的数据进行计算
static void update_offset(printbuffer * const buffer)		//更新打印信息节点数据补充量
{
    const unsigned char *buffer_pointer = NULL;
    if ((buffer == NULL) || (buffer->buffer == NULL))		//数据为空返回
    {
        return;
    }
    buffer_pointer = buffer->buffer + buffer->offset;		
    buffer->offset += strlen((const char*)buffer_pointer);	//数据补充长度为节点新增加量
}
static cJSON_bool print_number(const cJSON * const item, printbuffer * const output_buffer)		//打印数值
{
    unsigned char *output_pointer = NULL;
    unsigned char number_buffer[26]; 
    unsigned char decimal_point = get_decimal_point();
    double test;
    if (output_buffer == NULL)		//输出缓冲区为空返回
    {
        return false;
    }
    if ((d * 0) != 0)
    {
        length = sprintf((char*)number_buffer, "null");		//获取打印数据串长度
    }
    else
    {
        length = sprintf((char*)number_buffer, "%1.15g", d);//获取打印数据串长度
      
        if ((sscanf((char*)number_buffer, "%lg", &test) != 1) || ((double)test != d))
        {
            length = sprintf((char*)number_buffer, "%1.17g", d);//获取读取数据串长度
        }
    }
    if ((length < 0) || (length > (int)(sizeof(number_buffer) - 1)))
    {
        return false;
    }
    output_pointer = ensure(output_buffer, (size_t)length);		//输出信息及长度
    if (output_pointer == NULL)		//输出信息为空返回
    {
        return false;
    }
    for (i = 0; i < ((size_t)length); i++)		//循环整理要输出的数据
    {
        if (number_buffer[i] == decimal_point)
        {
            output_pointer[i] = '.';
            continue;
        }
        output_pointer[i] = number_buffer[i];
    }
    output_pointer[i] = '\0';
    output_buffer->offset += (size_t)length;
    return true;
}//调用数据进行计算

static cJSON *get_array_item(const cJSON *array, size_t item)		
{
    cJSON *child = array ? array->child : NULL;
    while ((child != NULL) && (item > 0))
    {
        item--;
        child = child->next;
    }
    return child;
}

static cJSON_bool decode_array_index_from_pointer(const unsigned char * const pointer, size_t * const index)
{
    size_t parsed_index = 0;
    size_t position = 0;
    if ((pointer[0] == '0') && ((pointer[1] != '\0') && (pointer[1] != '/')))		//分析数据是否结束
    {
        return 0;
    }
    for (position = 0; (pointer[position] >= '0') && (pointer[0] <= '9'); position++)		
    {
        parsed_index = (10 * parsed_index) + (size_t)(pointer[position] - '0');
    }
    if ((pointer[position] != '\0') && (pointer[position] != '/'))
    {
        return 0;
    }
    *index = parsed_index;
    return 1;
}//同上，调用数据进行计算

static cJSON *get_item_from_pointer(cJSON * const object, const char * pointer, const cJSON_bool case_sensitive)
{
    cJSON *current_element = object;
    if (pointer == NULL)		//数据为空返回
    {
        return NULL;
    }
    while ((pointer[0] == '/') && (current_element != NULL))
    {
        pointer++;
        if (cJSON_IsArray(current_element))
        {
            size_t index = 0;
            if (!decode_array_index_from_pointer((const unsigned char*)pointer, &index))
            {
                return NULL;
            }
            current_element = get_array_item(current_element, index);
        }
        else if (cJSON_IsObject(current_element))
        {
            current_element = current_element->child;
            
            while ((current_element != NULL) && !compare_pointers((unsigned char*)current_element->string, (const unsigned char*)pointer, case_sensitive))
            {
                current_element = current_element->next;
            }
        }
        else
        {
            return NULL;
        }
        while ((pointer[0] != '\0') && (pointer[0] != '/'))
        {
            pointer++;
        }
    }
    return current_element;		//此函数就是对传入的数据进行整理，将指针移到正确的位置
}//同上，调用数据进行计算

CJSON_PUBLIC(cJSON *) cJSONUtils_GetPointer(cJSON * const object, const char *pointer)//检测指针，返回指针位置
{
    return get_item_from_pointer(object, pointer, false);
}
CJSON_PUBLIC(cJSON *) cJSONUtils_GetPointerCaseSensitive(cJSON * const object, const char *pointer)
{
    return get_item_from_pointer(object, pointer, true);
}

static void decode_pointer_inplace(unsigned char *string)		//编译识别字符串信息
{
    unsigned char *decoded_string = string;
    if (string == NULL) {
        return;
    }
    for (; *string; (void)decoded_string++, string++)
    {
        if (string[0] == '~')		//循环检测，
        {
            if (string[1] == '0')
            {
                decoded_string[0] = '~';
            }
            else if (string[1] == '1')
            {
                decoded_string[1] = '/';
            }
            else
            {
                return;
            }
            string++;
        }
    }
    decoded_string[0] = '\0';		//清除该字符串
}
static cJSON *detach_item_from_array(cJSON *array, size_t which)
{
    cJSON *c = array->child;
    while (c && (which > 0))
    {
        c = c->next;
        which--;
    }
    if (!c)
    {
        return NULL;
    }
    if (c->prev)
    {
        c->prev->next = c->next;
    }
    if (c->next)
    {
        c->next->prev = c->prev;
    }
    if (c==array->child)
    {
        array->child = c->next;
    }
    c->prev = c->next = NULL;
    return c;
}//同上，调用数据进行计算
static cJSON *detach_path(cJSON *object, const unsigned char *path, const cJSON_bool case_sensitive)
{
    unsigned char *parent_pointer = NULL;
    unsigned char *child_pointer = NULL;
    cJSON *parent = NULL;
    cJSON *detached_item = NULL;
    parent_pointer = cJSONUtils_strdup(path);
    if (parent_pointer == NULL) {
        goto cleanup;
    }
    child_pointer = (unsigned char*)strrchr((char*)parent_pointer, '/'); 
    if (child_pointer == NULL)
    {
        goto cleanup;
    }
    child_pointer[0] = '\0';
    child_pointer++;
    parent = get_item_from_pointer(object, (char*)parent_pointer, case_sensitive);
    decode_pointer_inplace(child_pointer);
    if (cJSON_IsArray(parent))
    {
        size_t index = 0;
        detached_item = detach_item_from_array(parent, index);
    }
    else if (cJSON_IsObject(parent))
    {
        detached_item = cJSON_DetachItemFromObject(parent, (char*)child_pointer);
    }
    else
    {
        goto cleanup;
    }
cleanup:
    if (parent_pointer != NULL)
    {
        cJSON_free(parent_pointer);
    }
    return detached_item;
}//同上，调用数据进行计算
static cJSON *sort_list(cJSON *list, const cJSON_bool case_sensitive)
{
    cJSON *first = list;
    cJSON *second = list;
    cJSON *current_item = list;
    cJSON *result = list;
    cJSON *result_tail = NULL;
    if ((list == NULL) || (list->next == NULL))
    {
        return result;
    }
    while ((current_item != NULL) && (current_item->next != NULL) && (compare_strings((unsigned char*)current_item->string, (unsigned char*)current_item->next->string, case_sensitive) < 0))
    {
        current_item = current_item->next;
    }
    if ((current_item == NULL) || (current_item->next == NULL))
    {
        return result;
    }
    current_item = list;
    while (current_item != NULL)
    {
        second = second->next;
        current_item = current_item->next;
       
        if (current_item != NULL)
        {
            current_item = current_item->next;
        }
    }
    if ((second != NULL) && (second->prev != NULL))
    {
        second->prev->next = NULL;
    }
    first = sort_list(first, case_sensitive);
    second = sort_list(second, case_sensitive);
    result = NULL;
    while ((first != NULL) && (second != NULL))
    {
        cJSON *smaller = NULL;
        if (compare_strings((unsigned char*)first->string, (unsigned char*)second->string, false) < 0)
        {
            smaller = first;
        }
        else
        {
            smaller = second;
        }
        if (result == NULL)
        {
            result_tail = smaller;
            result = smaller;
        }
        else
        {
            result_tail->next = smaller;
            smaller->prev = result_tail;
            result_tail = smaller;
        }
        if (first == smaller)
        {
            first = first->next;
        }
        else
        {
            second = second->next;
        }
    }
    if (first != NULL)
    {
        if (result == NULL)
        {
            return first;
        }
        result_tail->next = first;
        first->prev = result_tail;
    }
    if (second != NULL)
    {
        if (result == NULL)
        {
            return second;
        }
        result_tail->next = second;
        second->prev = result_tail;
    }
    return result;
}//同上，调用数据进行计算
static void sort_object(cJSON * const object, const cJSON_bool case_sensitive)		//排序项目
{
    if (object == NULL)
    {
        return;
    }
    object->child = sort_list(object->child, case_sensitive);
}
static cJSON_bool compare_json(cJSON *a, cJSON *b, const cJSON_bool case_sensitive)
{
    if ((a == NULL) || (b == NULL) || ((a->type & 0xFF) != (b->type & 0xFF)))
    {
        return false;
    }
    switch (a->type & 0xFF)
    {
        case cJSON_Number:
            
            if ((a->valueint != b->valueint) || (a->valuedouble != b->valuedouble))
            {
                return false;
            }
            else
            {
                return true;
            }
        case cJSON_String:
         
            if (strcmp(a->valuestring, b->valuestring) != 0)
            {
                return false;
            }
            else
            {
                return true;
            }
        case cJSON_Array:
            for ((void)(a = a->child), b = b->child; (a != NULL) && (b != NULL); (void)(a = a->next), b = b->next)
            {
                cJSON_bool identical = compare_json(a, b, case_sensitive);
                if (!identical)
                {
                    return false;
                }
            }
            if ((a != NULL) || (b != NULL))
            {
                return false;
            }
            else
            {
                return true;
            }
        case cJSON_Object:
            sort_object(a, case_sensitive);
            sort_object(b, case_sensitive);
            for ((void)(a = a->child), b = b->child; (a != NULL) && (b != NULL); (void)(a = a->next), b = b->next)
            {
                cJSON_bool identical = false;
                
                if (compare_strings((unsigned char*)a->string, (unsigned char*)b->string, case_sensitive))
                {
                    return false;
                }
                identical = compare_json(a, b, case_sensitive);
                if (!identical)
                {
                    return false;
                }
            }
            if ((a != NULL) || (b != NULL))
            {
                return false;
            }
            else
            {
                return true;
            }
        default:
            break;
    }
    return true;
}//同上，调用数据进行计算
static cJSON_bool insert_item_in_array(cJSON *array, size_t which, cJSON *newitem)
{
    cJSON *child = array->child;
    while (child && (which > 0))
    {
        child = child->next;
        which--;
    }
    if (which > 0)
    {
        return 0;
    }
    if (child == NULL)
    {
        cJSON_AddItemToArray(array, newitem);
        return 1;
    }
    newitem->next = child;
    newitem->prev = child->prev;
    child->prev = newitem;
    if (child == array->child)
    {
        array->child = newitem;
    }
    else
    {
        newitem->prev->next = newitem;
    }
    return 1;
}//同上，调用数据进行计算
static cJSON *get_object_item(const cJSON * const object, const char* name, const cJSON_bool case_sensitive)
{
    if (case_sensitive)
    {
        return cJSON_GetObjectItemCaseSensitive(object, name);
    }
    return cJSON_GetObjectItem(object, name);
}
enum patch_operation { INVALID, ADD, REMOVE, REPLACE, MOVE, COPY, TEST };
static enum patch_operation decode_patch_operation(const cJSON * const patch, const cJSON_bool case_sensitive)
{
    cJSON *operation = get_object_item(patch, "op", case_sensitive);
    if (!cJSON_IsString(operation))
    {
        return INVALID;
    }
    if (strcmp(operation->valuestring, "add") == 0)
    {
        return ADD;
    }
    if (strcmp(operation->valuestring, "remove") == 0)
    {
        return REMOVE;
    }
    if (strcmp(operation->valuestring, "replace") == 0)
    {
        return REPLACE;
    }
    if (strcmp(operation->valuestring, "move") == 0)
    {
        return MOVE;
    }
    if (strcmp(operation->valuestring, "copy") == 0)
    {
        return COPY;
    }
    if (strcmp(operation->valuestring, "test") == 0)
    {
        return TEST;
    }
    return INVALID;
}//同上，调用数据进行计算
static void overwrite_item(cJSON * const root, const cJSON replacement)
{
    if (root == NULL)
    {
        return;
    }
    if (root->string != NULL)
    {
        cJSON_free(root->string);
    }
    if (root->valuestring != NULL)
    {
        cJSON_free(root->valuestring);
    }
    if (root->child != NULL)
    {
        cJSON_Delete(root->child);
    }
    memcpy(root, &replacement, sizeof(cJSON));
}//同上，调用数据进行计算
static int apply_patch(cJSON *object, const cJSON *patch, const cJSON_bool case_sensitive)
{
    cJSON *path = NULL;
    cJSON *value = NULL;
    cJSON *parent = NULL;
    enum patch_operation opcode = INVALID;
    unsigned char *parent_pointer = NULL;
    unsigned char *child_pointer = NULL;
    int status = 0;
    path = get_object_item(patch, "path", case_sensitive);
    if (!cJSON_IsString(path))
    {
        status = 2;
        goto cleanup;
    }
    opcode = decode_patch_operation(patch, case_sensitive);
    if (opcode == INVALID)
    {
        status = 3;
        goto cleanup;
    }
    else if (opcode == TEST)
    {
        status = !compare_json(get_item_from_pointer(object, path->valuestring, case_sensitive), get_object_item(patch, "value", case_sensitive), case_sensitive);
        goto cleanup;
    }
    if (path->valuestring[0] == '\0')
    {
        if (opcode == REMOVE)
        {
            static const cJSON invalid = { NULL, NULL, NULL, cJSON_Invalid, NULL, 0, 0, NULL};
            overwrite_item(object, invalid);
            status = 0;
            goto cleanup;
        }
        if ((opcode == REPLACE) || (opcode == ADD))
        {
            value = get_object_item(patch, "value", case_sensitive);
            if (value == NULL)
            {
                status = 7;
                goto cleanup;
            }
            value = cJSON_Duplicate(value, 1);
            if (value == NULL)
            {
                status = 8;
                goto cleanup;
            }
            overwrite_item(object, *value);
            cJSON_free(value);
            value = NULL;
            if (object->string != NULL)
            {
                cJSON_free(object->string);
                object->string = NULL;
            }
            status = 0;
            goto cleanup;
        }
    }
    if ((opcode == REMOVE) || (opcode == REPLACE))
    {
        cJSON *old_item = detach_path(object, (unsigned char*)path->valuestring, case_sensitive);
        if (old_item == NULL)
        {
            status = 13;
            goto cleanup;
        }
        cJSON_Delete(old_item);
        if (opcode == REMOVE)
        {
            status = 0;
            goto cleanup;
        }
    }
    if ((opcode == MOVE) || (opcode == COPY))
    {
        cJSON *from = get_object_item(patch, "from", case_sensitive);
        if (from == NULL)
        {
            status = 4;
            goto cleanup;
        }
        if (opcode == MOVE)
        {
            value = detach_path(object, (unsigned char*)from->valuestring, case_sensitive);
        }
        if (opcode == COPY)
        {
            value = get_item_from_pointer(object, from->valuestring, case_sensitive);
        }
        if (value == NULL)
        {
            status = 5;
            goto cleanup;
        }
        if (opcode == COPY)
        {
            value = cJSON_Duplicate(value, 1);
        }
        if (value == NULL)
        {
            status = 6;
            goto cleanup;
        }
    }
    else 
    {
        value = get_object_item(patch, "value", case_sensitive);
        if (value == NULL)
        {
            status = 7;
            goto cleanup;
        }
        value = cJSON_Duplicate(value, 1);
        if (value == NULL)
        {
            status = 8;
            goto cleanup;
        }
    }//同上，调用数据进行计算
    parent_pointer = cJSONUtils_strdup((unsigned char*)path->valuestring);
    child_pointer = (unsigned char*)strrchr((char*)parent_pointer, '/');
    if (child_pointer != NULL)
    {
        child_pointer[0] = '\0';
        child_pointer++;
    }
    parent = get_item_from_pointer(object, (char*)parent_pointer, case_sensitive);
    decode_pointer_inplace(child_pointer);
    if ((parent == NULL) || (child_pointer == NULL))
    {
        status = 9;
        goto cleanup;
    }
    else if (cJSON_IsArray(parent))
    {
        if (strcmp((char*)child_pointer, "-") == 0)
        {
            cJSON_AddItemToArray(parent, value);
            value = NULL;
        }
        else
        {
            size_t index = 0;
            if (!decode_array_index_from_pointer(child_pointer, &index))
            {
                status = 11;
                goto cleanup;
            }
            if (!insert_item_in_array(parent, index, value))
            {
                status = 10;
                goto cleanup;
            }
            value = NULL;
        }
    }
    else if (cJSON_IsObject(parent))
    {
        if (case_sensitive)
        {
            cJSON_DeleteItemFromObjectCaseSensitive(parent, (char*)child_pointer);
        }
        else
        {
            cJSON_DeleteItemFromObject(parent, (char*)child_pointer);
        }
        cJSON_AddItemToObject(parent, (char*)child_pointer, value);
        value = NULL;
    }
cleanup:
    if (value != NULL)
    {
        cJSON_Delete(value);
    }
    if (parent_pointer != NULL)
    {
        cJSON_free(parent_pointer);
    }
    return status;
}//同上，调用数据进行计算
CJSON_PUBLIC(int) cJSONUtils_ApplyPatches(cJSON * const object, const cJSON * const patches)
{
    const cJSON *current_patch = NULL;
    int status = 0;
    if (!cJSON_IsArray(patches))
    {
        return 1;
    }
    if (patches != NULL)
    {
        current_patch = patches->child;
    }
    while (current_patch != NULL)
    {
        status = apply_patch(object, current_patch, false);
        if (status != 0)
        {
            return status;
        }
        current_patch = current_patch->next;
    }
    return 0;
}//同上，调用数据进行计算
CJSON_PUBLIC(int) cJSONUtils_ApplyPatchesCaseSensitive(cJSON * const object, const cJSON * const patches)
{
    const cJSON *current_patch = NULL;
    int status = 0;
    if (!cJSON_IsArray(patches))
    {
        return 1;
    }
    if (patches != NULL)
    {
        current_patch = patches->child;
    }
    while (current_patch != NULL)
    {
        status = apply_patch(object, current_patch, true);
        if (status != 0)
        {
            return status;
        }
        current_patch = current_patch->next;
    }
    return 0;
}
static void compose_patch(cJSON * const patches, const unsigned char * const operation, const unsigned char * const path, const unsigned char *suffix, const cJSON * const value)
{
    cJSON *patch = NULL;
    if ((patches == NULL) || (operation == NULL) || (path == NULL))
    {
        return;
    }
    patch = cJSON_CreateObject();
    if (patch == NULL)
    {
        return;
    }
    cJSON_AddItemToObject(patch, "op", cJSON_CreateString((const char*)operation));
    if (suffix == NULL)
    {
        cJSON_AddItemToObject(patch, "path", cJSON_CreateString((const char*)path));
    }
    else
    {
        size_t suffix_length = pointer_encoded_length(suffix);
        size_t path_length = strlen((const char*)path);
        unsigned char *full_path = (unsigned char*)cJSON_malloc(path_length + suffix_length + sizeof("/"));
        sprintf((char*)full_path, "%s/", (const char*)path);
        encode_string_as_pointer(full_path + path_length + 1, suffix);
        cJSON_AddItemToObject(patch, "path", cJSON_CreateString((const char*)full_path));
        cJSON_free(full_path);
    }
    if (value != NULL)
    {
        cJSON_AddItemToObject(patch, "value", cJSON_Duplicate(value, 1));
    }
    cJSON_AddItemToArray(patches, patch);
}
CJSON_PUBLIC(void) cJSONUtils_AddPatchToArray(cJSON * const array, const char * const operation, const char * const path, const cJSON * const value)
{
    compose_patch(array, (const unsigned char*)operation, (const unsigned char*)path, NULL, value);
}
static void create_patches(cJSON * const patches, const unsigned char * const path, cJSON * const from, cJSON * const to, const cJSON_bool case_sensitive)
{
    if ((from == NULL) || (to == NULL))
    {
        return;
    }
    if ((from->type & 0xFF) != (to->type & 0xFF))
    {
        compose_patch(patches, (const unsigned char*)"replace", path, 0, to);
        return;
    }
    switch (from->type & 0xFF)
    {
        case cJSON_Number:
            if ((from->valueint != to->valueint) || (from->valuedouble != to->valuedouble))
            {
                compose_patch(patches, (const unsigned char*)"replace", path, NULL, to);
            }
            return;
        case cJSON_String:
            if (strcmp(from->valuestring, to->valuestring) != 0)
            {
                compose_patch(patches, (const unsigned char*)"replace", path, NULL, to);
            }
            return;
        case cJSON_Array:
        {
            size_t index = 0;
            cJSON *from_child = from->child;
            cJSON *to_child = to->child;
            unsigned char *new_path = (unsigned char*)cJSON_malloc(strlen((const char*)path) + 20 + sizeof("/")); 
            
            for (index = 0; (from_child != NULL) && (to_child != NULL); (void)(from_child = from_child->next), (void)(to_child = to_child->next), index++)
            {
                if (index > ULONG_MAX)
                {
                    cJSON_free(new_path);
                    return;
                }
                sprintf((char*)new_path, "%s/%lu", path, (unsigned long)index); 
                create_patches(patches, new_path, from_child, to_child, case_sensitive);
            }
            for (; (from_child != NULL); (void)(from_child = from_child->next))
            {
                if (index > ULONG_MAX)
                {
                    cJSON_free(new_path);
                    return;
                }
                sprintf((char*)new_path, "%lu", (unsigned long)index);
                compose_patch(patches, (const unsigned char*)"remove", path, new_path, NULL);
            }
            for (; (to_child != NULL); (void)(to_child = to_child->next), index++)
            {
                compose_patch(patches, (const unsigned char*)"add", path, (const unsigned char*)"-", to_child);
            }
            cJSON_free(new_path);
            return;
        }
        case cJSON_Object:
        {
            cJSON *from_child = NULL;
            cJSON *to_child = NULL;
            sort_object(from, case_sensitive);
            sort_object(to, case_sensitive);
            from_child = from->child;
            to_child = to->child;
          
            while ((from_child != NULL) || (to_child != NULL))
            {
                int diff;
                if (from_child == NULL)
                {
                    diff = 1;
                }
                else if (to_child == NULL)
                {
                    diff = -1;
                }
                else
                {
                    diff = compare_strings((unsigned char*)from_child->string, (unsigned char*)to_child->string, case_sensitive);
                }
                if (diff == 0)
                {
                    size_t path_length = strlen((const char*)path);
                    size_t from_child_name_length = pointer_encoded_length((unsigned char*)from_child->string);
                    unsigned char *new_path = (unsigned char*)cJSON_malloc(path_length + from_child_name_length + sizeof("/"));
                    sprintf((char*)new_path, "%s/", path);
                    encode_string_as_pointer(new_path + path_length + 1, (unsigned char*)from_child->string);
                    create_patches(patches, new_path, from_child, to_child, case_sensitive);
                    cJSON_free(new_path);
                    from_child = from_child->next;
                    to_child = to_child->next;
                }
                else if (diff < 0)
                {
                   
                    compose_patch(patches, (const unsigned char*)"remove", path, (unsigned char*)from_child->string, NULL);
                    from_child = from_child->next;
                }
                else
                {
                    compose_patch(patches, (const unsigned char*)"add", path, (unsigned char*)to_child->string, to_child);
                    to_child = to_child->next;
                }
            }
            return;
        }
        default:
            break;
    }
}//同上，调用数据进行计算


CJSON_PUBLIC(cJSON *) cJSONUtils_GeneratePatches(cJSON * const from, cJSON * const to)
{
    cJSON *patches = NULL;
    if ((from == NULL) || (to == NULL))
    {
        return NULL;
    }
    patches = cJSON_CreateArray();
    create_patches(patches, (const unsigned char*)"", from, to, false);
    return patches;
}
CJSON_PUBLIC(cJSON *) cJSONUtils_GeneratePatchesCaseSensitive(cJSON * const from, cJSON * const to)
{
    cJSON *patches = NULL;
    if ((from == NULL) || (to == NULL))
    {
        return NULL;
    }
    patches = cJSON_CreateArray();
    create_patches(patches, (const unsigned char*)"", from, to, true);
    return patches;
}
CJSON_PUBLIC(void) cJSONUtils_SortObject(cJSON * const object)
{
    sort_object(object, false);
}
CJSON_PUBLIC(void) cJSONUtils_SortObjectCaseSensitive(cJSON * const object)
{
    sort_object(object, true);
}
static cJSON *merge_patch(cJSON *target, const cJSON * const patch, const cJSON_bool case_sensitive)
{
    cJSON *patch_child = NULL;
    if (!cJSON_IsObject(patch))
    {
        cJSON_Delete(target);
        return cJSON_Duplicate(patch, 1);
    }
    if (!cJSON_IsObject(target))
    {
        cJSON_Delete(target);
        target = cJSON_CreateObject();
    }
    patch_child = patch->child;
    while (patch_child != NULL)
    {
        if (cJSON_IsNull(patch_child))
        {
            if (case_sensitive)
            {
                cJSON_DeleteItemFromObjectCaseSensitive(target, patch_child->string);
            }
            else
            {
                cJSON_DeleteItemFromObject(target, patch_child->string);
            }
        }
        else
        {
            cJSON *replace_me = NULL;
            cJSON *replacement = NULL;
            if (case_sensitive)
            {
                replace_me = cJSON_DetachItemFromObjectCaseSensitive(target, patch_child->string);
            }
            else
            {
                replace_me = cJSON_DetachItemFromObject(target, patch_child->string);
            }
            replacement = merge_patch(replace_me, patch_child, case_sensitive);
            if (replacement == NULL)
            {
                return NULL;
            }
            cJSON_AddItemToObject(target, patch_child->string, replacement);
        }
        patch_child = patch_child->next;
    }
    return target;
}//同上，调用数据进行计算
CJSON_PUBLIC(cJSON *) cJSONUtils_MergePatch(cJSON *target, const cJSON * const patch)
{
    return merge_patch(target, patch, false);
}
CJSON_PUBLIC(cJSON *) cJSONUtils_MergePatchCaseSensitive(cJSON *target, const cJSON * const patch)
{
    return merge_patch(target, patch, true);
}
static cJSON *generate_merge_patch(cJSON * const from, cJSON * const to, const cJSON_bool case_sensitive)
{
    cJSON *from_child = NULL;
    cJSON *to_child = NULL;
    cJSON *patch = NULL;
    if (to == NULL)
    {
        return cJSON_CreateNull();
    }
    if (!cJSON_IsObject(to) || !cJSON_IsObject(from))
    {
        return cJSON_Duplicate(to, 1);
    }
    sort_object(from, case_sensitive);
    sort_object(to, case_sensitive);
    from_child = from->child;
    to_child = to->child;
    patch = cJSON_CreateObject();
    while (from_child || to_child)
    {
        int diff;
        if (from_child != NULL)
        {
            if (to_child != NULL)
            {
                diff = strcmp(from_child->string, to_child->string);
            }
            else
            {
                diff = -1;
            }
        }
        else
        {
            diff = 1;
        }
        if (diff < 0)
        {
            cJSON_AddItemToObject(patch, from_child->string, cJSON_CreateNull());
            from_child = from_child->next;
        }
        else if (diff > 0)
        {
            cJSON_AddItemToObject(patch, to_child->string, cJSON_Duplicate(to_child, 1));
            to_child = to_child->next;
        }
        else
        {
            if (!compare_json(from_child, to_child, case_sensitive))
            {
                cJSON_AddItemToObject(patch, to_child->string, cJSONUtils_GenerateMergePatch(from_child, to_child));
            }
            from_child = from_child->next;
            to_child = to_child->next;
        }
    }
    if (patch->child == NULL)
    {
        cJSON_Delete(patch);
        return NULL;
    }
    return patch;
}//同上，调用数据进行计算
CJSON_PUBLIC(cJSON *) cJSONUtils_GenerateMergePatch(cJSON * const from, cJSON * const to)
{
    return generate_merge_patch(from, to, false);
}
CJSON_PUBLIC(cJSON *) cJSONUtils_GenerateMergePatchCaseSensitive(cJSON * const from, cJSON * const to)
{
    return generate_merge_patch(from, to, true);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
struct record
{
    const char *precision;
    double lat;
    double lon;
    const char *address;
    const char *city;
    const char *state;
    const char *zip;
    const char *country;
};
static int print_preallocated(cJSON *root)		//打印初始化的数据
{
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;
    out = cJSON_Print(root);
    
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }
    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }
    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        printf("cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            printf("cJSON_Print result:\n%s\n", out);
            printf("cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }
    printf("%s\n", buf);
    if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
        printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        printf("cJSON_Print result:\n%s\n", out);
        printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }
    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}//实现界面计算功能，通过计算得到结果
static void create_objects(void)			//创建项目
{
    cJSON *root = NULL;
    cJSON *fmt = NULL;
    cJSON *img = NULL;
    cJSON *thm = NULL;
    cJSON *fld = NULL;
    int i = 0;
    const char *strings[7] =
    {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };//定义新的main函数
   
    int numbers[3][3] =
    {
        {0, -1, 0},
        {1, 0, 0},
        {0 ,0, 1}
    };
    
    int ids[4] = { 116, 943, 234, 38793 };
   
    struct record fields[2] =
    {
        {
            "zip",
            37.7668,
            -1.223959e+2,
            "",
            "SAN FRANCISCO",
            "CA",
            "94107",
            "US"
        },
        {
            "zip",
            37.371991,
            -1.22026e+2,
            "",
            "SUNNYVALE",
            "CA",
            "94085",
            "US"
        }
    };//底层数据库
    volatile double zero = 0.0;
   
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
    cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "type", "rect");
    cJSON_AddNumberToObject(fmt, "width", 1920);
    cJSON_AddNumberToObject(fmt, "height", 1080);
    cJSON_AddFalseToObject (fmt, "interlace");
    cJSON_AddNumberToObject(fmt, "frame rate", 24);
    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }//定义函数的功能
    cJSON_Delete(root);
    root = cJSON_CreateStringArray(strings, 7);
    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);
    root = cJSON_CreateArray();
    for (i = 0; i < 3; i++)
    {
        cJSON_AddItemToArray(root, cJSON_CreateIntArray(numbers[i], 3));
    }
    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "Image", img = cJSON_CreateObject());
    cJSON_AddNumberToObject(img, "Width", 800);
    cJSON_AddNumberToObject(img, "Height", 600);
    cJSON_AddStringToObject(img, "Title", "View from 15th Floor");
    cJSON_AddItemToObject(img, "Thumbnail", thm = cJSON_CreateObject());
    cJSON_AddStringToObject(thm, "Url", "http: www.example.com/image/481989943");
    cJSON_AddNumberToObject(thm, "Height", 125);
    cJSON_AddStringToObject(thm, "Width", "100");
    cJSON_AddItemToObject(img, "IDs", cJSON_CreateIntArray(ids, 4));
    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);
    root = cJSON_CreateArray();
    for (i = 0; i < 2; i++)//登录界面功能
    {
        cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
        cJSON_AddStringToObject(fld, "precision", fields[i].precision);
        cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
        cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
        cJSON_AddStringToObject(fld, "Address", fields[i].address);
        cJSON_AddStringToObject(fld, "City", fields[i].city);
        cJSON_AddStringToObject(fld, "State", fields[i].state);
        cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
        cJSON_AddStringToObject(fld, "Country", fields[i].country);
    }
    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }//定义函数功能
    cJSON_Delete(root);
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "number", 1.0 / zero);
    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);
}
int main(void)
{
    printf("Version: %s\n", cJSON_Version());
    create_objects();
    return 0;
}
#include "server.h"
#include "cluster.h"
#include <fcntl.h>
#include <sys/stat.h>
typedef struct configEnum {
    const char *name;    
    const int val;
} configEnum;
configEnum maxmemory_policy_enum[] = {
    {"volatile-lru", MAXMEMORY_VOLATILE_LRU},
    {"volatile-lfu", MAXMEMORY_VOLATILE_LFU},
    {"volatile-random",MAXMEMORY_VOLATILE_RANDOM},
    {"volatile-ttl",MAXMEMORY_VOLATILE_TTL},
    {"allkeys-lru",MAXMEMORY_ALLKEYS_LRU},
    {"allkeys-lfu",MAXMEMORY_ALLKEYS_LFU},
    {"allkeys-random",MAXMEMORY_ALLKEYS_RANDOM},
    {"noeviction",MAXMEMORY_NO_EVICTION},
    {NULL, 0}
};
configEnum syslog_facility_enum[] = {
    {"user",    LOG_USER},
    {"local0",  LOG_LOCAL0},
    {"local1",  LOG_LOCAL1},
    {"local2",  LOG_LOCAL2},
    {"local3",  LOG_LOCAL3},
    {"local4",  LOG_LOCAL4},
    {"local5",  LOG_LOCAL5},
    {"local6",  LOG_LOCAL6},
    {"local7",  LOG_LOCAL7},
    {NULL, 0}
};
configEnum loglevel_enum[] = {
    {"debug", LL_DEBUG},
    {"verbose", LL_VERBOSE},
    {"notice", LL_NOTICE},
    {"warning", LL_WARNING},
    {NULL,0}
};
configEnum supervised_mode_enum[] = {
    {"upstart", SUPERVISED_UPSTART},
    {"systemd", SUPERVISED_SYSTEMD},
    {"auto", SUPERVISED_AUTODETECT},
    {"no", SUPERVISED_NONE},
    {NULL, 0}
};
configEnum aof_fsync_enum[] = {
    {"everysec", AOF_FSYNC_EVERYSEC},
    {"always", AOF_FSYNC_ALWAYS},
    {"no", AOF_FSYNC_NO},
    {NULL, 0}
};//计算数据
clientBufferLimitsConfig clientBufferLimitsDefaults[CLIENT_TYPE_OBUF_COUNT] = {
    {0, 0, 0}, 
    {1024*1024*256, 1024*1024*64, 60}, 
    {1024*1024*32, 1024*1024*8, 60} 
};
int configEnumGetValue(configEnum *ce, char *name) {
    while(ce->name != NULL) {
        if (!strcasecmp(ce->name,name)) return ce->val;
        ce++;
    }
    return INT_MIN;
}
const char *configEnumGetName(configEnum *ce, int val) {
    while(ce->name != NULL) {
        if (ce->val == val) return ce->name;
        ce++;
    }
    return NULL;
}
const char *configEnumGetNameOrUnknown(configEnum *ce, int val) {
    const char *name = configEnumGetName(ce,val);
    return name ? name : "unknown";
}
const char *evictPolicyToString(void) {
    return configEnumGetNameOrUnknown(maxmemory_policy_enum,server.maxmemory_policy);
}
int yesnotoi(char *s) {
    if (!strcasecmp(s,"yes")) return 1;
    else if (!strcasecmp(s,"no")) return 0;
    else return -1;
}//数据采集跟计算
void appendServerSaveParams(time_t seconds, int changes) {		//添加数据保存服务参数
    server.saveparams = zrealloc(server.saveparams,sizeof(struct saveparam)*(server.saveparamslen+1));
    server.saveparams[server.saveparamslen].seconds = seconds;
    server.saveparams[server.saveparamslen].changes = changes;
    server.saveparamslen++;
}
void resetServerSaveParams(void) {		//重设保存服务参数
    zfree(server.saveparams);
    server.saveparams = NULL;
    server.saveparamslen = 0;
}
void queueLoadModule(sds path, sds *argv, int argc) {		//下载队列模块管理
    int i;
    struct moduleLoadQueueEntry *loadmod;
    loadmod = zmalloc(sizeof(struct moduleLoadQueueEntry));
    loadmod->argv = zmalloc(sizeof(robj*)*argc);
    loadmod->path = sdsnew(path);
    loadmod->argc = argc;
    for (i = 0; i < argc; i++) {
        loadmod->argv[i] = createRawStringObject(argv[i],sdslen(argv[i]));
    }
    listAddNodeTail(server.loadmodule_queue,loadmod);
}
void loadServerConfigFromString(char *config) {		//从字符数据认证数据下载服务
    char *err = NULL;
    int linenum = 0, totlines, i;
    int slaveof_linenum = 0;
    sds *lines;
    lines = sdssplitlen(config,strlen(config),"\n",1,&totlines);
    for (i = 0; i < totlines; i++) {
        sds *argv;
        int argc;
        linenum = i+1;
        lines[i] = sdstrim(lines[i]," \t\r\n");
        if (lines[i][0] == '#' || lines[i][0] == '\0') continue;
        argv = sdssplitargs(lines[i],&argc);
        if (argv == NULL) {
            err = "Unbalanced quotes in configuration line";
            goto loaderr;
        }
        if (argc == 0) {
            sdsfreesplitres(argv,argc);
            continue;
        }//定义要实现的功能
        sdstolower(argv[0]);
        if (!strcasecmp(argv[0],"timeout") && argc == 2) {
            server.maxidletime = atoi(argv[1]);
            if (server.maxidletime < 0) {
                err = "Invalid timeout value"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"tcp-keepalive") && argc == 2) {
            server.tcpkeepalive = atoi(argv[1]);
            if (server.tcpkeepalive < 0) {
                err = "Invalid tcp-keepalive value"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"protected-mode") && argc == 2) {
            if ((server.protected_mode = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"port") && argc == 2) {
            server.port = atoi(argv[1]);
            if (server.port < 0 || server.port > 65535) {
                err = "Invalid port"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"tcp-backlog") && argc == 2) {
            server.tcp_backlog = atoi(argv[1]);
            if (server.tcp_backlog < 0) {
                err = "Invalid backlog value"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"bind") && argc >= 2) {
            int j, addresses = argc-1;
            if (addresses > CONFIG_BINDADDR_MAX) {
                err = "Too many bind addresses specified"; goto loaderr;
            }
            for (j = 0; j < addresses; j++)
                server.bindaddr[j] = zstrdup(argv[j+1]);
            server.bindaddr_count = addresses;
        } else if (!strcasecmp(argv[0],"unixsocket") && argc == 2) {
            server.unixsocket = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"unixsocketperm") && argc == 2) {
            errno = 0;
            server.unixsocketperm = (mode_t)strtol(argv[1], NULL, 8);
            if (errno || server.unixsocketperm > 0777) {
                err = "Invalid socket file permissions"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"save")) {
            if (argc == 3) {
                int seconds = atoi(argv[1]);
                int changes = atoi(argv[2]);
                if (seconds < 1 || changes < 0) {
                    err = "Invalid save parameters"; goto loaderr;
                }
                appendServerSaveParams(seconds,changes);
            } else if (argc == 2 && !strcasecmp(argv[1],"")) {
                resetServerSaveParams();
            }
        } else if (!strcasecmp(argv[0],"dir") && argc == 2) {
            if (chdir(argv[1]) == -1) {
                serverLog(LL_WARNING,"Can't chdir to '%s': %s",
                    argv[1], strerror(errno));
                exit(1);
            }
        } else if (!strcasecmp(argv[0],"loglevel") && argc == 2) {
            server.verbosity = configEnumGetValue(loglevel_enum,argv[1]);
            if (server.verbosity == INT_MIN) {
                err = "Invalid log level. "
                      "Must be one of debug, verbose, notice, warning";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"logfile") && argc == 2) {
            FILE *logfp;
            zfree(server.logfile);
            server.logfile = zstrdup(argv[1]);
            if (server.logfile[0] != '\0') {
                
                logfp = fopen(server.logfile,"a");
                if (logfp == NULL) {
                    err = sdscatprintf(sdsempty(),
                        "Can't open the log file: %s", strerror(errno));
                    goto loaderr;
                }
                fclose(logfp);
            }
        } else if (!strcasecmp(argv[0],"always-show-logo") && argc == 2) {
            if ((server.always_show_logo = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"syslog-enabled") && argc == 2) {
            if ((server.syslog_enabled = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"syslog-ident") && argc == 2) {
            if (server.syslog_ident) zfree(server.syslog_ident);
            server.syslog_ident = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"syslog-facility") && argc == 2) {
            server.syslog_facility =
                configEnumGetValue(syslog_facility_enum,argv[1]);
            if (server.syslog_facility == INT_MIN) {
                err = "Invalid log facility. Must be one of USER or between LOCAL0-LOCAL7";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"databases") && argc == 2) {
            server.dbnum = atoi(argv[1]);
            if (server.dbnum < 1) {
                err = "Invalid number of databases"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"include") && argc == 2) {
            loadServerConfig(argv[1],NULL);
        } else if (!strcasecmp(argv[0],"maxclients") && argc == 2) {
            server.maxclients = atoi(argv[1]);
            if (server.maxclients < 1) {
                err = "Invalid max clients limit"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"maxmemory") && argc == 2) {
            server.maxmemory = memtoll(argv[1],NULL);
        } else if (!strcasecmp(argv[0],"maxmemory-policy") && argc == 2) {
            server.maxmemory_policy =
                configEnumGetValue(maxmemory_policy_enum,argv[1]);
            if (server.maxmemory_policy == INT_MIN) {
                err = "Invalid maxmemory policy";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"maxmemory-samples") && argc == 2) {
            server.maxmemory_samples = atoi(argv[1]);
            if (server.maxmemory_samples <= 0) {
                err = "maxmemory-samples must be 1 or greater";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"lfu-log-factor") && argc == 2) {
            server.lfu_log_factor = atoi(argv[1]);
            if (server.maxmemory_samples < 0) {
                err = "lfu-log-factor must be 0 or greater";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"lfu-decay-time") && argc == 2) {
            server.lfu_decay_time = atoi(argv[1]);
            if (server.maxmemory_samples < 1) {
                err = "lfu-decay-time must be 0 or greater";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"slaveof") && argc == 3) {
            slaveof_linenum = linenum;
            server.masterhost = sdsnew(argv[1]);
            server.masterport = atoi(argv[2]);
            server.repl_state = REPL_STATE_CONNECT;
        } else if (!strcasecmp(argv[0],"repl-ping-slave-period") && argc == 2) {
            server.repl_ping_slave_period = atoi(argv[1]);
            if (server.repl_ping_slave_period <= 0) {
                err = "repl-ping-slave-period must be 1 or greater";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"repl-timeout") && argc == 2) {
            server.repl_timeout = atoi(argv[1]);
            if (server.repl_timeout <= 0) {
                err = "repl-timeout must be 1 or greater";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"repl-disable-tcp-nodelay") && argc==2) {
            if ((server.repl_disable_tcp_nodelay = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"repl-diskless-sync") && argc==2) {
            if ((server.repl_diskless_sync = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"repl-diskless-sync-delay") && argc==2) {
            server.repl_diskless_sync_delay = atoi(argv[1]);
            if (server.repl_diskless_sync_delay < 0) {
                err = "repl-diskless-sync-delay can't be negative";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"repl-backlog-size") && argc == 2) {
            long long size = memtoll(argv[1],NULL);
            if (size <= 0) {
                err = "repl-backlog-size must be 1 or greater.";
                goto loaderr;
            }
            resizeReplicationBacklog(size);
        } else if (!strcasecmp(argv[0],"repl-backlog-ttl") && argc == 2) {
            server.repl_backlog_time_limit = atoi(argv[1]);
            if (server.repl_backlog_time_limit < 0) {
                err = "repl-backlog-ttl can't be negative ";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"masterauth") && argc == 2) {
            zfree(server.masterauth);
            server.masterauth = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"slave-serve-stale-data") && argc == 2) {
            if ((server.repl_serve_stale_data = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"slave-read-only") && argc == 2) {
            if ((server.repl_slave_ro = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"rdbcompression") && argc == 2) {
            if ((server.rdb_compression = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"rdbchecksum") && argc == 2) {
            if ((server.rdb_checksum = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"activerehashing") && argc == 2) {
            if ((server.activerehashing = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"lazyfree-lazy-eviction") && argc == 2) {
            if ((server.lazyfree_lazy_eviction = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"lazyfree-lazy-expire") && argc == 2) {
            if ((server.lazyfree_lazy_expire = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"lazyfree-lazy-server-del") && argc == 2){
            if ((server.lazyfree_lazy_server_del = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"slave-lazy-flush") && argc == 2) {
            if ((server.repl_slave_lazy_flush = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"activedefrag") && argc == 2) {
            if ((server.active_defrag_enabled = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"daemonize") && argc == 2) {
            if ((server.daemonize = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"hz") && argc == 2) {
            server.hz = atoi(argv[1]);
            if (server.hz < CONFIG_MIN_HZ) server.hz = CONFIG_MIN_HZ;
            if (server.hz > CONFIG_MAX_HZ) server.hz = CONFIG_MAX_HZ;
        } else if (!strcasecmp(argv[0],"appendonly") && argc == 2) {
            int yes;
            if ((yes = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
            server.aof_state = yes ? AOF_ON : AOF_OFF;
        } else if (!strcasecmp(argv[0],"appendfilename") && argc == 2) {
            if (!pathIsBaseName(argv[1])) {
                err = "appendfilename can't be a path, just a filename";
                goto loaderr;
            }
            zfree(server.aof_filename);
            server.aof_filename = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"no-appendfsync-on-rewrite")
                   && argc == 2) {
            if ((server.aof_no_fsync_on_rewrite= yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"appendfsync") && argc == 2) {
            server.aof_fsync = configEnumGetValue(aof_fsync_enum,argv[1]);
            if (server.aof_fsync == INT_MIN) {
                err = "argument must be 'no', 'always' or 'everysec'";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"auto-aof-rewrite-percentage") &&
                   argc == 2)
        {
            server.aof_rewrite_perc = atoi(argv[1]);
            if (server.aof_rewrite_perc < 0) {
                err = "Invalid negative percentage for AOF auto rewrite";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"auto-aof-rewrite-min-size") &&
                   argc == 2)
        {
            server.aof_rewrite_min_size = memtoll(argv[1],NULL);
        } else if (!strcasecmp(argv[0],"aof-rewrite-incremental-fsync") &&
                   argc == 2)
        {
            if ((server.aof_rewrite_incremental_fsync =
                 yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"aof-load-truncated") && argc == 2) {
            if ((server.aof_load_truncated = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"aof-use-rdb-preamble") && argc == 2) {
            if ((server.aof_use_rdb_preamble = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"requirepass") && argc == 2) {
            if (strlen(argv[1]) > CONFIG_AUTHPASS_MAX_LEN) {
                err = "Password is longer than CONFIG_AUTHPASS_MAX_LEN";
                goto loaderr;
            }
            server.requirepass = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"pidfile") && argc == 2) {
            zfree(server.pidfile);
            server.pidfile = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"dbfilename") && argc == 2) {
            if (!pathIsBaseName(argv[1])) {
                err = "dbfilename can't be a path, just a filename";
                goto loaderr;
            }
            zfree(server.rdb_filename);
            server.rdb_filename = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"active-defrag-threshold-lower") && argc == 2) {
            server.active_defrag_threshold_lower = atoi(argv[1]);
            if (server.active_defrag_threshold_lower < 0) {
                err = "active-defrag-threshold-lower must be 0 or greater";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"active-defrag-threshold-upper") && argc == 2) {
            server.active_defrag_threshold_upper = atoi(argv[1]);
            if (server.active_defrag_threshold_upper < 0) {
                err = "active-defrag-threshold-upper must be 0 or greater";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"active-defrag-ignore-bytes") && argc == 2) {
            server.active_defrag_ignore_bytes = memtoll(argv[1], NULL);
            if (server.active_defrag_ignore_bytes <= 0) {
                err = "active-defrag-ignore-bytes must above 0";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"active-defrag-cycle-min") && argc == 2) {
            server.active_defrag_cycle_min = atoi(argv[1]);
            if (server.active_defrag_cycle_min < 1 || server.active_defrag_cycle_min > 99) {
                err = "active-defrag-cycle-min must be between 1 and 99";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"active-defrag-cycle-max") && argc == 2) {
            server.active_defrag_cycle_max = atoi(argv[1]);
            if (server.active_defrag_cycle_max < 1 || server.active_defrag_cycle_max > 99) {
                err = "active-defrag-cycle-max must be between 1 and 99";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"hash-max-ziplist-entries") && argc == 2) {
            server.hash_max_ziplist_entries = memtoll(argv[1], NULL);
        } else if (!strcasecmp(argv[0],"hash-max-ziplist-value") && argc == 2) {
            server.hash_max_ziplist_value = memtoll(argv[1], NULL);
        } else if (!strcasecmp(argv[0],"list-max-ziplist-entries") && argc == 2){
            
        } else if (!strcasecmp(argv[0],"list-max-ziplist-value") && argc == 2) {
           
        } else if (!strcasecmp(argv[0],"list-max-ziplist-size") && argc == 2) {
            server.list_max_ziplist_size = atoi(argv[1]);
        } else if (!strcasecmp(argv[0],"list-compress-depth") && argc == 2) {
            server.list_compress_depth = atoi(argv[1]);
        } else if (!strcasecmp(argv[0],"set-max-intset-entries") && argc == 2) {
            server.set_max_intset_entries = memtoll(argv[1], NULL);
        } else if (!strcasecmp(argv[0],"zset-max-ziplist-entries") && argc == 2) {
            server.zset_max_ziplist_entries = memtoll(argv[1], NULL);
        } else if (!strcasecmp(argv[0],"zset-max-ziplist-value") && argc == 2) {
            server.zset_max_ziplist_value = memtoll(argv[1], NULL);
        } else if (!strcasecmp(argv[0],"hll-sparse-max-bytes") && argc == 2) {
            server.hll_sparse_max_bytes = memtoll(argv[1], NULL);
        } else if (!strcasecmp(argv[0],"rename-command") && argc == 3) {
            struct redisCommand *cmd = lookupCommand(argv[1]);
            int retval;
            if (!cmd) {
                err = "No such command in rename-command";
                goto loaderr;
            }
            retval = dictDelete(server.commands, argv[1]);
            serverAssert(retval == DICT_OK);
            if (sdslen(argv[2]) != 0) {
                sds copy = sdsdup(argv[2]);
                retval = dictAdd(server.commands, copy, cmd);
                if (retval != DICT_OK) {
                    sdsfree(copy);
                    err = "Target command name already exists"; goto loaderr;
                }
            }
        } else if (!strcasecmp(argv[0],"cluster-enabled") && argc == 2) {
            if ((server.cluster_enabled = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"cluster-config-file") && argc == 2) {
            zfree(server.cluster_configfile);
            server.cluster_configfile = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"cluster-announce-ip") && argc == 2) {
            zfree(server.cluster_announce_ip);
            server.cluster_announce_ip = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"cluster-announce-port") && argc == 2) {
            server.cluster_announce_port = atoi(argv[1]);
            if (server.cluster_announce_port < 0 ||
                server.cluster_announce_port > 65535)
            {
                err = "Invalid port"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"cluster-announce-bus-port") &&
                   argc == 2)
        {
            server.cluster_announce_bus_port = atoi(argv[1]);
            if (server.cluster_announce_bus_port < 0 ||
                server.cluster_announce_bus_port > 65535)
            {
                err = "Invalid port"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"cluster-require-full-coverage") &&
                    argc == 2)
        {
            if ((server.cluster_require_full_coverage = yesnotoi(argv[1])) == -1)
            {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"cluster-node-timeout") && argc == 2) {
            server.cluster_node_timeout = strtoll(argv[1],NULL,10);
            if (server.cluster_node_timeout <= 0) {
                err = "cluster node timeout must be 1 or greater"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"cluster-migration-barrier")
                   && argc == 2)
        {
            server.cluster_migration_barrier = atoi(argv[1]);
            if (server.cluster_migration_barrier < 0) {
                err = "cluster migration barrier must zero or positive";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"cluster-slave-validity-factor")
                   && argc == 2)
        {
            server.cluster_slave_validity_factor = atoi(argv[1]);
            if (server.cluster_slave_validity_factor < 0) {
                err = "cluster slave validity factor must be zero or positive";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"lua-time-limit") && argc == 2) {
            server.lua_time_limit = strtoll(argv[1],NULL,10);
        } else if (!strcasecmp(argv[0],"slowlog-log-slower-than") &&
                   argc == 2)
        {
            server.slowlog_log_slower_than = strtoll(argv[1],NULL,10);
        } else if (!strcasecmp(argv[0],"latency-monitor-threshold") &&
                   argc == 2)
        {
            server.latency_monitor_threshold = strtoll(argv[1],NULL,10);
            if (server.latency_monitor_threshold < 0) {
                err = "The latency threshold can't be negative";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"slowlog-max-len") && argc == 2) {
            server.slowlog_max_len = strtoll(argv[1],NULL,10);
        } else if (!strcasecmp(argv[0],"client-output-buffer-limit") &&
                   argc == 5)
        {
            int class = getClientTypeByName(argv[1]);
            unsigned long long hard, soft;
            int soft_seconds;
            if (class == -1 || class == CLIENT_TYPE_MASTER) {
                err = "Unrecognized client limit class: the user specified "
                "an invalid one, or 'master' which has no buffer limits.";
                goto loaderr;
            }
            hard = memtoll(argv[2],NULL);
            soft = memtoll(argv[3],NULL);
            soft_seconds = atoi(argv[4]);
            if (soft_seconds < 0) {
                err = "Negative number of seconds in soft limit is invalid";
                goto loaderr;
            }
            server.client_obuf_limits[class].hard_limit_bytes = hard;
            server.client_obuf_limits[class].soft_limit_bytes = soft;
            server.client_obuf_limits[class].soft_limit_seconds = soft_seconds;
        } else if (!strcasecmp(argv[0],"stop-writes-on-bgsave-error") &&
                   argc == 2) {
            if ((server.stop_writes_on_bgsave_err = yesnotoi(argv[1])) == -1) {
                err = "argument must be 'yes' or 'no'"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"slave-priority") && argc == 2) {
            server.slave_priority = atoi(argv[1]);
        } else if (!strcasecmp(argv[0],"slave-announce-ip") && argc == 2) {
            zfree(server.slave_announce_ip);
            server.slave_announce_ip = zstrdup(argv[1]);
        } else if (!strcasecmp(argv[0],"slave-announce-port") && argc == 2) {
            server.slave_announce_port = atoi(argv[1]);
            if (server.slave_announce_port < 0 ||
                server.slave_announce_port > 65535)
            {
                err = "Invalid port"; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"min-slaves-to-write") && argc == 2) {
            server.repl_min_slaves_to_write = atoi(argv[1]);
            if (server.repl_min_slaves_to_write < 0) {
                err = "Invalid value for min-slaves-to-write."; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"min-slaves-max-lag") && argc == 2) {
            server.repl_min_slaves_max_lag = atoi(argv[1]);
            if (server.repl_min_slaves_max_lag < 0) {
                err = "Invalid value for min-slaves-max-lag."; goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"notify-keyspace-events") && argc == 2) {
            int flags = keyspaceEventsStringToFlags(argv[1]);
            if (flags == -1) {
                err = "Invalid event class character. Use 'g$lshzxeA'.";
                goto loaderr;
            }
            server.notify_keyspace_events = flags;
        } else if (!strcasecmp(argv[0],"supervised") && argc == 2) {
            server.supervised_mode =
                configEnumGetValue(supervised_mode_enum,argv[1]);
            if (server.supervised_mode == INT_MIN) {
                err = "Invalid option for 'supervised'. "
                    "Allowed values: 'upstart', 'systemd', 'auto', or 'no'";
                goto loaderr;
            }
        } else if (!strcasecmp(argv[0],"loadmodule") && argc >= 2) {
            queueLoadModule(argv[1],&argv[2],argc-2);
        } else if (!strcasecmp(argv[0],"sentinel")) {
            
            if (argc != 1) {
                if (!server.sentinel_mode) {
                    err = "sentinel directive while not in sentinel mode";
                    goto loaderr;
                }
                err = sentinelHandleConfiguration(argv+1,argc-1);
                if (err) goto loaderr;
            }
        } else {
            err = "Bad directive or wrong number of arguments"; goto loaderr;
        }
        sdsfreesplitres(argv,argc);
    }
    if (server.cluster_enabled && server.masterhost) {
        linenum = slaveof_linenum;
        i = linenum-1;
        err = "slaveof directive not allowed in cluster mode";
        goto loaderr;		//调至loaderr执行
    }
    sdsfreesplitres(lines,totlines);
    return;
loaderr:
    fprintf(stderr, "\n*** FATAL CONFIG FILE ERROR ***\n");		//
    fprintf(stderr, "Reading the configuration file, at line %d\n", linenum);
    fprintf(stderr, ">>> '%s'\n", lines[i]);
    fprintf(stderr, "%s\n", err);
    exit(1);
}//界面绘制功能，创作一个界面
void loadServerConfig(char *filename, char *options) {		//数据下载服务认证
    sds config = sdsempty();
    char buf[CONFIG_MAX_LINE+1];
  
    if (filename) {
        FILE *fp;
        if (filename[0] == '-' && filename[1] == '\0') {
            fp = stdin;
        } else {
            if ((fp = fopen(filename,"r")) == NULL) {
                serverLog(LL_WARNING,
                    "Fatal error, can't open config file '%s'", filename);
                exit(1);
            }
        }
        while(fgets(buf,CONFIG_MAX_LINE+1,fp) != NULL)
            config = sdscat(config,buf);
        if (fp != stdin) fclose(fp);
    }
    if (options) {
        config = sdscat(config,"\n");
        config = sdscat(config,options);
    }
    loadServerConfigFromString(config);
    sdsfree(config);
}
#define config_set_bool_field(_name,_var) \
    } else if (!strcasecmp(c->argv[2]->ptr,_name)) { \
        int yn = yesnotoi(o->ptr); \
        if (yn == -1) goto badfmt; \
        _var = yn;
#define config_set_numerical_field(_name,_var,min,max) \
    } else if (!strcasecmp(c->argv[2]->ptr,_name)) { \
        if (getLongLongFromObject(o,&ll) == C_ERR) goto badfmt; \
        if (min != LLONG_MIN && ll < min) goto badfmt; \
        if (max != LLONG_MAX && ll > max) goto badfmt; \
        _var = ll;
#define config_set_memory_field(_name,_var) \
    } else if (!strcasecmp(c->argv[2]->ptr,_name)) { \
        ll = memtoll(o->ptr,&err); \
        if (err || ll < 0) goto badfmt; \
        _var = ll;
#define config_set_enum_field(_name,_var,_enumvar) \
    } else if (!strcasecmp(c->argv[2]->ptr,_name)) { \
        int enumval = configEnumGetValue(_enumvar,o->ptr); \
        if (enumval == INT_MIN) goto badfmt; \
        _var = enumval;
#define config_set_special_field(_name) \
    } else if (!strcasecmp(c->argv[2]->ptr,_name)) {
#define config_set_else } else//界面里绘制工作框
void configSetCommand(client *c) {			//认证指令设置
    robj *o;
    long long ll;
    int err;
    serverAssertWithInfo(c,c->argv[2],sdsEncodedObject(c->argv[2]));
    serverAssertWithInfo(c,c->argv[3],sdsEncodedObject(c->argv[3]));
    o = c->argv[3];
    if (0) { 
    config_set_special_field("dbfilename") {
        if (!pathIsBaseName(o->ptr)) {
            addReplyError(c, "dbfilename can't be a path, just a filename");
            return;
        }
        zfree(server.rdb_filename);
        server.rdb_filename = zstrdup(o->ptr);
    } config_set_special_field("requirepass") {
        if (sdslen(o->ptr) > CONFIG_AUTHPASS_MAX_LEN) goto badfmt;
        zfree(server.requirepass);
        server.requirepass = ((char*)o->ptr)[0] ? zstrdup(o->ptr) : NULL;
    } config_set_special_field("masterauth") {
        zfree(server.masterauth);
        server.masterauth = ((char*)o->ptr)[0] ? zstrdup(o->ptr) : NULL;
    } config_set_special_field("cluster-announce-ip") {
        zfree(server.cluster_announce_ip);
        server.cluster_announce_ip = ((char*)o->ptr)[0] ? zstrdup(o->ptr) : NULL;
    } config_set_special_field("maxclients") {
        int orig_value = server.maxclients;
        if (getLongLongFromObject(o,&ll) == C_ERR || ll < 1) goto badfmt;
        server.maxclients = ll;
        if (ll > orig_value) {
            adjustOpenFilesLimit();
            if (server.maxclients != ll) {
                addReplyErrorFormat(c,"The operating system is not able to handle the specified number of clients, try with %d", server.maxclients);
                server.maxclients = orig_value;
                return;
            }
            if ((unsigned int) aeGetSetSize(server.el) <
                server.maxclients + CONFIG_FDSET_INCR)
            {
                if (aeResizeSetSize(server.el,
                    server.maxclients + CONFIG_FDSET_INCR) == AE_ERR)
                {
                    addReplyError(c,"The event loop API used by Redis is not able to handle the specified number of clients");
                    server.maxclients = orig_value;
                    return;
                }
            }
        }
    } config_set_special_field("appendonly") {
        int enable = yesnotoi(o->ptr);
        if (enable == -1) goto badfmt;
        if (enable == 0 && server.aof_state != AOF_OFF) {
            stopAppendOnly();
        } else if (enable && server.aof_state == AOF_OFF) {
            if (startAppendOnly() == C_ERR) {
                addReplyError(c,
                    "Unable to turn on AOF. Check server logs.");
                return;
            }
        }
    } config_set_special_field("save") {
        int vlen, j;
        sds *v = sdssplitlen(o->ptr,sdslen(o->ptr)," ",1,&vlen);
        if (vlen & 1) {
            sdsfreesplitres(v,vlen);
            goto badfmt;
        }
        for (j = 0; j < vlen; j++) {
            char *eptr;
            long val;
            val = strtoll(v[j], &eptr, 10);
            if (eptr[0] != '\0' ||
                ((j & 1) == 0 && val < 1) ||
                ((j & 1) == 1 && val < 0)) {
                sdsfreesplitres(v,vlen);
                goto badfmt;
            }
        }
        resetServerSaveParams();
        for (j = 0; j < vlen; j += 2) {
            time_t seconds;
            int changes;
            seconds = strtoll(v[j],NULL,10);
            changes = strtoll(v[j+1],NULL,10);
            appendServerSaveParams(seconds, changes);
        }
        sdsfreesplitres(v,vlen);
    } config_set_special_field("dir") {
        if (chdir((char*)o->ptr) == -1) {
            addReplyErrorFormat(c,"Changing directory: %s", strerror(errno));
            return;
        }
    } config_set_special_field("client-output-buffer-limit") {
        int vlen, j;
        sds *v = sdssplitlen(o->ptr,sdslen(o->ptr)," ",1,&vlen);
        if (vlen % 4) {
            sdsfreesplitres(v,vlen);
            goto badfmt;
        }
        for (j = 0; j < vlen; j++) {
            long val;
            if ((j % 4) == 0) {
                int class = getClientTypeByName(v[j]);
                if (class == -1 || class == CLIENT_TYPE_MASTER) {
                    sdsfreesplitres(v,vlen);
                    goto badfmt;
                }
            } else {
                val = memtoll(v[j], &err);
                if (err || val < 0) {
                    sdsfreesplitres(v,vlen);
                    goto badfmt;
                }
            }
        }
        for (j = 0; j < vlen; j += 4) {
            int class;
            unsigned long long hard, soft;
            int soft_seconds;
            class = getClientTypeByName(v[j]);
            hard = strtoll(v[j+1],NULL,10);
            soft = strtoll(v[j+2],NULL,10);
            soft_seconds = strtoll(v[j+3],NULL,10);
            server.client_obuf_limits[class].hard_limit_bytes = hard;
            server.client_obuf_limits[class].soft_limit_bytes = soft;
            server.client_obuf_limits[class].soft_limit_seconds = soft_seconds;
        }
        sdsfreesplitres(v,vlen);
    } config_set_special_field("notify-keyspace-events") {
        int flags = keyspaceEventsStringToFlags(o->ptr);
        if (flags == -1) goto badfmt;
        server.notify_keyspace_events = flags;
    } config_set_special_field("slave-announce-ip") {
        zfree(server.slave_announce_ip);
        server.slave_announce_ip = ((char*)o->ptr)[0] ? zstrdup(o->ptr) : NULL;
    } config_set_bool_field(
      "rdbcompression", server.rdb_compression) {
    } config_set_bool_field(
      "repl-disable-tcp-nodelay",server.repl_disable_tcp_nodelay) {
    } config_set_bool_field(
      "repl-diskless-sync",server.repl_diskless_sync) {
    } config_set_bool_field(
      "cluster-require-full-coverage",server.cluster_require_full_coverage) {
    } config_set_bool_field(
      "aof-rewrite-incremental-fsync",server.aof_rewrite_incremental_fsync) {
    } config_set_bool_field(
      "aof-load-truncated",server.aof_load_truncated) {
    } config_set_bool_field(
      "aof-use-rdb-preamble",server.aof_use_rdb_preamble) {
    } config_set_bool_field(
      "slave-serve-stale-data",server.repl_serve_stale_data) {
    } config_set_bool_field(
      "slave-read-only",server.repl_slave_ro) {
    } config_set_bool_field(
      "activerehashing",server.activerehashing) {
    } config_set_bool_field(
      "activedefrag",server.active_defrag_enabled) {
#ifndef HAVE_DEFRAG
        if (server.active_defrag_enabled) {
            server.active_defrag_enabled = 0;
            addReplyError(c,
                "Active defragmentation cannot be enabled: it requires a "
                "Redis server compiled with a modified Jemalloc like the "
                "one shipped by default with the Redis source distribution");
            return;
        }//定义工作框的函数属性，并赋值
#endif
    } config_set_bool_field(
      "protected-mode",server.protected_mode) {
    } config_set_bool_field(
      "stop-writes-on-bgsave-error",server.stop_writes_on_bgsave_err) {
    } config_set_bool_field(
      "lazyfree-lazy-eviction",server.lazyfree_lazy_eviction) {
    } config_set_bool_field(
      "lazyfree-lazy-expire",server.lazyfree_lazy_expire) {
    } config_set_bool_field(
      "lazyfree-lazy-server-del",server.lazyfree_lazy_server_del) {
    } config_set_bool_field(
      "slave-lazy-flush",server.repl_slave_lazy_flush) {
    } config_set_bool_field(
      "no-appendfsync-on-rewrite",server.aof_no_fsync_on_rewrite) {
    } config_set_numerical_field(
      "tcp-keepalive",server.tcpkeepalive,0,LLONG_MAX) {
    } config_set_numerical_field(
      "maxmemory-samples",server.maxmemory_samples,1,LLONG_MAX) {
    } config_set_numerical_field(
      "lfu-log-factor",server.lfu_log_factor,0,LLONG_MAX) {
    } config_set_numerical_field(
      "lfu-decay-time",server.lfu_decay_time,0,LLONG_MAX) {
    } config_set_numerical_field(
      "timeout",server.maxidletime,0,LONG_MAX) {
    } config_set_numerical_field(
      "active-defrag-threshold-lower",server.active_defrag_threshold_lower,0,1000) {
    } config_set_numerical_field(
      "active-defrag-threshold-upper",server.active_defrag_threshold_upper,0,1000) {
    } config_set_memory_field(
      "active-defrag-ignore-bytes",server.active_defrag_ignore_bytes) {
    } config_set_numerical_field(
      "active-defrag-cycle-min",server.active_defrag_cycle_min,1,99) {
    } config_set_numerical_field(
      "active-defrag-cycle-max",server.active_defrag_cycle_max,1,99) {
    } config_set_numerical_field(
      "auto-aof-rewrite-percentage",server.aof_rewrite_perc,0,LLONG_MAX){
    } config_set_numerical_field(
      "hash-max-ziplist-entries",server.hash_max_ziplist_entries,0,LLONG_MAX) {
    } config_set_numerical_field(
      "hash-max-ziplist-value",server.hash_max_ziplist_value,0,LLONG_MAX) {
    } config_set_numerical_field(
      "list-max-ziplist-size",server.list_max_ziplist_size,INT_MIN,INT_MAX) {
    } config_set_numerical_field(
      "list-compress-depth",server.list_compress_depth,0,INT_MAX) {
    } config_set_numerical_field(
      "set-max-intset-entries",server.set_max_intset_entries,0,LLONG_MAX) {
    } config_set_numerical_field(
      "zset-max-ziplist-entries",server.zset_max_ziplist_entries,0,LLONG_MAX) {
    } config_set_numerical_field(
      "zset-max-ziplist-value",server.zset_max_ziplist_value,0,LLONG_MAX) {
    } config_set_numerical_field(
      "hll-sparse-max-bytes",server.hll_sparse_max_bytes,0,LLONG_MAX) {
    } config_set_numerical_field(
      "lua-time-limit",server.lua_time_limit,0,LLONG_MAX) {
    } config_set_numerical_field(
      "slowlog-log-slower-than",server.slowlog_log_slower_than,0,LLONG_MAX) {
    } config_set_numerical_field(
      "slowlog-max-len",ll,0,LLONG_MAX) {
     
        server.slowlog_max_len = (unsigned)ll;
    } config_set_numerical_field(
      "latency-monitor-threshold",server.latency_monitor_threshold,0,LLONG_MAX){
    } config_set_numerical_field(
      "repl-ping-slave-period",server.repl_ping_slave_period,1,LLONG_MAX) {
    } config_set_numerical_field(
      "repl-timeout",server.repl_timeout,1,LLONG_MAX) {
    } config_set_numerical_field(
      "repl-backlog-ttl",server.repl_backlog_time_limit,0,LLONG_MAX) {
    } config_set_numerical_field(
      "repl-diskless-sync-delay",server.repl_diskless_sync_delay,0,LLONG_MAX) {
    } config_set_numerical_field(
      "slave-priority",server.slave_priority,0,LLONG_MAX) {
    } config_set_numerical_field(
      "slave-announce-port",server.slave_announce_port,0,65535) {
    } config_set_numerical_field(
      "min-slaves-to-write",server.repl_min_slaves_to_write,0,LLONG_MAX) {
        refreshGoodSlavesCount();
    } config_set_numerical_field(
      "min-slaves-max-lag",server.repl_min_slaves_max_lag,0,LLONG_MAX) {
        refreshGoodSlavesCount();
    } config_set_numerical_field(
      "cluster-node-timeout",server.cluster_node_timeout,0,LLONG_MAX) {
    } config_set_numerical_field(
      "cluster-announce-port",server.cluster_announce_port,0,65535) {
    } config_set_numerical_field(
      "cluster-announce-bus-port",server.cluster_announce_bus_port,0,65535) {
    } config_set_numerical_field(
      "cluster-migration-barrier",server.cluster_migration_barrier,0,LLONG_MAX){
    } config_set_numerical_field(
      "cluster-slave-validity-factor",server.cluster_slave_validity_factor,0,LLONG_MAX) {
    } config_set_numerical_field(
      "hz",server.hz,0,LLONG_MAX) {
       
        if (server.hz < CONFIG_MIN_HZ) server.hz = CONFIG_MIN_HZ;
        if (server.hz > CONFIG_MAX_HZ) server.hz = CONFIG_MAX_HZ;
    } config_set_numerical_field(
      "watchdog-period",ll,0,LLONG_MAX) {
        if (ll)
            enableWatchdog(ll);
        else
            disableWatchdog();
    } config_set_memory_field("maxmemory",server.maxmemory) {
        if (server.maxmemory) {
            if (server.maxmemory < zmalloc_used_memory()) {
                serverLog(LL_WARNING,"WARNING: the new maxmemory value set via CONFIG SET is smaller than the current memory usage. This will result in keys eviction and/or inability to accept new write commands depending on the maxmemory-policy.");
            }
            freeMemoryIfNeeded();
        }
    } config_set_memory_field("repl-backlog-size",ll) {
        resizeReplicationBacklog(ll);
    } config_set_memory_field("auto-aof-rewrite-min-size",ll) {
        server.aof_rewrite_min_size = ll;
    } config_set_enum_field(
      "loglevel",server.verbosity,loglevel_enum) {
    } config_set_enum_field(
      "maxmemory-policy",server.maxmemory_policy,maxmemory_policy_enum) {
    } config_set_enum_field(
      "appendfsync",server.aof_fsync,aof_fsync_enum) {
    } config_set_else {
        addReplyErrorFormat(c,"Unsupported CONFIG parameter: %s",
            (char*)c->argv[2]->ptr);
        return;
    }
    addReply(c,shared.ok);
    return;
badfmt: 
    addReplyErrorFormat(c,"Invalid argument '%s' for CONFIG SET '%s'",
            (char*)o->ptr,
            (char*)c->argv[2]->ptr);
}
#define config_get_string_field(_name,_var) do { \
    if (stringmatch(pattern,_name,1)) { \
        addReplyBulkCString(c,_name); \
        addReplyBulkCString(c,_var ? _var : ""); \
        matches++; \
    } \
} while(0);
#define config_get_bool_field(_name,_var) do { \
    if (stringmatch(pattern,_name,1)) { \
        addReplyBulkCString(c,_name); \
        addReplyBulkCString(c,_var ? "yes" : "no"); \
        matches++; \
    } \
} while(0);
#define config_get_numerical_field(_name,_var) do { \
    if (stringmatch(pattern,_name,1)) { \
        ll2string(buf,sizeof(buf),_var); \
        addReplyBulkCString(c,_name); \
        addReplyBulkCString(c,buf); \
        matches++; \
    } \
} while(0);
#define config_get_enum_field(_name,_var,_enumvar) do { \
    if (stringmatch(pattern,_name,1)) { \
        addReplyBulkCString(c,_name); \
        addReplyBulkCString(c,configEnumGetNameOrUnknown(_enumvar,_var)); \
        matches++; \
    } \
} while(0);
void configGetCommand(client *c) {
    robj *o = c->argv[2];
    void *replylen = addDeferredMultiBulkLength(c);
    char *pattern = o->ptr;
    char buf[128];
    int matches = 0;
    serverAssertWithInfo(c,o,sdsEncodedObject(o));
    config_get_string_field("dbfilename",server.rdb_filename);
    config_get_string_field("requirepass",server.requirepass);
    config_get_string_field("masterauth",server.masterauth);
    config_get_string_field("cluster-announce-ip",server.cluster_announce_ip);
    config_get_string_field("unixsocket",server.unixsocket);
    config_get_string_field("logfile",server.logfile);
    config_get_string_field("pidfile",server.pidfile);
    config_get_string_field("slave-announce-ip",server.slave_announce_ip);
    config_get_numerical_field("maxmemory",server.maxmemory);
    config_get_numerical_field("maxmemory-samples",server.maxmemory_samples);
    config_get_numerical_field("timeout",server.maxidletime);
    config_get_numerical_field("active-defrag-threshold-lower",server.active_defrag_threshold_lower);
    config_get_numerical_field("active-defrag-threshold-upper",server.active_defrag_threshold_upper);
    config_get_numerical_field("active-defrag-ignore-bytes",server.active_defrag_ignore_bytes);
    config_get_numerical_field("active-defrag-cycle-min",server.active_defrag_cycle_min);
    config_get_numerical_field("active-defrag-cycle-max",server.active_defrag_cycle_max);
    config_get_numerical_field("auto-aof-rewrite-percentage",
            server.aof_rewrite_perc);
    config_get_numerical_field("auto-aof-rewrite-min-size",
            server.aof_rewrite_min_size);
    config_get_numerical_field("hash-max-ziplist-entries",
            server.hash_max_ziplist_entries);
    config_get_numerical_field("hash-max-ziplist-value",
            server.hash_max_ziplist_value);
    config_get_numerical_field("list-max-ziplist-size",
            server.list_max_ziplist_size);
    config_get_numerical_field("list-compress-depth",
            server.list_compress_depth);
    config_get_numerical_field("set-max-intset-entries",
            server.set_max_intset_entries);
    config_get_numerical_field("zset-max-ziplist-entries",
            server.zset_max_ziplist_entries);
    config_get_numerical_field("zset-max-ziplist-value",
            server.zset_max_ziplist_value);
    config_get_numerical_field("hll-sparse-max-bytes",
            server.hll_sparse_max_bytes);
    config_get_numerical_field("lua-time-limit",server.lua_time_limit);
    config_get_numerical_field("slowlog-log-slower-than",
            server.slowlog_log_slower_than);
    config_get_numerical_field("latency-monitor-threshold",
            server.latency_monitor_threshold);
    config_get_numerical_field("slowlog-max-len",
            server.slowlog_max_len);
    config_get_numerical_field("port",server.port);
    config_get_numerical_field("cluster-announce-port",server.cluster_announce_port);
    config_get_numerical_field("cluster-announce-bus-port",server.cluster_announce_bus_port);
    config_get_numerical_field("tcp-backlog",server.tcp_backlog);
    config_get_numerical_field("databases",server.dbnum);
    config_get_numerical_field("repl-ping-slave-period",server.repl_ping_slave_period);
    config_get_numerical_field("repl-timeout",server.repl_timeout);
    config_get_numerical_field("repl-backlog-size",server.repl_backlog_size);
    config_get_numerical_field("repl-backlog-ttl",server.repl_backlog_time_limit);
    config_get_numerical_field("maxclients",server.maxclients);
    config_get_numerical_field("watchdog-period",server.watchdog_period);
    config_get_numerical_field("slave-priority",server.slave_priority);
    config_get_numerical_field("slave-announce-port",server.slave_announce_port);
    config_get_numerical_field("min-slaves-to-write",server.repl_min_slaves_to_write);
    config_get_numerical_field("min-slaves-max-lag",server.repl_min_slaves_max_lag);
    config_get_numerical_field("hz",server.hz);
    config_get_numerical_field("cluster-node-timeout",server.cluster_node_timeout);
    config_get_numerical_field("cluster-migration-barrier",server.cluster_migration_barrier);
    config_get_numerical_field("cluster-slave-validity-factor",server.cluster_slave_validity_factor);
    config_get_numerical_field("repl-diskless-sync-delay",server.repl_diskless_sync_delay);
    config_get_numerical_field("tcp-keepalive",server.tcpkeepalive);
    config_get_bool_field("cluster-require-full-coverage",
            server.cluster_require_full_coverage);
    config_get_bool_field("no-appendfsync-on-rewrite",
            server.aof_no_fsync_on_rewrite);
    config_get_bool_field("slave-serve-stale-data",
            server.repl_serve_stale_data);
    config_get_bool_field("slave-read-only",
            server.repl_slave_ro);
    config_get_bool_field("stop-writes-on-bgsave-error",
            server.stop_writes_on_bgsave_err);
    config_get_bool_field("daemonize", server.daemonize);
    config_get_bool_field("rdbcompression", server.rdb_compression);
    config_get_bool_field("rdbchecksum", server.rdb_checksum);
    config_get_bool_field("activerehashing", server.activerehashing);
    config_get_bool_field("activedefrag", server.active_defrag_enabled);
    config_get_bool_field("protected-mode", server.protected_mode);
    config_get_bool_field("repl-disable-tcp-nodelay",
            server.repl_disable_tcp_nodelay);
    config_get_bool_field("repl-diskless-sync",
            server.repl_diskless_sync);
    config_get_bool_field("aof-rewrite-incremental-fsync",
            server.aof_rewrite_incremental_fsync);
    config_get_bool_field("aof-load-truncated",
            server.aof_load_truncated);
    config_get_bool_field("aof-use-rdb-preamble",
            server.aof_use_rdb_preamble);
    config_get_bool_field("lazyfree-lazy-eviction",
            server.lazyfree_lazy_eviction);
    config_get_bool_field("lazyfree-lazy-expire",
            server.lazyfree_lazy_expire);
    config_get_bool_field("lazyfree-lazy-server-del",
            server.lazyfree_lazy_server_del);
    config_get_bool_field("slave-lazy-flush",
            server.repl_slave_lazy_flush);
    config_get_enum_field("maxmemory-policy",
            server.maxmemory_policy,maxmemory_policy_enum);
    config_get_enum_field("loglevel",
            server.verbosity,loglevel_enum);
    config_get_enum_field("supervised",
            server.supervised_mode,supervised_mode_enum);
    config_get_enum_field("appendfsync",
            server.aof_fsync,aof_fsync_enum);
    config_get_enum_field("syslog-facility",
            server.syslog_facility,syslog_facility_enum);
    if (stringmatch(pattern,"appendonly",1)) {
        addReplyBulkCString(c,"appendonly");
        addReplyBulkCString(c,server.aof_state == AOF_OFF ? "no" : "yes");
        matches++;
    }
    if (stringmatch(pattern,"dir",1)) {
        char buf[1024];
        if (getcwd(buf,sizeof(buf)) == NULL)
            buf[0] = '\0';
        addReplyBulkCString(c,"dir");
        addReplyBulkCString(c,buf);
        matches++;
    }
    if (stringmatch(pattern,"save",1)) {
        sds buf = sdsempty();
        int j;
        for (j = 0; j < server.saveparamslen; j++) {
            buf = sdscatprintf(buf,"%jd %d",
                    (intmax_t)server.saveparams[j].seconds,
                    server.saveparams[j].changes);
            if (j != server.saveparamslen-1)
                buf = sdscatlen(buf," ",1);
        }
        addReplyBulkCString(c,"save");
        addReplyBulkCString(c,buf);
        sdsfree(buf);
        matches++;
    }
    if (stringmatch(pattern,"client-output-buffer-limit",1)) {
        sds buf = sdsempty();
        int j;
        for (j = 0; j < CLIENT_TYPE_OBUF_COUNT; j++) {
            buf = sdscatprintf(buf,"%s %llu %llu %ld",
                    getClientTypeName(j),
                    server.client_obuf_limits[j].hard_limit_bytes,
                    server.client_obuf_limits[j].soft_limit_bytes,
                    (long) server.client_obuf_limits[j].soft_limit_seconds);
            if (j != CLIENT_TYPE_OBUF_COUNT-1)
                buf = sdscatlen(buf," ",1);
        }
        addReplyBulkCString(c,"client-output-buffer-limit");
        addReplyBulkCString(c,buf);
        sdsfree(buf);
        matches++;
    }
    if (stringmatch(pattern,"unixsocketperm",1)) {
        char buf[32];
        snprintf(buf,sizeof(buf),"%o",server.unixsocketperm);
        addReplyBulkCString(c,"unixsocketperm");
        addReplyBulkCString(c,buf);
        matches++;
    }
    if (stringmatch(pattern,"slaveof",1)) {
        char buf[256];
        addReplyBulkCString(c,"slaveof");
        if (server.masterhost)
            snprintf(buf,sizeof(buf),"%s %d",
                server.masterhost, server.masterport);
        else
            buf[0] = '\0';
        addReplyBulkCString(c,buf);
        matches++;
    }
    if (stringmatch(pattern,"notify-keyspace-events",1)) {
        robj *flagsobj = createObject(OBJ_STRING,
            keyspaceEventsFlagsToString(server.notify_keyspace_events));
        addReplyBulkCString(c,"notify-keyspace-events");
        addReplyBulk(c,flagsobj);
        decrRefCount(flagsobj);
        matches++;
    }
    if (stringmatch(pattern,"bind",1)) {
        sds aux = sdsjoin(server.bindaddr,server.bindaddr_count," ");
        addReplyBulkCString(c,"bind");
        addReplyBulkCString(c,aux);
        sdsfree(aux);
        matches++;
    }
    setDeferredMultiBulkLength(c,replylen,matches*2);
}//框内数据的定义及计算
#define REDIS_CONFIG_REWRITE_SIGNATURE "# Generated by CONFIG REWRITE"
uint64_t dictSdsCaseHash(const void *key);
int dictSdsKeyCaseCompare(void *privdata, const void *key1, const void *key2);
void dictSdsDestructor(void *privdata, void *val);
void dictListDestructor(void *privdata, void *val);
void rewriteConfigSentinelOption(struct rewriteConfigState *state);
dictType optionToLineDictType = {
    dictSdsCaseHash,            
    NULL,                      
    NULL,                     
    dictSdsKeyCaseCompare,     
    dictSdsDestructor,         
    dictListDestructor         
};
dictType optionSetDictType = {
    dictSdsCaseHash,          
    NULL,                       
    NULL,                    
    dictSdsKeyCaseCompare,      
    dictSdsDestructor,          
    NULL                       
};
struct rewriteConfigState {
    dict *option_to_line; 
    dict *rewritten;     
    int numlines;        
    sds *lines;        
    int has_tail;        
};
void rewriteConfigAppendLine(struct rewriteConfigState *state, sds line) {
    state->lines = zrealloc(state->lines, sizeof(char*) * (state->numlines+1));
    state->lines[state->numlines++] = line;
}
void rewriteConfigAddLineNumberToOption(struct rewriteConfigState *state, sds option, int linenum) {
    list *l = dictFetchValue(state->option_to_line,option);
    listAddNodeTail(l,(void*)(long)linenum);
}
void rewriteConfigMarkAsProcessed(struct rewriteConfigState *state, const char *option) {
    sds opt = sdsnew(option);
    if (dictAdd(state->rewritten,opt,NULL) != DICT_OK) sdsfree(opt);
}
struct rewriteConfigState *rewriteConfigReadOldFile(char *path) {
    FILE *fp = fopen(path,"r");
    struct rewriteConfigState *state = zmalloc(sizeof(*state));
    if (fp == NULL) return state;
    while(fgets(buf,CONFIG_MAX_LINE+1,fp) != NULL) {
        int argc;
        sds *argv;
        sds line = sdstrim(sdsnew(buf),"\r\n\t ");
        linenum++; 
        if (line[0] == '#' || line[0] == '\0') {
            if (!state->has_tail && !strcmp(line,REDIS_CONFIG_REWRITE_SIGNATURE))
                state->has_tail = 1;
            rewriteConfigAppendLine(state,line);
            continue;
        }
        argv = sdssplitargs(line,&argc);
        if (argv == NULL) {
           
            sds aux = sdsnew("# ??? ");
            aux = sdscatsds(aux,line);
            sdsfree(line);
            rewriteConfigAppendLine(state,aux);
            continue;
        }
        sdstolower(argv[0]); 
        rewriteConfigAppendLine(state,line);
        rewriteConfigAddLineNumberToOption(state,argv[0],linenum);
        sdsfreesplitres(argv,argc);
    }
    fclose(fp);
    return state;
}
void rewriteConfigRewriteLine(struct rewriteConfigState *state, const char *option, sds line, int force) {
    sds o = sdsnew(option);
    list *l = dictFetchValue(state->option_to_line,o);
    rewriteConfigMarkAsProcessed(state,option);
    if (!l && !force) {
        
        sdsfree(line);
        sdsfree(o);
        return;
    }
    if (l) {
        listNode *ln = listFirst(l);
        int linenum = (long) ln->value;
        listDelNode(l,ln);
        if (listLength(l) == 0) dictDelete(state->option_to_line,o);
        sdsfree(state->lines[linenum]);
        state->lines[linenum] = line;
    } else {
    
        if (!state->has_tail) {
            rewriteConfigAppendLine(state,
                sdsnew(REDIS_CONFIG_REWRITE_SIGNATURE));
            state->has_tail = 1;
        }
        rewriteConfigAppendLine(state,line);
    }
    sdsfree(o);
}
int rewriteConfigFormatMemory(char *buf, size_t len, long long bytes) {
    int gb = 1024*1024*1024;
    int mb = 1024*1024;
    int kb = 1024;
    if (bytes && (bytes % gb) == 0) {
        return snprintf(buf,len,"%lldgb",bytes/gb);
    } else if (bytes && (bytes % mb) == 0) {
        return snprintf(buf,len,"%lldmb",bytes/mb);
    } else if (bytes && (bytes % kb) == 0) {
        return snprintf(buf,len,"%lldkb",bytes/kb);
    } else {
        return snprintf(buf,len,"%lld",bytes);
    }
}//在框内输入数据，进行运算

