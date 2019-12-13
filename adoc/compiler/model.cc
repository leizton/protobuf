const
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

CommandLineInterface
  input_files_ vector<string>

DescriptorPool
  mutex_ mutex
  underlay_ = nullptr
  lazily_build_dependencies_ = false
  fallback_database_ DescriptorDatabase* = source_tree_database
  tables_ = make_unique<DescriptorPool::Tables>()

DescriptorPool::Tables
  // class DescriptorPool::Tables {...}
  files_by_name_ unordered_map<const char*, const FileDescriptor*>

SourceTreeDescriptorDatabase ~> DescriptorDatabase
  source_tree_ SourceTree*

TokenType
  // enum TokenType
  TYPE_START, TYPE_END
  TYPE_IDENTIFIER, TYPE_INTEGER,
  TYPE_FLOAT, TYPE_STRING, TYPE_SYMBOL

Token
  type                      TokenType
  lint, column, end_column  int
  text                      string  // 整个token文本

Tokenizer
  current_   Token{type=TYPE_START,line=column=end_column=0}
  previous_  Token
  input_     ZeroCopyInputStream*

LocationRecorder
  // wrap Location
  parser_            Parser*
  source_code_info_  SourceCodeInfo*
  location_          SourceCodeInfo::Location*

descriptor.proto
  --
  FileDescriptorProto
    message_type DescriptorProto[]
  --
  DescriptorProto
    name         string
    nested_type  DescriptorProto[]
    options      MessageOptions
    enum_type    EnumDescriptorProto[]
    field        FieldDescriptorProto[]
  --
  MessageOptions
  --
  SourceCodeInfo
    location  Location[]
  --
  Location
    // SourceCodeInfo::Location
    path int[]
    span int[]
