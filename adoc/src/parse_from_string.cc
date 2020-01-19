// @[message_lite.cc:01]
MessageLite::ParseFromString(const string& data):bool {
  return ParseFrom<kParse>(data)  // @[message_lite.cc:01.01]
}

// @[message_lite.cc:01.01]
MessageLite::<T=string>ParseFrom(const T& inp):bool {
  // 返回是否解析成功
  Clear()
  MergePartialFromImpl<alias=false>(inp, msg=this)
    // string 隐式转换成 StringPiece
    var raw_data = (const uint8*)inp.data
    io::CodedinpStream decoder(raw_data, inp.size)
    InlineMergePartialEntireStream(&decoder, msg)
      msg.MergePartialFromCodeStream(&decoder)  // @[message.cc:01]
}

// @[message.cc:01]
Message::MergePartialFromCodedStream(io::CodedinpStream* inp) {
  WireFormat::ParseAndMergePartial(inp, this)  // @[wire_format.cc:01]
}

// @[wire_format.cc:01]
WireFormat::ParseAndMergePartial(io::CodedinpStream* inp, Message* msg) {
  while true {
    //
    uint32 tag = inp.ReadTag()
    if tag == 0, return
    // tag: field_number + wire_type
    WireType wire_type = WireFormatLite::GetTagWireType(tag)
    if wire_type == WIRETYPE_END_GROUP, return
    int field_number = WireFormatLite::GetTagFieldNumber(tag)
    //
    Descriptor* desc = msg.GetDescriptor()
    FieldDescriptor* fe = desc.FindFieldByNumber(field_number)
    //
    ParseAndMergeField(tag, fe, msg, inp)  // @[wire_format.cc:01.01]
  }
}

// @[wire_format.cc:01.01]
WireFormat::ParseAndMergeField(uint32 tag, FieldDescriptor* fe, Message* msg, io::CodedinpStream* inp) {
  Reflection* refl = msg.GetReflection()
  if wire_type == WireTypeForFieldType(fe.type())
    switch fe.type() {
    => FieldDescriptor::TYPE_INT32
      int32_t v
      WireFormatLite::ReadPrimitive<int32_t,WireFormatLite::TYPE_INT32>(inp, &v)
      // class Reflection 定义在 message.h
      // SetInt32() AddInt32() 实现在 generated_message_reflection.cc 的宏 DEFINE_PRIMITIVE_ACCESSORS
      // Reflection::SetInt32() -> Reflection::SetField<int32_t>(), @[generated_message_reflection.cc:01]
      // Reflection::AddInt32() -> Reflection::AddField<int32_t>(), @[generated_message_reflection.cc:02]
      fe.is_repeated() ? refl.AddInt32(msg, fe, v) : refl.SetInt32(msg, fe, v)
    => FieldDescriptor::TYPE_INT64
     ...
    => FieldDescriptor::TYPE_MESSAGE
      MessageFactory* factory = inp.GetExtensionFactory()
      Message* sub_msg = fe.is_repeated() ? refl.AddMessage(msg, fe, factory) : refl.MutableMessage(msg, fe, factory)
      WireFormatLite::ReadMessage(inp, sub_msg)
    }
  else if wire_type == WIRETYPE_LENGTH_DELIMITED && fe.is_packable()
    uint32_t len_limit
    inp.ReadVarint32(&len_limit)
    // 设置一个标识位, 读到这个限制点时, inp.BytesUntilLimit()返回0
    io::CodedInputStream::Limit limit_point = inp.PushLimit(len_limit)
    // 解析方式和上面类似, 只是这里的fe都是repeated
    switch fe.type() {
    => FieldDescriptor::TYPE_INT32
      while inp.BytesUntilLimit() > 0
        int32_t v
        WireFormatLite::ReadPrimitive<int32_t,WireFormatLite::TYPE_INT32>(inp, &v)
        refl.AddInt32(msg, fe, v)
    }
    // 移除限制点
    inp.PopLimit(limit_point)
}
