// @[main.cc:01]
CommandLineInterface::Run(int argc, const char* const argv[]) {
  // 解析参数
  ParseArguments(argc, argv)  // @[main.cc:01.01]
  --
  // 构造 source_tree_database 用于查找依赖的其他proto文件
  auto disk_source_tree = std::make_unique<DiskSourceTree>()  // c++14
  InitializeDiskSourceTree(disk_source_tree, nullptr)
    for string& e : proto_path_
      disk_source_tree.MapPath(e.first, e.second)
  auto source_tree_database = make_unique<SourceTreeDescriptorDatabase>(disk_source_tree, nullptr)
  auto descriptor_pool = make_unique<DescriptorPool>(fallback_database=source_tree_database)
  --
  // 把 input_files_ 解析成 FileDescriptor
  vector<FileDescriptor*> parsed_files
  ParseInputFiles(descriptor_pool, &parsed_files)  // @[main.cc:01.02]
  --
  OutputDirective& out_dir = output_directives_[0]
  auto* gen_ctx = new GeneratorContextImpl(parsed_files)
  GenerateOutput(parsed_files, out_dir, gen_ctx)
    // @[code_gen.cc:01]
    out_dir.generator.GenerateAll(parsed_files, "", gen_ctx)
  // 写磁盘文件
  gen_ctx.WriteAllToDisk(out_dir.output_location)
}

// @[main.cc:01.01]
CommandLineInterface::ParseArguments(int argc, const char* const argv[]):ParseArgumentStatus {
  // protoc --cpp_out=. ./proto/hello.proto
  executable_name_ = argv[0]
  for arg : argv[1:argc]
    // arg 的格式是 "--name=value" 或 "value"
    name, value = ParseArgument(arg)
    //
    InterpretArgument(name, value)
      if name.empty()
        input_files_.push_back(value)
      else
        assert(name, "cpp_out")
        // generators_by_flag_name_: map<string,GeneratorInfo>
        CppGenerator* generator = dynamic_cast<CppGenerator*>(generators_by_flag_name_[name].generator)
        output_directives_.push_back(OutputDirective{
          name:name, generator:generator, output_location:value, parameter:"",
        })
  proto_path_.push_back(pair<string,string>("", "."))
}

// @[main.cc:01.02]
CommandLineInterface::ParseInputFiles(DescriptorPool* descriptor_pool, vector<FileDescriptor*>* parsed_files) {
  for string& input_file : input_files_
    descriptor_pool.unused_import_track_files_.insert(input_file)
    FileDescriptor* parsed_file = descriptor_pool.FindFileByName(input_file)  // @[main.cc:01.02.01]
    descriptor_pool.unused_import_track_files_.clear()
    parsed_files.push_back(parsed_file)
}

// @[main.cc:01.02.01]
DescriptorPool::FindFileByName(string& name):FileDescriptor* {
  FileDescriptor* ret = this.FindFileDesc(name)
    return tables_.files_by_name_.getOrDefault(name, nullptr)
  if (ret != nullptr) return ret
  --
  TryFindFileInFallbackDatabase(name)
    FileDescriptorProto file_proto
    fallback_database_.FindFileByName(name, &file_proto)  // @[main.cc:01.02.01.01]
    --
    // 把 file_proto 转化成 FileDescriptor*, 并存到 tables_ 里
    BuildFileFromDatabase(file_proto) // descriptor.cc
      DescriptorBuilder b(this, tables_, nullptr)
      b.BuildFile(file_proto)  // @[main.cc:01.02.01.02], tables_.AddFile
  return tables_.FindFile(name)
}

// @[main.cc:01.02.01.01]
SourceTreeDescriptorDatabase::FindFileByName(string& filename, FileDescriptorProto* output) {
  output.set_name(filename)
  auto input = make_unique<ZeroCopyInputStream>(source_tree_.Open(filename))
  Tokenizer tokenizer(input, nullptr)
  Parser parser
  parser.Parse(&tokenizer, output)  // @[parser.cc:01]
}

// @[main.cc:01.02.01.02]
DescriptorBuilder::BuildFile(FileDescriptorProto& proto):FileDescriptor* {
  return BuildFileImpl(proto)
    auto* ret = new FileDescriptor
    ret.source_code_info_ = new SourceCodeInfo(proto.source_code_info)
    ret.tables_ = file_tables_ = new FileDescriptorTables*
    ret.name_ = proto.name
    ret.package_ = proto.package
    copy message_type/enum_type from proto to ret
    tables_.AddFile(ret)
    return ret
}
