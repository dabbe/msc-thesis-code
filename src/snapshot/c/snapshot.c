#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <openssl/md5.h>

#define BUFF_SIZE 1024
#define NONCE_SIZE 52

char* str2md5(const char* str, int length);
char* send_request_1(int* sockfd, char* host);
char* create_digest_response(char* user, char* pass, char* realm, char* nonce);
void send_request_2(int* sockfd, char* host, char* realm, char* nonce, char* response);

int main(int argc, char** argv)
{
    char* host = "192.168.0.90";

    int sockfd;
    int portno = 80;
    struct hostent *server;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("ERROR opening socket");
    }

    server = gethostbyname(host);
    serv_addr.sin_family = AF_INET;
    memcpy((char*)&serv_addr.sin_addr.s_addr, (char*)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR connecting\n");
    }

    char* responsebuf = send_request_1(&sockfd, host);

    char* user = "root";
    char* pass = "pass";
    char* realm = "AXIS_00408C1865DB";
    char nonce[NONCE_SIZE + 1];
    strncpy(nonce, strstr(responsebuf, "nonce=") + 7, NONCE_SIZE);
    nonce[NONCE_SIZE] = '\0';
    char* response = create_digest_response(user, pass, realm, nonce);
    send_request_2(&sockfd, host, realm, nonce, response);

    FILE* output = fopen("image.jpg", "wb");
    char responsebuf2[BUFF_SIZE];

    int bytes_read = 0;

    do {
        bytes_read = read(sockfd, responsebuf2, BUFF_SIZE);
        fwrite(responsebuf2, sizeof(char), bytes_read, output);
    } while (bytes_read != 0);

    fclose(output);

    free(response);
    free(responsebuf);

    return 0;
}

// From Stackoverflow
char* str2md5(const char* str, int length)
{
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    MD5_Init(&c);

    while (length > 0) {
        if (length > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= 512;
        str += 512;
    }

    MD5_Final(digest, &c);

    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    return out;
}

char* send_request_1(int* sockfd, char* host)
{
    char requestbuf[BUFF_SIZE] = {0};
    char responsebuf[BUFF_SIZE] = {0};
    char* path = "/jpg/1/image.jpg";

    snprintf(requestbuf,
            sizeof(requestbuf),
            "GET %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Accept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.2\r\n"
            "Connection: keep-alive\r\n"
            "\r\n",
            path,
            host);
    write(*sockfd, requestbuf, strlen(requestbuf));
    read(*sockfd, responsebuf, BUFF_SIZE);


    char* ret = malloc(strlen(responsebuf) + 1);
    strncpy(ret, responsebuf, strlen(responsebuf));
    return ret;
}

char* create_digest_response(char* user, char* pass, char* realm, char* nonce)
{
    char* ha1_in = malloc(strlen(user) + strlen(pass) + strlen(realm) + 2 + 1);

    strncpy(ha1_in, user, strlen(user));
    strncat(ha1_in, ":", 1);
    strncat(ha1_in, realm, strlen(realm));
    strncat(ha1_in, ":", 1);
    strncat(ha1_in, pass, strlen(pass));

    char* ha1 = str2md5(ha1_in, strlen(ha1_in));
    char* ha2_in = "GET:/jpg/1/image.jpg";
    char* ha2 = str2md5(ha2_in, strlen(ha2_in));

    char* nonce_cnt = "00000001";
    char* client_nonce = "1337";
    char* qop = "auth";

    char* response_in = malloc(strlen(ha1) + strlen(nonce) + strlen(nonce_cnt) + strlen(client_nonce) + strlen(qop) + strlen(ha2) + 5 + 1);
    strncpy(response_in, ha1, strlen(ha1));
    strncat(response_in, ":", 1);
    strncat(response_in, nonce, strlen(nonce));
    strncat(response_in, ":", 1);
    strncat(response_in, nonce_cnt, strlen(nonce_cnt));
    strncat(response_in, ":", 1);
    strncat(response_in, client_nonce, strlen(client_nonce));
    strncat(response_in, ":", 1);
    strncat(response_in, qop, strlen(qop));   
    strncat(response_in, ":", 1);
    strncat(response_in, ha2, strlen(ha2));
    
    char* response = str2md5(response_in, strlen(response_in));
    char* ret = malloc(strlen(response) + 1);
    strncpy(ret, response, strlen(response));

    free(response_in);
    free(ha1_in);
    free(ha1);
    free(ha2);
    free(response);

    return ret;
}

void send_request_2(int* sockfd, char* host, char* realm, char* nonce, char* response)
{
    char requestbuf[BUFF_SIZE];

    snprintf(requestbuf,
             sizeof(requestbuf),
             "GET /jpg/1/image.jpg HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Accept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.2\r\n"
             "Connection: keep-alive\r\n"
             "Authorization: Digest username=\"root\", realm=\"%s\", nonce=\"%s\", nc=00000001, uri=\"/jpg/1/image.jpg\", response=\"%s\", algorithm=\"MD5\", cnonce=\"1337\", qop=\"auth\"\r\n"
             "\r\n",
             host,
             realm,
             nonce,
             response);
    write(*sockfd, requestbuf, strlen(requestbuf));

    char c[1];
    int br = 0;
    while ((br = read(*sockfd, c, 1)) != 0){
        if (*c == '\r'){
            read(*sockfd, c, 1);
            if (*c == '\n'){
                read(*sockfd, c, 1);
                if (*c == '\r'){
                    read(*sockfd, c, 1);
                    if (*c == '\n'){
                        break;
                    }       
                }                
            }
        }
    }
}


// #include <curl/curl.h>


// void upload_to_ftp(const char* host, const char* user, const char* pass)
// {

//     char* url = calloc(6 + strlen(user) + 1 + strlen(pass) + 1 + strlen(host) + 1, 1);


//     CURL *curl = curl_easy_init();
//     if (curl) {
//         /* enable uploading */ 
//         curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
     
//         /* specify target */ 
//         curl_easy_setopt(curl, CURLOPT_URL, REMOTE_URL);
//     }
// }
