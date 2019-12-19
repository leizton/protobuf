// @[code_gen.cc:01]
CodeGenerator::GenerateAll(vector<FileDescriptor*> files, parameter="", GeneratorContext* gen_ctx) {
  for auto* file : files
    Generate(file, "", gen_ctx)  // @[code_gen.cc:01.01]
      Generate()是virtual, 由派生类实现
}

// @[code_gen.cc:01.01]
CppGenerator::Generate(FileDescriptor* file, parameter="", GeneratorContext* gen_ctx) {
  Options file_opt
  file_opt.opensource_runtime = opensource_runtime_
  file_opt.runtime_include_base = runtime_include_base_
  FileGenerator file_gen(file, file_opt)
    file_(file), file_opt_(file_opt)
    variables_ = map<string, string> {
      "filename": file_.name
    }
  --
  string pb_h_name = StripProto(file.name) + ".pb.h"
  ZeroCopyOutputStream* out1 = new MemoryOutputStream(gen_ctx, pb_h_name, false)
  Printer printer1(out1, '$')  // '$'字符串里的变量占位符
  file_gen.GeneratePBHeader(&printer1)  // @[code_gen.cc:01.01.01]
  --
  string pb_cc_name = StripProto(file.name) + ".pb.cc"
  ZeroCopyOutputStream* out2 = new MemoryOutputStream(gen_ctx, pb_cc_name, false)
  Printer printer2(out2, '$')
  file_gen.GenerateSource(&printer2)  // @[code_gen.cc:01.01.02]
}

// @[code_gen.cc:01.01.01]
// cpp_file.cc
FileGenerator::GeneratePBHeader(printer) {
  GenerateTopHeaderGuard(printer, true);
  GenerateLibraryIncludes(printer);
  GenerateHeader(printer);
  GenerateBottomHeaderGuard(printer, true);
}

// @[code_gen.cc:01.01.02]
// cpp_file.cc
FileGenerator::GenerateSource(printer) {
}
