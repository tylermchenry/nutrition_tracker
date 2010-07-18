// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: libnutrition/proto/data/food_component.proto

#ifndef PROTOBUF_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto__INCLUDED
#define PROTOBUF_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto__INCLUDED

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
void  protobuf_AddDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();
void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();
void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();

class FoodComponentData;

enum FoodComponentData_FoodType {
  FoodComponentData_FoodType_SingleFood = 0,
  FoodComponentData_FoodType_CompositeFood = 1
};
bool FoodComponentData_FoodType_IsValid(int value);
const FoodComponentData_FoodType FoodComponentData_FoodType_FoodType_MIN = FoodComponentData_FoodType_SingleFood;
const FoodComponentData_FoodType FoodComponentData_FoodType_FoodType_MAX = FoodComponentData_FoodType_CompositeFood;
const int FoodComponentData_FoodType_FoodType_ARRAYSIZE = FoodComponentData_FoodType_FoodType_MAX + 1;

const ::google::protobuf::EnumDescriptor* FoodComponentData_FoodType_descriptor();
inline const ::std::string& FoodComponentData_FoodType_Name(FoodComponentData_FoodType value) {
  return ::google::protobuf::internal::NameOfEnum(
    FoodComponentData_FoodType_descriptor(), value);
}
inline bool FoodComponentData_FoodType_Parse(
    const ::std::string& name, FoodComponentData_FoodType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<FoodComponentData_FoodType>(
    FoodComponentData_FoodType_descriptor(), name, value);
}
// ===================================================================

class FoodComponentData : public ::google::protobuf::Message {
 public:
  FoodComponentData();
  virtual ~FoodComponentData();
  
  FoodComponentData(const FoodComponentData& from);
  
  inline FoodComponentData& operator=(const FoodComponentData& from) {
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
  static const FoodComponentData& default_instance();
  
  void Swap(FoodComponentData* other);
  
  // implements Message ----------------------------------------------
  
  FoodComponentData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FoodComponentData& from);
  void MergeFrom(const FoodComponentData& from);
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
  
  typedef FoodComponentData_FoodType FoodType;
  static const FoodType SingleFood = FoodComponentData_FoodType_SingleFood;
  static const FoodType CompositeFood = FoodComponentData_FoodType_CompositeFood;
  static inline bool FoodType_IsValid(int value) {
    return FoodComponentData_FoodType_IsValid(value);
  }
  static const FoodType FoodType_MIN =
    FoodComponentData_FoodType_FoodType_MIN;
  static const FoodType FoodType_MAX =
    FoodComponentData_FoodType_FoodType_MAX;
  static const int FoodType_ARRAYSIZE =
    FoodComponentData_FoodType_FoodType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  FoodType_descriptor() {
    return FoodComponentData_FoodType_descriptor();
  }
  static inline const ::std::string& FoodType_Name(FoodType value) {
    return FoodComponentData_FoodType_Name(value);
  }
  static inline bool FoodType_Parse(const ::std::string& name,
      FoodType* value) {
    return FoodComponentData_FoodType_Parse(name, value);
  }
  
  // accessors -------------------------------------------------------
  
  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);
  
  // optional int32 foodId = 2;
  inline bool has_foodid() const;
  inline void clear_foodid();
  static const int kFoodIdFieldNumber = 2;
  inline ::google::protobuf::int32 foodid() const;
  inline void set_foodid(::google::protobuf::int32 value);
  
  // optional .FoodComponentData.FoodType foodType = 3;
  inline bool has_foodtype() const;
  inline void clear_foodtype();
  static const int kFoodTypeFieldNumber = 3;
  inline ::FoodComponentData_FoodType foodtype() const;
  inline void set_foodtype(::FoodComponentData_FoodType value);
  
  // optional double amount = 4;
  inline bool has_amount() const;
  inline void clear_amount();
  static const int kAmountFieldNumber = 4;
  inline double amount() const;
  inline void set_amount(double value);
  
  // optional string unitAbbreviation = 5 [default = "g"];
  inline bool has_unitabbreviation() const;
  inline void clear_unitabbreviation();
  static const int kUnitAbbreviationFieldNumber = 5;
  inline const ::std::string& unitabbreviation() const;
  inline void set_unitabbreviation(const ::std::string& value);
  inline void set_unitabbreviation(const char* value);
  inline void set_unitabbreviation(const char* value, size_t size);
  inline ::std::string* mutable_unitabbreviation();
  
  // optional bool includesRefuse = 6;
  inline bool has_includesrefuse() const;
  inline void clear_includesrefuse();
  static const int kIncludesRefuseFieldNumber = 6;
  inline bool includesrefuse() const;
  inline void set_includesrefuse(bool value);
  
  // optional int32 order = 7;
  inline bool has_order() const;
  inline void clear_order();
  static const int kOrderFieldNumber = 7;
  inline ::google::protobuf::int32 order() const;
  inline void set_order(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:FoodComponentData)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 foodid_;
  int foodtype_;
  double amount_;
  ::std::string* unitabbreviation_;
  static const ::std::string _default_unitabbreviation_;
  bool includesrefuse_;
  ::google::protobuf::int32 order_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];
  
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
  static FoodComponentData* default_instance_;
};
// ===================================================================


// ===================================================================

// FoodComponentData

// optional int32 id = 1;
inline bool FoodComponentData::has_id() const {
  return _has_bit(0);
}
inline void FoodComponentData::clear_id() {
  id_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 FoodComponentData::id() const {
  return id_;
}
inline void FoodComponentData::set_id(::google::protobuf::int32 value) {
  _set_bit(0);
  id_ = value;
}

// optional int32 foodId = 2;
inline bool FoodComponentData::has_foodid() const {
  return _has_bit(1);
}
inline void FoodComponentData::clear_foodid() {
  foodid_ = 0;
  _clear_bit(1);
}
inline ::google::protobuf::int32 FoodComponentData::foodid() const {
  return foodid_;
}
inline void FoodComponentData::set_foodid(::google::protobuf::int32 value) {
  _set_bit(1);
  foodid_ = value;
}

// optional .FoodComponentData.FoodType foodType = 3;
inline bool FoodComponentData::has_foodtype() const {
  return _has_bit(2);
}
inline void FoodComponentData::clear_foodtype() {
  foodtype_ = 0;
  _clear_bit(2);
}
inline ::FoodComponentData_FoodType FoodComponentData::foodtype() const {
  return static_cast< ::FoodComponentData_FoodType >(foodtype_);
}
inline void FoodComponentData::set_foodtype(::FoodComponentData_FoodType value) {
  GOOGLE_DCHECK(::FoodComponentData_FoodType_IsValid(value));
  _set_bit(2);
  foodtype_ = value;
}

// optional double amount = 4;
inline bool FoodComponentData::has_amount() const {
  return _has_bit(3);
}
inline void FoodComponentData::clear_amount() {
  amount_ = 0;
  _clear_bit(3);
}
inline double FoodComponentData::amount() const {
  return amount_;
}
inline void FoodComponentData::set_amount(double value) {
  _set_bit(3);
  amount_ = value;
}

// optional string unitAbbreviation = 5 [default = "g"];
inline bool FoodComponentData::has_unitabbreviation() const {
  return _has_bit(4);
}
inline void FoodComponentData::clear_unitabbreviation() {
  if (unitabbreviation_ != &_default_unitabbreviation_) {
    unitabbreviation_->assign(_default_unitabbreviation_);
  }
  _clear_bit(4);
}
inline const ::std::string& FoodComponentData::unitabbreviation() const {
  return *unitabbreviation_;
}
inline void FoodComponentData::set_unitabbreviation(const ::std::string& value) {
  _set_bit(4);
  if (unitabbreviation_ == &_default_unitabbreviation_) {
    unitabbreviation_ = new ::std::string;
  }
  unitabbreviation_->assign(value);
}
inline void FoodComponentData::set_unitabbreviation(const char* value) {
  _set_bit(4);
  if (unitabbreviation_ == &_default_unitabbreviation_) {
    unitabbreviation_ = new ::std::string;
  }
  unitabbreviation_->assign(value);
}
inline void FoodComponentData::set_unitabbreviation(const char* value, size_t size) {
  _set_bit(4);
  if (unitabbreviation_ == &_default_unitabbreviation_) {
    unitabbreviation_ = new ::std::string;
  }
  unitabbreviation_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FoodComponentData::mutable_unitabbreviation() {
  _set_bit(4);
  if (unitabbreviation_ == &_default_unitabbreviation_) {
    unitabbreviation_ = new ::std::string(_default_unitabbreviation_);
  }
  return unitabbreviation_;
}

// optional bool includesRefuse = 6;
inline bool FoodComponentData::has_includesrefuse() const {
  return _has_bit(5);
}
inline void FoodComponentData::clear_includesrefuse() {
  includesrefuse_ = false;
  _clear_bit(5);
}
inline bool FoodComponentData::includesrefuse() const {
  return includesrefuse_;
}
inline void FoodComponentData::set_includesrefuse(bool value) {
  _set_bit(5);
  includesrefuse_ = value;
}

// optional int32 order = 7;
inline bool FoodComponentData::has_order() const {
  return _has_bit(6);
}
inline void FoodComponentData::clear_order() {
  order_ = 0;
  _clear_bit(6);
}
inline ::google::protobuf::int32 FoodComponentData::order() const {
  return order_;
}
inline void FoodComponentData::set_order(::google::protobuf::int32 value) {
  _set_bit(6);
  order_ = value;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::FoodComponentData_FoodType>() {
  return ::FoodComponentData_FoodType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto__INCLUDED