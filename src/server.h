typedef struct _http_header
{                                    ///// read each line of header and whatever found, update that  ;
    // char *(*read)(char *p, int *pn); //// http_read_store_and_echo
    // int (*store)(char *p);
    // int (*echo)(void *);
#define MREQTYPE_PUBLIC 1
#define MREQTYPE_HTTP1 2
    int mreqtypepp; //// req type 1.public 2.http1  3.both
    char method[20];
    char url[0x400];
    char http_version[0x40];
#define action_plus_url sub_url
#define requested_cmd_data query_string
    char action[100], query_string[0x100], baction[60]; ///// api :: and base_api:: base action ,  requested_cmd_data
    char Host[40];                                      //// "Host: 192.168.9.16:8088";
    char User_Agent[0x100];                             //// User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:95.0) Gecko/20100101 Firefox/95.0
    char Accept[0x100];                                 //// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
    char Accept_Language[40];                           //// Accept-Language: en-US,en;q=0.5
    char Accept_Encoding[40];                           //// Accept-Encoding: gzip, deflate
    char Content_Type[40];                              //// Content-Type: application/x-www-form-urlencoded
    char Content_Length[40];                            //// Content-Length: 49
    char Origin[100];                                   //// Origin: http://192.168.9.16:8088
    char Connection[40];                                //// Connection: keep-alive
    char Referer[0x100];                                //// Referer: http://192.168.9.16:8088/mobile
    char Upgrade_Insecure_Request[40];
    ; //// Upgrade-Insecure-Requests: 1
    ////////////////////////////////////////////////////////////////////////////////////////

    char A_IM[40];                          //// A-IM: feed
    char Accept_Charset[40];                //// Accept-Charset: utf-8
    char Accept_Datetime[60];               //// Accept-Datetime: Thu, 31 May 2007 20:35:00 GMT
    char Access_Control_Request_Method[40]; //// Access-Control-Request-Method: GET
    char Access_Control_Request_Headers[40];
    char Authorization[0x100];     //// Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
    char Cache_Control[40];        //// Cache-Control: no-cache
    char Content_Encoding[40];     //// Content-Encoding: gzip
    char Content_MD5[60];          //// Content-MD5: Q2hlY2sgSW50ZWdyaXR5IQ==
    char Cookie[0x100];            //// Cookie: $Version=1; Skin=new;
    char Date[50];                 //// Date: Tue, 15 Nov 1994 08:12:31 GMT
    char Expect[50];               //// Expect: 100-continue
    char Forwarded[0x200];         //// Forwarded: for=192.0.2.60;proto=http;by=203.0.113.43 Forwarded: for=192.0.2.43, for=198.51.100.17
    char From[100];                //// From: user@example.com
    char HTTP2_Settings[20];       //// HTTP2-Settings: token64
    char If_Match[100];            //// If-Match: "737060cd8c284d8af7ad3082f209582d"
    char If_Modified_Since[40];    //// If-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT
    char If_None_Match[100];       //// If-None-Match: "737060cd8c284d8af7ad3082f209582d"
    char If_Range[100];            //// If-Range: "737060cd8c284d8af7ad3082f209582d"
    char If_Unmodified_Since[60];  //// If-Unmodified-Since: Sat, 29 Oct 1994 19:43:31 GMT
    char Max_Forwards[40];         //// Max-Forwards: 10
    char Pragma[40];               //// Pragma: no-cache
    char Prefer[40];               //// Prefer: return=representation
    char Proxy_Authorization[100]; //// Proxy-Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
    char Range[40];                //// Range: bytes=500-999
    // duplicate char Referer[100]                      ;      //// Referer: http://en.wikipedia.org/wiki/Main_Page
    char TE[40];                //// TE: trailers, deflate
    char Trailer[40];           //// Trailer: Max-Forwards
    char Transfer_Encoding[40]; //// Transfer-Encoding: chunked
    // duplicate char User_Agent[0x100]                 ;      //// User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:12.0) Gecko/20100101 Firefox/12.0
    char Upgrade[100]; //// Upgrade: h2c, HTTPS/1.3, IRC/6.9, RTA/x11, websocket
    char Via[0x100];   //// Via: 1.0 fred, 1.1 example.com
    char Warning[100]; //// Warning: 199 Miscellaneous warning
    //
    /// non standard request:
    /// duplicate Upgrade-Insecure-Requests       //// Upgrade-Insecure-Requests: 1
    char X_Requested_With[60];       //// X-Requested-With: XMLHttpRequest
    char DNT[60];                    //// DNT: 1 (Do Not Track Enabled)
    char X_Forwarded_For[60];        //// X-Forwarded-For: client1, proxy1, proxy2
    char X_Forwarded_Host[60];       //// X-Forwarded-Host: en.wikipedia.org:8080
    char X_Forwarded_Proto[60];      //// X-Forwarded-Proto: https
    char Front_End_Https[60];        //// Front-End-Https: on
    char X_Http_Method_Override[60]; //// X-HTTP-Method-Override: DELETE
    char X_ATT_DeviceId[60];         //// X-Att-Deviceid: GT-P7320/P7320XXLPG
    char X_Wap_Profile[100];         //// x-wap-profile: http://wap.samsungmobile.com/uaprof/SGH-I777.xml
    char Proxy_Connection[60];       //// Proxy-Connection: keep-alive
    char X_UIDH[60];                 ////
    char X_Csrf_Token[100];          //// X-Csrf-Token: i8XNjC4b8KVok4uw5RftR38Wgp2BFwql
    char X_Request_ID[100];          //// X-Request-ID: f058ebd6-02f7-4d3f-942e-904344e8cde5
    char X_Correlation_ID[60];       ////
    char Save_Data[60];              //// Save-Data: on
    char Accept_Ranges[100];         //// ###@@@
    char Age[100];                   //// ###@@@
    char Allow[100];                 //// ###@@@
    char dummy[0x100];
    ;

} http_header;

#define SUCCESS 0
#define FAILURE 1
#define MATCH(x,y)  !strcmp(x,y)
#define BUFFER_SIZE 65000
// #define PUBLIC_DIRECTORY 