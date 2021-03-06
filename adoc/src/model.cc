const {
  kNameFieldNumber = 1
  kPackageFieldNumber = 2
  kDependencyFieldNumber = 3
  kMessageTypeFieldNumber = 4
  kEnumTypeFieldNumber = 5
  kServiceFieldNumber = 6
  kExtensionFieldNumber = 7
  kOptionsFieldNumber = 8
  kSourceCodeInfoFieldNumber = 9
  kPublicDependencyFieldNumber = 10
  kWeakDependencyFieldNumber = 11
  kSyntaxFieldNumber = 12
}


command_line_interface.h {
  --
  CommandLineInterface
    input_files_ vector<string>
}


message.h {
  --
  Reflection
    schema_  ReflectionSchema
  --
  Metadata
    descriptor  Descriptor*
    reflection  Reflection*
  --
  Message ~> MessageLite
    // proto的message的基类
    GetDescriptor():Descriptor* => GetMetadata().descriptor
    GetReflection():Reflection* => GetMetadata().reflection
    GetMetadata():Metadata => GetMetadataStatic()  // 返回.pb.cc定义的static变量
}


repeated_field.h {
  --
  RepeatedField<E>
    empty():bool
    size():int
    operator[](int i):const-E&:const => Get(i)  // const Element& RepeatedField<Element>::Get(int i)
    operator[](int i):E& => *Mutable(i)  // Element* RepeatedField<Element>::Mutable(int i)
    Set(int i, E& e)
    Add(E& e)
    Add():E*
}


generated_message_reflection.h {
  --
  ReflectionSchema
}


descriptor.h {
  --
  Descriptor
    // message的描述体(描述数据结构)
    name_             string
    full_name_        string
    file_             FileDescriptor*
    containing_type_  FileDescriptor*  // 包含当前descriptor的parent_descriptor
    fields_           FieldDescriptor*
    enum_types_       EnumDescriptor*
    nested_types_     Descriptor*
    --
    FindFieldByName(string key):FieldDescriptor*
      Symbol result = file_->tables_->FindNestedSymbolOfType(this, key, Symbol::FIELD)
      return result.IsNull ? nullptr : result.field_descriptor
  --
  FieldDescripotr
    name():string&
    number():int
    type():Type
    --
    is_reqiured():bool
    is_optional():bool
    is_repeated():bool
    --
    has_default_value():bool
    default_value_bool():bool
    default_value_int32():int32
    default_value_int64():int64
    default_value_float():float
    --
    file():FileDescriptor*
    --
    Type enum {
      TYPE_DOUBLE=1, TYPE_FLOAT, TYPE_INT64, ..., TYPE_MESSAGE, ...
    }
  --
  FileDescriptor
    // proto文件的描述体
    tables_            FileDescriptorTables*
    source_code_info_  SourceCodeInfo
  --
  FileDescriptorTables

  --
  DescriptorPool
    underlay_                   =nullptr
    lazily_build_dependencies_  =false
    fallback_database_          DescriptorDatabase* =source_tree_database
    tables_                     =make_unique<DescriptorPool::Tables>()
  --
  DescriptorPool::Tables
    files_by_name_  unordered_map<const char*, const FileDescriptor*>
}


descriptor_database.h {
  --
  SourceTreeDescriptorDatabase
    ~> DescriptorDatabase
    source_tree_  SourceTree*
}


descriptor.proto {
  --
  FileDescriptorProto
    message_type DescriptorProto[]
  --
  DescriptorProto
    name         string
    nested_type  DescriptorProto[]
    field        FieldDescriptorProto[]
    enum_type    EnumDescriptorProto[]
  --
  FieldDescriptorProto
    Type enum {
      TYPE_DOUBLE=1, TYPE_FLOAT, TYPE_INT64, TYPE_UINT64, TYPE_INT32, ...
    }
    Lable enum {
      LABEL_OPTIONAL=1, LABEL_REQUIRED, LABEL_REPEATED
    }
    name       string
    number     int32
    lable      Label
    type       Type
    type_name  string

  --
  SourceCodeInfo
    location  Location[]
  --
  Location
    // SourceCodeInfo::Location
    path int[]
    span int[]
}


tokenizer.h {
  --
  Tokenizer
    current_   Token{type=TYPE_START,line=column=end_column=0}
    previous_  Token
    input_     ZeroCopyInputStream*
  --
  Token
    type                      TokenType
    lint, column, end_column  int
    text                      string  // 整个token文本
  --
  TokenType enum {
    TYPE_START, TYPE_END,
    TYPE_IDENTIFIER, TYPE_INTEGER,
    TYPE_FLOAT, TYPE_STRING, TYPE_SYMBOL
  }
}


parser.h {
  --
  LocationRecorder
    // wrap Location
    parser_            Parser*
    source_code_info_  SourceCodeInfo*
    location_          SourceCodeInfo::Location*
    --
    LocationRecorder(LocationRecorder* parent, int path1, int path2=None) {
      parser_, source_code_info_ = parent.parser_, parent.source_code_info_
      location_ = source_code_info_.add_location()
      // path
      location_.path = parent.location_.path + {path1}
      if path2 != None, location_.path += {path2}
      // span
      location_.span = [parser_.input_.current.line, parser_.input_.current.column]
    }
}


wire_format_lite.h {
  --
  WireType enum {
    WIRETYPE_VARINT=0, WIRETYPE_FIXED64, WIRETYPE_LENGTH_DELIMITED,
    WIRETYPE_START_GROUP, WIRETYPE_END_GROUP, WIRETYPE_FIXED32,
  }
}
