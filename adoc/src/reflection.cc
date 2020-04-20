class Reflection {
  comment() {
    声明在 message.h
    实现在 generated_message_reflection.cc
  }
  fields() {
    schema_  const ReflectionSchema
  }
  MutableRaw<T>(Message* msg, FieldDescriptor* fe) {
    uint32_t offset = schema_.GetFieldOffset(fe)
    return reinterpret_cast<T*>(reinterpret_cast<char*>(msg) + offset)
  }
  SetField<T>(Message* msg, FieldDescriptor* fe, T& v) {
    T* t = MutableRaw<T>(msg, fe)
    *t = v
  }
  AddField<T>(Message* msg, FieldDescriptor* fe, T& v) {
    MutableRaw<RepeatedField<T>>(msg, fe)->Add(v)
  }
}