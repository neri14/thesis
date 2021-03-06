// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messages.proto

#ifndef PROTOBUF_messages_2eproto__INCLUDED
#define PROTOBUF_messages_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace common {
namespace proto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_messages_2eproto();
void protobuf_AssignDesc_messages_2eproto();
void protobuf_ShutdownFile_messages_2eproto();

class Event;
class Register;

// ===================================================================

class Event : public ::google::protobuf::Message {
 public:
  Event();
  virtual ~Event();

  Event(const Event& from);

  inline Event& operator=(const Event& from) {
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
  static const Event& default_instance();

  void Swap(Event* other);

  // implements Message ----------------------------------------------

  Event* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Event& from);
  void MergeFrom(const Event& from);
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

  // required int32 type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::google::protobuf::int32 type() const;
  inline void set_type(::google::protobuf::int32 value);

  // required int32 scope = 2;
  inline bool has_scope() const;
  inline void clear_scope();
  static const int kScopeFieldNumber = 2;
  inline ::google::protobuf::int32 scope() const;
  inline void set_scope(::google::protobuf::int32 value);

  // required bytes payload = 3;
  inline bool has_payload() const;
  inline void clear_payload();
  static const int kPayloadFieldNumber = 3;
  inline const ::std::string& payload() const;
  inline void set_payload(const ::std::string& value);
  inline void set_payload(const char* value);
  inline void set_payload(const void* value, size_t size);
  inline ::std::string* mutable_payload();
  inline ::std::string* release_payload();
  inline void set_allocated_payload(::std::string* payload);

  // @@protoc_insertion_point(class_scope:common.proto.Event)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_scope();
  inline void clear_has_scope();
  inline void set_has_payload();
  inline void clear_has_payload();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 type_;
  ::google::protobuf::int32 scope_;
  ::std::string* payload_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_messages_2eproto();
  friend void protobuf_AssignDesc_messages_2eproto();
  friend void protobuf_ShutdownFile_messages_2eproto();

  void InitAsDefaultInstance();
  static Event* default_instance_;
};
// -------------------------------------------------------------------

class Register : public ::google::protobuf::Message {
 public:
  Register();
  virtual ~Register();

  Register(const Register& from);

  inline Register& operator=(const Register& from) {
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
  static const Register& default_instance();

  void Swap(Register* other);

  // implements Message ----------------------------------------------

  Register* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Register& from);
  void MergeFrom(const Register& from);
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

  // required int32 type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::google::protobuf::int32 type() const;
  inline void set_type(::google::protobuf::int32 value);

  // required int32 scope = 2;
  inline bool has_scope() const;
  inline void clear_scope();
  static const int kScopeFieldNumber = 2;
  inline ::google::protobuf::int32 scope() const;
  inline void set_scope(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:common.proto.Register)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_scope();
  inline void clear_has_scope();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 type_;
  ::google::protobuf::int32 scope_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_messages_2eproto();
  friend void protobuf_AssignDesc_messages_2eproto();
  friend void protobuf_ShutdownFile_messages_2eproto();

  void InitAsDefaultInstance();
  static Register* default_instance_;
};
// ===================================================================


// ===================================================================

// Event

// required int32 type = 1;
inline bool Event::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Event::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Event::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Event::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::google::protobuf::int32 Event::type() const {
  return type_;
}
inline void Event::set_type(::google::protobuf::int32 value) {
  set_has_type();
  type_ = value;
}

// required int32 scope = 2;
inline bool Event::has_scope() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Event::set_has_scope() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Event::clear_has_scope() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Event::clear_scope() {
  scope_ = 0;
  clear_has_scope();
}
inline ::google::protobuf::int32 Event::scope() const {
  return scope_;
}
inline void Event::set_scope(::google::protobuf::int32 value) {
  set_has_scope();
  scope_ = value;
}

// required bytes payload = 3;
inline bool Event::has_payload() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Event::set_has_payload() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Event::clear_has_payload() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Event::clear_payload() {
  if (payload_ != &::google::protobuf::internal::kEmptyString) {
    payload_->clear();
  }
  clear_has_payload();
}
inline const ::std::string& Event::payload() const {
  return *payload_;
}
inline void Event::set_payload(const ::std::string& value) {
  set_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    payload_ = new ::std::string;
  }
  payload_->assign(value);
}
inline void Event::set_payload(const char* value) {
  set_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    payload_ = new ::std::string;
  }
  payload_->assign(value);
}
inline void Event::set_payload(const void* value, size_t size) {
  set_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    payload_ = new ::std::string;
  }
  payload_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Event::mutable_payload() {
  set_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    payload_ = new ::std::string;
  }
  return payload_;
}
inline ::std::string* Event::release_payload() {
  clear_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = payload_;
    payload_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Event::set_allocated_payload(::std::string* payload) {
  if (payload_ != &::google::protobuf::internal::kEmptyString) {
    delete payload_;
  }
  if (payload) {
    set_has_payload();
    payload_ = payload;
  } else {
    clear_has_payload();
    payload_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// Register

// required int32 type = 1;
inline bool Register::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Register::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Register::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Register::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::google::protobuf::int32 Register::type() const {
  return type_;
}
inline void Register::set_type(::google::protobuf::int32 value) {
  set_has_type();
  type_ = value;
}

// required int32 scope = 2;
inline bool Register::has_scope() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Register::set_has_scope() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Register::clear_has_scope() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Register::clear_scope() {
  scope_ = 0;
  clear_has_scope();
}
inline ::google::protobuf::int32 Register::scope() const {
  return scope_;
}
inline void Register::set_scope(::google::protobuf::int32 value) {
  set_has_scope();
  scope_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace common

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_messages_2eproto__INCLUDED
