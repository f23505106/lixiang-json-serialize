# 使用rapidjson，c++对象序列化到json和反序列化的小程序

* 暂时只实现了int std::string std::vector基本数据类型
* 自定义对象通过实现Serializable的ToJson和ToValue实现对序列化的支持
* 暂未实现对继承，多重继承的支持
* 异常处理并未完全覆盖，性能，代码逻辑未做完全优化

编译方法：
1. clone代码库，注意子工程rapidjson也需要更新
2. 使用clion，或着cmake，执行

示例

```cpp
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
```
输出结果

```
p1 json:{"name_":"p1","age_":4,"address_":{"country_":"china","city_":"beijing","street_":"wangjing","neighbors_":[{"name_":"p2","age_":3,"address_":{"country_":"china","city_":"shanghai","street_":"putuo","neighbors_":[]},"friends_":[]}]},"friends_":[{"relation_":"my best friend","singer_":{"type_":"rocker","age_":18}},{"relation_":"new friend","singer_":{"type_":"little girl","age_":0}},{"relation_":"third friend","singer_":{"type_":"","age_":3}}]}
```

参考资料
1. [android Parcelable实现](https://developer.android.com/reference/android/os/Parcelable)
2. 腾讯内部类protocol buff协议jce的实现
3. [C++/json序列化](https://zhuanlan.zhihu.com/p/57974752)
