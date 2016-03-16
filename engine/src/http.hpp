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
        get,
        post,
        put,
        delete_,
        head,
        options
    };

    enum http_status : long
    {
        unknown,
        continue_ = 100,
        switching_protocols,
        processing,
        ok = 200,
        created,
        accepted,
        non_authoritative_information,
        no_content,
        reset_content,
        partial_content,
        multi_statis,
        already_reported,
        im_used,
        multiple_choices = 300,
        moved_permanently,
        found,
        see_other,
        not_modified,
        use_proxy,
        switch_proxy,
        temporary_redirect,
        permanent_redirect,
        bad_request = 400,
        unauthorized,
        payment_required,
        forbidden,
        not_found,
        method_not_allowed,
        not_acceptable,
        proxy_authentication_required,
        request_timeout,
        conflict,
        gone,
        length_requred,
        precondition_failed,
        payload_too_large,
        uri_too_long,
        unsupported_media_type,
        range_not_satisfiable,
        expectation_failed,
        im_a_teapot,
        misdirected_request = 421,
        unprocessable_entity,
        locked,
        failed_dependency,
        upgrade_required,
        precondition_reqired,
        too_many_requests,
        request_header_fields_too_large,
        internal_server_error = 500,
        not_implemented,
        bad_gateway,
        service_unavailable,
        gateway_timeout,
        http_version_not_supported,
        variant_also_negotiates,
        unsufficient_storage,
        loop_detected,
        not_extended,
        network_authentication_reqired
    };
}