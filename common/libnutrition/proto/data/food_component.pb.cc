// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "libnutrition/proto/data/food_component.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* FoodComponentData_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  FoodComponentData_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* FoodComponentData_FoodType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto() {
  protobuf_AddDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "libnutrition/proto/data/food_component.proto");
  GOOGLE_CHECK(file != NULL);
  FoodComponentData_descriptor_ = file->message_type(0);
  static const int FoodComponentData_offsets_[7] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, foodid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, foodtype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, amount_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, unitabbreviation_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, includesrefuse_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, order_),
  };
  FoodComponentData_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      FoodComponentData_descriptor_,
      FoodComponentData::default_instance_,
      FoodComponentData_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FoodComponentData, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(FoodComponentData));
  FoodComponentData_FoodType_descriptor_ = FoodComponentData_descriptor_->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    FoodComponentData_descriptor_, &FoodComponentData::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto() {
  delete FoodComponentData::default_instance_;
  delete FoodComponentData_reflection_;
}

void protobuf_AddDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n,libnutrition/proto/data/food_component"
    ".proto\"\341\001\n\021FoodComponentData\022\n\n\002id\030\001 \001(\005"
    "\022\016\n\006foodId\030\002 \001(\005\022-\n\010foodType\030\003 \001(\0162\033.Foo"
    "dComponentData.FoodType\022\016\n\006amount\030\004 \001(\001\022"
    "\033\n\020unitAbbreviation\030\005 \001(\t:\001g\022\026\n\016includes"
    "Refuse\030\006 \001(\010\022\r\n\005order\030\007 \001(\005\"-\n\010FoodType\022"
    "\016\n\nSingleFood\020\000\022\021\n\rCompositeFood\020\001", 274);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "libnutrition/proto/data/food_component.proto", &protobuf_RegisterTypes);
  FoodComponentData::default_instance_ = new FoodComponentData();
  FoodComponentData::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto {
  StaticDescriptorInitializer_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto() {
    protobuf_AddDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();
  }
} static_descriptor_initializer_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto_;


// ===================================================================

const ::google::protobuf::EnumDescriptor* FoodComponentData_FoodType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return FoodComponentData_FoodType_descriptor_;
}
bool FoodComponentData_FoodType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const FoodComponentData_FoodType FoodComponentData::SingleFood;
const FoodComponentData_FoodType FoodComponentData::CompositeFood;
const FoodComponentData_FoodType FoodComponentData::FoodType_MIN;
const FoodComponentData_FoodType FoodComponentData::FoodType_MAX;
const int FoodComponentData::FoodType_ARRAYSIZE;
#endif  // _MSC_VER
const ::std::string FoodComponentData::_default_unitabbreviation_("g");
#ifndef _MSC_VER
const int FoodComponentData::kIdFieldNumber;
const int FoodComponentData::kFoodIdFieldNumber;
const int FoodComponentData::kFoodTypeFieldNumber;
const int FoodComponentData::kAmountFieldNumber;
const int FoodComponentData::kUnitAbbreviationFieldNumber;
const int FoodComponentData::kIncludesRefuseFieldNumber;
const int FoodComponentData::kOrderFieldNumber;
#endif  // !_MSC_VER

FoodComponentData::FoodComponentData()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void FoodComponentData::InitAsDefaultInstance() {
}

FoodComponentData::FoodComponentData(const FoodComponentData& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void FoodComponentData::SharedCtor() {
  _cached_size_ = 0;
  id_ = 0;
  foodid_ = 0;
  foodtype_ = 0;
  amount_ = 0;
  unitabbreviation_ = const_cast< ::std::string*>(&_default_unitabbreviation_);
  includesrefuse_ = false;
  order_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

FoodComponentData::~FoodComponentData() {
  SharedDtor();
}

void FoodComponentData::SharedDtor() {
  if (unitabbreviation_ != &_default_unitabbreviation_) {
    delete unitabbreviation_;
  }
  if (this != default_instance_) {
  }
}

void FoodComponentData::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* FoodComponentData::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return FoodComponentData_descriptor_;
}

const FoodComponentData& FoodComponentData::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_libnutrition_2fproto_2fdata_2ffood_5fcomponent_2eproto();  return *default_instance_;
}

FoodComponentData* FoodComponentData::default_instance_ = NULL;

FoodComponentData* FoodComponentData::New() const {
  return new FoodComponentData;
}

void FoodComponentData::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    id_ = 0;
    foodid_ = 0;
    foodtype_ = 0;
    amount_ = 0;
    if (_has_bit(4)) {
      if (unitabbreviation_ != &_default_unitabbreviation_) {
        unitabbreviation_->assign(_default_unitabbreviation_);
      }
    }
    includesrefuse_ = false;
    order_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool FoodComponentData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &id_)));
          _set_bit(0);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_foodId;
        break;
      }
      
      // optional int32 foodId = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_foodId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &foodid_)));
          _set_bit(1);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_foodType;
        break;
      }
      
      // optional .FoodComponentData.FoodType foodType = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_foodType:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::FoodComponentData_FoodType_IsValid(value)) {
            set_foodtype(static_cast< ::FoodComponentData_FoodType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(3, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(33)) goto parse_amount;
        break;
      }
      
      // optional double amount = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED64) {
         parse_amount:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &amount_)));
          _set_bit(3);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_unitAbbreviation;
        break;
      }
      
      // optional string unitAbbreviation = 5 [default = "g"];
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_unitAbbreviation:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_unitabbreviation()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->unitabbreviation().data(), this->unitabbreviation().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_includesRefuse;
        break;
      }
      
      // optional bool includesRefuse = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_includesRefuse:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &includesrefuse_)));
          _set_bit(5);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_order;
        break;
      }
      
      // optional int32 order = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_order:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &order_)));
          _set_bit(6);
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

void FoodComponentData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 id = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->id(), output);
  }
  
  // optional int32 foodId = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->foodid(), output);
  }
  
  // optional .FoodComponentData.FoodType foodType = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      3, this->foodtype(), output);
  }
  
  // optional double amount = 4;
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(4, this->amount(), output);
  }
  
  // optional string unitAbbreviation = 5 [default = "g"];
  if (_has_bit(4)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->unitabbreviation().data(), this->unitabbreviation().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->unitabbreviation(), output);
  }
  
  // optional bool includesRefuse = 6;
  if (_has_bit(5)) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(6, this->includesrefuse(), output);
  }
  
  // optional int32 order = 7;
  if (_has_bit(6)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(7, this->order(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* FoodComponentData::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 id = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->id(), target);
  }
  
  // optional int32 foodId = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->foodid(), target);
  }
  
  // optional .FoodComponentData.FoodType foodType = 3;
  if (_has_bit(2)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      3, this->foodtype(), target);
  }
  
  // optional double amount = 4;
  if (_has_bit(3)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(4, this->amount(), target);
  }
  
  // optional string unitAbbreviation = 5 [default = "g"];
  if (_has_bit(4)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->unitabbreviation().data(), this->unitabbreviation().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->unitabbreviation(), target);
  }
  
  // optional bool includesRefuse = 6;
  if (_has_bit(5)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(6, this->includesrefuse(), target);
  }
  
  // optional int32 order = 7;
  if (_has_bit(6)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(7, this->order(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int FoodComponentData::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->id());
    }
    
    // optional int32 foodId = 2;
    if (has_foodid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->foodid());
    }
    
    // optional .FoodComponentData.FoodType foodType = 3;
    if (has_foodtype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->foodtype());
    }
    
    // optional double amount = 4;
    if (has_amount()) {
      total_size += 1 + 8;
    }
    
    // optional string unitAbbreviation = 5 [default = "g"];
    if (has_unitabbreviation()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->unitabbreviation());
    }
    
    // optional bool includesRefuse = 6;
    if (has_includesrefuse()) {
      total_size += 1 + 1;
    }
    
    // optional int32 order = 7;
    if (has_order()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->order());
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

void FoodComponentData::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const FoodComponentData* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const FoodComponentData*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void FoodComponentData::MergeFrom(const FoodComponentData& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_id(from.id());
    }
    if (from._has_bit(1)) {
      set_foodid(from.foodid());
    }
    if (from._has_bit(2)) {
      set_foodtype(from.foodtype());
    }
    if (from._has_bit(3)) {
      set_amount(from.amount());
    }
    if (from._has_bit(4)) {
      set_unitabbreviation(from.unitabbreviation());
    }
    if (from._has_bit(5)) {
      set_includesrefuse(from.includesrefuse());
    }
    if (from._has_bit(6)) {
      set_order(from.order());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void FoodComponentData::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void FoodComponentData::CopyFrom(const FoodComponentData& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FoodComponentData::IsInitialized() const {
  
  return true;
}

void FoodComponentData::Swap(FoodComponentData* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(foodid_, other->foodid_);
    std::swap(foodtype_, other->foodtype_);
    std::swap(amount_, other->amount_);
    std::swap(unitabbreviation_, other->unitabbreviation_);
    std::swap(includesrefuse_, other->includesrefuse_);
    std::swap(order_, other->order_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata FoodComponentData::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = FoodComponentData_descriptor_;
  metadata.reflection = FoodComponentData_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
