# DescriptorPool
// fallback_database 是 SourceTreeDescriptorDatabase
// 把 .proto文件 解析成 FileDescriptorProto 的逻辑在 fallback_database.FindFileByName,
//   即 SourceTreeDescriptorDatabase::FindFileByName
DescriptorPool(fallback_database DescriptorDatabase*)
  fallback_database_ = fallback_database
  tables_ Tables
  underlay_ = nullptr
FindFileByName(name):FileDescriptor*
  fd FileDescriptor* = tables_.FindFile(name)
  if fd != nullptr, return fd
  TryFindFileInFallbackDatabase(name)
  return tables_.FindFile(name)
TryFindFileInFallbackDatabase(name)
  var file_proto FileDescriptorProto
  fallback_database_.FindFileByName(name, &file_proto`out`)
  BuildFileFromDatabase(file_proto)  // 把 file_proto 转成 FileDescriptor, 并存到 tables_


# Tables
Tables()
  symbols_by_name_ map<string, Symbol>
  files_by_name_ map<string, FileDescriptor*>
AddFile(fd FileDescriptor*)
  files_by_name_[fd.name.c_str()] = fd


# SourceTreeDescriptorDatabase
FindFileByName(filename string, output FileDescriptorProto*)
  output.set_name(filename)
  input = unique_ptr<io::ZeroCopyInputStream>(source_tree_.Open(filename))
  tokenizer = io::Tokenizer(input.get(), nullptr)
  Parser.new().Parser(&tokenizer, output)


# Symbol
enum Type {
  NULL_SYMBOL, MESSAGE, FIELD, ONEOF, ENUM, ENUM_VALUE, SERVICE, METHOD, PACKAGE
}
union {
  Descriptor* descriptor
  FieldDescriptor* field_descriptor
  OneofDescriptor* oneof_descriptor
  EnumDescriptor* enum_descriptor
  EnumValueDescriptor* enum_value_descriptor
  ServiceDescriptor* service_descriptor
  MethodDescriptor* method_descriptor
  FileDescriptor* package_file_descriptor
}


# FileDescriptor
name():string&  // foo/bar/baz.proto
package():string&  // google.protobuf.compiler