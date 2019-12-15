// @[01]
Run(int argc, const char* const argv[])
  ParseArguments(argc, argv)
  --
  auto disk_source_tree = std::make_unique<DiskSourceTree>()  // c++14
  InitializeDiskSourceTree(disk_source_tree, nullptr)
  --
  auto source_tree_database = make_unique<SourceTreeDescriptorDatabase>(disk_source_tree, nullptr)
  auto descriptor_pool = make_unique<DescriptorPool>(fallback_database=source_tree_database)
  descriptor_pool.enforce_weak_ = true
  --
  vector<FileDescriptor*> parsed_files
  ParseInputFiles(descriptor_pool, &parsed_files)  // 把 input_files_ 解析成 FileDescriptor

// @[01.01]
ParseArguments(int argc, const char* const argv[]) ParseArgumentStatus
  executable_name_ = argv[0]
  argv[1:argc] 的格式是 "value" 或 "--name=value"
  input_files_.push_back(格式是"value"的argv)
  proto_path_.push_back(pair<string,string>("", "."))

// @[01.02]
InitializeDiskSourceTree(DiskSourceTree* source_tree, DescriptorDatabase* fallback_database=nullptr)
  for string& e : proto_path_
    source_tree.MapPath(e.first, e.second)

// @[01.03]
ParseInputFiles(DescriptorPool* descriptor_pool, vector<FileDescriptor*>* parsed_files)
  for string& input_file : input_files_
    descriptor_pool.unused_import_track_files_.insert(input_file)
    FileDescriptor* parsed_file = descriptor_pool.FindFileByName(input_file)  // @[02]
    descriptor_pool.unused_import_track_files_.clear()
    parsed_files.push_back(parsed_file)


// @[02]
DescriptorPool::FindFileByName(string& name) FileDescriptor*
  FileDescriptor* ret = this.FindFileDesc(name)
  if (ret != nullptr) return ret
  --
  BuildFileDesc(name)
  return tables_.FindFile(name)
##
DescriptorPool::FindFileDesc(string& name) FileDescriptor*
  return tables_.files_by_name_.getOrDefault(name, nullptr)
##
DescriptorPool::BuildFileDesc(string& name)
  FileDescriptorProto file_proto
  fallback_database_.FindFileByName(name, &file_proto)  // @[02.01]
  BuildFileFromDatabase(file_proto)
    // descriptor.cc
    DescriptorBuilder b(this, tables_, nullptr)
    b.BuildFile(file_proto)  // @[02.02]

// @[02.01]
SourceTreeDescriptorDatabase::FindFileByName(string& filename, FileDescriptorProto* output)
  output.set_name(filename)
  auto input = make_unique<ZeroCopyInputStream>(source_tree_.Open(filename))
  Tokenizer tokenizer(input, nullptr)
  Parser parser
  parser.Parse(&tokenizer, output)  // @[02.01.01]

// @[02.01.01]
// 假设待解析的proto文件是 无注释 无语法错误
Parser::Parse(Tokenizer* input, FileDescriptorProto* file)
  input_ = input
  SourceCodeInfo source_code_info
  source_code_info_ = &source_code_info
  LocationRecorder root_loc(parser=this)
  --
  ParseSyntaxIdentifier(root_loc)
  file->set_syntax(syntax_identifier_)  // syntax_identifier_=="proto3"
  --
  while input_.current().type != TYPE_END
    ParseTopLevelStatement(file, root_loc)  // @[02.01.01.01]
  --
  source_code_info_.Swap(file.mutable_source_code_info)

// @[02.02]
DescriptorBuilder::BuildFile(FileDescriptorProto& proto) FileDescriptor*
  // handle proto's dependencies
  tables_.pending_files_.push_back(proto.name)
  for i = [0, proto.dependency_size)
    string& dep_name = proto.dependency(i)
    if pool_.FindFileDesc(dep_name) == nullptr
      pool_.BuildFileDesc(dep_name)
  tables_.pending_files_.pop_back()
  --
  auto* ret = tables_.Allocate<FileDescriptor>()
  ret.syntax_ = FileDescriptor::SYNTAX_PROTO3
