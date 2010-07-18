// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "libnutrition/proto/service/nutrient_messages.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* NutrientLoadRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  NutrientLoadRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* NutrientLoadResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  NutrientLoadResponse_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto() {
  protobuf_AddDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "libnutrition/proto/service/nutrient_messages.proto");
  GOOGLE_CHECK(file != NULL);
  NutrientLoadRequest_descriptor_ = file->message_type(0);
  static const int NutrientLoadRequest_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadRequest, omit_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadRequest, all_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadRequest, requestedids_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadRequest, requestednames_),
  };
  NutrientLoadRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      NutrientLoadRequest_descriptor_,
      NutrientLoadRequest::default_instance_,
      NutrientLoadRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(NutrientLoadRequest));
  NutrientLoadResponse_descriptor_ = file->message_type(1);
  static const int NutrientLoadResponse_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadResponse, nutrients_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadResponse, error_),
  };
  NutrientLoadResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      NutrientLoadResponse_descriptor_,
      NutrientLoadResponse::default_instance_,
      NutrientLoadResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NutrientLoadResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(NutrientLoadResponse));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    NutrientLoadRequest_descriptor_, &NutrientLoadRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    NutrientLoadResponse_descriptor_, &NutrientLoadResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto() {
  delete NutrientLoadRequest::default_instance_;
  delete NutrientLoadRequest_reflection_;
  delete NutrientLoadResponse::default_instance_;
  delete NutrientLoadResponse_reflection_;
}

void protobuf_AddDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_AddDesc_libnutrition_2fproto_2fdata_2fnutrient_2eproto();
  ::protobuf_AddDesc_libnutrition_2fproto_2fservice_2ferror_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n2libnutrition/proto/service/nutrient_me"
    "ssages.proto\032&libnutrition/proto/data/nu"
    "trient.proto\032&libnutrition/proto/service"
    "/error.proto\"l\n\023NutrientLoadRequest\022\023\n\004o"
    "mit\030\001 \001(\010:\005false\022\022\n\003all\030\002 \001(\010:\005false\022\024\n\014"
    "requestedIds\030\003 \003(\t\022\026\n\016requestedNames\030\004 \003"
    "(\t\"O\n\024NutrientLoadResponse\022 \n\tnutrients\030"
    "\001 \003(\0132\r.NutrientData\022\025\n\005error\030d \001(\0132\006.Er"
    "ror", 323);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "libnutrition/proto/service/nutrient_messages.proto", &protobuf_RegisterTypes);
  NutrientLoadRequest::default_instance_ = new NutrientLoadRequest();
  NutrientLoadResponse::default_instance_ = new NutrientLoadResponse();
  NutrientLoadRequest::default_instance_->InitAsDefaultInstance();
  NutrientLoadResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto {
  StaticDescriptorInitializer_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto() {
    protobuf_AddDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
  }
} static_descriptor_initializer_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int NutrientLoadRequest::kOmitFieldNumber;
const int NutrientLoadRequest::kAllFieldNumber;
const int NutrientLoadRequest::kRequestedIdsFieldNumber;
const int NutrientLoadRequest::kRequestedNamesFieldNumber;
#endif  // !_MSC_VER

NutrientLoadRequest::NutrientLoadRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void NutrientLoadRequest::InitAsDefaultInstance() {
}

NutrientLoadRequest::NutrientLoadRequest(const NutrientLoadRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void NutrientLoadRequest::SharedCtor() {
  _cached_size_ = 0;
  omit_ = false;
  all_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

NutrientLoadRequest::~NutrientLoadRequest() {
  SharedDtor();
}

void NutrientLoadRequest::SharedDtor() {
  if (this != default_instance_) {
  }
}

void NutrientLoadRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* NutrientLoadRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return NutrientLoadRequest_descriptor_;
}

const NutrientLoadRequest& NutrientLoadRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();  return *default_instance_;
}

NutrientLoadRequest* NutrientLoadRequest::default_instance_ = NULL;

NutrientLoadRequest* NutrientLoadRequest::New() const {
  return new NutrientLoadRequest;
}

void NutrientLoadRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    omit_ = false;
    all_ = false;
  }
  requestedids_.Clear();
  requestednames_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool NutrientLoadRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional bool omit = 1 [default = false];
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &omit_)));
          _set_bit(0);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_all;
        break;
      }
      
      // optional bool all = 2 [default = false];
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_all:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &all_)));
          _set_bit(1);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_requestedIds;
        break;
      }
      
      // repeated string requestedIds = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_requestedIds:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_requestedids()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->requestedids(0).data(), this->requestedids(0).length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_requestedIds;
        if (input->ExpectTag(34)) goto parse_requestedNames;
        break;
      }
      
      // repeated string requestedNames = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_requestedNames:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_requestednames()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->requestednames(0).data(), this->requestednames(0).length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_requestedNames;
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void NutrientLoadRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional bool omit = 1 [default = false];
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(1, this->omit(), output);
  }
  
  // optional bool all = 2 [default = false];
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(2, this->all(), output);
  }
  
  // repeated string requestedIds = 3;
  for (int i = 0; i < this->requestedids_size(); i++) {
  ::google::protobuf::internal::WireFormat::VerifyUTF8String(
    this->requestedids(i).data(), this->requestedids(i).length(),
    ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->requestedids(i), output);
  }
  
  // repeated string requestedNames = 4;
  for (int i = 0; i < this->requestednames_size(); i++) {
  ::google::protobuf::internal::WireFormat::VerifyUTF8String(
    this->requestednames(i).data(), this->requestednames(i).length(),
    ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->requestednames(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* NutrientLoadRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional bool omit = 1 [default = false];
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(1, this->omit(), target);
  }
  
  // optional bool all = 2 [default = false];
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(2, this->all(), target);
  }
  
  // repeated string requestedIds = 3;
  for (int i = 0; i < this->requestedids_size(); i++) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->requestedids(i).data(), this->requestedids(i).length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(3, this->requestedids(i), target);
  }
  
  // repeated string requestedNames = 4;
  for (int i = 0; i < this->requestednames_size(); i++) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->requestednames(i).data(), this->requestednames(i).length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(4, this->requestednames(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int NutrientLoadRequest::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional bool omit = 1 [default = false];
    if (has_omit()) {
      total_size += 1 + 1;
    }
    
    // optional bool all = 2 [default = false];
    if (has_all()) {
      total_size += 1 + 1;
    }
    
  }
  // repeated string requestedIds = 3;
  total_size += 1 * this->requestedids_size();
  for (int i = 0; i < this->requestedids_size(); i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->requestedids(i));
  }
  
  // repeated string requestedNames = 4;
  total_size += 1 * this->requestednames_size();
  for (int i = 0; i < this->requestednames_size(); i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->requestednames(i));
  }
  
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void NutrientLoadRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const NutrientLoadRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const NutrientLoadRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void NutrientLoadRequest::MergeFrom(const NutrientLoadRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  requestedids_.MergeFrom(from.requestedids_);
  requestednames_.MergeFrom(from.requestednames_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_omit(from.omit());
    }
    if (from._has_bit(1)) {
      set_all(from.all());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void NutrientLoadRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void NutrientLoadRequest::CopyFrom(const NutrientLoadRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NutrientLoadRequest::IsInitialized() const {
  
  return true;
}

void NutrientLoadRequest::Swap(NutrientLoadRequest* other) {
  if (other != this) {
    std::swap(omit_, other->omit_);
    std::swap(all_, other->all_);
    requestedids_.Swap(&other->requestedids_);
    requestednames_.Swap(&other->requestednames_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata NutrientLoadRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = NutrientLoadRequest_descriptor_;
  metadata.reflection = NutrientLoadRequest_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int NutrientLoadResponse::kNutrientsFieldNumber;
const int NutrientLoadResponse::kErrorFieldNumber;
#endif  // !_MSC_VER

NutrientLoadResponse::NutrientLoadResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void NutrientLoadResponse::InitAsDefaultInstance() {
  error_ = const_cast< ::Error*>(&::Error::default_instance());
}

NutrientLoadResponse::NutrientLoadResponse(const NutrientLoadResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void NutrientLoadResponse::SharedCtor() {
  _cached_size_ = 0;
  error_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

NutrientLoadResponse::~NutrientLoadResponse() {
  SharedDtor();
}

void NutrientLoadResponse::SharedDtor() {
  if (this != default_instance_) {
    delete error_;
  }
}

void NutrientLoadResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* NutrientLoadResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return NutrientLoadResponse_descriptor_;
}

const NutrientLoadResponse& NutrientLoadResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();  return *default_instance_;
}

NutrientLoadResponse* NutrientLoadResponse::default_instance_ = NULL;

NutrientLoadResponse* NutrientLoadResponse::New() const {
  return new NutrientLoadResponse;
}

void NutrientLoadResponse::Clear() {
  if (_has_bits_[1 / 32] & (0xffu << (1 % 32))) {
    if (_has_bit(1)) {
      if (error_ != NULL) error_->::Error::Clear();
    }
  }
  nutrients_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool NutrientLoadResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .NutrientData nutrients = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_nutrients:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_nutrients()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_nutrients;
        if (input->ExpectTag(802)) goto parse_error;
        break;
      }
      
      // optional .Error error = 100;
      case 100: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_error:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_error()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void NutrientLoadResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .NutrientData nutrients = 1;
  for (int i = 0; i < this->nutrients_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->nutrients(i), output);
  }
  
  // optional .Error error = 100;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      100, this->error(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* NutrientLoadResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // repeated .NutrientData nutrients = 1;
  for (int i = 0; i < this->nutrients_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->nutrients(i), target);
  }
  
  // optional .Error error = 100;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        100, this->error(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int NutrientLoadResponse::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[1 / 32] & (0xffu << (1 % 32))) {
    // optional .Error error = 100;
    if (has_error()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->error());
    }
    
  }
  // repeated .NutrientData nutrients = 1;
  total_size += 1 * this->nutrients_size();
  for (int i = 0; i < this->nutrients_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->nutrients(i));
  }
  
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void NutrientLoadResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const NutrientLoadResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const NutrientLoadResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void NutrientLoadResponse::MergeFrom(const NutrientLoadResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  nutrients_.MergeFrom(from.nutrients_);
  if (from._has_bits_[1 / 32] & (0xffu << (1 % 32))) {
    if (from._has_bit(1)) {
      mutable_error()->::Error::MergeFrom(from.error());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void NutrientLoadResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void NutrientLoadResponse::CopyFrom(const NutrientLoadResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NutrientLoadResponse::IsInitialized() const {
  
  return true;
}

void NutrientLoadResponse::Swap(NutrientLoadResponse* other) {
  if (other != this) {
    nutrients_.Swap(&other->nutrients_);
    std::swap(error_, other->error_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata NutrientLoadResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = NutrientLoadResponse_descriptor_;
  metadata.reflection = NutrientLoadResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
