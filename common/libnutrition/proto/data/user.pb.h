// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: libnutrition/proto/data/user.proto

#ifndef PROTOBUF_libnutrition_2fproto_2fdata_2fuser_2eproto__INCLUDED
#define PROTOBUF_libnutrition_2fproto_2fdata_2fuser_2eproto__INCLUDED

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
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_libnutrition_2fproto_2fdata_2fuser_2eproto();
void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2fuser_2eproto();
void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2fuser_2eproto();

class UserData;

// ===================================================================

class UserData : public ::google::protobuf::Message {
 public:
  UserData();
  virtual ~UserData();
  
  UserData(const UserData& from);
  
  inline UserData& operator=(const UserData& from) {
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
  static const UserData& default_instance();
  
  void Swap(UserData* other);
  
  // implements Message ----------------------------------------------
  
  UserData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const UserData& from);
  void MergeFrom(const UserData& from);
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
  
  // optional string password_SHA1_hex = 4;
  inline bool has_password_sha1_hex() const;
  inline void clear_password_sha1_hex();
  static const int kPasswordSHA1HexFieldNumber = 4;
  inline const ::std::string& password_sha1_hex() const;
  inline void set_password_sha1_hex(const ::std::string& value);
  inline void set_password_sha1_hex(const char* value);
  inline void set_password_sha1_hex(const char* value, size_t size);
  inline ::std::string* mutable_password_sha1_hex();
  
  // optional string password_plain = 5;
  inline bool has_password_plain() const;
  inline void clear_password_plain();
  static const int kPasswordPlainFieldNumber = 5;
  inline const ::std::string& password_plain() const;
  inline void set_password_plain(const ::std::string& value);
  inline void set_password_plain(const char* value);
  inline void set_password_plain(const char* value, size_t size);
  inline ::std::string* mutable_password_plain();
  
  // @@protoc_insertion_point(class_scope:UserData)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::int32 userid_;
  ::std::string* username_;
  static const ::std::string _default_username_;
  ::std::string* realname_;
  static const ::std::string _default_realname_;
  ::std::string* password_sha1_hex_;
  static const ::std::string _default_password_sha1_hex_;
  ::std::string* password_plain_;
  static const ::std::string _default_password_plain_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fdata_2fuser_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2fuser_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2fuser_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
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
  static UserData* default_instance_;
};
// ===================================================================


// ===================================================================

// UserData

// optional int32 userId = 1;
inline bool UserData::has_userid() const {
  return _has_bit(0);
}
inline void UserData::clear_userid() {
  userid_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 UserData::userid() const {
  return userid_;
}
inline void UserData::set_userid(::google::protobuf::int32 value) {
  _set_bit(0);
  userid_ = value;
}

// optional string username = 2;
inline bool UserData::has_username() const {
  return _has_bit(1);
}
inline void UserData::clear_username() {
  if (username_ != &_default_username_) {
    username_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& UserData::username() const {
  return *username_;
}
inline void UserData::set_username(const ::std::string& value) {
  _set_bit(1);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void UserData::set_username(const char* value) {
  _set_bit(1);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void UserData::set_username(const char* value, size_t size) {
  _set_bit(1);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* UserData::mutable_username() {
  _set_bit(1);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  return username_;
}

// optional string realName = 3;
inline bool UserData::has_realname() const {
  return _has_bit(2);
}
inline void UserData::clear_realname() {
  if (realname_ != &_default_realname_) {
    realname_->clear();
  }
  _clear_bit(2);
}
inline const ::std::string& UserData::realname() const {
  return *realname_;
}
inline void UserData::set_realname(const ::std::string& value) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void UserData::set_realname(const char* value) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void UserData::set_realname(const char* value, size_t size) {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  realname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* UserData::mutable_realname() {
  _set_bit(2);
  if (realname_ == &_default_realname_) {
    realname_ = new ::std::string;
  }
  return realname_;
}

// optional string password_SHA1_hex = 4;
inline bool UserData::has_password_sha1_hex() const {
  return _has_bit(3);
}
inline void UserData::clear_password_sha1_hex() {
  if (password_sha1_hex_ != &_default_password_sha1_hex_) {
    password_sha1_hex_->clear();
  }
  _clear_bit(3);
}
inline const ::std::string& UserData::password_sha1_hex() const {
  return *password_sha1_hex_;
}
inline void UserData::set_password_sha1_hex(const ::std::string& value) {
  _set_bit(3);
  if (password_sha1_hex_ == &_default_password_sha1_hex_) {
    password_sha1_hex_ = new ::std::string;
  }
  password_sha1_hex_->assign(value);
}
inline void UserData::set_password_sha1_hex(const char* value) {
  _set_bit(3);
  if (password_sha1_hex_ == &_default_password_sha1_hex_) {
    password_sha1_hex_ = new ::std::string;
  }
  password_sha1_hex_->assign(value);
}
inline void UserData::set_password_sha1_hex(const char* value, size_t size) {
  _set_bit(3);
  if (password_sha1_hex_ == &_default_password_sha1_hex_) {
    password_sha1_hex_ = new ::std::string;
  }
  password_sha1_hex_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* UserData::mutable_password_sha1_hex() {
  _set_bit(3);
  if (password_sha1_hex_ == &_default_password_sha1_hex_) {
    password_sha1_hex_ = new ::std::string;
  }
  return password_sha1_hex_;
}

// optional string password_plain = 5;
inline bool UserData::has_password_plain() const {
  return _has_bit(4);
}
inline void UserData::clear_password_plain() {
  if (password_plain_ != &_default_password_plain_) {
    password_plain_->clear();
  }
  _clear_bit(4);
}
inline const ::std::string& UserData::password_plain() const {
  return *password_plain_;
}
inline void UserData::set_password_plain(const ::std::string& value) {
  _set_bit(4);
  if (password_plain_ == &_default_password_plain_) {
    password_plain_ = new ::std::string;
  }
  password_plain_->assign(value);
}
inline void UserData::set_password_plain(const char* value) {
  _set_bit(4);
  if (password_plain_ == &_default_password_plain_) {
    password_plain_ = new ::std::string;
  }
  password_plain_->assign(value);
}
inline void UserData::set_password_plain(const char* value, size_t size) {
  _set_bit(4);
  if (password_plain_ == &_default_password_plain_) {
    password_plain_ = new ::std::string;
  }
  password_plain_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* UserData::mutable_password_plain() {
  _set_bit(4);
  if (password_plain_ == &_default_password_plain_) {
    password_plain_ = new ::std::string;
  }
  return password_plain_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_libnutrition_2fproto_2fdata_2fuser_2eproto__INCLUDED