#pragma once

#include <vector>
#include <string>
#include <map>

namespace naga
{
    typedef std::vector<std::pair<std::string, std::string>> http_headers_type;
	typedef std::vector<std::pair<std::string, std::string>> http_data_type;

    enum http_method
    {
        GET,
        POST,
        PUT,
        DELETE_,
        HEAD,
        OPTIONS
    };

    enum http_status : long
    {
        UNKNOWN,
        CONTINUE_ = 100,
        SWITCHING_PROTOCOLS,
        PROCESSING,
        OK = 200,
        CREATED,
        ACCEPTED,
        NON_AUTHORITATIVE_INFORMATION,
        NO_CONTENT,
        RESET_CONTENT,
        PARTIAL_CONTENT,
        MULTI_STATUS,
        ALREADY_REPORTED,
        IM_USED,
        MULTIPLE_CHOICES = 300,
        MOVED_PERMANENTLY,
        FOUND,
        SEE_OTHER,
        NOT_MODIFIED,
        USE_PROXY,
        SWITCH_PROXY,
        TEMPORARY_REDIRECT,
        PERMANENT_REDIRECT,
        BAD_REQUEST = 400,
        UNAUTHORIZED,
        PAYMENT_REQUIRED,
        FORBIDDEN,
        NOT_FOUND,
        METHOD_NOT_ALLOWED,
        NOT_ACCEPTABLE,
        PROXY_AUTHENTICATION_REQUIRED,
        REQUEST_TIMEOUT,
        CONFLICT,
        GONE,
        LENGTH_REQURED,
        PRECONDITION_FAILED,
        PAYLOAD_TOO_LARGE,
        URI_TOO_LONG,
        UNSUPPORTED_MEDIA_TYPE,
        RANGE_NOT_SATISFIABLE,
        EXPECTATION_FAILED,
        IM_A_TEAPOT,
        MISDIRECTED_REQUEST = 421,
        UNPROCESSABLE_ENTITY,
        LOCKED,
        FAILED_DEPENDENCY,
        UPGRADE_REQUIRED,
        PRECONDITION_REQIRED,
        TOO_MANY_REQUESTS,
        REQUEST_HEADER_FIELDS_TOO_LARGE,
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED,
        BAD_GATEWAY,
        SERVICE_UNAVAILABLE,
        GATEWAY_TIMEOUT,
        HTTP_VERSION_NOT_SUPPORTED,
        VARIANT_ALSO_NEGOTIATES,
        UNSUFFICIENT_STORAGE,
        LOOP_DETECTED,
        NOT_EXTENDED,
        NETWORK_AUTHENTICATION_REQIRED
    };
}