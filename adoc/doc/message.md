# ref
https://developers.google.com/protocol-buffers/docs/proto
https://colobu.com/2015/01/07/Protobuf-language-guide/


# protobuf的优缺点
- 优点
性能好, 兼顾空间和时间
支持前向兼容(兼容老版本的结果,proto的新字段用默认值)和后向兼容(兼容新版本的结果,proto可忽略data的新字段)
- 缺点
编码后的二进制不易读
编码后的二进制不具有自描述性, 解码时必须有proto描述文件
-
protobuf编码后本质上是kv_pair数组
只要key在各版本中是唯一的就能保证前后兼容
key是无意义的数字编号, 所以节省空间、不能自描述


# extend
Extensions let you declare that a range of field numbers in a message are available for third-party extensions
类似于继承, 在其他proto文件里扩展已有message, 而不用修改原始proto文件
- example
~~~proto
// a.proto
message Aoo {
  extensions 100 to 199;
}
// b.proto
extend Aoo {
  optional int v = 101;
}
~~~


# oneof
类似union, 只有一个字段会被设置
- example
~~~proto
message Aoo {
  oneof test_oneof {
    string name = 1;
    string reserve_name = 2;
  }
};
//
Aoo a;
a.set_name("---");
CHECK(a.has_name());
a.set_reserve_name("___");
CHECK(!a.has_name());
~~~
