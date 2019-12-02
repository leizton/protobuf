// @[01]
int Run(int argc, const char* const argv[])
  ParseArguments(argc, argv)
  --
  auto disk_source_tree = std::make_unique<DiskSourceTree>()  // c++14
  InitializeDiskSourceTree(disk_source_tree, nullptr)
  --
  auto source_tree_database = make_unique<SourceTreeDescriptorDatabase>(disk_source_tree, nullptr)
  auto descriptor_pool = make_unique<DescriptorPool>(fallback_database=source_tree_database)
  descriptor_pool->enforce_weak_ = true
  --
  vector<const FileDescriptor*> parsed_files
  ParseInputFiles(descriptor_pool, &parsed_files)  // 把 input_files_ 解析成 FileDescriptor

// @[01.01]
ParseArgumentStatus ParseArguments(int argc, const char* const argv[])
  executable_name_ = argv[0]
  argv[1:argc] 的格式是 "value" 或 "--name=value"
  input_files_.push_back(格式是"value"的argv)
  proto_path_.push_back(pair<string,string>("", "."))

// @[01.02]
bool InitializeDiskSourceTree(DiskSourceTree* source_tree, DescriptorDatabase* fallback_database=nullptr)
  for (string& e : proto_path_)
    source_tree->MapPath(e.first, e.second)

// @[01.03]
bool ParseInputFiles(DescriptorPool* descriptor_pool, std::vector<const FileDescriptor*>* parsed_files)
  for (string& input_file : input_files_)
    descriptor_pool->unused_import_track_files_.insert(input_file)
    const FileDescriptor* parsed_file = descriptor_pool->FindFileByName(input_file)  // @[02]
    descriptor_pool->unused_import_track_files_.clear()
    parsed_files->push_back(parsed_file)


// @[02]
const FileDescriptor* DescriptorPool::FindFileByName(const std::string& name)
  unique_lock<mutex> lk(mutex_)
  const FileDescriptor* ret = tables_->files_by_name_.getOrDefault(name, nullptr)
  if (ret != nullptr) return ret
  --
  FileDescriptorProto file_proto
  fallback_database_->FindFileByName(name, &file_proto)  // @[02.01]
  DescriptorBuilder(this, tables_, nullptr).BuildFile(file_proto)
  // BuildFileFromDatabase(file_proto)
  --
  return tables_->FindFile(name)

// @[02.01]
bool SourceTreeDescriptorDatabase::FindFileByName(const std::string& filename, FileDescriptorProto* output)
  auto input = make_unique<ZeroCopyInputStream>(source_tree_->Open(filename))
  Tokenizer tokenizer(input, nullptr)
  Parser parser
  output->set_name(filename)
  return parser.Parse(&tokenizer, output)
