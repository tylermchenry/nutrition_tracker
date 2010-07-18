// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: libnutrition/proto/data/template.proto

#ifndef PROTOBUF_libnutrition_2fproto_2fdata_2ftemplate_2eproto__INCLUDED
#define PROTOBUF_libnutrition_2fproto_2fdata_2ftemplate_2eproto__INCLUDED

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
#include "libnutrition/proto/data/food_component.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_libnutrition_2fproto_2fdata_2ftemplate_2eproto();
void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2ftemplate_2eproto();
void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2ftemplate_2eproto();

class TemplateData;

// ===================================================================

class TemplateData : public ::google::protobuf::Message {
 public:
  TemplateData();
  virtual ~TemplateData();
  
  TemplateData(const TemplateData& from);
  
  inline TemplateData& operator=(const TemplateData& from) {
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
  static const TemplateData& default_instance();
  
  void Swap(TemplateData* other);
  
  // implements Message ----------------------------------------------
  
  TemplateData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TemplateData& from);
  void MergeFrom(const TemplateData& from);
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
  
  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);
  
  // optional string name = 2;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 2;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  
  // optional int32 ownerId = 3;
  inline bool has_ownerid() const;
  inline void clear_ownerid();
  static const int kOwnerIdFieldNumber = 3;
  inline ::google::protobuf::int32 ownerid() const;
  inline void set_ownerid(::google::protobuf::int32 value);
  
  // repeated .FoodComponentData components = 4;
  inline int components_size() const;
  inline void clear_components();
  static const int kComponentsFieldNumber = 4;
  inline const ::FoodComponentData& components(int index) const;
  inline ::FoodComponentData* mutable_components(int index);
  inline ::FoodComponentData* add_components();
  inline const ::google::protobuf::RepeatedPtrField< ::FoodComponentData >&
      components() const;
  inline ::google::protobuf::RepeatedPtrField< ::FoodComponentData >*
      mutable_components();
  
  // @@protoc_insertion_point(class_scope:TemplateData)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::int32 id_;
  ::std::string* name_;
  static const ::std::string _default_name_;
  ::google::protobuf::int32 ownerid_;
  ::google::protobuf::RepeatedPtrField< ::FoodComponentData > components_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fdata_2ftemplate_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2ftemplate_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2ftemplate_2eproto();
  
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
  static TemplateData* default_instance_;
};
// ===================================================================


// ===================================================================

// TemplateData

// optional int32 id = 1;
inline bool TemplateData::has_id() const {
  return _has_bit(0);
}
inline void TemplateData::clear_id() {
  id_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 TemplateData::id() const {
  return id_;
}
inline void TemplateData::set_id(::google::protobuf::int32 value) {
  _set_bit(0);
  id_ = value;
}

// optional string name = 2;
inline bool TemplateData::has_name() const {
  return _has_bit(1);
}
inline void TemplateData::clear_name() {
  if (name_ != &_default_name_) {
    name_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& TemplateData::name() const {
  return *name_;
}
inline void TemplateData::set_name(const ::std::string& value) {
  _set_bit(1);
  if (name_ == &_default_name_) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void TemplateData::set_name(const char* value) {
  _set_bit(1);
  if (name_ == &_default_name_) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void TemplateData::set_name(const char* value, size_t size) {
  _set_bit(1);
  if (name_ == &_default_name_) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* TemplateData::mutable_name() {
  _set_bit(1);
  if (name_ == &_default_name_) {
    name_ = new ::std::string;
  }
  return name_;
}

// optional int32 ownerId = 3;
inline bool TemplateData::has_ownerid() const {
  return _has_bit(2);
}
inline void TemplateData::clear_ownerid() {
  ownerid_ = 0;
  _clear_bit(2);
}
inline ::google::protobuf::int32 TemplateData::ownerid() const {
  return ownerid_;
}
inline void TemplateData::set_ownerid(::google::protobuf::int32 value) {
  _set_bit(2);
  ownerid_ = value;
}

// repeated .FoodComponentData components = 4;
inline int TemplateData::components_size() const {
  return components_.size();
}
inline void TemplateData::clear_components() {
  components_.Clear();
}
inline const ::FoodComponentData& TemplateData::components(int index) const {
  return components_.Get(index);
}
inline ::FoodComponentData* TemplateData::mutable_components(int index) {
  return components_.Mutable(index);
}
inline ::FoodComponentData* TemplateData::add_components() {
  return components_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::FoodComponentData >&
TemplateData::components() const {
  return components_;
}
inline ::google::protobuf::RepeatedPtrField< ::FoodComponentData >*
TemplateData::mutable_components() {
  return &components_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_libnutrition_2fproto_2fdata_2ftemplate_2eproto__INCLUDED
