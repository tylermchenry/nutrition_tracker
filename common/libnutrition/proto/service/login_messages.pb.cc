// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "libnutrition/proto/service/login_messages.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* LogInRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  LogInRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* LogInResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  LogInResponse_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto() {
  protobuf_AddDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "libnutrition/proto/service/login_messages.proto");
  GOOGLE_CHECK(file != NULL);
  LogInRequest_descriptor_ = file->message_type(0);
  static const int LogInRequest_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInRequest, username_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInRequest, password_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInRequest, realname_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInRequest, createuser_),
  };
  LogInRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      LogInRequest_descriptor_,
      LogInRequest::default_instance_,
      LogInRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(LogInRequest));
  LogInResponse_descriptor_ = file->message_type(1);
  static const int LogInResponse_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInResponse, userid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInResponse, username_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInResponse, realname_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInResponse, error_),
  };
  LogInResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      LogInResponse_descriptor_,
      LogInResponse::default_instance_,
      LogInResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogInResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(LogInResponse));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    LogInRequest_descriptor_, &LogInRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    LogInResponse_descriptor_, &LogInResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto() {
  delete LogInRequest::default_instance_;
  delete LogInRequest_reflection_;
  delete LogInResponse::default_instance_;
  delete LogInResponse_reflection_;
}

void protobuf_AddDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_AddDesc_libnutrition_2fproto_2fservice_2ferror_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n/libnutrition/proto/service/login_messa"
    "ges.proto\032&libnutrition/proto/service/er"
    "ror.proto\"_\n\014LogInRequest\022\020\n\010username\030\001 "
    "\001(\t\022\020\n\010password\030\002 \001(\t\022\020\n\010realName\030\003 \001(\t\022"
    "\031\n\ncreateUser\030\004 \001(\010:\005false\"Z\n\rLogInRespo"
    "nse\022\016\n\006userId\030\001 \001(\005\022\020\n\010username\030\002 \001(\t\022\020\n"
    "\010realName\030\003 \001(\t\022\025\n\005error\030d \001(\0132\006.Error", 278);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "libnutrition/proto/service/login_messages.proto", &protobuf_RegisterTypes);
  LogInRequest::default_instance_ = new LogInRequest();
  LogInResponse::default_instance_ = new LogInResponse();
  LogInRequest::default_instance_->InitAsDefaultInstance();
  LogInResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto {
  StaticDescriptorInitializer_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto() {
    protobuf_AddDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
  }
} static_descriptor_initializer_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto_;


// ===================================================================

const ::std::string LogInRequest::_default_username_;
const ::std::string LogInRequest::_default_password_;
const ::std::string LogInRequest::_default_realname_;
#ifndef _MSC_VER
const int LogInRequest::kUsernameFieldNumber;
const int LogInRequest::kPasswordFieldNumber;
const int LogInRequest::kRealNameFieldNumber;
const int LogInRequest::kCreateUserFieldNumber;
#endif  // !_MSC_VER

LogInRequest::LogInRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void LogInRequest::InitAsDefaultInstance() {
}

LogInRequest::LogInRequest(const LogInRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void LogInRequest::SharedCtor() {
  _cached_size_ = 0;
  username_ = const_cast< ::std::string*>(&_default_username_);
  password_ = const_cast< ::std::string*>(&_default_password_);
  realname_ = const_cast< ::std::string*>(&_default_realname_);
  createuser_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LogInRequest::~LogInRequest() {
  SharedDtor();
}

void LogInRequest::SharedDtor() {
  if (username_ != &_default_username_) {
    delete username_;
  }
  if (password_ != &_default_password_) {
    delete password_;
  }
  if (realname_ != &_default_realname_) {
    delete realname_;
  }
  if (this != default_instance_) {
  }
}

void LogInRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* LogInRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return LogInRequest_descriptor_;
}

const LogInRequest& LogInRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();  return *default_instance_;
}

LogInRequest* LogInRequest::default_instance_ = NULL;

LogInRequest* LogInRequest::New() const {
  return new LogInRequest;
}

void LogInRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (username_ != &_default_username_) {
        username_->clear();
      }
    }
    if (_has_bit(1)) {
      if (password_ != &_default_password_) {
        password_->clear();
      }
    }
    if (_has_bit(2)) {
      if (realname_ != &_default_realname_) {
        realname_->clear();
      }
    }
    createuser_ = false;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool LogInRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string username = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_username()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->username().data(), this->username().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_password;
        break;
      }
      
      // optional string password = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_password:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_password()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->password().data(), this->password().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_realName;
        break;
      }
      
      // optional string realName = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_realName:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_realname()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->realname().data(), this->realname().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_createUser;
        break;
      }
      
      // optional bool createUser = 4 [default = false];
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_createUser:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &createuser_)));
          _set_bit(3);
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

void LogInRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string username = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->username().data(), this->username().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->username(), output);
  }
  
  // optional string password = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->password().data(), this->password().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->password(), output);
  }
  
  // optional string realName = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->realname().data(), this->realname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->realname(), output);
  }
  
  // optional bool createUser = 4 [default = false];
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(4, this->createuser(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* LogInRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string username = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->username().data(), this->username().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->username(), target);
  }
  
  // optional string password = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->password().data(), this->password().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->password(), target);
  }
  
  // optional string realName = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->realname().data(), this->realname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->realname(), target);
  }
  
  // optional bool createUser = 4 [default = false];
  if (_has_bit(3)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(4, this->createuser(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int LogInRequest::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string username = 1;
    if (has_username()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->username());
    }
    
    // optional string password = 2;
    if (has_password()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->password());
    }
    
    // optional string realName = 3;
    if (has_realname()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->realname());
    }
    
    // optional bool createUser = 4 [default = false];
    if (has_createuser()) {
      total_size += 1 + 1;
    }
    
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

void LogInRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const LogInRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const LogInRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void LogInRequest::MergeFrom(const LogInRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_username(from.username());
    }
    if (from._has_bit(1)) {
      set_password(from.password());
    }
    if (from._has_bit(2)) {
      set_realname(from.realname());
    }
    if (from._has_bit(3)) {
      set_createuser(from.createuser());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void LogInRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LogInRequest::CopyFrom(const LogInRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LogInRequest::IsInitialized() const {
  
  return true;
}

void LogInRequest::Swap(LogInRequest* other) {
  if (other != this) {
    std::swap(username_, other->username_);
    std::swap(password_, other->password_);
    std::swap(realname_, other->realname_);
    std::swap(createuser_, other->createuser_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata LogInRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = LogInRequest_descriptor_;
  metadata.reflection = LogInRequest_reflection_;
  return metadata;
}


// ===================================================================

const ::std::string LogInResponse::_default_username_;
const ::std::string LogInResponse::_default_realname_;
#ifndef _MSC_VER
const int LogInResponse::kUserIdFieldNumber;
const int LogInResponse::kUsernameFieldNumber;
const int LogInResponse::kRealNameFieldNumber;
const int LogInResponse::kErrorFieldNumber;
#endif  // !_MSC_VER

LogInResponse::LogInResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void LogInResponse::InitAsDefaultInstance() {
  error_ = const_cast< ::Error*>(&::Error::default_instance());
}

LogInResponse::LogInResponse(const LogInResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void LogInResponse::SharedCtor() {
  _cached_size_ = 0;
  userid_ = 0;
  username_ = const_cast< ::std::string*>(&_default_username_);
  realname_ = const_cast< ::std::string*>(&_default_realname_);
  error_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LogInResponse::~LogInResponse() {
  SharedDtor();
}

void LogInResponse::SharedDtor() {
  if (username_ != &_default_username_) {
    delete username_;
  }
  if (realname_ != &_default_realname_) {
    delete realname_;
  }
  if (this != default_instance_) {
    delete error_;
  }
}

void LogInResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* LogInResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return LogInResponse_descriptor_;
}

const LogInResponse& LogInResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();  return *default_instance_;
}

LogInResponse* LogInResponse::default_instance_ = NULL;

LogInResponse* LogInResponse::New() const {
  return new LogInResponse;
}

void LogInResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    userid_ = 0;
    if (_has_bit(1)) {
      if (username_ != &_default_username_) {
        username_->clear();
      }
    }
    if (_has_bit(2)) {
      if (realname_ != &_default_realname_) {
        realname_->clear();
      }
    }
    if (_has_bit(3)) {
      if (error_ != NULL) error_->::Error::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool LogInResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 userId = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &userid_)));
          _set_bit(0);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_username;
        break;
      }
      
      // optional string username = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_username:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_username()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->username().data(), this->username().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_realName;
        break;
      }
      
      // optional string realName = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_realName:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_realname()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->realname().data(), this->realname().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
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

void LogInResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 userId = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->userid(), output);
  }
  
  // optional string username = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->username().data(), this->username().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->username(), output);
  }
  
  // optional string realName = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->realname().data(), this->realname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->realname(), output);
  }
  
  // optional .Error error = 100;
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      100, this->error(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* LogInResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 userId = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->userid(), target);
  }
  
  // optional string username = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->username().data(), this->username().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->username(), target);
  }
  
  // optional string realName = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->realname().data(), this->realname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->realname(), target);
  }
  
  // optional .Error error = 100;
  if (_has_bit(3)) {
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

int LogInResponse::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 userId = 1;
    if (has_userid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->userid());
    }
    
    // optional string username = 2;
    if (has_username()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->username());
    }
    
    // optional string realName = 3;
    if (has_realname()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->realname());
    }
    
    // optional .Error error = 100;
    if (has_error()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->error());
    }
    
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

void LogInResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const LogInResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const LogInResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void LogInResponse::MergeFrom(const LogInResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_userid(from.userid());
    }
    if (from._has_bit(1)) {
      set_username(from.username());
    }
    if (from._has_bit(2)) {
      set_realname(from.realname());
    }
    if (from._has_bit(3)) {
      mutable_error()->::Error::MergeFrom(from.error());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void LogInResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LogInResponse::CopyFrom(const LogInResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LogInResponse::IsInitialized() const {
  
  return true;
}

void LogInResponse::Swap(LogInResponse* other) {
  if (other != this) {
    std::swap(userid_, other->userid_);
    std::swap(username_, other->username_);
    std::swap(realname_, other->realname_);
    std::swap(error_, other->error_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata LogInResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = LogInResponse_descriptor_;
  metadata.reflection = LogInResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)