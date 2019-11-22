# src/google/protobuf/compiler/main.cc
main(argc, argv)
  CommandLineInterface cli
  cli.AllowPlugins("protoc-")
  //
  var cpp_gen CppGenerator
  cli.RegisterGenerator("--cpp_out", "--cpp_opt", &cpp_gen)
  //
  cli.Run(argc, argv)


# CommandLineInterface
RegisterGenerator(flag_name, option_flag_name string, generator CodeGenerator*)
  generators_by_flag_name_[flag_name] = generators_by_option_name_[option_flag_name] =
    GeneratorInfo{flag_name, option_flag_name, generator}
Run(argc, argv)
  // protoc --cpp_out=$proto_out $proto_src/user.proto
  // 在 $proto_out/protobuf 目录下生成 user.pb.cc user.pb.h
  ParseArguments(argc, argv)
    从 argv 里解析出proto文件名，存到 input_files_
    // proto_path_ 和 descriptor_set_in_names_ 基本上不会被用到
    从 argv 的 "--proto_path" 解析 proto_path_
    从 argv 的 "--descriptor_set_in" 解析 descriptor_set_in_names_
    if proto_path_.empty() && descriptor_set_in_names_.empty()
      // 使用当前目录, pair.first是virtual_path, pair.second是disk_path(物理路径)
      proto_path_.push_back(pair<string,string>("", "."))
  //
  disk_source_tree = unique_str<>(new DiskSourceTree())
  InitializeDiskSourceTree(disk_source_tree.get(), nullptr)
  source_tree_database = unique_str<>(new SourceTreeDescriptorDatabase(disk_source_tree.get(), nullptr))
  //
  unique_ptr<DescriptorPool> descriptor_pool
  descriptor_pool.reset(new DescriptorPool(source_tree_database.get()))
    source_tree_database 在 descriptor_pool 里是 fallback_database_
  vector<FileDescriptor*> parsed_files
  ParseInputFiles(descriptor_pool.get(), &parsed_files`out`)
  //
  ...
ParseInputFiles(descriptor_pool, parsed_files`out`)
  for file_name string : input_files_
    fd FileDescriptor* = descriptor_pool.FindFileByName(file_name)
    assert fd != nullptr
    parsed_files.push_back(fd)