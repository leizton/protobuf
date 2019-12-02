FlattenMessagesInFile(file FileDescriptor*, vector<Descriptor*>* ret)
  for i : [0, file.message_type_count())
    Flatten(file.message_type(i), ret)

Flatten(desc Descriptor*, vector<Descriptor*>* flatten)
  for i : [0, desc.nested_type_count())
    Flatten(desc.nested_type(i), flatten)  // 递归
  flatten.push_back(desc)