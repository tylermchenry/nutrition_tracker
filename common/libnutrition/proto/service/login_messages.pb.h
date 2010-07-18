// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: libnutrition/proto/service/login_messages.proto

#ifndef PROTOBUF_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto__INCLUDED
#define PROTOBUF_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "libnutrition/proto/service/error.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();

class LogInRequest;
class LogInResponse;

// ===================================================================

class LogInRequest : public ::google::protobuf::Message {
 public:
  LogInRequest();
  virtual ~LogInRequest();
  
  LogInRequest(const LogInRequest& from);
  
  inline LogInRequest& operator=(const LogInRequest& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const LogInRequest& default_instance();
  
  void Swap(LogInRequest* other);
  
  // implements Message ----------------------------------------------
  
  LogInRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LogInRequest& from);
  void MergeFrom(const LogInRequest& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional string username = 1;
  inline bool has_username() const;
  inline void clear_username();
  static const int kUsernameFieldNumber = 1;
  inline const ::std::string& username() const;
  inline void set_username(const ::std::string& value);
  inline void set_username(const char* value);
  inline void set_username(const char* value, size_t size);
  inline ::std::string* mutable_username();
  
  // optional string password = 2;
  inline bool has_password() const;
  inline void clear_password();
  static const int kPasswordFieldNumber = 2;
  inline const ::std::string& password() const;
  inline void set_password(const ::std::string& value);
  inline void set_password(const char* value);
  inline void set_password(const char* value, size_t size);
  inline ::std::string* mutable_password();
  
  // optional string realName = 3;
  inline bool has_realname() const;
  inline void clear_realname();
  static const int kRealNameFieldNumber = 3;
  inline const ::std::string& realname() const;
  inline void set_realname(const ::std::string& value);
  inline void set_realname(const char* value);
  inline void set_realname(const char* value, size_t size);
  inline ::std::string* mutable_realname();
  
  // optional bool createUser = 4 [default = false];
  inline bool has_createuser() const;
  inline void clear_createuser();
  static const int kCreateUserFieldNumber = 4;
  inline bool createuser() const;
  inline void set_createuser(bool value);
  
  // @@protoc_insertion_point(class_scope:LogInRequest)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::std::string* username_;
  static const ::std::string _default_username_;
  ::std::string* password_;
  static const ::std::string _default_password_;
  ::std::string* realname_;
  static const ::std::string _default_realname_;
  bool createuser_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static LogInRequest* default_instance_;
};
// -------------------------------------------------------------------

class LogInResponse : public ::google::protobuf::Message {
 public:
  LogInResponse();
  virtual ~LogInResponse();
  
  LogInResponse(const LogInResponse& from);
  
  inline LogInResponse& operator=(const LogInResponse& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const LogInResponse& default_instance();
  
  void Swap(LogInResponse* other);
  
  // implements Message ----------------------------------------------
  
  LogInResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LogInResponse& from);
  void MergeFrom(const LogInResponse& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional int32 userId = 1;
  inline bool has_userid() const;
  inline void clear_userid();
  static const int kUserIdFieldNumber = 1;
  inline ::google::protobuf::int32 userid() const;
  inline void set_userid(::google::protobuf::int32 value);
  
  // optional string username = 2;
  inline bool has_username() const;
  inline void clear_username();
  static const int kUsernameFieldNumber = 2;
  inline const ::std::string& username() const;
  inline void set_username(const ::std::string& value);
  inline void set_username(const char* value);
  inline void set_username(const char* value, size_t size);
  inline ::std::string* mutable_username();
  
  // optional string realName = 3;
  inline bool has_realname() const;
  inline void clear_realname();
  static const int kRealNameFieldNumber = 3;
  inline const ::std::string& realname() const;
  inline void set_realname(const ::std::string& value);
  inline void set_realname(const char* value);
  inline void set_realname(const char* value, size_t size);
  inline ::std::string* mutable_realname();
  
  // optional .Error error = 100;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 100;
  inline const ::Error& error() const;
  inline ::Error* mutable_error();
  
  // @@protoc_insertion_point(class_scope:LogInResponse)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::int32 userid_;
  ::std::string* username_;
  static const ::std::string _default_username_;
  ::std::string* realname_;
  static const ::std::string _default_realname_;
  ::Error* error_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static LogInResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// LogInRequest

// optional string username = 1;
inline bool LogInRequest::has_username() const {
  return _has_bit(0);
}
inline void LogInRequest::clear_username() {
  if (username_ != &_default_username_) {
    username_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& LogInRequest::username() const {
  return *username_;
}
inline void LogInRequest::set_username(const ::std::string& value) {
  _set_bit(0);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void LogInRequest::set_username(const char* value) {
  _set_bit(0);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void LogInRequest::set_username(const char* value, size_t size) {
  _set_bit(0);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* LogInRequest::mutable_username() {
  _set_bit(0);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  return username_;
}

// optional string password = 2;
inline bool LogInRequest::has_password() const {
  return _has_bit(1);
}
inline void LogInRequest::clear_password() {
  if (password_ != &_default_password_) {
    password_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& LogInRequest::password() const {
  return *password_;
}
inline void LogInRequest::set_password(const ::std::string& value) {
  _set_bit(1);
  if (password_ == &_default_password_) {
    password_ = new ::std::string;
  }
  password_->assign(value);
}
inline void LogInRequest::set_password(const char* value) {
  _set_bit(1);
  if (password_ == &_default_password_) {
    password_ = new ::std::string;
  }
  password_->assign(value);
}
inline void LogInRequest::set_password(const char* value, size_t size) {
  _set_bit(1);
  if (password_ == &_default_password_) {
    password_ = new ::std::string;
  }
  password_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* LogInRequest::mutable_password() {
  _set_bit(1);
  if (password_ == &_default_password_) {
    password_ = new ::std::string;
  }
  return password_;
}

// optional string realName = 3;
inline bool LogInRequest::has_realname() const {
  return _has_bit(2);
}
inline void LogInRequest::clear_realname() {
  if (realname_ != &_default_realname_) {
    realname_->clear();
  }
  _clear_bit(2);
}
inline const ::std::string& LogInRequest::realname() const {
  return *realname_;
}
inline void LogInRequest::set_realname(const ::std::string& value) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void LogInRequest::set_realname(const char* value) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void LogInRequest::set_realname(const char* value, size_t size) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* LogInRequest::mutable_realname() {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  return realname_;
}

// optional bool createUser = 4 [default = false];
inline bool LogInRequest::has_createuser() const {
  return _has_bit(3);
}
inline void LogInRequest::clear_createuser() {
  createuser_ = false;
  _clear_bit(3);
}
inline bool LogInRequest::createuser() const {
  return createuser_;
}
inline void LogInRequest::set_createuser(bool value) {
  _set_bit(3);
  createuser_ = value;
}

// -------------------------------------------------------------------

// LogInResponse

// optional int32 userId = 1;
inline bool LogInResponse::has_userid() const {
  return _has_bit(0);
}
inline void LogInResponse::clear_userid() {
  userid_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 LogInResponse::userid() const {
  return userid_;
}
inline void LogInResponse::set_userid(::google::protobuf::int32 value) {
  _set_bit(0);
  userid_ = value;
}

// optional string username = 2;
inline bool LogInResponse::has_username() const {
  return _has_bit(1);
}
inline void LogInResponse::clear_username() {
  if (username_ != &_default_username_) {
    username_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& LogInResponse::username() const {
  return *username_;
}
inline void LogInResponse::set_username(const ::std::string& value) {
  _set_bit(1);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void LogInResponse::set_username(const char* value) {
  _set_bit(1);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void LogInResponse::set_username(const char* value, size_t size) {
  _set_bit(1);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* LogInResponse::mutable_username() {
  _set_bit(1);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  return username_;
}

// optional string realName = 3;
inline bool LogInResponse::has_realname() const {
  return _has_bit(2);
}
inline void LogInResponse::clear_realname() {
  if (realname_ != &_default_realname_) {
    realname_->clear();
  }
  _clear_bit(2);
}
inline const ::std::string& LogInResponse::realname() const {
  return *realname_;
}
inline void LogInResponse::set_realname(const ::std::string& value) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void LogInResponse::set_realname(const char* value) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void LogInResponse::set_realname(const char* value, size_t size) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* LogInResponse::mutable_realname() {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  return realname_;
}

// optional .Error error = 100;
inline bool LogInResponse::has_error() const {
  return _has_bit(3);
}
inline void LogInResponse::clear_error() {
  if (error_ != NULL) error_->::Error::Clear();
  _clear_bit(3);
}
inline const ::Error& LogInResponse::error() const {
  return error_ != NULL ? *error_ : *default_instance_->error_;
}
inline ::Error* LogInResponse::mutable_error() {
  _set_bit(3);
  if (error_ == NULL) error_ = new ::Error;
  return error_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_libnutrition_2fproto_2fservice_2flogin_5fmessages_2eproto__INCLUDED