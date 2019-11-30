// @[01]
int CommandLineInterface::Run(int argc, const char* const argv[])
  ParseArguments(argc, argv)
  auto disk_source_tree = std::make_unique<DiskSourceTree>();  // c++14
  InitializeDiskSourceTree(disk_source_tree.get(), nullptr)

// @[01.01]
CommandLineInterface::ParseArgumentStatus CommandLineInterface::ParseArguments(int argc, const char* const argv[])
  executable_name_ = argv[0]
  argv[1:argc] 的格式是 "value" 或 "--name=value"
  input_files_.push_back(格式是"value"的argv)
  proto_path_.push_back(pair<string,string>("", "."))

// @[01.02]
bool CommandLineInterface::InitializeDiskSourceTree(DiskSourceTree* source_tree, DescriptorDatabase* fallback_database=nullptr)
  for (auto& e : proto_path_)
    source_tree->MapPath(e.first, e.second)