// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: libnutrition/proto/service/nutrient_messages.proto

#ifndef PROTOBUF_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto__INCLUDED
#define PROTOBUF_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto__INCLUDED

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
#include "libnutrition/proto/data/nutrient.pb.h"
#include "libnutrition/proto/service/error.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();

class NutrientLoadRequest;
class NutrientLoadResponse;

// ===================================================================

class NutrientLoadRequest : public ::google::protobuf::Message {
 public:
  NutrientLoadRequest();
  virtual ~NutrientLoadRequest();
  
  NutrientLoadRequest(const NutrientLoadRequest& from);
  
  inline NutrientLoadRequest& operator=(const NutrientLoadRequest& from) {
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
  static const NutrientLoadRequest& default_instance();
  
  void Swap(NutrientLoadRequest* other);
  
  // implements Message ----------------------------------------------
  
  NutrientLoadRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const NutrientLoadRequest& from);
  void MergeFrom(const NutrientLoadRequest& from);
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
  
  // optional bool omit = 1 [default = false];
  inline bool has_omit() const;
  inline void clear_omit();
  static const int kOmitFieldNumber = 1;
  inline bool omit() const;
  inline void set_omit(bool value);
  
  // optional bool all = 2 [default = false];
  inline bool has_all() const;
  inline void clear_all();
  static const int kAllFieldNumber = 2;
  inline bool all() const;
  inline void set_all(bool value);
  
  // repeated string requestedIds = 3;
  inline int requestedids_size() const;
  inline void clear_requestedids();
  static const int kRequestedIdsFieldNumber = 3;
  inline const ::std::string& requestedids(int index) const;
  inline ::std::string* mutable_requestedids(int index);
  inline void set_requestedids(int index, const ::std::string& value);
  inline void set_requestedids(int index, const char* value);
  inline void set_requestedids(int index, const char* value, size_t size);
  inline ::std::string* add_requestedids();
  inline void add_requestedids(const ::std::string& value);
  inline void add_requestedids(const char* value);
  inline void add_requestedids(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& requestedids() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_requestedids();
  
  // repeated string requestedNames = 4;
  inline int requestednames_size() const;
  inline void clear_requestednames();
  static const int kRequestedNamesFieldNumber = 4;
  inline const ::std::string& requestednames(int index) const;
  inline ::std::string* mutable_requestednames(int index);
  inline void set_requestednames(int index, const ::std::string& value);
  inline void set_requestednames(int index, const char* value);
  inline void set_requestednames(int index, const char* value, size_t size);
  inline ::std::string* add_requestednames();
  inline void add_requestednames(const ::std::string& value);
  inline void add_requestednames(const char* value);
  inline void add_requestednames(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& requestednames() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_requestednames();
  
  // @@protoc_insertion_point(class_scope:NutrientLoadRequest)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  bool omit_;
  bool all_;
  ::google::protobuf::RepeatedPtrField< ::std::string> requestedids_;
  ::google::protobuf::RepeatedPtrField< ::std::string> requestednames_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
  
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
  static NutrientLoadRequest* default_instance_;
};
// -------------------------------------------------------------------

class NutrientLoadResponse : public ::google::protobuf::Message {
 public:
  NutrientLoadResponse();
  virtual ~NutrientLoadResponse();
  
  NutrientLoadResponse(const NutrientLoadResponse& from);
  
  inline NutrientLoadResponse& operator=(const NutrientLoadResponse& from) {
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
  static const NutrientLoadResponse& default_instance();
  
  void Swap(NutrientLoadResponse* other);
  
  // implements Message ----------------------------------------------
  
  NutrientLoadResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const NutrientLoadResponse& from);
  void MergeFrom(const NutrientLoadResponse& from);
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
  
  // repeated .NutrientData nutrients = 1;
  inline int nutrients_size() const;
  inline void clear_nutrients();
  static const int kNutrientsFieldNumber = 1;
  inline const ::NutrientData& nutrients(int index) const;
  inline ::NutrientData* mutable_nutrients(int index);
  inline ::NutrientData* add_nutrients();
  inline const ::google::protobuf::RepeatedPtrField< ::NutrientData >&
      nutrients() const;
  inline ::google::protobuf::RepeatedPtrField< ::NutrientData >*
      mutable_nutrients();
  
  // optional .Error error = 100;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 100;
  inline const ::Error& error() const;
  inline ::Error* mutable_error();
  
  // @@protoc_insertion_point(class_scope:NutrientLoadResponse)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::NutrientData > nutrients_;
  ::Error* error_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
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
  static NutrientLoadResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// NutrientLoadRequest

// optional bool omit = 1 [default = false];
inline bool NutrientLoadRequest::has_omit() const {
  return _has_bit(0);
}
inline void NutrientLoadRequest::clear_omit() {
  omit_ = false;
  _clear_bit(0);
}
inline bool NutrientLoadRequest::omit() const {
  return omit_;
}
inline void NutrientLoadRequest::set_omit(bool value) {
  _set_bit(0);
  omit_ = value;
}

// optional bool all = 2 [default = false];
inline bool NutrientLoadRequest::has_all() const {
  return _has_bit(1);
}
inline void NutrientLoadRequest::clear_all() {
  all_ = false;
  _clear_bit(1);
}
inline bool NutrientLoadRequest::all() const {
  return all_;
}
inline void NutrientLoadRequest::set_all(bool value) {
  _set_bit(1);
  all_ = value;
}

// repeated string requestedIds = 3;
inline int NutrientLoadRequest::requestedids_size() const {
  return requestedids_.size();
}
inline void NutrientLoadRequest::clear_requestedids() {
  requestedids_.Clear();
}
inline const ::std::string& NutrientLoadRequest::requestedids(int index) const {
  return requestedids_.Get(index);
}
inline ::std::string* NutrientLoadRequest::mutable_requestedids(int index) {
  return requestedids_.Mutable(index);
}
inline void NutrientLoadRequest::set_requestedids(int index, const ::std::string& value) {
  requestedids_.Mutable(index)->assign(value);
}
inline void NutrientLoadRequest::set_requestedids(int index, const char* value) {
  requestedids_.Mutable(index)->assign(value);
}
inline void NutrientLoadRequest::set_requestedids(int index, const char* value, size_t size) {
  requestedids_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* NutrientLoadRequest::add_requestedids() {
  return requestedids_.Add();
}
inline void NutrientLoadRequest::add_requestedids(const ::std::string& value) {
  requestedids_.Add()->assign(value);
}
inline void NutrientLoadRequest::add_requestedids(const char* value) {
  requestedids_.Add()->assign(value);
}
inline void NutrientLoadRequest::add_requestedids(const char* value, size_t size) {
  requestedids_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
NutrientLoadRequest::requestedids() const {
  return requestedids_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
NutrientLoadRequest::mutable_requestedids() {
  return &requestedids_;
}

// repeated string requestedNames = 4;
inline int NutrientLoadRequest::requestednames_size() const {
  return requestednames_.size();
}
inline void NutrientLoadRequest::clear_requestednames() {
  requestednames_.Clear();
}
inline const ::std::string& NutrientLoadRequest::requestednames(int index) const {
  return requestednames_.Get(index);
}
inline ::std::string* NutrientLoadRequest::mutable_requestednames(int index) {
  return requestednames_.Mutable(index);
}
inline void NutrientLoadRequest::set_requestednames(int index, const ::std::string& value) {
  requestednames_.Mutable(index)->assign(value);
}
inline void NutrientLoadRequest::set_requestednames(int index, const char* value) {
  requestednames_.Mutable(index)->assign(value);
}
inline void NutrientLoadRequest::set_requestednames(int index, const char* value, size_t size) {
  requestednames_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* NutrientLoadRequest::add_requestednames() {
  return requestednames_.Add();
}
inline void NutrientLoadRequest::add_requestednames(const ::std::string& value) {
  requestednames_.Add()->assign(value);
}
inline void NutrientLoadRequest::add_requestednames(const char* value) {
  requestednames_.Add()->assign(value);
}
inline void NutrientLoadRequest::add_requestednames(const char* value, size_t size) {
  requestednames_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
NutrientLoadRequest::requestednames() const {
  return requestednames_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
NutrientLoadRequest::mutable_requestednames() {
  return &requestednames_;
}

// -------------------------------------------------------------------

// NutrientLoadResponse

// repeated .NutrientData nutrients = 1;
inline int NutrientLoadResponse::nutrients_size() const {
  return nutrients_.size();
}
inline void NutrientLoadResponse::clear_nutrients() {
  nutrients_.Clear();
}
inline const ::NutrientData& NutrientLoadResponse::nutrients(int index) const {
  return nutrients_.Get(index);
}
inline ::NutrientData* NutrientLoadResponse::mutable_nutrients(int index) {
  return nutrients_.Mutable(index);
}
inline ::NutrientData* NutrientLoadResponse::add_nutrients() {
  return nutrients_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::NutrientData >&
NutrientLoadResponse::nutrients() const {
  return nutrients_;
}
inline ::google::protobuf::RepeatedPtrField< ::NutrientData >*
NutrientLoadResponse::mutable_nutrients() {
  return &nutrients_;
}

// optional .Error error = 100;
inline bool NutrientLoadResponse::has_error() const {
  return _has_bit(1);
}
inline void NutrientLoadResponse::clear_error() {
  if (error_ != NULL) error_->::Error::Clear();
  _clear_bit(1);
}
inline const ::Error& NutrientLoadResponse::error() const {
  return error_ != NULL ? *error_ : *default_instance_->error_;
}
inline ::Error* NutrientLoadResponse::mutable_error() {
  _set_bit(1);
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

#endif  // PROTOBUF_libnutrition_2fproto_2fservice_2fnutrient_5fmessages_2eproto__INCLUDED
