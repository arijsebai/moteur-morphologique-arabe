#include "HttpUtils.h"
#include <sstream>
#include <cstdlib>

std::string url_decode(const std::string& value){
    std::string result;
    result.reserve(value.size());
    for(size_t i=0;i<value.size();i++){
        if(value[i]=='%' && i+2<value.size()){
            char hex[3]={value[i+1], value[i+2], '\0'};
            result.push_back(static_cast<char>(strtol(hex,nullptr,16)));
            i+=2;
        } else if(value[i]=='+'){
            result.push_back(' ');
        } else {
            result.push_back(value[i]);
        }
    }
    return result;
}

std::string json_escape(const std::string& value){
    std::string out;
    out.reserve(value.size());
    for(char c: value){
        switch(c){
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out.push_back(c); break;
        }
    }
    return out;
}

std::string json_array(const std::vector<std::string>& items){
    std::ostringstream ss;
    ss << "[";
    for(size_t i=0;i<items.size();i++){
        if(i>0) ss << ",";
        ss << "\"" << json_escape(items[i]) << "\"";
    }
    ss << "]";
    return ss.str();
}

std::string get_query_param(const std::string& target, const std::string& key){
    auto pos = target.find('?');
    if(pos==std::string::npos) return "";
    std::string query = target.substr(pos+1);
    std::string pattern = key + "=";
    size_t start = query.find(pattern);
    if(start==std::string::npos) return "";
    start += pattern.size();
    size_t end = query.find('&', start);
    std::string value = query.substr(start, end==std::string::npos ? std::string::npos : end-start);
    return url_decode(value);
}

std::string content_type_for(const std::string& path){
    if(path.rfind(".html")!=std::string::npos) return "text/html; charset=utf-8";
    if(path.rfind(".css")!=std::string::npos) return "text/css; charset=utf-8";
    if(path.rfind(".js")!=std::string::npos) return "application/javascript; charset=utf-8";
    if(path.rfind(".json")!=std::string::npos) return "application/json; charset=utf-8";
    return "text/plain; charset=utf-8";
}
