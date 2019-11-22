# CodeGenerator
Generate(file FileDescriptor*, param string&, ctx GeneratorContext*, err string*):bool =0
GenerateAll(files vector<FileDescriptor*>&, param, ctx, err):bool
  // 每个文件独立解析
  succ = true
  for auto* fd : files
    succ = Generate(fd, param, ctx, err)
    if !succ, break
  return succ


# CppGenerator ~> CodeGenerator
Generate(file, param, ctx, err) ~> _
  basename = StripProto(file.name())
  file_generator FileGenerator(file)
  // .pb.h
  output unique_ptr<ZeroCopyOutputStream> = ctx.Open("{basename}.pb.h")
  printer Printer(output.get())
  file_generator.GeneratePBHeader(&printer)
  output.reset(nullptr)
  // .pb.cc
  output.reset(ctx.Open("{basename}.pb.cc"))
  printer Printer(output.get())
  file_generator.GenerateSource(&printer)


# FileGenerator
FileGenerator(file FileDescriptor*)
  file_ = file
  // message_generators_
  msgs vector<Descriptor*> = "cpp_helpers.cc"::FlattenMessagesInFile(file)
  for i : [0, msgs.size())
    msg_gen = new MessageGenerator(msgs[i], variables_, i, options, &scc_analyzer_)
    message_generators_.emplace_back(msg_gen)
GeneratePBHeader(pt Printer*)
  GenerateTopHeaderGuard(pt)  // 写入'#ifndef XXX; #define XXX'
  GenerateLibraryIncludes(pt)  // include protobuf库的头文件
GenerateSource(pt Printer*)
  fmt Formatter(pt)
  GenerateSourceIncludes(pt)
    #include "{basename}.pb.h"
    #include "net/proto2/io/public/coded_stream.h"
    ...
  ns NamespaceOpener("::{file_.package()}", fmt)
  for i : [0, message_generators_.size())
    GenerateSourceDefaultInstance(i, pt)