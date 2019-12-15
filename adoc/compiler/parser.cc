##
// 检查当前token的text/type
Parser::LookingAt(string text)
  return input_.current().text == text
Parser::LookingAtType(Tokenizer::TokenType type)
  return input_.current().type == type
##
Parser::Consume(string text)
  // 检查token_text, 然后读入下一个token
  if LookingAt(text)
    input_.Next()
    return true
  else
    return false
Parser::ConsumeIdentifier() string
  // 检查token_type
  if LookingAtType(Tokenizer::TYPE_IDENTIFIER)
    string ret = input_.current().text
    input_.Next()
    return ret
  else
    throw exception


// @[02.01.01.01]
Parser::ParseTopLevelStatement(FileDescriptorProto* file, root_loc)
  if LookingAt("message") {
    LocationRecorder loc(root_loc, path1=kMessageTypeFieldNumber, path2=file.message_type_size)
      path2: 第几个message(从0开始)
    ParseMessageDefinition(file.add_message_type(), loc, file)
  }
  else if LookingAt("option")
    LocationRecorder loc(root_loc, path1=kOptionsFieldNumber)
    ParseOption(file.mutable_options(), loc, file)
  else if LookingAt("enum")
    LocationRecorder loc(root_loc, path1=kEnumTypeFieldNumber, path2=file.enum_type_size)
    ParseEnumDefinition(file.add_enum_type(), loc, file)
  else if LookingAt("import")
    ParseImport(file.mutable_dependency(), file.mutable_public_dependency(),
                file.mutable_weak_dependency(), root_loc, file)
  else if LookingAt("service")
  else if LookingAt("extend")
  else if LookingAt("package")
##
Parser::ParseMessageDefinition(DescriptorProto* message, message_loc, FileDescriptorProto* file)
  Consume("message")
  // 解析message名字
  message->mutable_name() = ConsumeIdentifier()
  ConsumeIdentifier(message->mutable_name())
  // 解析message内容
  ParseMessageBlock(&)
    Consume("{")
    while !LookingAt("}")
      // @ref Parser::ParseMessageStatement()
      switch input_.current.text
        "message":
          // 递归解析message
          ParseMessageDefinition(message.add_nested_type(), message_loc, file)
        "enum":
          LocationRecorder loc(message_loc, path1=kEnumTypeFieldNumber, path2=message.enum_type_size)
          ParseEnumDefinition(message.add_enum_type(), loc, file)
        "extensions":
        "reserved":
        "extend":
        "option":
          //...
        else
          // optional repeated required
          // 在proto3里, 不用填写optional, 默认就是
          LocationRecorder field_loc(message_loc, path1=kFieldFieldNumber, path2=message.field_size)
          ParseMessageField(message.add_field(), message->mutable_nested_type(),
                            message_loc, kNestedTypeFieldNumber, field_loc, file)
    Consume("}")
##
Parser::ParseMessageField(FieldDescriptorProto* field, RepeatedPtrField<DescriptorProto>* messages,
                          message_loc, kNestedTypeFieldNumber, field_loc, file) {
  // label
  LocationRecorder label_loc(field_loc, path1=kLabelFieldNumber)
  FieldDescriptorProto::Label label = {
    TryConsume("optional") => LABEL_OPTIONAL
    TryConsume("repeated") => LABEL_REPEATED
    TryConsume("required") => LABEL_REQUIRED
  }
  field.set_label(label)
  // @ref Parser::ParseMessageFieldNoLabel()
  // type
  MapField map_field
  auto type_loc(field_loc, path1=kTypeNameFieldNumber)
  if TryConsume("map")
    field.set_label(LABEL_REPEATED)
    Consume("<");  ParseType(&map_field.key_type, &map_field.key_type_name)
    Consume(",");  ParseType(&map_field.value_type, &map_field.value_type_name)
    Consume(">")
  else
    FieldDescriptorProto::Type type;  string type_name
    ParseType(&type, &type_name)
    field.set_type_name(type_name)
  // name
  auto name_loc(field_loc, path1=kNameFieldNumber)
  field.mutable_name() = ConsumeIdentifier()
  Consume("=")
  // field_number
  auto field_no_loc(field, path1=kNumberFieldNumber)
  field.set_number(ConsumeInteger())
  // 附加项
  if TryConsume("[")
    auto loc(field_loc, kOptionsFieldNumber)
    if TryConsume("default")
      // 默认值
      ParseDefaultAssignment(field, field_loc, file)
  ParseFieldOptions(field, field_loc, file)
  Consume(";")
  --
  if map_field.is_map_field
    GenerateMapEntry(map_field, field, messages)
}
##
Parser::GenerateMapEntry
  /*
  message Foo {
    map<string,string> boo = 1;
  }
  wile be interpreted as:
  message Foo {
    message BooEntry {
      option map_entry = true;
      string key = 1;
      string value = 2;
    }
    repeated BooEntry boo = 1;
  }
  */
