#pragma once

#include<string>
#include<type_traits>
#include<utility>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

/*
 * 只支持了int std::string std::vector基本数据类型
 * */
template <typename T>
inline bool  ToValue(T&obj, const rapidjson::Value &root){
    return obj.ToValue(root);
}

template <typename T>
inline rapidjson::Value ToJson(const T&obj,rapidjson::Document::AllocatorType& allocator){
    return obj.ToJson(allocator);
}

template <>
inline bool ToValue(int& obj,const rapidjson::Value &root){
    if(!root.IsInt()){
        return false;
    }
    obj = root.GetInt();
    return true;
}

template <>
inline rapidjson::Value ToJson(const int& obj,rapidjson::Document::AllocatorType& allocator){
    rapidjson::Value ret(rapidjson::kNumberType);
    ret.SetInt(obj);
    return ret;

}

template <>
inline bool ToValue(std::string& obj,const rapidjson::Value &root){
    if(!root.IsString()){
        return false;
    }
    obj = root.GetString();
    return true;
}

template <>
inline rapidjson::Value ToJson(const std::string& obj,rapidjson::Document::AllocatorType& allocator){
    rapidjson::Value ret(rapidjson::kStringType);
    ret.SetString(obj.c_str(),obj.size());
    return ret;

}

template <typename T>
inline bool ToValue(std::vector<T>& obj,const rapidjson::Value &root){
    if(!root.IsArray()){
        return false;
    }
    obj.clear();
    bool ret = true;
    for(int i=0;i<root.Size();++i){
        T tmp;
        if(!ToValue(tmp,root[i]))
            ret = false;
        obj.push_back(tmp);
    }
    return ret;
}

template <typename T>
inline rapidjson::Value ToJson(const std::vector<T>& obj,rapidjson::Document::AllocatorType& allocator){
    rapidjson::Value ret(rapidjson::kArrayType);
    for(const T& i:obj){
        ret.PushBack(::ToJson(i,allocator),allocator);
    }
    return ret;
}
/*
 * 支持json序列化的接口
 * 可以通过继承该类实现ToJson和ToValue，即可值支持序列化
 * */
struct Serializable{
    public:
        static constexpr char* kParentKey = "parent";
        virtual rapidjson::Value ToJson(rapidjson::Document::AllocatorType& allocator) const = 0;
        virtual bool ToValue(const rapidjson::Value &root) = 0;
};

/*
 * 将一个rapidjson::Value值转化为对应的值
 * 需要外部提供 bool类型返回ret，root代表rapidjson::Value的json对象，将从该对象获取子成员field
 * */
#define TO_VALUE(field)                     \
        if(!::ToValue(field,root[#field])){   \
            ret = false;                    \
        }
/*
 * 将一个成员变量转化为对应的rapidjson::Value格式
 * ret类型为rapidjson::Value，会在ret下添加子成员，对应的key为field的名称
 * */
#define TO_JSON(field,allocator)                     \
        ret.AddMember(#field,::ToJson(field,allocator) ,allocator);

/*
 * 将json_str对应的json字符串转化为T类型对象
 * */
template <typename T>
T parse(const std::string& json_str){
    rapidjson::Document d;
    d.Parse(json_str.data());
    T ret;
    ::ToValue(ret,d);
    return ret;
}

/*
 * 将obj对象转化为对应的json字符串
 * */
template <typename T>
std::string dump(const T& obj){
    rapidjson::Document d;
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    rapidjson::Value v = ::ToJson(obj,allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    v.Accept(writer);
    return buffer.GetString();
}


