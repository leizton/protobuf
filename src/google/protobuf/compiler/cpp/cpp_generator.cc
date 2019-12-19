// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include <google/protobuf/compiler/cpp/cpp_generator.h>

#include <memory>
#include <utility>
#include <vector>

#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/compiler/cpp/cpp_file.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>



namespace google {
namespace protobuf {
namespace compiler {
namespace cpp {

CppGenerator::CppGenerator() {}
CppGenerator::~CppGenerator() {}

bool CppGenerator::Generate(const FileDescriptor* file,
                            const std::string& parameter,
                            GeneratorContext* gen_ctx,
                            std::string* error) const {
  Options file_opt;
  file_opt.opensource_runtime = opensource_runtime_;
  file_opt.runtime_include_base = runtime_include_base_;

  std::string basename = StripProto(file->name());
  FileGenerator file_gen(file, file_opt);

  // .pb.h
  {
    std::unique_ptr<io::ZeroCopyOutputStream> output(gen_ctx->Open(basename + ".pb.h"));
    io::Printer printer(output.get(), '$');
    file_gen.GeneratePBHeader(&printer, "");
  }

  // .pb.cc
  {
    std::unique_ptr<io::ZeroCopyOutputStream> output(gen_ctx->Open(basename + ".pb.cc"));
    io::Printer printer(output.get(), '$');
    file_gen.GenerateSource(&printer);
  }

  return true;
}

}  // namespace cpp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
