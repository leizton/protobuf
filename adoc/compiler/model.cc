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
