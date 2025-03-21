// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: block.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_block_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_block_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_block_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_block_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_block_2eproto;
namespace block_chain {
class BlockProto;
struct BlockProtoDefaultTypeInternal;
extern BlockProtoDefaultTypeInternal _BlockProto_default_instance_;
}  // namespace block_chain
PROTOBUF_NAMESPACE_OPEN
template<> ::block_chain::BlockProto* Arena::CreateMaybeMessage<::block_chain::BlockProto>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace block_chain {

// ===================================================================

class BlockProto final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:block_chain.BlockProto) */ {
 public:
  inline BlockProto() : BlockProto(nullptr) {}
  ~BlockProto() override;
  explicit PROTOBUF_CONSTEXPR BlockProto(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  BlockProto(const BlockProto& from);
  BlockProto(BlockProto&& from) noexcept
    : BlockProto() {
    *this = ::std::move(from);
  }

  inline BlockProto& operator=(const BlockProto& from) {
    CopyFrom(from);
    return *this;
  }
  inline BlockProto& operator=(BlockProto&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const BlockProto& default_instance() {
    return *internal_default_instance();
  }
  static inline const BlockProto* internal_default_instance() {
    return reinterpret_cast<const BlockProto*>(
               &_BlockProto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(BlockProto& a, BlockProto& b) {
    a.Swap(&b);
  }
  inline void Swap(BlockProto* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(BlockProto* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  BlockProto* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<BlockProto>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const BlockProto& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const BlockProto& from) {
    BlockProto::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(BlockProto* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "block_chain.BlockProto";
  }
  protected:
  explicit BlockProto(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kDataFieldNumber = 1,
    kHashFieldNumber = 3,
    kPrevHashFieldNumber = 4,
    kTimestampFieldNumber = 2,
    kNonceFieldNumber = 5,
    kIsValidFieldNumber = 6,
  };
  // bytes data = 1;
  void clear_data();
  const std::string& data() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_data(ArgT0&& arg0, ArgT... args);
  std::string* mutable_data();
  PROTOBUF_NODISCARD std::string* release_data();
  void set_allocated_data(std::string* data);
  private:
  const std::string& _internal_data() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_data(const std::string& value);
  std::string* _internal_mutable_data();
  public:

  // bytes hash = 3;
  void clear_hash();
  const std::string& hash() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_hash(ArgT0&& arg0, ArgT... args);
  std::string* mutable_hash();
  PROTOBUF_NODISCARD std::string* release_hash();
  void set_allocated_hash(std::string* hash);
  private:
  const std::string& _internal_hash() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_hash(const std::string& value);
  std::string* _internal_mutable_hash();
  public:

  // bytes prev_hash = 4;
  void clear_prev_hash();
  const std::string& prev_hash() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_prev_hash(ArgT0&& arg0, ArgT... args);
  std::string* mutable_prev_hash();
  PROTOBUF_NODISCARD std::string* release_prev_hash();
  void set_allocated_prev_hash(std::string* prev_hash);
  private:
  const std::string& _internal_prev_hash() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_prev_hash(const std::string& value);
  std::string* _internal_mutable_prev_hash();
  public:

  // int64 timestamp = 2;
  void clear_timestamp();
  int64_t timestamp() const;
  void set_timestamp(int64_t value);
  private:
  int64_t _internal_timestamp() const;
  void _internal_set_timestamp(int64_t value);
  public:

  // int64 nonce = 5;
  void clear_nonce();
  int64_t nonce() const;
  void set_nonce(int64_t value);
  private:
  int64_t _internal_nonce() const;
  void _internal_set_nonce(int64_t value);
  public:

  // bool is_valid = 6;
  void clear_is_valid();
  bool is_valid() const;
  void set_is_valid(bool value);
  private:
  bool _internal_is_valid() const;
  void _internal_set_is_valid(bool value);
  public:

  // @@protoc_insertion_point(class_scope:block_chain.BlockProto)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr hash_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr prev_hash_;
    int64_t timestamp_;
    int64_t nonce_;
    bool is_valid_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_block_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// BlockProto

// bytes data = 1;
inline void BlockProto::clear_data() {
  _impl_.data_.ClearToEmpty();
}
inline const std::string& BlockProto::data() const {
  // @@protoc_insertion_point(field_get:block_chain.BlockProto.data)
  return _internal_data();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void BlockProto::set_data(ArgT0&& arg0, ArgT... args) {
 
 _impl_.data_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:block_chain.BlockProto.data)
}
inline std::string* BlockProto::mutable_data() {
  std::string* _s = _internal_mutable_data();
  // @@protoc_insertion_point(field_mutable:block_chain.BlockProto.data)
  return _s;
}
inline const std::string& BlockProto::_internal_data() const {
  return _impl_.data_.Get();
}
inline void BlockProto::_internal_set_data(const std::string& value) {
  
  _impl_.data_.Set(value, GetArenaForAllocation());
}
inline std::string* BlockProto::_internal_mutable_data() {
  
  return _impl_.data_.Mutable(GetArenaForAllocation());
}
inline std::string* BlockProto::release_data() {
  // @@protoc_insertion_point(field_release:block_chain.BlockProto.data)
  return _impl_.data_.Release();
}
inline void BlockProto::set_allocated_data(std::string* data) {
  if (data != nullptr) {
    
  } else {
    
  }
  _impl_.data_.SetAllocated(data, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_.IsDefault()) {
    _impl_.data_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:block_chain.BlockProto.data)
}

// int64 timestamp = 2;
inline void BlockProto::clear_timestamp() {
  _impl_.timestamp_ = int64_t{0};
}
inline int64_t BlockProto::_internal_timestamp() const {
  return _impl_.timestamp_;
}
inline int64_t BlockProto::timestamp() const {
  // @@protoc_insertion_point(field_get:block_chain.BlockProto.timestamp)
  return _internal_timestamp();
}
inline void BlockProto::_internal_set_timestamp(int64_t value) {
  
  _impl_.timestamp_ = value;
}
inline void BlockProto::set_timestamp(int64_t value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:block_chain.BlockProto.timestamp)
}

// bytes hash = 3;
inline void BlockProto::clear_hash() {
  _impl_.hash_.ClearToEmpty();
}
inline const std::string& BlockProto::hash() const {
  // @@protoc_insertion_point(field_get:block_chain.BlockProto.hash)
  return _internal_hash();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void BlockProto::set_hash(ArgT0&& arg0, ArgT... args) {
 
 _impl_.hash_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:block_chain.BlockProto.hash)
}
inline std::string* BlockProto::mutable_hash() {
  std::string* _s = _internal_mutable_hash();
  // @@protoc_insertion_point(field_mutable:block_chain.BlockProto.hash)
  return _s;
}
inline const std::string& BlockProto::_internal_hash() const {
  return _impl_.hash_.Get();
}
inline void BlockProto::_internal_set_hash(const std::string& value) {
  
  _impl_.hash_.Set(value, GetArenaForAllocation());
}
inline std::string* BlockProto::_internal_mutable_hash() {
  
  return _impl_.hash_.Mutable(GetArenaForAllocation());
}
inline std::string* BlockProto::release_hash() {
  // @@protoc_insertion_point(field_release:block_chain.BlockProto.hash)
  return _impl_.hash_.Release();
}
inline void BlockProto::set_allocated_hash(std::string* hash) {
  if (hash != nullptr) {
    
  } else {
    
  }
  _impl_.hash_.SetAllocated(hash, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.hash_.IsDefault()) {
    _impl_.hash_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:block_chain.BlockProto.hash)
}

// bytes prev_hash = 4;
inline void BlockProto::clear_prev_hash() {
  _impl_.prev_hash_.ClearToEmpty();
}
inline const std::string& BlockProto::prev_hash() const {
  // @@protoc_insertion_point(field_get:block_chain.BlockProto.prev_hash)
  return _internal_prev_hash();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void BlockProto::set_prev_hash(ArgT0&& arg0, ArgT... args) {
 
 _impl_.prev_hash_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:block_chain.BlockProto.prev_hash)
}
inline std::string* BlockProto::mutable_prev_hash() {
  std::string* _s = _internal_mutable_prev_hash();
  // @@protoc_insertion_point(field_mutable:block_chain.BlockProto.prev_hash)
  return _s;
}
inline const std::string& BlockProto::_internal_prev_hash() const {
  return _impl_.prev_hash_.Get();
}
inline void BlockProto::_internal_set_prev_hash(const std::string& value) {
  
  _impl_.prev_hash_.Set(value, GetArenaForAllocation());
}
inline std::string* BlockProto::_internal_mutable_prev_hash() {
  
  return _impl_.prev_hash_.Mutable(GetArenaForAllocation());
}
inline std::string* BlockProto::release_prev_hash() {
  // @@protoc_insertion_point(field_release:block_chain.BlockProto.prev_hash)
  return _impl_.prev_hash_.Release();
}
inline void BlockProto::set_allocated_prev_hash(std::string* prev_hash) {
  if (prev_hash != nullptr) {
    
  } else {
    
  }
  _impl_.prev_hash_.SetAllocated(prev_hash, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.prev_hash_.IsDefault()) {
    _impl_.prev_hash_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:block_chain.BlockProto.prev_hash)
}

// int64 nonce = 5;
inline void BlockProto::clear_nonce() {
  _impl_.nonce_ = int64_t{0};
}
inline int64_t BlockProto::_internal_nonce() const {
  return _impl_.nonce_;
}
inline int64_t BlockProto::nonce() const {
  // @@protoc_insertion_point(field_get:block_chain.BlockProto.nonce)
  return _internal_nonce();
}
inline void BlockProto::_internal_set_nonce(int64_t value) {
  
  _impl_.nonce_ = value;
}
inline void BlockProto::set_nonce(int64_t value) {
  _internal_set_nonce(value);
  // @@protoc_insertion_point(field_set:block_chain.BlockProto.nonce)
}

// bool is_valid = 6;
inline void BlockProto::clear_is_valid() {
  _impl_.is_valid_ = false;
}
inline bool BlockProto::_internal_is_valid() const {
  return _impl_.is_valid_;
}
inline bool BlockProto::is_valid() const {
  // @@protoc_insertion_point(field_get:block_chain.BlockProto.is_valid)
  return _internal_is_valid();
}
inline void BlockProto::_internal_set_is_valid(bool value) {
  
  _impl_.is_valid_ = value;
}
inline void BlockProto::set_is_valid(bool value) {
  _internal_set_is_valid(value);
  // @@protoc_insertion_point(field_set:block_chain.BlockProto.is_valid)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace block_chain

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_block_2eproto
