#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

// #define LOCAL_FILE	"img.tmp" 
// #define REMOTE_URL	"ftp://192.168.0.31/" LOCAL_FILE 

void upload_to_ftp();

int main(int argc, char** argv)
{
	if (argc != 4) {
		printf("invalid arguments\n");
		exit(1);
	}

	int i;
	for (i = 0; i < 10; i++) {
		CURL *curl;
		FILE *fp;
		char *url = "http://localhost/jpg/1/image.jpg";
		char outfilename[FILENAME_MAX] = "image.jpg";

		curl = curl_easy_init();                                                                                                                                                                                                                                                           
		printf("Going for curling!\n");

		if (curl) {   
			printf("Curl was available\n");

		    fp = fopen(outfilename,"wb");
		    curl_easy_setopt(curl, CURLOPT_URL, url);
		    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_setopt(curl, CURLOPT_USERNAME, "root");
			curl_easy_setopt(curl, CURLOPT_PASSWORD, "pass");
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);

			printf("All parameters set\n");

		    curl_easy_perform(curl);
		    curl_easy_cleanup(curl);
		    fclose(fp);

			printf("Cleanup..\n");
		}

		upload_to_ftp(outfilename, argv[1], argv[2], argv[3]);
	}

    return 0;
}

char* get_date() {
    time_t now;
    struct tm *now_tm;
    int year, month, day, hour, minute, second;

    now 	= time(NULL);
    now_tm 	= localtime(&now);
    year 	= now_tm->tm_year + 1900;
    month 	= now_tm->tm_mon + 1;
    day 	= now_tm->tm_mday;
    hour 	= now_tm->tm_hour;
    minute  	= now_tm->tm_min;
    second  	= now_tm->tm_sec;

    static char s[25] = "";

    snprintf(s, 
	sizeof(s), 
	"%04d-%02d-%02d_%02d:%02d:%02d.jpg",
	year,
	month,
	day,
	hour,
	minute,
	second);

    strcat(s, "\n");
    return s;
}

void upload_to_ftp(char* filename, char* host, char* user, char* pass) {

    printf("Uploading to ftp..!\n");

    CURL* curl;

    curl = curl_easy_init();


    FILE* src;
    src = fopen(filename, "rb");

    struct curl_slist* headerlist = NULL;
    static const char buf_1 [] = "RNFR image.jpg";
    // static char buf_2 [50] = "";

    // strcpy(buf_2, "RNTO ");
    // strcat(buf_2, get_date());

    char ftp_url[50];
    snprintf(ftp_url, 7, "ftp://");
    snprintf(ftp_url+6, strlen(host) + 1, host);
    snprintf(ftp_url+6 + strlen(host), 11, "/image.jpg");

    printf("%s\n", ftp_url);

    headerlist = curl_slist_append(headerlist, buf_1);
    // headerlist = curl_slist_append(headerlist, buf_2);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, ftp_url);
    curl_easy_setopt(curl, CURLOPT_USERNAME, user);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, pass);

    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);


    curl_easy_setopt(curl, CURLOPT_READDATA, src);

    CURLcode res;    
    res = curl_easy_perform(curl);


    if(res == CURLE_OK){
        printf("Is okaj\n");
    } else {
		fprintf(stderr, "is no ok :(: %s\n",  curl_easy_strerror(res));
    }



    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);

    fclose(src);
    curl_global_cleanup();
}


