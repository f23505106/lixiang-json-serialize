
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "serialization.h"

/*
 * 打印测试函数
 * */
void printValue(const rapidjson::Value&v,std::string s){
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    v.Accept(writer);
    std::cout <<s<<buffer.GetString()<<std::endl;
}

///////////数据结构定义
struct Person;
struct Singer :public Serializable{
    std::string type_;
    int age_;

    Singer(std::string type,int age=0): type_(type), age_(age){}
    Singer(int age): type_(""), age_(age){}
    Singer() = default;

    bool operator==(const Singer& rhs)const{
        return type_ == rhs.type_ && age_ == rhs.age_;
    }

    rapidjson::Value ToJson(rapidjson::Document::AllocatorType& allocator) const override{
        rapidjson::Value ret(rapidjson::kObjectType);
        TO_JSON(type_, allocator)
        TO_JSON(age_, allocator)
        return ret;
    }
    bool ToValue(const rapidjson::Value &root) override {
        bool ret = true;
        TO_VALUE(type_)
        TO_VALUE(age_)
        return ret;
    }
};

struct Address :public Serializable{
    std::string country_;
    std::string city_;
    std::string street_;
    std::vector<Person> neighbors_;
    Address(std::string country,std::string city,std::string street,std::vector<Person> neighbors={}):
            country_(country),city_(city),street_(street),neighbors_(neighbors){}
    Address()=default;
    bool operator==(const Address& rhs)const{
        return country_ == rhs.country_ && city_ == rhs.city_ && street_ == rhs.street_ && neighbors_ == rhs.neighbors_;
    }

    rapidjson::Value ToJson(rapidjson::Document::AllocatorType& allocator) const override{
        rapidjson::Value ret(rapidjson::kObjectType);
        TO_JSON(country_, allocator)
        TO_JSON(city_, allocator)
        TO_JSON(street_, allocator)
        TO_JSON(neighbors_, allocator)
        return ret;
    }
    bool ToValue(const rapidjson::Value &root) override {
        bool ret = true;
        TO_VALUE(country_)
        TO_VALUE(city_)
        TO_VALUE(street_)
        TO_VALUE(neighbors_)
        return ret;
    }
};
struct Friend :public Serializable{
    std::string relation_;
    Singer singer_;

    Friend(std::string relation,Singer singer=Singer()):relation_(relation),singer_(singer){};
    Friend()=default;
    bool operator==(const Friend& rhs) const {
        return relation_ == rhs.relation_ && singer_ == rhs.singer_;
    }

    rapidjson::Value ToJson(rapidjson::Document::AllocatorType& allocator) const override{
        rapidjson::Value ret(rapidjson::kObjectType);
        TO_JSON(relation_,allocator)
        TO_JSON(singer_,allocator)
        return ret;
    }
    bool ToValue(const rapidjson::Value &root) override {
        bool ret = true;
        TO_VALUE(relation_)
        TO_VALUE(singer_)
        return ret;
    }
};
struct Person :public Serializable{
    std::string name_;
    int age_;
    Address address_;
    std::vector<Friend> friends_;

    Person(std::string name,int age,Address address,std::vector<Friend> friends={}):
            name_(name),age_(age), address_(address),friends_(friends){}
    Person()=default;
    bool operator==(const Person& rhs) const {
        return name_ == rhs.name_ && age_ == rhs.age_ && address_ == rhs.address_ && friends_ == rhs.friends_;
    }

    rapidjson::Value ToJson(rapidjson::Document::AllocatorType& allocator) const override{
        rapidjson::Value ret(rapidjson::kObjectType);
        TO_JSON(name_, allocator)
        TO_JSON(age_, allocator)
        TO_JSON(address_, allocator)
        TO_JSON(friends_, allocator)
        return ret;
    }
    bool ToValue(const rapidjson::Value &root) override {
        bool ret = true;
        TO_VALUE(name_)
        TO_VALUE(age_)
        TO_VALUE(address_)
        TO_VALUE(friends_)
        return ret;
    }
};
int main() {
    Singer s = {"abd",23};
    Friend f1{"my best friend", {"rocker", 18}};
    Friend f2{"new friend", {"little girl"}};
    Friend f3{"third friend", 3};
    Person p2{"p2", 3, {"china", "shanghai", "putuo"}};
    Address addr1{"china", "beijing", "wangjing", {p2}};
    Person p1{"p1", 4, addr1, {f1, f2, f3}};
    std::string json = dump(p1);
    std::cout<<"p1 json:"<<json<<std::endl;
    Person pp = parse<Person>(json);
    assert(pp == p1);
    return 0;
}