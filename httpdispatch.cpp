#include "httpdispatch.h"

static const char* env[] = {
  "REMOTE_ADDR",
  "DOCUMENT_ROOT",
  "GATEWAY_INTERFACE",
  "SERVER_SOFTWARE",
  "SERVER_NAME",
  "SERVER_PROTOCOL",
  "SERVER_PORT",
  "SERVER_ADMIN",
  "SERVER_SIGNATURE",
  "REQUEST_METHOD",
  "REQUEST_URI",
  "PATH_INFO",
  "PATH_TRANSLATED",
  "SCRIPT_NAME",
  "SCRIPT_FILENAME",
  "QUERY_STRING",
  "REMOTE_HOST",
  "REMOTE_USER",
  "REMOTE_IDENT",
  "CONTENT_TYPE",
  "CONTENT_LENGTH",
  "HTTP_ACCEPT",
  "HTTP_ACCEPT_CHARSET",
  "HTTP_ACCEPT_LANGUAGE",
  "HTTP_ENCODING",
  "HTTP_USER_AGENT",
  "HTTP_CONNECTION",
  "HTTP_HOST",
  "HTTP_REFERER",
  "HTTP_COOKIE",
  "AUTH_TYPE",
  "HTTPS"
};

std::map<std::string, std::string> _cgi;
std::map<std::string, std::string> GET;
std::string method;
std::string content_type;
std::string content_length;
std::string query_string;
std::string http_user_agent;

short int HexChar2Dec(char c) {
  if ( '0'<=c && c<='9' ) {
    return short(c-'0');
  } else if ( 'a'<=c && c<='f' ) {
    return ( short(c-'a') + 10 );
  } else if ( 'A'<=c && c<='F' ) {
    return ( short(c-'A') + 10 );
  } else {
    return -1;
  }
}


std::string DecodeUrlComponent(const std::string& value){
  std::string result = "";
  for ( unsigned int i=0; i<value.length(); i++ ) {
    char c = value[i];
    if ( c != '%' ) {
      result += c;
    } else {
      char c1 = value[++i];
      char c0 = value[++i];
      int num = 0;
      num += HexChar2Dec(c1) * 16 + HexChar2Dec(c0);
      result += char(num);
    }
  }
  return result;
}

int splitString(std::map<std::string, std::string> & out, const std::string & in, char sep, char assign){
  std::string key, value;
  std::string *p = &key;
  int n = 0;

  for(unsigned int i = 0; i < in.length(); i++) {
    if (in[i] == assign) {
      p = &value;
      value.clear();
      n++;
    } else if (in[i] == sep) {
      out[key] = DecodeUrlComponent(value);//解码http转码后的字符
      key.clear();
      p = &key;
    } else {
      p->append(1, in[i]);
    }
  }
  //特殊处理，为兼容最后一个＆不写的情况，例如action=read&_op=4
  out[key] = DecodeUrlComponent(value);

  return n;
}

int cgiInt(){
  unsigned int i;
  for(i = 0; i < sizeof(env)/sizeof(char *); i++){
    const char* cgienv = getenv(env[i]);
    if(cgienv){
      _cgi[env[i]] = std::string(cgienv);
    }
  }
  method = _cgi["method"];
  query_string = _cgi["QUERY_STRING"];
  content_type = _cgi["CONTENT_TYPE"];
  content_length = _cgi["CONTENT_LENGTH"];
  http_user_agent = _cgi["HTTP_USER_AGENT"];
  if(method == "GET"){
    splitString(GET, query_string, '&', '=');
    return 0;
  }else if(method == "POST"){
    //长度限定不做判断,如需要自行添加
    //len = toInt(content_length);

    std::stringstream s;
    s << std::cin;
    s >> query_string;
    splitString(GET, query_string, '&', '=');
  }
}

void cgiHead(const char* type){
  std::cout << "Content-Type: " << type << "; charset=utf-8" << std::endl << std::endl;
}
