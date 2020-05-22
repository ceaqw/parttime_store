#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>//头文件定义功能
int Socket(const char *host, int clientPort)
{
    int sock;
    unsigned long inaddr;
    struct sockaddr_in ad;
    struct hostent *hp;
    
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    inaddr = inet_addr(host);
    if (inaddr != INADDR_NONE)
        memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
    else
    {
        hp = gethostbyname(host);
        if (hp == NULL)
            return -1;
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
    }
    ad.sin_port = htons(clientPort);
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return sock;
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)
        return -1;
    return sock;
}

#include "socket.c"
#include <unistd.h>
#include <sys/param.h>
#include <rpc/types.h>
#include <getopt.h>
#include <strings.h>
#include <time.h>
#include <signal.h>
volatile int timerexpired=0;
int speed=0;
int failed=0;
int bytes=0;
int http10=1; 
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define PROGRAM_VERSION "1.5"
int method=METHOD_GET;
int clients=1;
int force=0;
int force_reload=0;
int proxyport=80;
char *proxyhost=NULL;
int benchtime=30;
int mypipe[2];
char host[MAXHOSTNAMELEN];
#define REQUEST_SIZE 2048
char request[REQUEST_SIZE];
static const struct option long_options[]=
{
    {"force",no_argument,&force,1},
    {"reload",no_argument,&force_reload,1},
    {"time",required_argument,NULL,'t'},
    {"help",no_argument,NULL,'?'},
    {"http09",no_argument,NULL,'9'},
    {"http10",no_argument,NULL,'1'},
    {"http11",no_argument,NULL,'2'},
    {"get",no_argument,&method,METHOD_GET},
    {"head",no_argument,&method,METHOD_HEAD},
    {"options",no_argument,&method,METHOD_OPTIONS},
    {"trace",no_argument,&method,METHOD_TRACE},
    {"version",no_argument,NULL,'V'},
    {"proxy",required_argument,NULL,'p'},
    {"clients",required_argument,NULL,'c'},
    {NULL,0,NULL,0}
};//装置设置参数
static void benchcore(const char* host,const int port, const char *request);
static int bench(void);
static void build_request(const char *url);
static void alarm_handler(int signal)
{
    timerexpired=1;
}	
static void usage(void)
{
    fprintf(stderr,			
            "webbench [option]... URL\n"
            "  -f|--force               Don't wait for reply from server.\n"
            "  -r|--reload              Send reload request - Pragma: no-cache.\n"
            "  -t|--time <sec>          Run benchmark for <sec> seconds. Default 30.\n"
            "  -p|--proxy <server:port> Use proxy server for request.\n"
            "  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
            "  -9|--http09              Use HTTP/0.9 style requests.\n"
            "  -1|--http10              Use HTTP/1.0 protocol.\n"
            "  -2|--http11              Use HTTP/1.1 protocol.\n"
            "  --get                    Use GET request method.\n"
            "  --head                   Use HEAD request method.\n"
            "  --options                Use OPTIONS request method.\n"
            "  --trace                  Use TRACE request method.\n"
            "  -?|-h|--help             This information.\n"
            "  -V|--version             Display program version.\n"
           );
}//时间功能设置
int main(int argc, char *argv[])
{
    int opt=0;
    int options_index=0;
    char *tmp=NULL;
    if(argc==1)
    {
        usage();
        return 2;
    } 
    while((opt=getopt_long(argc,argv,"912Vfrt:p:c:?h",long_options,&options_index))!=EOF )
    {
        switch(opt)
        {
            case  0 : break;
            case 'f': force=1;break;
            case 'r': force_reload=1;break; 
            case '9': http10=0;break;
            case '1': http10=1;break;
            case '2': http10=2;break;
            case 'V': printf(PROGRAM_VERSION"\n");exit(0);
            case 't': benchtime=atoi(optarg);break;	     
            case 'p': 
           
            tmp=strrchr(optarg,':');
            proxyhost=optarg;
            if(tmp==NULL)
            {
                break;
            }
            if(tmp==optarg)
            {
                fprintf(stderr,"Error in option --proxy %s: Missing hostname.\n",optarg);
                return 2;
            }
            if(tmp==optarg+strlen(optarg)-1)
            {
                fprintf(stderr,"Error in option --proxy %s Port number is missing.\n",optarg);
                return 2;
            }
            *tmp='\0';
            proxyport=atoi(tmp+1);break;
            case ':':
            case 'h':
            case '?': usage();return 2;break;
            case 'c': clients=atoi(optarg);break;
        }
    }
    if(optind==argc) {
        fprintf(stderr,"webbench: Missing URL!\n");
        usage();
        return 2;
    }
    if(clients==0) clients=1;
    if(benchtime==0) benchtime=30;
 
   
    fprintf(stderr,"Webbench - Simple Web Benchmark "PROGRAM_VERSION"\n"
            " (c) Radim Kolar 1997-2004, GPL Open Source Software.\n"
            );
 
    build_request(argv[optind]);
 
    // print request info ,do it in function build_request
    printf("Benchmarking: ");设定输出的标准
 
    switch(method)
    {
        case METHOD_GET:
        default:
        printf("GET");break;
        case METHOD_OPTIONS:
        printf("OPTIONS");break;
        case METHOD_HEAD:
        printf("HEAD");break;
        case METHOD_TRACE:
        printf("TRACE");break;
    }
    
    printf(" %s",argv[optind]);
    
    switch(http10)
    {
        case 0: printf(" (using HTTP/0.9)");break;
        case 2: printf(" (using HTTP/1.1)");break;
    }
 
    printf("\n");
   
    printf("Runing info: ");
    if(clients==1) 
        printf("1 client");
    else
        printf("%d clients",clients);
    printf(", running %d sec", benchtime);
    
    if(force) printf(", early socket close");
    if(proxyhost!=NULL) printf(", via proxy server %s:%d",proxyhost,proxyport);
    if(force_reload) printf(", forcing reload");
    
    printf(".\n");
    
    return bench();
}//开启，关闭功能的定义
void build_request(const char *url)
{
    char tmp[10];
    int i;
    //bzero(host,MAXHOSTNAMELEN);
    //bzero(request,REQUEST_SIZE);
    memset(host,0,MAXHOSTNAMELEN);
    memset(request,0,REQUEST_SIZE);
    if(force_reload && proxyhost!=NULL && http10<1) http10=1;
    if(method==METHOD_HEAD && http10<1) http10=1;
    if(method==METHOD_OPTIONS && http10<2) http10=2;
    if(method==METHOD_TRACE && http10<2) http10=2;
    switch(method)
    {
        default:
        case METHOD_GET: strcpy(request,"GET");break;
        case METHOD_HEAD: strcpy(request,"HEAD");break;
        case METHOD_OPTIONS: strcpy(request,"OPTIONS");break;
        case METHOD_TRACE: strcpy(request,"TRACE");break;
    }
    strcat(request," ");
    if(NULL==strstr(url,"://"))
    {
        fprintf(stderr, "\n%s: is not a valid URL.\n",url);
        exit(2);
    }
    if(strlen(url)>1500)
    {
        fprintf(stderr,"URL is too long.\n");
        exit(2);
    }
    if (0!=strncasecmp("http://",url,7)) 
    { 
        fprintf(stderr,"\nOnly HTTP protocol is directly supported, set --proxy for others.\n");
        exit(2);
    }
    
   
    i=strstr(url,"://")-url+3;
    if(strchr(url+i,'/')==NULL) {
        fprintf(stderr,"\nInvalid URL syntax - hostname don't ends with '/'.\n");
        exit(2);
    }
    
    if(proxyhost==NULL)
    {
        
        if(index(url+i,':')!=NULL && index(url+i,':')<index(url+i,'/'))
        {
            strncpy(host,url+i,strchr(url+i,':')-url-i);
            //bzero(tmp,10);
            memset(tmp,0,10);
            strncpy(tmp,index(url+i,':')+1,strchr(url+i,'/')-index(url+i,':')-1);
           
            proxyport=atoi(tmp);
            if(proxyport==0) proxyport=80;
        } 
        else
        {
            strncpy(host,url+i,strcspn(url+i,"/"));
        }
        // printf("Host=%s\n",host);
        strcat(request+strlen(request),url+i+strcspn(url+i,"/"));
    } 
    else
    {
        // printf("ProxyHost=%s\nProxyPort=%d\n",proxyhost,proxyport);
        strcat(request,url);
    }
    if(http10==1)
        strcat(request," HTTP/1.0");
    else if (http10==2)
        strcat(request," HTTP/1.1");
  
    strcat(request,"\r\n");
  
    if(http10>0)
        strcat(request,"User-Agent: WebBench "PROGRAM_VERSION"\r\n");
    if(proxyhost==NULL && http10>0)
    {
        strcat(request,"Host: ");
        strcat(request,host);
        strcat(request,"\r\n");
    }
 
    if(force_reload && proxyhost!=NULL)
    {
        strcat(request,"Pragma: no-cache\r\n");
    }
  
    if(http10>1)
        strcat(request,"Connection: close\r\n");
    
 
    if(http10>0) strcat(request,"\r\n"); 
    
    printf("\nRequest:\n%s\n",request);
}//设置User-Agent: WebBench 用户界面功能
static int bench(void)
{
    int i,j,k;	
    pid_t pid=0;
    FILE *f;
  
    i=Socket(proxyhost==NULL?host:proxyhost,proxyport);
    if(i<0) { 
        fprintf(stderr,"\nConnect to server failed. Aborting benchmark.\n");
        return 1;
    }
    close(i);
   
    if(pipe(mypipe))
    {
        perror("pipe failed.");
        return 3;
    }
    cas=time(NULL);
    while(time(NULL)==cas)
    sched_yield();
 
    for(i=0;i<clients;i++)
    {
        pid=fork();
        if(pid <= (pid_t) 0)
        {
           
            sleep(1); 
        }
    }
    if( pid < (pid_t) 0)
    {
        fprintf(stderr,"problems forking worker no. %d\n",i);
        perror("fork failed.");
        return 3;
    }
    if(pid == (pid_t) 0)
    {
        if(proxyhost==NULL)
            benchcore(host,proxyport,request);
        else
            benchcore(proxyhost,proxyport,request);
        f=fdopen(mypipe[1],"w");
        if(f==NULL)
        {
            perror("open pipe for writing failed.");
            return 3;
        }
        fprintf(f,"%d %d %d\n",speed,failed,bytes);
        fclose(f);
        return 0;
    } 
    else
    {
        f=fdopen(mypipe[0],"r");
        if(f==NULL) 
        {
            perror("open pipe for reading failed.");
            return 3;
        }
        
        setvbuf(f,NULL,_IONBF,0);
        
        speed=0;
        failed=0;
        bytes=0;
    
        while(1)
        {
            pid=fscanf(f,"%d %d %d",&i,&j,&k);
            if(pid<2)
            {
                fprintf(stderr,"Some of our childrens died.\n");
                break;
            }
            
            speed+=i;
            failed+=j;
            bytes+=k;
        
             fprintf(stderr,"*Knock* %d %d read=%d\n",speed,failed,pid); 
            if(--clients==0) break;
        }
    
        fclose(f);
        printf("\nSpeed=%d pages/min, %d bytes/sec.\nRequests: %d susceed, %d failed.\n",
            (int)((speed+failed)/(benchtime/60.0f)),
            (int)(bytes/(float)benchtime),
            speed,
            failed);
    }
    
    return i;
}//设置进入功能
static unsigned parse_hex4(const unsigned char * const input)
{
    unsigned int h = 0;
    size_t i = 0;
    for (i = 0; i < 4; i++)
    {
        if ((input[i] >= '0') && (input[i] <= '9'))
        {
            h += (unsigned int) input[i] - '0';
        }
        else if ((input[i] >= 'A') && (input[i] <= 'F'))
        {
            h += (unsigned int) 10 + input[i] - 'A';
        }
        else if ((input[i] >= 'a') && (input[i] <= 'f'))
        {
            h += (unsigned int) 10 + input[i] - 'a';
        }
        else
        {
            return 0;
        }
        if (i < 3)
        {
            h = h << 4;
        }
    }
    return h;
}
static unsigned char utf16_literal_to_utf8(const unsigned char * const input_pointer, const unsigned char * const input_end, unsigned char **output_pointer)
{
    long unsigned int codepoint = 0;
    unsigned int first_code = 0;
    const unsigned char *first_sequence = input_pointer;
    unsigned char utf8_length = 0;
    unsigned char utf8_position = 0;
    unsigned char sequence_length = 0;
    unsigned char first_byte_mark = 0;
    if ((input_end - first_sequence) < 6)
    {
        goto fail;
    }
    first_code = parse_hex4(first_sequence + 2);
    if (((first_code >= 0xDC00) && (first_code <= 0xDFFF)))
    {
        goto fail;
    }
    if ((first_code >= 0xD800) && (first_code <= 0xDBFF))
    {
        const unsigned char *second_sequence = first_sequence + 6;
        unsigned int second_code = 0;
        sequence_length = 12; 
        if ((input_end - second_sequence) < 6)
        {
            goto fail;
        }
        if ((second_sequence[0] != '\\') || (second_sequence[1] != 'u'))
        {
           
            goto fail;
        }
      
        second_code = parse_hex4(second_sequence + 2);
     
        if ((second_code < 0xDC00) || (second_code > 0xDFFF))
        {
            goto fail;
        }
        codepoint = 0x10000 + (((first_code & 0x3FF) << 10) | (second_code & 0x3FF));
    }
    else
    {
        sequence_length = 6; 
        codepoint = first_code;
    }
    if (codepoint < 0x80)
    {
        utf8_length = 1;
    }
    else if (codepoint < 0x800)
    {
        utf8_length = 2;
        first_byte_mark = 0xC0; 
    }
    else if (codepoint < 0x10000)
    {
        utf8_length = 3;
        first_byte_mark = 0xE0; 
    }
    else if (codepoint <= 0x10FFFF)
    {
        utf8_length = 4;
        first_byte_mark = 0xF0; 
    }
    else
    {
        goto fail;
    }
    for (utf8_position = (unsigned char)(utf8_length - 1); utf8_position > 0; utf8_position--)
    {
        (*output_pointer)[utf8_position] = (unsigned char)((codepoint | 0x80) & 0xBF);
        codepoint >>= 6;
    }
    if (utf8_length > 1)
    {
        (*output_pointer)[0] = (unsigned char)((codepoint | first_byte_mark) & 0xFF);
    }
    else
    {
        (*output_pointer)[0] = (unsigned char)(codepoint & 0x7F);
    }
    *output_pointer += utf8_length;
    return sequence_length;
fail:
    return 0;
}//自动计时功能
static cJSON_bool parse_string(cJSON * const item, parse_buffer * const input_buffer)
{
    const unsigned char *input_pointer = buffer_at_offset(input_buffer) + 1;
    const unsigned char *input_end = buffer_at_offset(input_buffer) + 1;
    unsigned char *output_pointer = NULL;
    unsigned char *output = NULL;
   
    if (buffer_at_offset(input_buffer)[0] != '\"')
    {
        goto fail;
    }
    {
        size_t allocation_length = 0;
        size_t skipped_bytes = 0;
        while (((size_t)(input_end - input_buffer->content) < input_buffer->length) && (*input_end != '\"'))
        {
            if (input_end[0] == '\\')
            {
                if ((size_t)(input_end + 1 - input_buffer->content) >= input_buffer->length)
                {
             
                    goto fail;
                }
                skipped_bytes++;
                input_end++;
            }
            input_end++;
        }
        if (((size_t)(input_end - input_buffer->content) >= input_buffer->length) || (*input_end != '\"'))
        {
            goto fail; 
        }
        allocation_length = (size_t) (input_end - buffer_at_offset(input_buffer)) - skipped_bytes;
        output = (unsigned char*)input_buffer->hooks.allocate(allocation_length + sizeof(""));
        if (output == NULL)
        {
            goto fail; 
        }
    }
    output_pointer = output;
   
    while (input_pointer < input_end)
    {
        if (*input_pointer != '\\')
        {
            *output_pointer++ = *input_pointer++;
        }
        else
        {
            unsigned char sequence_length = 2;
            if ((input_end - input_pointer) < 1)
            {
                goto fail;
            }
            switch (input_pointer[1])
            {
                case 'b':
                    *output_pointer++ = '\b';
                    break;
                case 'f':
                    *output_pointer++ = '\f';
                    break;
                case 'n':
                    *output_pointer++ = '\n';
                    break;
                case 'r':
                    *output_pointer++ = '\r';
                    break;
                case 't':
                    *output_pointer++ = '\t';
                    break;
                case '\"':
                case '\\':
                case '/':
                    *output_pointer++ = input_pointer[1];
                    break;
                case 'u':
                    sequence_length = utf16_literal_to_utf8(input_pointer, input_end, &output_pointer);
                    if (sequence_length == 0)
                    {
                        goto fail;
                    }
                    break;
                default:
                    goto fail;
            }
            input_pointer += sequence_length;
        }
    }//界面框输入数据功能
    *output_pointer = '\0';
    item->type = cJSON_String;
    item->valuestring = (char*)output;
    input_buffer->offset = (size_t) (input_end - input_buffer->content);
    input_buffer->offset++;
    return true;
fail:
    if (output != NULL)
    {
        input_buffer->hooks.deallocate(output);
    }
    if (input_pointer != NULL)
    {
        input_buffer->offset = (size_t)(input_pointer - input_buffer->content);
    }
    return false;
}
static cJSON_bool print_string_ptr(const unsigned char * const input, printbuffer * const output_buffer)
{
    const unsigned char *input_pointer = NULL;
    unsigned char *output = NULL;
    unsigned char *output_pointer = NULL;
    size_t output_length = 0;
  
    size_t escape_characters = 0;
    if (output_buffer == NULL)
    {
        return false;
    }
    if (input == NULL)
    {
        output = ensure(output_buffer, sizeof("\"\""));
        if (output == NULL)
        {
            return false;
        }
        strcpy((char*)output, "\"\"");
        return true;
    }
    for (input_pointer = input; *input_pointer; input_pointer++)
    {
        switch (*input_pointer)
        {
            case '\"':
            case '\\':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
                
                escape_characters++;
                break;
            default:
                if (*input_pointer < 32)
                {
                    
                    escape_characters += 5;
                }
                break;
        }
    }//界面框输出数据功能
    output_length = (size_t)(input_pointer - input) + escape_characters;
    output = ensure(output_buffer, output_length + sizeof("\"\""));
    if (output == NULL)
    {
        return false;
    }
    if (escape_characters == 0)
    {
        output[0] = '\"';
        memcpy(output + 1, input, output_length);
        output[output_length + 1] = '\"';
        output[output_length + 2] = '\0';
        return true;
    }
    output[0] = '\"';
    output_pointer = output + 1;
   
    for (input_pointer = input; *input_pointer != '\0'; (void)input_pointer++, output_pointer++)
    {
        if ((*input_pointer > 31) && (*input_pointer != '\"') && (*input_pointer != '\\'))
        {
            *output_pointer = *input_pointer;
        }
        else
        {
            *output_pointer++ = '\\';
            switch (*input_pointer)
            {
                case '\\':
                    *output_pointer = '\\';
                    break;
                case '\"':
                    *output_pointer = '\"';
                    break;
                case '\b':
                    *output_pointer = 'b';
                    break;
                case '\f':
                    *output_pointer = 'f';
                    break;
                case '\n':
                    *output_pointer = 'n';
                    break;
                case '\r':
                    *output_pointer = 'r';
                    break;
                case '\t':
                    *output_pointer = 't';
                    break;
                default:
                  
                    sprintf((char*)output_pointer, "u%04x", *input_pointer);
                    output_pointer += 4;
                    break;
            }
        }
    }//界面框输出数据
    output[output_length + 1] = '\"';
    output[output_length + 2] = '\0';
    return true;
}
static cJSON_bool print_string(const cJSON * const item, printbuffer * const p)
{
    return print_string_ptr((unsigned char*)item->valuestring, p);
}
static cJSON_bool parse_value(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_value(const cJSON * const item, printbuffer * const output_buffer);
static cJSON_bool parse_array(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_array(const cJSON * const item, printbuffer * const output_buffer);
static cJSON_bool parse_object(cJSON * const item, parse_buffer * const input_buffer);
static cJSON_bool print_object(const cJSON * const item, printbuffer * const output_buffer);
static parse_buffer *buffer_skip_whitespace(parse_buffer * const buffer)
{
    if ((buffer == NULL) || (buffer->content == NULL))
    {
        return NULL;
    }
    while (can_access_at_index(buffer, 0) && (buffer_at_offset(buffer)[0] <= 32))
    {
       buffer->offset++;
    }
    if (buffer->offset == buffer->length)
    {
        buffer->offset--;
    }
    return buffer;
}//界面中输入某个值，计算分析结果
CJSON_PUBLIC(cJSON *) cJSON_ParseWithOpts(const char *value, const char **return_parse_end, cJSON_bool require_null_terminated)
{
    parse_buffer buffer = { 0, 0, 0, 0, { 0, 0, 0 } };
    cJSON *item = NULL;
    global_error.json = NULL;
    global_error.position = 0;
    if (value == NULL)
    {
        goto fail;
    }
    buffer.content = (const unsigned char*)value;
    buffer.length = strlen((const char*)value) + sizeof("");
    buffer.offset = 0;
    buffer.hooks = global_hooks;
    item = cJSON_New_Item(&global_hooks);
    if (item == NULL) 
    {
        goto fail;
    }
    if (!parse_value(item, buffer_skip_whitespace(&buffer)))
    {
        goto fail;
    }
    if (require_null_terminated)
    {
        buffer_skip_whitespace(&buffer);
        if ((buffer.offset >= buffer.length) || buffer_at_offset(&buffer)[0] != '\0')
        {
            goto fail;
        }
    }
    if (return_parse_end)
    {
        *return_parse_end = (const char*)buffer_at_offset(&buffer);
    }
    return item;
fail:
    if (item != NULL)
    {
        cJSON_Delete(item);
    }
    if (value != NULL)
    {
        error local_error;
        local_error.json = (const unsigned char*)value;
        local_error.position = 0;
        if (buffer.offset < buffer.length)
        {
            local_error.position = buffer.offset;
        }
        else if (buffer.length > 0)
        {
            local_error.position = buffer.length - 1;
        }
        if (return_parse_end != NULL)
        {
            *return_parse_end = (const char*)local_error.json + local_error.position;
        }
        else
        {
            global_error = local_error;
        }
    }
    return NULL;
}
CJSON_PUBLIC(cJSON *) cJSON_Parse(const char *value)
{
    return cJSON_ParseWithOpts(value, 0, 0);
}
#define cjson_min(a, b) ((a < b) ? a : b)
static unsigned char *print(const cJSON * const item, cJSON_bool format, const internal_hooks * const hooks)
{
    printbuffer buffer[1];
    unsigned char *printed = NULL;
    memset(buffer, 0, sizeof(buffer));
 
    buffer->buffer = (unsigned char*) hooks->allocate(256);
    buffer->format = format;
    buffer->hooks = *hooks;
    if (buffer->buffer == NULL)
    {
        goto fail;
    }
    if (!print_value(item, buffer))
    {
        goto fail;
    }
    update_offset(buffer);
   
    if (hooks->reallocate != NULL)
    {
        printed = (unsigned char*) hooks->reallocate(buffer->buffer, buffer->length);
        buffer->buffer = NULL;
        if (printed == NULL) {
            goto fail;
        }
    }
    else 
    {
        printed = (unsigned char*) hooks->allocate(buffer->offset + 1);
        if (printed == NULL)
        {
            goto fail;
        }
        memcpy(printed, buffer->buffer, cjson_min(buffer->length, buffer->offset + 1));
        printed[buffer->offset] = '\0'; 
   
        hooks->deallocate(buffer->buffer);
    }//界面框内输入数据，进行运算 
    return printed;
fail:
    if (buffer->buffer != NULL)
    {
        hooks->deallocate(buffer->buffer);
    }
    if (printed != NULL)
    {
        hooks->deallocate(printed);
    }
    return NULL;
}
CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item)
{
    return (char*)print(item, true, &global_hooks);
}
CJSON_PUBLIC(char *) cJSON_PrintUnformatted(const cJSON *item)
{
    return (char*)print(item, false, &global_hooks);
}
CJSON_PUBLIC(char *) cJSON_PrintBuffered(const cJSON *item, int prebuffer, cJSON_bool fmt)
{
    printbuffer p = { 0, 0, 0, 0, 0, 0, { 0, 0, 0 } };
    if (prebuffer < 0)
    {
        return NULL;
    }
    p.buffer = (unsigned char*)global_hooks.allocate((size_t)prebuffer);
    if (!p.buffer)
    {
        return NULL;
    }
    p.length = (size_t)prebuffer;
    p.offset = 0;
    p.noalloc = false;
    p.format = fmt;
    p.hooks = global_hooks;
    if (!print_value(item, &p))
    {
        global_hooks.deallocate(p.buffer);
        return NULL;
    }
    return (char*)p.buffer;
}//子界面菜单
CJSON_PUBLIC(cJSON_bool) cJSON_PrintPreallocated(cJSON *item, char *buf, const int len, const cJSON_bool fmt)
{
    printbuffer p = { 0, 0, 0, 0, 0, 0, { 0, 0, 0 } };
    if ((len < 0) || (buf == NULL))
    {
        return false;
    }
    p.buffer = (unsigned char*)buf;
    p.length = (size_t)len;
    p.offset = 0;
    p.noalloc = true;
    p.format = fmt;
    p.hooks = global_hooks;
    return print_value(item, &p);
}
static cJSON_bool parse_value(cJSON * const item, parse_buffer * const input_buffer)
{
    if ((input_buffer == NULL) || (input_buffer->content == NULL))
    {
        return false; 
    }
    if (can_read(input_buffer, 4) && (strncmp((const char*)buffer_at_offset(input_buffer), "null", 4) == 0))
    {
        item->type = cJSON_NULL;
        input_buffer->offset += 4;
        return true;
    }
    if (can_read(input_buffer, 5) && (strncmp((const char*)buffer_at_offset(input_buffer), "false", 5) == 0))
    {
        item->type = cJSON_False;
        input_buffer->offset += 5;
        return true;
    }
    if (can_read(input_buffer, 4) && (strncmp((const char*)buffer_at_offset(input_buffer), "true", 4) == 0))
    {
        item->type = cJSON_True;
        item->valueint = 1;
        input_buffer->offset += 4;
        return true;
    }
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '\"'))
    {
        return parse_string(item, input_buffer);
    }
    if (can_access_at_index(input_buffer, 0) && ((buffer_at_offset(input_buffer)[0] == '-') || ((buffer_at_offset(input_buffer)[0] >= '0') && (buffer_at_offset(input_buffer)[0] <= '9'))))
    {
        return parse_number(item, input_buffer);
    }
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '['))
    {
        return parse_array(item, input_buffer);
    }
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '{'))
    {
        return parse_object(item, input_buffer);
    }

    return false;
}//子界面菜单
static cJSON_bool print_value(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output = NULL;
    if ((item == NULL) || (output_buffer == NULL))
    {
        return false;
    }
    switch ((item->type) & 0xFF)
    {
        case cJSON_NULL:
            output = ensure(output_buffer, 5);
            if (output == NULL)
            {
                return false;
            }
            strcpy((char*)output, "null");
            return true;
        case cJSON_False:
            output = ensure(output_buffer, 6);
            if (output == NULL)
            {
                return false;
            }
            strcpy((char*)output, "false");
            return true;
        case cJSON_True:
            output = ensure(output_buffer, 5);
            if (output == NULL)
            {
                return false;
            }
            strcpy((char*)output, "true");
            return true;
        case cJSON_Number:
            return print_number(item, output_buffer);
        case cJSON_Raw:
        {
            size_t raw_length = 0;
            if (item->valuestring == NULL)
            {
                if (!output_buffer->noalloc)
                {
                    output_buffer->hooks.deallocate(output_buffer->buffer);
                }
                return false;
            }
            raw_length = strlen(item->valuestring) + sizeof("");
            output = ensure(output_buffer, raw_length);
            if (output == NULL)
            {
                return false;
            }
            memcpy(output, item->valuestring, raw_length);
            return true;
        }
        case cJSON_String:
            return print_string(item, output_buffer);
        case cJSON_Array:
            return print_array(item, output_buffer);
        case cJSON_Object:
            return print_object(item, output_buffer);
        default:
            return false;
    }
}//采集传感器测试数据，统计功能
static cJSON_bool parse_array(cJSON * const item, parse_buffer * const input_buffer)
{
    cJSON *head = NULL; 
    cJSON *current_item = NULL;
    if (input_buffer->depth >= CJSON_NESTING_LIMIT)
    {
        return false; 
    }
    input_buffer->depth++;
    if (buffer_at_offset(input_buffer)[0] != '[')
    {
        goto fail;
    }
    input_buffer->offset++;
    buffer_skip_whitespace(input_buffer);
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ']'))
    {
        goto success;
    }
    if (cannot_access_at_index(input_buffer, 0))
    {
        input_buffer->offset--;
        goto fail;
    }
    input_buffer->offset--;
   
    do
    {
        cJSON *new_item = cJSON_New_Item(&(input_buffer->hooks));
        if (new_item == NULL)
        {
            goto fail; 
        }
        if (head == NULL)
        {
            current_item = head = new_item;
        }
        else
        {
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        if (!parse_value(current_item, input_buffer))
        {
            goto fail; 
        }
        buffer_skip_whitespace(input_buffer);
    }
    while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));
    if (cannot_access_at_index(input_buffer, 0) || buffer_at_offset(input_buffer)[0] != ']')
    {
        goto fail; 
    }
success:
    input_buffer->depth--;
    item->type = cJSON_Array;
    item->child = head;
    input_buffer->offset++;
    return true;
fail:
    if (head != NULL)
    {
        cJSON_Delete(head);
    }
    return false;
}
static cJSON_bool print_array(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    size_t length = 0;
    cJSON *current_element = item->child;
    if (output_buffer == NULL)
    {
        return false;
    }
    output_pointer = ensure(output_buffer, 1);
    if (output_pointer == NULL)
    {
        return false;
    }
    *output_pointer = '[';
    output_buffer->offset++;
    output_buffer->depth++;
    while (current_element != NULL)
    {
        if (!print_value(current_element, output_buffer))
        {
            return false;
        }
        update_offset(output_buffer);
        if (current_element->next)
        {
            length = (size_t) (output_buffer->format ? 2 : 1);
            output_pointer = ensure(output_buffer, length + 1);
            if (output_pointer == NULL)
            {
                return false;
            }
            *output_pointer++ = ',';
            if(output_buffer->format)
            {
                *output_pointer++ = ' ';
            }
            *output_pointer = '\0';
            output_buffer->offset += length;
        }
        current_element = current_element->next;
    }
    output_pointer = ensure(output_buffer, 2);
    if (output_pointer == NULL)
    {
        return false;
    }
    *output_pointer++ = ']';
    *output_pointer = '\0';
    output_buffer->depth--;
    return true;
}
static cJSON_bool parse_object(cJSON * const item, parse_buffer * const input_buffer)
{
    cJSON *head = NULL; 
    cJSON *current_item = NULL;
    if (input_buffer->depth >= CJSON_NESTING_LIMIT)
    {
        return false;
    }
    input_buffer->depth++;
    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '{'))
    {
        goto fail; 
    }
    input_buffer->offset++;
    buffer_skip_whitespace(input_buffer);
    if (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == '}'))
    {
        goto success; 
    }
    if (cannot_access_at_index(input_buffer, 0))
    {
        input_buffer->offset--;
        goto fail;
    }
    input_buffer->offset--;
   
    do
    {
        cJSON *new_item = cJSON_New_Item(&(input_buffer->hooks));
        if (new_item == NULL)
        {
            goto fail; 
        }
        if (head == NULL)
        {
            current_item = head = new_item;
        }
        else
        {
            current_item->next = new_item;
            new_item->prev = current_item;
            current_item = new_item;
        }
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        if (!parse_string(current_item, input_buffer))
        {
            goto fail; 
        }
        buffer_skip_whitespace(input_buffer);
        
        current_item->string = current_item->valuestring;
        current_item->valuestring = NULL;
        if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != ':'))
        {
            goto fail; 
        }
        input_buffer->offset++;
        buffer_skip_whitespace(input_buffer);
        if (!parse_value(current_item, input_buffer))
        {
            goto fail; 
        }
        buffer_skip_whitespace(input_buffer);
    }
    while (can_access_at_index(input_buffer, 0) && (buffer_at_offset(input_buffer)[0] == ','));
    if (cannot_access_at_index(input_buffer, 0) || (buffer_at_offset(input_buffer)[0] != '}'))
    {
        goto fail; 
    }//数据运算，调用算法进行数据处理
success:
    input_buffer->depth--;
    item->type = cJSON_Object;
    item->child = head;
    input_buffer->offset++;
    return true;
fail:
    if (head != NULL)
    {
        cJSON_Delete(head);
    }
    return false;
}
static cJSON_bool print_object(const cJSON * const item, printbuffer * const output_buffer)
{
    unsigned char *output_pointer = NULL;
    size_t length = 0;
    cJSON *current_item = item->child;
    if (output_buffer == NULL)
    {
        return false;
    }
    
    length = (size_t) (output_buffer->format ? 2 : 1); 
    output_pointer = ensure(output_buffer, length + 1);
    if (output_pointer == NULL)
    {
        return false;
    }
    *output_pointer++ = '{';
    output_buffer->depth++;
    if (output_buffer->format)
    {
        *output_pointer++ = '\n';
    }
    output_buffer->offset += length;
    while (current_item)
    {
        if (output_buffer->format)
        {
            size_t i;
            output_pointer = ensure(output_buffer, output_buffer->depth);
            if (output_pointer == NULL)
            {
                return false;
            }
            for (i = 0; i < output_buffer->depth; i++)
            {
                *output_pointer++ = '\t';
            }
            output_buffer->offset += output_buffer->depth;
        }
        if (!print_string_ptr((unsigned char*)current_item->string, output_buffer))
        {
            return false;
        }
        update_offset(output_buffer);
        length = (size_t) (output_buffer->format ? 2 : 1);
        output_pointer = ensure(output_buffer, length);
        if (output_pointer == NULL)
        {
            return false;
        }
        *output_pointer++ = ':';
        if (output_buffer->format)
        {
            *output_pointer++ = '\t';
        }
        output_buffer->offset += length;
      
        if (!print_value(current_item, output_buffer))
        {
            return false;
        }
        update_offset(output_buffer);
       
        length = (size_t) ((output_buffer->format ? 1 : 0) + (current_item->next ? 1 : 0));
        output_pointer = ensure(output_buffer, length + 1);
        if (output_pointer == NULL)
        {
            return false;
        }
        if (current_item->next)
        {
            *output_pointer++ = ',';
        }
        if (output_buffer->format)
        {
            *output_pointer++ = '\n';
        }
        *output_pointer = '\0';
        output_buffer->offset += length;
        current_item = current_item->next;
    }
    output_pointer = ensure(output_buffer, output_buffer->format ? (output_buffer->depth + 1) : 2);
    if (output_pointer == NULL)
    {
        return false;
    }
    if (output_buffer->format)
    {
        size_t i;
        for (i = 0; i < (output_buffer->depth - 1); i++)
        {
            *output_pointer++ = '\t';
        }
    }
    *output_pointer++ = '}';
    *output_pointer = '\0';
    output_buffer->depth--;
    return true;
}
CJSON_PUBLIC(int) cJSON_GetArraySize(const cJSON *array)
{
    cJSON *child = NULL;
    size_t size = 0;
    if (array == NULL)
    {
        return 0;
    }
    child = array->child;
    while(child != NULL)
    {
        size++;
        child = child->next;
    }
    return (int)size;
}
static cJSON* get_array_item(const cJSON *array, size_t index)
{
    cJSON *current_child = NULL;
    if (array == NULL)
    {
        return NULL;
    }
    current_child = array->child;
    while ((current_child != NULL) && (index > 0))
    {
        index--;
        current_child = current_child->next;
    }
    return current_child;
}
CJSON_PUBLIC(cJSON *) cJSON_GetArrayItem(const cJSON *array, int index)
{
    if (index < 0)
    {
        return NULL;
    }
    return get_array_item(array, (size_t)index);
}
static cJSON *get_object_item(const cJSON * const object, const char * const name, const cJSON_bool case_sensitive)
{
    cJSON *current_element = NULL;
    if ((object == NULL) || (name == NULL))
    {
        return NULL;
    }
    current_element = object->child;
    if (case_sensitive)
    {
        while ((current_element != NULL) && (strcmp(name, current_element->string) != 0))
        {
            current_element = current_element->next;
        }
    }
    else
    {
        while ((current_element != NULL) && (case_insensitive_strcmp((const unsigned char*)name, (const unsigned char*)(current_element->string)) != 0))
        {
            current_element = current_element->next;
        }
    }
    return current_element;
}
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItem(const cJSON * const object, const char * const string)
{
    return get_object_item(object, string, false);
}
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItemCaseSensitive(const cJSON * const object, const char * const string)
{
    return get_object_item(object, string, true);
}
CJSON_PUBLIC(cJSON_bool) cJSON_HasObjectItem(const cJSON *object, const char *string)
{
    return cJSON_GetObjectItem(object, string) ? 1 : 0;
}
static void suffix_object(cJSON *prev, cJSON *item)
{
    prev->next = item;
    item->prev = prev;
}
static cJSON *create_reference(const cJSON *item, const internal_hooks * const hooks)
{
    cJSON *reference = NULL;
    if (item == NULL)
    {
        return NULL;
    }
    reference = cJSON_New_Item(hooks);
    if (reference == NULL)
    {
        return NULL;
    }
    memcpy(reference, item, sizeof(cJSON));
    reference->string = NULL;
    reference->type |= cJSON_IsReference;
    reference->next = reference->prev = NULL;
    return reference;
}
CJSON_PUBLIC(void) cJSON_AddItemToArray(cJSON *array, cJSON *item)
{
    cJSON *child = NULL;
    if ((item == NULL) || (array == NULL))
    {
        return;
    }
    child = array->child;
    if (child == NULL)
    {
        array->child = item;
    }
    else
    {
        while (child->next)
        {
            child = child->next;
        }
        suffix_object(child, item);
    }
}
CJSON_PUBLIC(void) cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item)
{
    if (item == NULL)
    {
        return;
    }
    cJSON_AddItemToObjectCS(object, (char*)cJSON_strdup((const unsigned char*)string, &global_hooks), item);
   
    item->type &= ~cJSON_StringIsConst;
}
#if defined (__clang__) || ((__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
    #pragma GCC diagnostic push
#endif
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
CJSON_PUBLIC(void) cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item)
{
    if ((item == NULL) || (string == NULL))
    {
        return;
    }
    if (!(item->type & cJSON_StringIsConst) && item->string)
    {
        global_hooks.deallocate(item->string);
    }
    item->string = (char*)string;
    item->type |= cJSON_StringIsConst;
    cJSON_AddItemToArray(object, item);
}
#if defined (__clang__) || ((__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
    #pragma GCC diagnostic pop
#endif
CJSON_PUBLIC(void) cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item)
{
    if (array == NULL)
    {
        return;
    }
    cJSON_AddItemToArray(array, create_reference(item, &global_hooks));
}
CJSON_PUBLIC(void) cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item)
{
    if ((object == NULL) || (string == NULL))
    {
        return;
    }
    cJSON_AddItemToObject(object, string, create_reference(item, &global_hooks));
}
CJSON_PUBLIC(cJSON *) cJSON_DetachItemViaPointer(cJSON *parent, cJSON * const item)
{
    if ((parent == NULL) || (item == NULL))
    {
        return NULL;
    }
    if (item->prev != NULL)
    {
        item->prev->next = item->next;
    }
    if (item->next != NULL)
    {
        item->next->prev = item->prev;
    }
    if (item == parent->child)
    {
        parent->child = item->next;
    }
    
    item->prev = NULL;
    item->next = NULL;
    return item;
}//数据判定，对计算的数据进行比较
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromArray(cJSON *array, int which)
{
    if (which < 0)
    {
        return NULL;
    }
    return cJSON_DetachItemViaPointer(array, get_array_item(array, (size_t)which));
}
CJSON_PUBLIC(void) cJSON_DeleteItemFromArray(cJSON *array, int which)
{
    cJSON_Delete(cJSON_DetachItemFromArray(array, which));
}
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObject(cJSON *object, const char *string)
{
    cJSON *to_detach = cJSON_GetObjectItem(object, string);
    return cJSON_DetachItemViaPointer(object, to_detach);
}
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObjectCaseSensitive(cJSON *object, const char *string)
{
    cJSON *to_detach = cJSON_GetObjectItemCaseSensitive(object, string);
    return cJSON_DetachItemViaPointer(object, to_detach);
}
CJSON_PUBLIC(void) cJSON_DeleteItemFromObject(cJSON *object, const char *string)
{
    cJSON_Delete(cJSON_DetachItemFromObject(object, string));
}
CJSON_PUBLIC(void) cJSON_DeleteItemFromObjectCaseSensitive(cJSON *object, const char *string)
{
    cJSON_Delete(cJSON_DetachItemFromObjectCaseSensitive(object, string));
}
CJSON_PUBLIC(void) cJSON_InsertItemInArray(cJSON *array, int which, cJSON *newitem)
{
    cJSON *after_inserted = NULL;
    if (which < 0)
    {
        return;
    }
    after_inserted = get_array_item(array, (size_t)which);
    if (after_inserted == NULL)
    {
        cJSON_AddItemToArray(array, newitem);
        return;
    }
    newitem->next = after_inserted;
    newitem->prev = after_inserted->prev;
    after_inserted->prev = newitem;
    if (after_inserted == array->child)
    {
        array->child = newitem;
    }
    else
    {
        newitem->prev->next = newitem;
    }
}
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemViaPointer(cJSON * const parent, cJSON * const item, cJSON * replacement)
{
    if ((parent == NULL) || (replacement == NULL) || (item == NULL))
    {
        return false;
    }
    if (replacement == item)
    {
        return true;
    }
    replacement->next = item->next;
    replacement->prev = item->prev;
    if (replacement->next != NULL)
    {
        replacement->next->prev = replacement;
    }
    if (replacement->prev != NULL)
    {
        replacement->prev->next = replacement;
    }
    if (parent->child == item)
    {
        parent->child = replacement;
    }
    item->next = NULL;
    item->prev = NULL;
    cJSON_Delete(item);
    return true;
}
CJSON_PUBLIC(void) cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem)
{
    if (which < 0)
    {
        return;
    }
    cJSON_ReplaceItemViaPointer(array, get_array_item(array, (size_t)which), newitem);
}
static cJSON_bool replace_item_in_object(cJSON *object, const char *string, cJSON *replacement, cJSON_bool case_sensitive)
{
    if ((replacement == NULL) || (string == NULL))
    {
        return false;
    }
    if (!(replacement->type & cJSON_StringIsConst) && (replacement->string != NULL))
    {
        cJSON_free(replacement->string);
    }
    replacement->string = (char*)cJSON_strdup((const unsigned char*)string, &global_hooks);
    replacement->type &= ~cJSON_StringIsConst;
    cJSON_ReplaceItemViaPointer(object, get_object_item(object, string, case_sensitive), replacement);
    return true;
}
CJSON_PUBLIC(void) cJSON_ReplaceItemInObject(cJSON *object, const char *string, cJSON *newitem)
{
    replace_item_in_object(object, string, newitem, false);
}
CJSON_PUBLIC(void) cJSON_ReplaceItemInObjectCaseSensitive(cJSON *object, const char *string, cJSON *newitem)
{
    replace_item_in_object(object, string, newitem, true);
}//在界面中输入数据
CJSON_PUBLIC(cJSON *) cJSON_CreateNull(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_NULL;
    }
    return item;
}
CJSON_PUBLIC(cJSON *) cJSON_CreateTrue(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_True;
    }
    return item;
}//在界面中输入数据
CJSON_PUBLIC(cJSON *) cJSON_CreateFalse(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_False;
    }
    return item;
}
CJSON_PUBLIC(cJSON *) cJSON_CreateBool(cJSON_bool b)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = b ? cJSON_True : cJSON_False;
    }
    return item;
}
CJSON_PUBLIC(cJSON *) cJSON_CreateNumber(double num)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_Number;
        item->valuedouble = num;
      
        if (num >= INT_MAX)
        {
            item->valueint = INT_MAX;
        }
        else if (num <= INT_MIN)
        {
            item->valueint = INT_MIN;
        }
        else
        {
            item->valueint = (int)num;
        }
    }
    return item;
}//在界面中输入数据
CJSON_PUBLIC(cJSON *) cJSON_CreateString(const char *string)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_String;
        item->valuestring = (char*)cJSON_strdup((const unsigned char*)string, &global_hooks);
        if(!item->valuestring)
        {
            cJSON_Delete(item);
            return NULL;
        }
    }
    return item;
}
CJSON_PUBLIC(cJSON *) cJSON_CreateRaw(const char *raw)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type = cJSON_Raw;
        item->valuestring = (char*)cJSON_strdup((const unsigned char*)raw, &global_hooks);
        if(!item->valuestring)
        {
            cJSON_Delete(item);
            return NULL;
        }
    }
    return item;
}
CJSON_PUBLIC(cJSON *) cJSON_CreateArray(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if(item)
    {
        item->type=cJSON_Array;
    }
    return item;
}
CJSON_PUBLIC(cJSON *) cJSON_CreateObject(void)
{
    cJSON *item = cJSON_New_Item(&global_hooks);
    if (item)
    {
        item->type = cJSON_Object;
    }
    return item;
}//在界面中输入数据
CJSON_PUBLIC(cJSON *) cJSON_CreateIntArray(const int *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;
    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }
    a = cJSON_CreateArray();
    for(i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber(numbers[i]);
        if (!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }
    return a;
}//在界面中输入数据
CJSON_PUBLIC(cJSON *) cJSON_CreateFloatArray(const float *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;
    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }
    a = cJSON_CreateArray();
    for(i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber((double)numbers[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }
    return a;
}//在界面中输入数据
CJSON_PUBLIC(cJSON *) cJSON_CreateDoubleArray(const double *numbers, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;
    if ((count < 0) || (numbers == NULL))
    {
        return NULL;
    }
    a = cJSON_CreateArray();
    for(i = 0;a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateNumber(numbers[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p, n);
        }
        p = n;
    }
    return a;
}//在界面中输入数据
CJSON_PUBLIC(cJSON *) cJSON_CreateStringArray(const char **strings, int count)
{
    size_t i = 0;
    cJSON *n = NULL;
    cJSON *p = NULL;
    cJSON *a = NULL;
    if ((count < 0) || (strings == NULL))
    {
        return NULL;
    }
    a = cJSON_CreateArray();
    for (i = 0; a && (i < (size_t)count); i++)
    {
        n = cJSON_CreateString(strings[i]);
        if(!n)
        {
            cJSON_Delete(a);
            return NULL;
        }
        if(!i)
        {
            a->child = n;
        }
        else
        {
            suffix_object(p,n);
        }
        p = n;
    }
    return a;
}//在界面中输入数据
CJSON_PUBLIC(cJSON *) cJSON_Duplicate(const cJSON *item, cJSON_bool recurse)
{
    cJSON *newitem = NULL;
    cJSON *child = NULL;
    cJSON *next = NULL;
    cJSON *newchild = NULL;
    if (!item)
    {
        goto fail;
    }
    newitem = cJSON_New_Item(&global_hooks);
    if (!newitem)
    {
        goto fail;
    }
    newitem->type = item->type & (~cJSON_IsReference);
    newitem->valueint = item->valueint;
    newitem->valuedouble = item->valuedouble;
    if (item->valuestring)
    {
        newitem->valuestring = (char*)cJSON_strdup((unsigned char*)item->valuestring, &global_hooks);
        if (!newitem->valuestring)
        {
            goto fail;
        }
    }
    if (item->string)
    {
        newitem->string = (item->type&cJSON_StringIsConst) ? item->string : (char*)cJSON_strdup((unsigned char*)item->string, &global_hooks);
        if (!newitem->string)
        {
            goto fail;
        }
    }
    if (!recurse)
    {
        return newitem;
    }
    child = item->child;
    while (child != NULL)
    {
        newchild = cJSON_Duplicate(child, true); 
        if (!newchild)
        {
            goto fail;
        }
        if (next != NULL)
        {
          
            next->next = newchild;
            newchild->prev = next;
            next = newchild;
        }
        else
        {
            newitem->child = newchild;
            next = newchild;
        }
        child = child->next;
    }
    return newitem;
fail:
    if (newitem != NULL)
    {
        cJSON_Delete(newitem);
    }
    return NULL;
}
CJSON_PUBLIC(void) cJSON_Minify(char *json)
{
    unsigned char *into = (unsigned char*)json;
    if (json == NULL)
    {
        return;
    }
    while (*json)
    {
        if (*json == ' ')
        {
            json++;
        }
        else if (*json == '\t')
        {
            json++;
        }
        else if (*json == '\r')
        {
            json++;
        }
        else if (*json=='\n')
        {
            json++;
        }
        else if ((*json == '/') && (json[1] == '/'))
        {
            while (*json && (*json != '\n'))
            {
                json++;
            }
        }
        else if ((*json == '/') && (json[1] == '*'))
        {
            while (*json && !((*json == '*') && (json[1] == '/')))
            {
                json++;
            }
            json += 2;
        }
        else if (*json == '\"')
        {
            *into++ = (unsigned char)*json++;
            while (*json && (*json != '\"'))
            {
                if (*json == '\\')
                {
                    *into++ = (unsigned char)*json++;
                }
                *into++ = (unsigned char)*json++;
            }
            *into++ = (unsigned char)*json++;
        }
        else
        {
            *into++ = (unsigned char)*json++;
        }
    }
    
    *into = '\0';
}//数据进行处理，并判定
CJSON_PUBLIC(cJSON_bool) cJSON_IsInvalid(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xFF) == cJSON_Invalid;
}
CJSON_PUBLIC(cJSON_bool) cJSON_IsFalse(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xFF) == cJSON_False;
}//在界面中输入数据
CJSON_PUBLIC(cJSON_bool) cJSON_IsTrue(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xff) == cJSON_True;
}//在界面中输入数据

CJSON_PUBLIC(cJSON_bool) cJSON_IsBool(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & (cJSON_True | cJSON_False)) != 0;
}//在界面中输入数据
CJSON_PUBLIC(cJSON_bool) cJSON_IsNull(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xFF) == cJSON_NULL;
}//在界面中输入数据
CJSON_PUBLIC(cJSON_bool) cJSON_IsNumber(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xFF) == cJSON_Number;
}//在界面中输入数据
CJSON_PUBLIC(cJSON_bool) cJSON_IsString(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xFF) == cJSON_String;
}//在界面中输入数据
CJSON_PUBLIC(cJSON_bool) cJSON_IsArray(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xFF) == cJSON_Array;
}//在界面中输入数据
CJSON_PUBLIC(cJSON_bool) cJSON_IsObject(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xFF) == cJSON_Object;
}//在界面中输入数据
CJSON_PUBLIC(cJSON_bool) cJSON_IsRaw(const cJSON * const item)
{
    if (item == NULL)
    {
        return false;
    }
    return (item->type & 0xFF) == cJSON_Raw;
}//在界面中输入数据
CJSON_PUBLIC(cJSON_bool) cJSON_Compare(const cJSON * const a, const cJSON * const b, const cJSON_bool case_sensitive)
{
    if ((a == NULL) || (b == NULL) || ((a->type & 0xFF) != (b->type & 0xFF)) || cJSON_IsInvalid(a))
    {
        return false;
    }
    switch (a->type & 0xFF)
    {
        case cJSON_False:
        case cJSON_True:
        case cJSON_NULL:
        case cJSON_Number:
        case cJSON_String:
        case cJSON_Raw:
        case cJSON_Array:
        case cJSON_Object:
            break;
        default:
            return false;
    }
    if (a == b)
    {
        return true;
    }
    switch (a->type & 0xFF)
    {
        case cJSON_False:
        case cJSON_True:
        case cJSON_NULL:
            return true;
        case cJSON_Number:
            if (a->valuedouble == b->valuedouble)
            {
                return true;
            }
            return false;
        case cJSON_String:
        case cJSON_Raw:
            if ((a->valuestring == NULL) || (b->valuestring == NULL))
            {
                return false;
            }
            if (strcmp(a->valuestring, b->valuestring) == 0)
            {
                return true;
            }
            return false;
        case cJSON_Array:
        {
            cJSON *a_element = a->child;
            cJSON *b_element = b->child;
            for (; (a_element != NULL) && (b_element != NULL);)
            {
                if (!cJSON_Compare(a_element, b_element, case_sensitive))
                {
                    return false;
                }
                a_element = a_element->next;
                b_element = b_element->next;
            }
            if (a_element != b_element) {
                return false;
            }
            return true;
        }
        case cJSON_Object:
        {
            cJSON *a_element = NULL;
            cJSON *b_element = NULL;
            cJSON_ArrayForEach(a_element, a)
            {
                b_element = get_object_item(b, a_element->string, case_sensitive);
                if (b_element == NULL)
                {
                    return false;
                }
                if (!cJSON_Compare(a_element, b_element, case_sensitive))
                {
                    return false;
                }
            }
            cJSON_ArrayForEach(b_element, b)
            {
                a_element = get_object_item(a, b_element->string, case_sensitive);
                if (a_element == NULL)
                {
                    return false;
                }
                if (!cJSON_Compare(b_element, a_element, case_sensitive))
                {
                    return false;
                }
            }
            return true;
        }
        default:
            return false;
    }
}//数据进行计算
CJSON_PUBLIC(void *) cJSON_malloc(size_t size)
{
    return global_hooks.allocate(size);
}//在界面中弹出结果
CJSON_PUBLIC(void) cJSON_free(void *object)
{
    global_hooks.deallocate(object);
}

#pragma GCC visibility push(default)
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#pragma GCC visibility pop
#include "cJSON_Utils.h"
#define true ((cJSON_bool)1)
#define false ((cJSON_bool)0)
static unsigned char* cJSONUtils_strdup(const unsigned char* const string)
{
    size_t length = 0;
    unsigned char *copy = NULL;
    length = strlen((const char*)string) + sizeof("");
    copy = (unsigned char*) cJSON_malloc(length);
    if (copy == NULL)
    {
        return NULL;
    }
    memcpy(copy, string, length);
    return copy;
}
static int compare_strings(const unsigned char *string1, const unsigned char *string2, const cJSON_bool case_sensitive)
{
    if ((string1 == NULL) || (string2 == NULL))
    {
        return 1;
    }
    if (string1 == string2)
    {
        return 0;
    }
    if (case_sensitive)
    {
        return strcmp((const char*)string1, (const char*)string2);
    }
    for(; tolower(*string1) == tolower(*string2); (void)string1++, string2++)
    {
        if (*string1 == '\0')
        {
            return 0;
        }
    }
    return tolower(*string1) - tolower(*string2);
}
static cJSON_bool compare_pointers(const unsigned char *name, const unsigned char *pointer, const cJSON_bool case_sensitive)
{
    if ((name == NULL) || (pointer == NULL))
    {
        return false;
    }
    for (; (*name != '\0') && (*pointer != '\0') && (*pointer != '/'); (void)name++, pointer++) 
    {
        if (*pointer == '~')
        {
            if (((pointer[1] != '0') || (*name != '~')) && ((pointer[1] != '1') || (*name != '/')))
            {
                return false;
            }
            else
            {
                pointer++;
            }
        }
        else if ((!case_sensitive && (tolower(*name) != tolower(*pointer))) || (case_sensitive && (*name != *pointer)))
        {
            return false;
        }
    }
    if (((*pointer != 0) && (*pointer != '/')) != (*name != 0))
    {
        return false;;
    }
    return true;
}
static size_t pointer_encoded_length(const unsigned char *string)
{
    size_t length;
    for (length = 0; *string != '\0'; (void)string++, length++)
    {
        if ((*string == '~') || (*string == '/'))
        {
            length++;
        }
    }
    return length;
}
static void encode_string_as_pointer(unsigned char *destination, const unsigned char *source)
{
    for (; source[0] != '\0'; (void)source++, destination++)
    {
        if (source[0] == '/')
        {
            destination[1] = '1';
            destination++;
        }
        else if (source[0] == '~')
        {
            destination[0] = '~';
            destination[1] = '1';
            destination++;
        }
        else
        {
            destination[0] = source[0];
        }
    }
    destination[0] = '\0';
}
CJSON_PUBLIC(char *) cJSONUtils_FindPointerFromObjectTo(const cJSON * const object, const cJSON * const target)
{
    size_t child_index = 0;
    cJSON *current_child = 0;
    if ((object == NULL) || (target == NULL))
    {
        return NULL;
    }
    if (object == target)
    {
   
        return (char*)cJSONUtils_strdup((const unsigned char*)"");
    }
    for (current_child = object->child; current_child != NULL; (void)(current_child = current_child->next), child_index++)
    {
        unsigned char *target_pointer = (unsigned char*)cJSONUtils_FindPointerFromObjectTo(current_child, target);
        
        if (target_pointer != NULL)
        {
            if (cJSON_IsArray(object))
            {
                unsigned char *full_pointer = (unsigned char*)cJSON_malloc(strlen((char*)target_pointer) + 20 + sizeof("/"));
                
                if (child_index > ULONG_MAX)
                {
                    cJSON_free(target_pointer);
                    return NULL;
                }
                sprintf((char*)full_pointer, "/%lu%s", (unsigned long)child_index, target_pointer); 
                cJSON_free(target_pointer);
                return (char*)full_pointer;
            }
            if (cJSON_IsObject(object))
            {
                unsigned char *full_pointer = (unsigned char*)cJSON_malloc(strlen((char*)target_pointer) + pointer_encoded_length((unsigned char*)current_child->string) + 2);
                full_pointer[0] = '/';
                encode_string_as_pointer(full_pointer + 1, (unsigned char*)current_child->string);
                strcat((char*)full_pointer, (char*)target_pointer);
                cJSON_free(target_pointer);
                return (char*)full_pointer;
            }
            cJSON_free(target_pointer);
            return NULL;
        }
    }
    return NULL;
}//界面的功能完全实现

void benchcore(const char *host,const int port,const char *req)
{
    int rlen;
    char buf[1500];
    int s,i;
    struct sigaction sa;
    sa.sa_handler=alarm_handler;
    sa.sa_flags=0;
    if(sigaction(SIGALRM,&sa,NULL))
        exit(3);
    
    alarm(benchtime); // after benchtime,then exit
    rlen=strlen(req);
    nexttry:while(1)
    {
        if(timerexpired)
        {
            if(failed>0)
            {
                 fprintf(stderr,"Correcting failed by signal\n"); 
                failed--;
            }
            return;
        }//定义一个对话框
        
        s=Socket(host,port);                          
        if(s<0) { failed++;continue;} 
        if(rlen!=write(s,req,rlen)) {failed++;close(s);continue;}
        if(http10==0) 
        if(shutdown(s,1)) { failed++;close(s);continue;}
        if(force==0) 
        {
            while(1)
            {
                if(timerexpired) break; 
                i=read(s,buf,1500);
                fprintf(stderr,"%d\n",i); 
                if(i<0) 
                { 
                    failed++;
                    close(s);
                    goto nexttry;
                }
                else
                if(i==0) break;
                else
                bytes+=i;
            }
        }
        if(close(s)) {failed++;continue;}
        speed++;
    }
}
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>
#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"
#define STDIN   0
#define STDOUT  1
#define STDERR  2
void accept_request(void *);
void bad_request(int);
void cat(int, FILE *);
void cannot_execute(int);
void error_die(const char *);
void execute_cgi(int, const char *, const char *, const char *);
int get_line(int, char *, int);
void headers(int, const char *);
void not_found(int);
void serve_file(int, const char *);
int startup(u_short *);
void unimplemented(int);
void accept_request(void *arg)//底层代码进行定义
{
    int client = (intptr_t)arg;
    char buf[1024];
    size_t numchars;
    char method[255];
    char url[255];
    char path[512];
    size_t i, j;
    struct stat st;
    int cgi = 0;      
    char *query_string = NULL;
    numchars = get_line(client, buf, sizeof(buf));
    i = 0; j = 0;
    while (!ISspace(buf[i]) && (i < sizeof(method) - 1))
    {
        method[i] = buf[i];
        i++;
    }
    j=i;
    method[i] = '\0';
    if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
    {
        unimplemented(client);
        return;
    }
    if (strcasecmp(method, "POST") == 0)
        cgi = 1;
    i = 0;
    while (ISspace(buf[j]) && (j < numchars))
        j++;
    while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < numchars))
    {
        url[i] = buf[j];
        i++; j++;
    }
    url[i] = '\0';
    if (strcasecmp(method, "GET") == 0)
    {
        query_string = url;
        while ((*query_string != '?') && (*query_string != '\0'))
            query_string++;
        if (*query_string == '?')
        {
            cgi = 1;
            *query_string = '\0';
            query_string++;
        }
    }//在界面中输入一个职
    sprintf(path, "htdocs%s", url);
    if (path[strlen(path) - 1] == '/')
        strcat(path, "index.html");
    if (stat(path, &st) == -1) {
        while ((numchars > 0) && strcmp("\n", buf))
            numchars = get_line(client, buf, sizeof(buf));
        not_found(client);
    }
    else
    {
        if ((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path, "/index.html");
        if ((st.st_mode & S_IXUSR) ||
                (st.st_mode & S_IXGRP) ||
                (st.st_mode & S_IXOTH)    )
            cgi = 1;
        if (!cgi)
            serve_file(client, path);
        else
            execute_cgi(client, path, method, query_string);
    }
    close(client);
}
void bad_request(int client)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "<P>Your browser sent a bad request, ");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    send(client, buf, sizeof(buf), 0);
}
void cat(int client, FILE *resource)
{
    char buf[1024];
    fgets(buf, sizeof(buf), resource);
    while (!feof(resource))
    {
        send(client, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), resource);
    }
}
void cannot_execute(int client)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
    send(client, buf, strlen(buf), 0);
}
void error_die(const char *sc)
{
    perror(sc);
    exit(1);
}
void execute_cgi(int client, const char *path,
        const char *method, const char *query_string)
{
    char buf[1024];
    int cgi_output[2];
    int cgi_input[2];
    pid_t pid;
    int status;
    int i;
    char c;
    int numchars = 1;
    int content_length = -1;
    buf[0] = 'A'; buf[1] = '\0';
    if (strcasecmp(method, "GET") == 0)
        while ((numchars > 0) && strcmp("\n", buf))  
            numchars = get_line(client, buf, sizeof(buf));
    else if (strcasecmp(method, "POST") == 0) 
    {
        numchars = get_line(client, buf, sizeof(buf));
        while ((numchars > 0) && strcmp("\n", buf))
        {
            buf[15] = '\0';
            if (strcasecmp(buf, "Content-Length:") == 0)
                content_length = atoi(&(buf[16]));
            numchars = get_line(client, buf, sizeof(buf));
        }
        if (content_length == -1) {
            bad_request(client);
            return;
        }
    }
    else
    {
    }

    if (pipe(cgi_output) < 0) {
        cannot_execute(client);
        return;
    }
    if (pipe(cgi_input) < 0) {
        cannot_execute(client);
        return;
    }
    if ( (pid = fork()) < 0 ) {
        cannot_execute(client);
        return;
    }
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    if (pid == 0) 
    {
        char meth_env[255];
        char query_env[255];
        char length_env[255];
        dup2(cgi_output[1], STDOUT);
        dup2(cgi_input[0], STDIN);
        close(cgi_output[0]);
        close(cgi_input[1]);
        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);
        if (strcasecmp(method, "GET") == 0) {
            sprintf(query_env, "QUERY_STRING=%s", query_string);
            putenv(query_env);
        }
        else {  
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }
        execl(path, NULL);
        exit(0);
    } else {    
        close(cgi_output[1]);
        close(cgi_input[0]);
        if (strcasecmp(method, "POST") == 0)
            for (i = 0; i < content_length; i++) {
                recv(client, &c, 1, 0);
                write(cgi_input[1], &c, 1);
            }
        while (read(cgi_output[0], &c, 1) > 0)
            send(client, &c, 1, 0);
        close(cgi_output[0]);
        close(cgi_input[1]);
        waitpid(pid, &status, 0);
    }
}
int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;
    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
               
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';
    return(i);
}//界面输入功能
void headers(int client, const char *filename)
{
    char buf[1024];
    (void)filename; 
    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}
void not_found(int client)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}
void serve_file(int client, const char *filename)
{
    FILE *resource = NULL;
    int numchars = 1;
    char buf[1024];
    buf[0] = 'A'; buf[1] = '\0';
    while ((numchars > 0) && strcmp("\n", buf))  
        numchars = get_line(client, buf, sizeof(buf));
    resource = fopen(filename, "r");
    if (resource == NULL)
        not_found(client);
    else
    {
        headers(client, filename);
        cat(client, resource);
    }
    fclose(resource);
}
int startup(u_short *port)
{
    int httpd = 0;
    int on = 1;
    struct sockaddr_in name;
    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1)
        error_die("socket");
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)  
    {  
        error_die("setsockopt failed");
    }
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        error_die("bind");
    if (*port == 0) 
    {
        socklen_t namelen = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
            error_die("getsockname");
        *port = ntohs(name.sin_port);
    }
    if (listen(httpd, 5) < 0)
        error_die("listen");
    return(httpd);
}
void unimplemented(int client)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}

int main(void)
{
    int server_sock = -1;
    u_short port = 4000;
    int client_sock = -1;
    struct sockaddr_in client_name;
    socklen_t  client_name_len = sizeof(client_name);
    pthread_t newthread;
    server_sock = startup(&port);
    printf("httpd running on port %d\n", port);
    while (1)
    {
        client_sock = accept(server_sock,
                (struct sockaddr *)&client_name,
                &client_name_len);
        if (client_sock == -1)
            error_die("accept");
       
        if (pthread_create(&newthread , NULL, (void *)accept_request, (void *)(intptr_t)client_sock) != 0)
            perror("pthread_create");
    }
    close(server_sock);
    return(0);
}
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char ch = 'A';
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1)
    {
        perror("oops: client1");
        exit(1);
    }
    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"
static char *read_file(const char *filename)
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;
  
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_END) != 0)
    {
        goto cleanup;
    }
    length = ftell(file);
    if (length < 0)
    {
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        goto cleanup;
    }
    content = (char*)malloc((size_t)length + sizeof(""));
    if (content == NULL)
    {
        goto cleanup;
    }
    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((long)read_chars != length)
    {
        free(content);
        content = NULL;
        goto cleanup;
    	}
}
}//定义功能性代码
