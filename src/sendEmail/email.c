#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>

static char* payload_text[4];

struct upload_status {
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = payload_text[upload_ctx->lines_read];

  if(data) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;

    return len;
  }

  return 0;
}

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

//To concate two pointers
char* concate(char *dest, char *src) {
  char *out;

  if((out = (char *)malloc(strlen(dest) + strlen(src) + 1)) != NULL)
  {
     strcpy(out, dest);
     strcat(out, src);
  }
  else
  {
     puts("Don't have enough memory");
  }

 return out;
}//end 

//sending email
int send_email(char *to, char *from, char *passwd, char *msg, char *subject, char *username){
  printf("To: %s\n",to);
  printf("From: %s\n",from);
  printf("Password: %s\n",passwd);
  printf("Msg: %s\n",msg);
  printf("Sub: %s\n",subject);
  printf("Username: %s\n",username);
  payload_text[0] = concate("To: ",to);
  payload_text[1] = concate("\r\nFrom: ",from);
  payload_text[2] = concate("\r\nSubject: ",subject);
  payload_text[3] = concate("\r\n",msg);
  payload_text[4] = NULL;

  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx;

  upload_ctx.lines_read = 0;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_USERNAME, username);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, passwd);
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    // curl_easy_setopt(curl, CURLOPT_CAINFO, "google.pem");
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
    recipients = curl_slist_append(recipients, to);
    // curl_easy_setopt(curl, CURLOPT_FILE, "data.txt"); 
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_slist_free_all(recipients);

    curl_easy_cleanup(curl);
  }
  return (int)res;

 }//end





int main(int argc, char *argv[]){

  puts("Enter your email: ");
  char from[80]; fgets(from,79,stdin);
  strtok(from, "\n");

  puts("Enter receiver email: ");
  char to[80]; fgets(to,79,stdin);
  strtok(to, "\n");

  puts("Enter Subject: ");
  char subject[80]; fgets(subject,79,stdin);
  strtok(subject, "\n");

  puts("Enter your msg: ");
  char *msg = inputString(stdin,1);
  strtok(msg, "\n");

  puts("Enter Username: ");
  char username[80]; fgets(username,79,stdin);
  strtok(username, "\n");

  char *passwd = getpass("Email Password: ");
  strtok(passwd, "\n");

  send_email(to,from,passwd,msg,subject,username);
  
  free(msg);
}//end