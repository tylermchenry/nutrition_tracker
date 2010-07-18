// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: libnutrition/proto/service/template_messages.proto

#ifndef PROTOBUF_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto__INCLUDED
#define PROTOBUF_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto__INCLUDED

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
#include "libnutrition/proto/data/template.pb.h"
#include "libnutrition/proto/service/error.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();

class TemplateLoadRequest;
class TemplateLoadResponse;
class TemplateStoreRequest;
class TemplateStoreResponse;
class TemplateDeleteRequest;
class TemplateDeleteResponse;

// ===================================================================

class TemplateLoadRequest : public ::google::protobuf::Message {
 public:
  TemplateLoadRequest();
  virtual ~TemplateLoadRequest();
  
  TemplateLoadRequest(const TemplateLoadRequest& from);
  
  inline TemplateLoadRequest& operator=(const TemplateLoadRequest& from) {
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
  static const TemplateLoadRequest& default_instance();
  
  void Swap(TemplateLoadRequest* other);
  
  // implements Message ----------------------------------------------
  
  TemplateLoadRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TemplateLoadRequest& from);
  void MergeFrom(const TemplateLoadRequest& from);
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
  
  // optional bool nameAndIdOnly = 2 [default = false];
  inline bool has_nameandidonly() const;
  inline void clear_nameandidonly();
  static const int kNameAndIdOnlyFieldNumber = 2;
  inline bool nameandidonly() const;
  inline void set_nameandidonly(bool value);
  
  // repeated int32 requestedIds = 3;
  inline int requestedids_size() const;
  inline void clear_requestedids();
  static const int kRequestedIdsFieldNumber = 3;
  inline ::google::protobuf::int32 requestedids(int index) const;
  inline void set_requestedids(int index, ::google::protobuf::int32 value);
  inline void add_requestedids(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      requestedids() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_requestedids();
  
  // repeated int32 requestedUserIds = 4;
  inline int requesteduserids_size() const;
  inline void clear_requesteduserids();
  static const int kRequestedUserIdsFieldNumber = 4;
  inline ::google::protobuf::int32 requesteduserids(int index) const;
  inline void set_requesteduserids(int index, ::google::protobuf::int32 value);
  inline void add_requesteduserids(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      requesteduserids() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_requesteduserids();
  
  // @@protoc_insertion_point(class_scope:TemplateLoadRequest)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  bool omit_;
  bool nameandidonly_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > requestedids_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > requesteduserids_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  
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
  static TemplateLoadRequest* default_instance_;
};
// -------------------------------------------------------------------

class TemplateLoadResponse : public ::google::protobuf::Message {
 public:
  TemplateLoadResponse();
  virtual ~TemplateLoadResponse();
  
  TemplateLoadResponse(const TemplateLoadResponse& from);
  
  inline TemplateLoadResponse& operator=(const TemplateLoadResponse& from) {
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
  static const TemplateLoadResponse& default_instance();
  
  void Swap(TemplateLoadResponse* other);
  
  // implements Message ----------------------------------------------
  
  TemplateLoadResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TemplateLoadResponse& from);
  void MergeFrom(const TemplateLoadResponse& from);
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
  
  // repeated .TemplateData templates = 1;
  inline int templates_size() const;
  inline void clear_templates();
  static const int kTemplatesFieldNumber = 1;
  inline const ::TemplateData& templates(int index) const;
  inline ::TemplateData* mutable_templates(int index);
  inline ::TemplateData* add_templates();
  inline const ::google::protobuf::RepeatedPtrField< ::TemplateData >&
      templates() const;
  inline ::google::protobuf::RepeatedPtrField< ::TemplateData >*
      mutable_templates();
  
  // optional .Error error = 100;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 100;
  inline const ::Error& error() const;
  inline ::Error* mutable_error();
  
  // @@protoc_insertion_point(class_scope:TemplateLoadResponse)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::TemplateData > templates_;
  ::Error* error_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  
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
  static TemplateLoadResponse* default_instance_;
};
// -------------------------------------------------------------------

class TemplateStoreRequest : public ::google::protobuf::Message {
 public:
  TemplateStoreRequest();
  virtual ~TemplateStoreRequest();
  
  TemplateStoreRequest(const TemplateStoreRequest& from);
  
  inline TemplateStoreRequest& operator=(const TemplateStoreRequest& from) {
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
  static const TemplateStoreRequest& default_instance();
  
  void Swap(TemplateStoreRequest* other);
  
  // implements Message ----------------------------------------------
  
  TemplateStoreRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TemplateStoreRequest& from);
  void MergeFrom(const TemplateStoreRequest& from);
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
  
  // repeated .TemplateData templatess = 1;
  inline int templatess_size() const;
  inline void clear_templatess();
  static const int kTemplatessFieldNumber = 1;
  inline const ::TemplateData& templatess(int index) const;
  inline ::TemplateData* mutable_templatess(int index);
  inline ::TemplateData* add_templatess();
  inline const ::google::protobuf::RepeatedPtrField< ::TemplateData >&
      templatess() const;
  inline ::google::protobuf::RepeatedPtrField< ::TemplateData >*
      mutable_templatess();
  
  // @@protoc_insertion_point(class_scope:TemplateStoreRequest)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::TemplateData > templatess_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
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
  static TemplateStoreRequest* default_instance_;
};
// -------------------------------------------------------------------

class TemplateStoreResponse : public ::google::protobuf::Message {
 public:
  TemplateStoreResponse();
  virtual ~TemplateStoreResponse();
  
  TemplateStoreResponse(const TemplateStoreResponse& from);
  
  inline TemplateStoreResponse& operator=(const TemplateStoreResponse& from) {
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
  static const TemplateStoreResponse& default_instance();
  
  void Swap(TemplateStoreResponse* other);
  
  // implements Message ----------------------------------------------
  
  TemplateStoreResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TemplateStoreResponse& from);
  void MergeFrom(const TemplateStoreResponse& from);
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
  
  // repeated int32 storedIds = 1;
  inline int storedids_size() const;
  inline void clear_storedids();
  static const int kStoredIdsFieldNumber = 1;
  inline ::google::protobuf::int32 storedids(int index) const;
  inline void set_storedids(int index, ::google::protobuf::int32 value);
  inline void add_storedids(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      storedids() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_storedids();
  
  // optional .Error error = 100;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 100;
  inline const ::Error& error() const;
  inline ::Error* mutable_error();
  
  // @@protoc_insertion_point(class_scope:TemplateStoreResponse)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > storedids_;
  ::Error* error_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  
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
  static TemplateStoreResponse* default_instance_;
};
// -------------------------------------------------------------------

class TemplateDeleteRequest : public ::google::protobuf::Message {
 public:
  TemplateDeleteRequest();
  virtual ~TemplateDeleteRequest();
  
  TemplateDeleteRequest(const TemplateDeleteRequest& from);
  
  inline TemplateDeleteRequest& operator=(const TemplateDeleteRequest& from) {
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
  static const TemplateDeleteRequest& default_instance();
  
  void Swap(TemplateDeleteRequest* other);
  
  // implements Message ----------------------------------------------
  
  TemplateDeleteRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TemplateDeleteRequest& from);
  void MergeFrom(const TemplateDeleteRequest& from);
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
  
  // repeated int32 deleteIds = 1;
  inline int deleteids_size() const;
  inline void clear_deleteids();
  static const int kDeleteIdsFieldNumber = 1;
  inline ::google::protobuf::int32 deleteids(int index) const;
  inline void set_deleteids(int index, ::google::protobuf::int32 value);
  inline void add_deleteids(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      deleteids() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_deleteids();
  
  // @@protoc_insertion_point(class_scope:TemplateDeleteRequest)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > deleteids_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
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
  static TemplateDeleteRequest* default_instance_;
};
// -------------------------------------------------------------------

class TemplateDeleteResponse : public ::google::protobuf::Message {
 public:
  TemplateDeleteResponse();
  virtual ~TemplateDeleteResponse();
  
  TemplateDeleteResponse(const TemplateDeleteResponse& from);
  
  inline TemplateDeleteResponse& operator=(const TemplateDeleteResponse& from) {
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
  static const TemplateDeleteResponse& default_instance();
  
  void Swap(TemplateDeleteResponse* other);
  
  // implements Message ----------------------------------------------
  
  TemplateDeleteResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TemplateDeleteResponse& from);
  void MergeFrom(const TemplateDeleteResponse& from);
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
  
  // repeated int32 deletedIds = 1;
  inline int deletedids_size() const;
  inline void clear_deletedids();
  static const int kDeletedIdsFieldNumber = 1;
  inline ::google::protobuf::int32 deletedids(int index) const;
  inline void set_deletedids(int index, ::google::protobuf::int32 value);
  inline void add_deletedids(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      deletedids() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_deletedids();
  
  // optional .Error error = 100;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 100;
  inline const ::Error& error() const;
  inline ::Error* mutable_error();
  
  // @@protoc_insertion_point(class_scope:TemplateDeleteResponse)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > deletedids_;
  ::Error* error_;
  friend void  protobuf_AddDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_AssignDesc_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  friend void protobuf_ShutdownFile_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto();
  
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
  static TemplateDeleteResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// TemplateLoadRequest

// optional bool omit = 1 [default = false];
inline bool TemplateLoadRequest::has_omit() const {
  return _has_bit(0);
}
inline void TemplateLoadRequest::clear_omit() {
  omit_ = false;
  _clear_bit(0);
}
inline bool TemplateLoadRequest::omit() const {
  return omit_;
}
inline void TemplateLoadRequest::set_omit(bool value) {
  _set_bit(0);
  omit_ = value;
}

// optional bool nameAndIdOnly = 2 [default = false];
inline bool TemplateLoadRequest::has_nameandidonly() const {
  return _has_bit(1);
}
inline void TemplateLoadRequest::clear_nameandidonly() {
  nameandidonly_ = false;
  _clear_bit(1);
}
inline bool TemplateLoadRequest::nameandidonly() const {
  return nameandidonly_;
}
inline void TemplateLoadRequest::set_nameandidonly(bool value) {
  _set_bit(1);
  nameandidonly_ = value;
}

// repeated int32 requestedIds = 3;
inline int TemplateLoadRequest::requestedids_size() const {
  return requestedids_.size();
}
inline void TemplateLoadRequest::clear_requestedids() {
  requestedids_.Clear();
}
inline ::google::protobuf::int32 TemplateLoadRequest::requestedids(int index) const {
  return requestedids_.Get(index);
}
inline void TemplateLoadRequest::set_requestedids(int index, ::google::protobuf::int32 value) {
  requestedids_.Set(index, value);
}
inline void TemplateLoadRequest::add_requestedids(::google::protobuf::int32 value) {
  requestedids_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
TemplateLoadRequest::requestedids() const {
  return requestedids_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
TemplateLoadRequest::mutable_requestedids() {
  return &requestedids_;
}

// repeated int32 requestedUserIds = 4;
inline int TemplateLoadRequest::requesteduserids_size() const {
  return requesteduserids_.size();
}
inline void TemplateLoadRequest::clear_requesteduserids() {
  requesteduserids_.Clear();
}
inline ::google::protobuf::int32 TemplateLoadRequest::requesteduserids(int index) const {
  return requesteduserids_.Get(index);
}
inline void TemplateLoadRequest::set_requesteduserids(int index, ::google::protobuf::int32 value) {
  requesteduserids_.Set(index, value);
}
inline void TemplateLoadRequest::add_requesteduserids(::google::protobuf::int32 value) {
  requesteduserids_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
TemplateLoadRequest::requesteduserids() const {
  return requesteduserids_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
TemplateLoadRequest::mutable_requesteduserids() {
  return &requesteduserids_;
}

// -------------------------------------------------------------------

// TemplateLoadResponse

// repeated .TemplateData templates = 1;
inline int TemplateLoadResponse::templates_size() const {
  return templates_.size();
}
inline void TemplateLoadResponse::clear_templates() {
  templates_.Clear();
}
inline const ::TemplateData& TemplateLoadResponse::templates(int index) const {
  return templates_.Get(index);
}
inline ::TemplateData* TemplateLoadResponse::mutable_templates(int index) {
  return templates_.Mutable(index);
}
inline ::TemplateData* TemplateLoadResponse::add_templates() {
  return templates_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::TemplateData >&
TemplateLoadResponse::templates() const {
  return templates_;
}
inline ::google::protobuf::RepeatedPtrField< ::TemplateData >*
TemplateLoadResponse::mutable_templates() {
  return &templates_;
}

// optional .Error error = 100;
inline bool TemplateLoadResponse::has_error() const {
  return _has_bit(1);
}
inline void TemplateLoadResponse::clear_error() {
  if (error_ != NULL) error_->::Error::Clear();
  _clear_bit(1);
}
inline const ::Error& TemplateLoadResponse::error() const {
  return error_ != NULL ? *error_ : *default_instance_->error_;
}
inline ::Error* TemplateLoadResponse::mutable_error() {
  _set_bit(1);
  if (error_ == NULL) error_ = new ::Error;
  return error_;
}

// -------------------------------------------------------------------

// TemplateStoreRequest

// repeated .TemplateData templatess = 1;
inline int TemplateStoreRequest::templatess_size() const {
  return templatess_.size();
}
inline void TemplateStoreRequest::clear_templatess() {
  templatess_.Clear();
}
inline const ::TemplateData& TemplateStoreRequest::templatess(int index) const {
  return templatess_.Get(index);
}
inline ::TemplateData* TemplateStoreRequest::mutable_templatess(int index) {
  return templatess_.Mutable(index);
}
inline ::TemplateData* TemplateStoreRequest::add_templatess() {
  return templatess_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::TemplateData >&
TemplateStoreRequest::templatess() const {
  return templatess_;
}
inline ::google::protobuf::RepeatedPtrField< ::TemplateData >*
TemplateStoreRequest::mutable_templatess() {
  return &templatess_;
}

// -------------------------------------------------------------------

// TemplateStoreResponse

// repeated int32 storedIds = 1;
inline int TemplateStoreResponse::storedids_size() const {
  return storedids_.size();
}
inline void TemplateStoreResponse::clear_storedids() {
  storedids_.Clear();
}
inline ::google::protobuf::int32 TemplateStoreResponse::storedids(int index) const {
  return storedids_.Get(index);
}
inline void TemplateStoreResponse::set_storedids(int index, ::google::protobuf::int32 value) {
  storedids_.Set(index, value);
}
inline void TemplateStoreResponse::add_storedids(::google::protobuf::int32 value) {
  storedids_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
TemplateStoreResponse::storedids() const {
  return storedids_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
TemplateStoreResponse::mutable_storedids() {
  return &storedids_;
}

// optional .Error error = 100;
inline bool TemplateStoreResponse::has_error() const {
  return _has_bit(1);
}
inline void TemplateStoreResponse::clear_error() {
  if (error_ != NULL) error_->::Error::Clear();
  _clear_bit(1);
}
inline const ::Error& TemplateStoreResponse::error() const {
  return error_ != NULL ? *error_ : *default_instance_->error_;
}
inline ::Error* TemplateStoreResponse::mutable_error() {
  _set_bit(1);
  if (error_ == NULL) error_ = new ::Error;
  return error_;
}

// -------------------------------------------------------------------

// TemplateDeleteRequest

// repeated int32 deleteIds = 1;
inline int TemplateDeleteRequest::deleteids_size() const {
  return deleteids_.size();
}
inline void TemplateDeleteRequest::clear_deleteids() {
  deleteids_.Clear();
}
inline ::google::protobuf::int32 TemplateDeleteRequest::deleteids(int index) const {
  return deleteids_.Get(index);
}
inline void TemplateDeleteRequest::set_deleteids(int index, ::google::protobuf::int32 value) {
  deleteids_.Set(index, value);
}
inline void TemplateDeleteRequest::add_deleteids(::google::protobuf::int32 value) {
  deleteids_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
TemplateDeleteRequest::deleteids() const {
  return deleteids_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
TemplateDeleteRequest::mutable_deleteids() {
  return &deleteids_;
}

// -------------------------------------------------------------------

// TemplateDeleteResponse

// repeated int32 deletedIds = 1;
inline int TemplateDeleteResponse::deletedids_size() const {
  return deletedids_.size();
}
inline void TemplateDeleteResponse::clear_deletedids() {
  deletedids_.Clear();
}
inline ::google::protobuf::int32 TemplateDeleteResponse::deletedids(int index) const {
  return deletedids_.Get(index);
}
inline void TemplateDeleteResponse::set_deletedids(int index, ::google::protobuf::int32 value) {
  deletedids_.Set(index, value);
}
inline void TemplateDeleteResponse::add_deletedids(::google::protobuf::int32 value) {
  deletedids_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
TemplateDeleteResponse::deletedids() const {
  return deletedids_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
TemplateDeleteResponse::mutable_deletedids() {
  return &deletedids_;
}

// optional .Error error = 100;
inline bool TemplateDeleteResponse::has_error() const {
  return _has_bit(1);
}
inline void TemplateDeleteResponse::clear_error() {
  if (error_ != NULL) error_->::Error::Clear();
  _clear_bit(1);
}
inline const ::Error& TemplateDeleteResponse::error() const {
  return error_ != NULL ? *error_ : *default_instance_->error_;
}
inline ::Error* TemplateDeleteResponse::mutable_error() {
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

#endif  // PROTOBUF_libnutrition_2fproto_2fservice_2ftemplate_5fmessages_2eproto__INCLUDED
