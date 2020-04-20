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
- comment
E是int float等基本类型
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

# RepeatedPtrField<E>
- comment
E是Message或string
repeated_field.h
```cpp
class RepeatedPtrField<E> : RepeatedPtrFieldBase {
}
class RepeatedPtrFieldBase {
  class Rep {
    allocated_size  int
    elements        void*[1]
  }
  fields() {
    current_size_   int
    total_size_     int
    rep_            Rep*
  }
  Add<TypeHandler, enable_if<TypeHandler::Movable::value>::type* = nullptr>(typename TypeHandler::Type&& val) {
    if rep_ and current_size_ < rep_.allocated_size
      return reinterpret_cast<typename TypeHandler::Type*>(rep_.elements[current_size_++])
    if !rep_ or rep_.allocated_size == total_size_
      Reserve(new_size=total_size_+1)  // 每次只增长 max(total_size_*2, new_size)
    ++rep_.allocated_size
    typename TypeHandler::Type* ret = TypeHandler::NewFromPrototype(val, arena_)
    rep_->elements[current_size_++] = ret
    return ret
  }
  MergeFrom<TypeHandler>(const RepeatedPtrFieldBase& rhs) {
  }
}
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
