# ref
https://developers.google.com/protocol-buffers/docs/encoding.html#structure


# Base 128 Varints
- 基于128的变长整数编码
- example
300 编码成 1010 1100 0000 0010, 解码如下
    1010 1100 0000 0010
     010 1100  000 0010    去掉每个字节的第一个bit
     000 0010  010 1100    网络字节序是大端序, 所以这里倒过来
      00 0001 0010 1100    高字节右移1个bit, 这个bit补到低字节的第一个bit


# Message Structure
a protocol buffer message is a series of key-value pairs
wire_type = {
  varint`0, int32 int64 bool enum ...`,
  64_bit`1, fixed64 sfixed64 double`,
  length_delimited`2, string bytes repeated_fields embedded_message`,
  32_bit`5, fixed32 sfixed32 float`,
}
key = encodeVarint((field_number << 3) | wire_type). 所以field_number最大是`2^29-1`


# signed varint
有符号数变长编码时, 转换成无符号数, 转换函数如下
               0 -> 0
  -1 -> 1      1 -> 2
  -2 -> 3      2 -> 4
  -3 -> 5      3 -> 6
这个转换函数称作`zig-zags`
sint32 n -> (n << 1) ^ (n >> 31)
               *2      符号位移到最后一位, 负数对应奇数, 正数对应偶数
sint64 n -> (n << 1) ^ (n >> 63)
转换成unsigned后再用varint编码


# string
- example
message Aoo {
  optional string s = 2;
}
当 s == "testing" 时, 编码如下
  12 07 74 65 73 74 69 6e 67
0x12 -> 0001 0010 -> field_number=2,wire_type=2
0x07 -> a编码后长度是7


# embedded message
- example
message Aoo {
  optional int v = 1;
}
message Boo {
  optional Aoo a = 3;
}
当 a.v == 150 时, 编码如下
  1a 03 08 96 01
0x1a -> 0001 1010 -> field_number=3,wire_type=2
0x03 -> a编码后长度是3`08 96 01`
0x08 -> 0000 1000 -> field_number=1,wire_type=0`int`


# repeated field
- MergeFrom
Normally, an encoded message would never have more than one instance of a non-repeated field.
If the same field appears multiple times, the parser accepts the last value it sees.
非repeated字段有多个实例值时, 需要进行合并操作, 即MergeFrom
- example
  Aoo a1;  a1.ParseFromString(s1);
  Aoo a2;  a2.ParseFromString(s2);
  a1.MergeFrom(a2);
  等效于
  a1.ParseFromString(s1+s2);
- packed repeated field
message Aoo {
  repeated int arr = 1; [packed=true]
}
带packed表示arr长度是0时, arr不出现在编码结果中
当 arr = [6, 270] 时, 编码如下
  0a 03 06 8E 02
0a -> field_number=1,wire_type=2
03 -> payload_size=3
06 -> arr[0]
8E 02 -> arr[1]
