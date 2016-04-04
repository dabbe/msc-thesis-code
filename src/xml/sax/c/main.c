/* Code taken from: http://stackoverflow.com/questions/609376/geting-xml-data-using-xml-parser-expat */

#include <expat.h>
#include <stdio.h>
#include <string.h>

/* track the current level in the xml tree */
static int      depth = 0;

// static char    *last_content;

static int max_dep = 0;

/* first when start element is encountered */
void
start_element(void *data, const char *element, const char **attribute)
{
    if (depth > max_dep) {
        max_dep = depth;
    }
    depth++;

}

/* decrement the current level of the tree */
void
end_element(void *data, const char *el)
{
    depth--;
}

// void
// handle_data(void *data, const char *content, int length)
// {
//     char           *tmp = malloc(length);
//     strncpy(tmp, content, length);
//     tmp[length] = '\0';
//     data = (void *) tmp;
//     last_content = tmp;         /* TODO: concatenate the text nodes? */
// }

int
parse_xml(char* filename)
{
    FILE           *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    // fseek(fp, 0, SEEK_END);
    // int buff_size = ftell(fp);
    // fseek(fp, 0, SEEK_SET);


    size_t buff_size = 2000000; //5000000;
    char* buff = malloc(buff_size);

    XML_Parser      parser = XML_ParserCreate(NULL);
    XML_SetElementHandler(parser, start_element, end_element);
    // XML_SetCharacterDataHandler(parser, handle_data);

    memset(buff, 0, buff_size);
    printf("strlen(buff) before parsing: %d\n", strlen(buff));

    // size_t          file_size = 0;
    // file_size = fread(buff, sizeof(char), buff_size, fp);
    
    // fread(buff, sizeof(char), buff_size, fp);

    /* parse the xml */
    // if (XML_Parse(parser, buff, strlen(buff), XML_TRUE) == XML_STATUS_ERROR) {
    //     printf("Error: %s\n", XML_ErrorString(XML_GetErrorCode(parser)));
    // }

    size_t len = 0;
    int done = 0;

    do {
        len = fread(buff, sizeof(char), buff_size, fp);
        done = len < sizeof(buff);
        if (!XML_Parse(parser, buff, len, done)) {
            // xml_free (ret);
            XML_ParserFree(parser);
            printf("Error: %s\n", XML_ErrorString(XML_GetErrorCode(parser)));
            exit(1);
        }
    } while (!done);


    fclose(fp);
    XML_ParserFree(parser);

    free(buff);

    return 0;
}

int
main(int argc, char **argv)
{
    // int             result;
    // char            buffer[BUFFER_SIZE];
    // char* buffer = malloc(BUFFER_SIZE);

    // if (buffer == NULL) {
    //     printf("malloc e arg\n");
    // }

    parse_xml(argv[1]);
    printf("Result is %i\n", max_dep);
    return 0;
}
