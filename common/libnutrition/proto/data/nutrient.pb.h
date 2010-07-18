// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: libnutrition/proto/data/nutrient.proto

#ifndef PROTOBUF_libnutrition_2fproto_2fdata_2fnutrient_2eproto__INCLUDED
#define PROTOBUF_libnutrition_2fproto_2fdata_2fnutrient_2eproto__INCLUDED

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
void  protobuf_AddDesc_libnutrition_2fproto_2fdata_2fnutrient_2eproto();
void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2fnutrient_2eproto();
void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2fnutrient_2eproto();

class NutrientData;

enum NutrientData_Category {
  NutrientData_Category_Energy = 0,
  NutrientData_Category_Basic = 1,
  NutrientData_Category_Vitamin = 2,
  NutrientData_Category_Mineral = 3,
  NutrientData_Category_Additional = 4,
  NutrientData_Category_Hidden = 5
};
bool NutrientData_Category_IsValid(int value);
const NutrientData_Category NutrientData_Category_Category_MIN = NutrientData_Category_Energy;
const NutrientData_Category NutrientData_Category_Category_MAX = NutrientData_Category_Hidden;
const int NutrientData_Category_Category_ARRAYSIZE = NutrientData_Category_Category_MAX + 1;

const ::google::protobuf::EnumDescriptor* NutrientData_Category_descriptor();
inline const ::std::string& NutrientData_Category_Name(NutrientData_Category value) {
  return ::google::protobuf::internal::NameOfEnum(
    NutrientData_Category_descriptor(), value);
}
inline bool NutrientData_Category_Parse(
    const ::std::string& name, NutrientData_Category* value) {
  return ::google::protobuf::internal::ParseNamedEnum<NutrientData_Category>(
    NutrientData_Category_descriptor(), name, value);
}
// ===================================================================

class NutrientData : public ::google::protobuf::Message {
 public:
  NutrientData();
  virtual ~NutrientData();
  
  NutrientData(const NutrientData& from);
  
  inline NutrientData& operator=(const NutrientData& from) {
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
  static const NutrientData& default_instance();
  
  void Swap(NutrientData* other);
  
  // implements Message ----------------------------------------------
  
  NutrientData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const NutrientData& from);
  void MergeFrom(const NutrientData& from);
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
  
  typedef NutrientData_Category Category;
  static const Category Energy = NutrientData_Category_Energy;
  static const Category Basic = NutrientData_Category_Basic;
  static const Category Vitamin = NutrientData_Category_Vitamin;
  static const Category Mineral = NutrientData_Category_Mineral;
  static const Category Additional = NutrientData_Category_Additional;
  static const Category Hidden = NutrientData_Category_Hidden;
  static inline bool Category_IsValid(int value) {
    return NutrientData_Category_IsValid(value);
  }
  static const Category Category_MIN =
    NutrientData_Category_Category_MIN;
  static const Category Category_MAX =
    NutrientData_Category_Category_MAX;
  static const int Category_ARRAYSIZE =
    NutrientData_Category_Category_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Category_descriptor() {
    return NutrientData_Category_descriptor();
  }
  static inline const ::std::string& Category_Name(Category value) {
    return NutrientData_Category_Name(value);
  }
  static inline bool Category_Parse(const ::std::string& name,
      Category* value) {
    return NutrientData_Category_Parse(name, value);
  }
  
  // accessors -------------------------------------------------------
  
  // optional string id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline const ::std::string& id() const;
  inline void set_id(const ::std::string& value);
  inline void set_id(const char* value);
  inline void set_id(const char* value, size_t size);
  inline ::std::string* mutable_id();
  
  // optional string name = 2;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 2;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  
  // optional .NutrientData.Category category = 3 [default = Additional];
  inline bool has_category() const;
  inline void clear_category();
  static const int kCategoryFieldNumber = 3;
  inline ::NutrientData_Category category() const;
  inline void set_category(::NutrientData_Category value);
  
  // optional string standardUnitAbbreviation = 4;
  inline bool has_standardunitabbreviation() const;
  inline void clear_standardunitabbreviation();
  static const int kStandardUnitAbbreviationFieldNumber = 4;
  inline const ::std::string& standardunitabbreviation() const;
  inline void set_standardunitabbreviation(const ::std::string& value);
  inline void set_standardunitabbreviation(const char* value);
  inline void set_standardunitabbreviation(const char* value, size_t size);
  inline ::std::string* mutable_standardunitabbreviation();
  
  // optional double RDI = 5;
  inline bool has_rdi() const;
  inline void clear_rdi();
  static const int kRDIFieldNumber = 5;
  inline double rdi() const;
  inline void set_rdi(double value);
  
  // @@protoc_insertion_point(class_scope:NutrientData)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::std::string* id_;
  static const ::std::string _default_id_;
  ::std::string* name_;
  static const ::std::string _default_name_;
  int category_;
  ::std::string* standardunitabbreviation_;
  static const ::std::string _default_standardunitabbreviation_;
  double rdi_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fdata_2fnutrient_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2fnutrient_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2fnutrient_2eproto();
  
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
  static NutrientData* default_instance_;
};
// ===================================================================


// ===================================================================

// NutrientData

// optional string id = 1;
inline bool NutrientData::has_id() const {
  return _has_bit(0);
}
inline void NutrientData::clear_id() {
  if (id_ != &_default_id_) {
    id_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& NutrientData::id() const {
  return *id_;
}
inline void NutrientData::set_id(const ::std::string& value) {
  _set_bit(0);
  if (id_ == &_default_id_) {
    id_ = new ::std::string;
  }
  id_->assign(value);
}
inline void NutrientData::set_id(const char* value) {
  _set_bit(0);
  if (id_ == &_default_id_) {
    id_ = new ::std::string;
  }
  id_->assign(value);
}
inline void NutrientData::set_id(const char* value, size_t size) {
  _set_bit(0);
  if (id_ == &_default_id_) {
    id_ = new ::std::string;
  }
  id_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* NutrientData::mutable_id() {
  _set_bit(0);
  if (id_ == &_default_id_) {
    id_ = new ::std::string;
  }
  return id_;
}

// optional string name = 2;
inline bool NutrientData::has_name() const {
  return _has_bit(1);
}
inline void NutrientData::clear_name() {
  if (name_ != &_default_name_) {
    name_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& NutrientData::name() const {
  return *name_;
}
inline void NutrientData::set_name(const ::std::string& value) {
  _set_bit(1);
  if (name_ == &_default_name_) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void NutrientData::set_name(const char* value) {
  _set_bit(1);
  if (name_ == &_default_name_) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void NutrientData::set_name(const char* value, size_t size) {
  _set_bit(1);
  if (name_ == &_default_name_) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* NutrientData::mutable_name() {
  _set_bit(1);
  if (name_ == &_default_name_) {
    name_ = new ::std::string;
  }
  return name_;
}

// optional .NutrientData.Category category = 3 [default = Additional];
inline bool NutrientData::has_category() const {
  return _has_bit(2);
}
inline void NutrientData::clear_category() {
  category_ = 4;
  _clear_bit(2);
}
inline ::NutrientData_Category NutrientData::category() const {
  return static_cast< ::NutrientData_Category >(category_);
}
inline void NutrientData::set_category(::NutrientData_Category value) {
  GOOGLE_DCHECK(::NutrientData_Category_IsValid(value));
  _set_bit(2);
  category_ = value;
}

// optional string standardUnitAbbreviation = 4;
inline bool NutrientData::has_standardunitabbreviation() const {
  return _has_bit(3);
}
inline void NutrientData::clear_standardunitabbreviation() {
  if (standardunitabbreviation_ != &_default_standardunitabbreviation_) {
    standardunitabbreviation_->clear();
  }
  _clear_bit(3);
}
inline const ::std::string& NutrientData::standardunitabbreviation() const {
  return *standardunitabbreviation_;
}
inline void NutrientData::set_standardunitabbreviation(const ::std::string& value) {
  _set_bit(3);
  if (standardunitabbreviation_ == &_default_standardunitabbreviation_) {
    standardunitabbreviation_ = new ::std::string;
  }
  standardunitabbreviation_->assign(value);
}
inline void NutrientData::set_standardunitabbreviation(const char* value) {
  _set_bit(3);
  if (standardunitabbreviation_ == &_default_standardunitabbreviation_) {
    standardunitabbreviation_ = new ::std::string;
  }
  standardunitabbreviation_->assign(value);
}
inline void NutrientData::set_standardunitabbreviation(const char* value, size_t size) {
  _set_bit(3);
  if (standardunitabbreviation_ == &_default_standardunitabbreviation_) {
    standardunitabbreviation_ = new ::std::string;
  }
  standardunitabbreviation_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* NutrientData::mutable_standardunitabbreviation() {
  _set_bit(3);
  if (standardunitabbreviation_ == &_default_standardunitabbreviation_) {
    standardunitabbreviation_ = new ::std::string;
  }
  return standardunitabbreviation_;
}

// optional double RDI = 5;
inline bool NutrientData::has_rdi() const {
  return _has_bit(4);
}
inline void NutrientData::clear_rdi() {
  rdi_ = 0;
  _clear_bit(4);
}
inline double NutrientData::rdi() const {
  return rdi_;
}
inline void NutrientData::set_rdi(double value) {
  _set_bit(4);
  rdi_ = value;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NutrientData_Category>() {
  return ::NutrientData_Category_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_libnutrition_2fproto_2fdata_2fnutrient_2eproto__INCLUDED
