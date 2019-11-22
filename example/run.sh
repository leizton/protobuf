#! /bin/bash

proto_src=./proto
proto_out=./target

PROTOCC="protoc --cpp_out=$proto_out"

rm -rf $proto_out
mkdir -p $proto_out

$PROTOCC $proto_src/goods.proto
$PROTOCC $proto_src/user.proto
$PROTOCC $proto_src/shop_item.proto
$PROTOCC $proto_src/shop.proto

proto_cpp_dir='./target/proto'
proto_cpp=" \
  $proto_cpp_dir/goods.pb.cc \
  $proto_cpp_dir/user.pb.cc \
  $proto_cpp_dir/shop_item.pb.cc \
  $proto_cpp_dir/shop.pb.cc "

rm -f a.out

if [ $# -lt 1 ]; then
  g++ -std=c++11 -I./target -g -O0 test.cc $proto_cpp -lpthread -lprotobuf
else
  g++ -std=c++11 -I./target -g -O0 $@ $proto_cpp -lpthread -lprotobuf
fi

if [ -f a.out ]; then
  ./a.out
fi
