// @[generated_message_reflection.cc:01]
Reflection::SetField<T>(Message* msg, FieldDescriptor* fe, T& v) {
  T* val = MutableRaw<T>(msg, fe)
    uint32_t offset = schema_.GetFieldOffset(fe)
    return reinterpret_cast<T*>(reinterpret_cast<char*>(msg) + offset)
  *val = v
}

// @[generated_message_reflection.cc:02]
Reflection::AddField<T>(Message* msg, FieldDescriptor* fe, T& v) {
  MutableRaw<RepeatedField<T>>(msg, fe)->Add(v)
}
