# ref
https://developers.google.com/protocol-buffers/docs/reference/cpp-generated

# package
package foo.bar;
--
namespace foo {
namespace bar {}
}

# message
```cpp
ParseFromString(const string&):bool
SerializeToString(string* out):bool
DebugString():string
Swap(Foo*)
descriptor():Descriptor*:static
```

# RepeatedField<E>
- ref
https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.repeated_field#RepeatedField
- api
```cpp
operator[](int i):E&
Get(int i):E&
at(int i):E&
Set(int i, E&)
Mutable(int i):E*
//
Add(E&)
Add():E*
Add(Iter begin, Iter end)
RemoveLast()
//
Reserve(int new_size)
Capacity():int
size():int
empty():bool
//
begin():iterator
end():iterator
```

# field
message Aoo {
  optional  int     ioo  = 1;
  optional  string  soo  = 2;
  repeated  int     rioo = 3;
  repeated  string  rsoo = 4;
  repeated  Boo     rboo = 5;
}
```cpp
// int
has_ioo():bool
ioo():int
set_ioo(int)
// string
has_soo():bool
soo():const-string&
set_soo(const string&)
// repeated int
rioo_size():int
rioo(int idx):int
set_rioo(int idx, int v)
add_rioo(int v)
rioo():RepeatedField<int>&
mutable_rioo():RepeatedField<int>*
// repeated string
rsoo_size():int
rsoo(int idx):const-string&
set_rsoo(int idx, const string&)/set_rsoo(int idx, const char*)
add_rsoo(const string&)/add_rsoo(const char*)
add_rsoo():string*
mutable_rsoo(int idx):string*
// repeated message
rboo_size():int
rboo(int idx):const-Boo&
add_rboo(Boo)
add_rboo():Boo*
mutable_rboo(int idx):Boo*
```
- summary
optional:  has_voo()   voo()   set_voo(v)
repeated:  voo_size()  voo(i)  set_voo(i,v)  add_voo(v)  add_voo():Voo*  mutable_voo(i):Voo*
