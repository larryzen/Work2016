// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CMD_Game.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "CMD_Game.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace game {
namespace Protobuf {

void protobuf_ShutdownFile_CMD_5fGame_2eproto() {
  delete CMD_CS_ListInfo::default_instance_;
  delete CMD_CS_UnRegGameServer::default_instance_;
  delete CMD_CS_ServerOnLineCount::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_CMD_5fGame_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_CMD_5fGame_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  CMD_CS_ListInfo::default_instance_ = new CMD_CS_ListInfo();
  CMD_CS_UnRegGameServer::default_instance_ = new CMD_CS_UnRegGameServer();
  CMD_CS_ServerOnLineCount::default_instance_ = new CMD_CS_ServerOnLineCount();
  CMD_CS_ListInfo::default_instance_->InitAsDefaultInstance();
  CMD_CS_UnRegGameServer::default_instance_->InitAsDefaultInstance();
  CMD_CS_ServerOnLineCount::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_CMD_5fGame_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_CMD_5fGame_2eproto_once_);
void protobuf_AddDesc_CMD_5fGame_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_CMD_5fGame_2eproto_once_,
                 &protobuf_AddDesc_CMD_5fGame_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_CMD_5fGame_2eproto {
  StaticDescriptorInitializer_CMD_5fGame_2eproto() {
    protobuf_AddDesc_CMD_5fGame_2eproto();
  }
} static_descriptor_initializer_CMD_5fGame_2eproto_;
#endif
bool GAMELISTCMD_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
      return true;
    default:
      return false;
  }
}

bool ROMMGRCMD_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int CMD_CS_ListInfo::kDwTypeCountFieldNumber;
const int CMD_CS_ListInfo::kDwKindCountFieldNumber;
const int CMD_CS_ListInfo::kDwServerCountFieldNumber;
const int CMD_CS_ListInfo::kDwStationCountFieldNumber;
#endif  // !_MSC_VER

CMD_CS_ListInfo::CMD_CS_ListInfo()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:com.game.Protobuf.CMD_CS_ListInfo)
}

void CMD_CS_ListInfo::InitAsDefaultInstance() {
}

CMD_CS_ListInfo::CMD_CS_ListInfo(const CMD_CS_ListInfo& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:com.game.Protobuf.CMD_CS_ListInfo)
}

void CMD_CS_ListInfo::SharedCtor() {
  _cached_size_ = 0;
  dwtypecount_ = GOOGLE_ULONGLONG(0);
  dwkindcount_ = GOOGLE_ULONGLONG(0);
  dwservercount_ = GOOGLE_ULONGLONG(0);
  dwstationcount_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CMD_CS_ListInfo::~CMD_CS_ListInfo() {
  // @@protoc_insertion_point(destructor:com.game.Protobuf.CMD_CS_ListInfo)
  SharedDtor();
}

void CMD_CS_ListInfo::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void CMD_CS_ListInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const CMD_CS_ListInfo& CMD_CS_ListInfo::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_CMD_5fGame_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_CMD_5fGame_2eproto();
#endif
  return *default_instance_;
}

CMD_CS_ListInfo* CMD_CS_ListInfo::default_instance_ = NULL;

CMD_CS_ListInfo* CMD_CS_ListInfo::New() const {
  return new CMD_CS_ListInfo;
}

void CMD_CS_ListInfo::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<CMD_CS_ListInfo*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(dwtypecount_, dwstationcount_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool CMD_CS_ListInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:com.game.Protobuf.CMD_CS_ListInfo)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint64 dwTypeCount = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &dwtypecount_)));
          set_has_dwtypecount();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_dwKindCount;
        break;
      }

      // required uint64 dwKindCount = 2;
      case 2: {
        if (tag == 16) {
         parse_dwKindCount:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &dwkindcount_)));
          set_has_dwkindcount();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_dwServerCount;
        break;
      }

      // required uint64 dwServerCount = 3;
      case 3: {
        if (tag == 24) {
         parse_dwServerCount:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &dwservercount_)));
          set_has_dwservercount();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_dwStationCount;
        break;
      }

      // required uint64 dwStationCount = 4;
      case 4: {
        if (tag == 32) {
         parse_dwStationCount:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &dwstationcount_)));
          set_has_dwstationcount();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:com.game.Protobuf.CMD_CS_ListInfo)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:com.game.Protobuf.CMD_CS_ListInfo)
  return false;
#undef DO_
}

void CMD_CS_ListInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:com.game.Protobuf.CMD_CS_ListInfo)
  // required uint64 dwTypeCount = 1;
  if (has_dwtypecount()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(1, this->dwtypecount(), output);
  }

  // required uint64 dwKindCount = 2;
  if (has_dwkindcount()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(2, this->dwkindcount(), output);
  }

  // required uint64 dwServerCount = 3;
  if (has_dwservercount()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(3, this->dwservercount(), output);
  }

  // required uint64 dwStationCount = 4;
  if (has_dwstationcount()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(4, this->dwstationcount(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:com.game.Protobuf.CMD_CS_ListInfo)
}

int CMD_CS_ListInfo::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint64 dwTypeCount = 1;
    if (has_dwtypecount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->dwtypecount());
    }

    // required uint64 dwKindCount = 2;
    if (has_dwkindcount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->dwkindcount());
    }

    // required uint64 dwServerCount = 3;
    if (has_dwservercount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->dwservercount());
    }

    // required uint64 dwStationCount = 4;
    if (has_dwstationcount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->dwstationcount());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CMD_CS_ListInfo::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const CMD_CS_ListInfo*>(&from));
}

void CMD_CS_ListInfo::MergeFrom(const CMD_CS_ListInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_dwtypecount()) {
      set_dwtypecount(from.dwtypecount());
    }
    if (from.has_dwkindcount()) {
      set_dwkindcount(from.dwkindcount());
    }
    if (from.has_dwservercount()) {
      set_dwservercount(from.dwservercount());
    }
    if (from.has_dwstationcount()) {
      set_dwstationcount(from.dwstationcount());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void CMD_CS_ListInfo::CopyFrom(const CMD_CS_ListInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CMD_CS_ListInfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;

  return true;
}

void CMD_CS_ListInfo::Swap(CMD_CS_ListInfo* other) {
  if (other != this) {
    std::swap(dwtypecount_, other->dwtypecount_);
    std::swap(dwkindcount_, other->dwkindcount_);
    std::swap(dwservercount_, other->dwservercount_);
    std::swap(dwstationcount_, other->dwstationcount_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string CMD_CS_ListInfo::GetTypeName() const {
  return "com.game.Protobuf.CMD_CS_ListInfo";
}


// ===================================================================

#ifndef _MSC_VER
const int CMD_CS_UnRegGameServer::kWKindIDFieldNumber;
const int CMD_CS_UnRegGameServer::kWServerIDFieldNumber;
#endif  // !_MSC_VER

CMD_CS_UnRegGameServer::CMD_CS_UnRegGameServer()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:com.game.Protobuf.CMD_CS_UnRegGameServer)
}

void CMD_CS_UnRegGameServer::InitAsDefaultInstance() {
}

CMD_CS_UnRegGameServer::CMD_CS_UnRegGameServer(const CMD_CS_UnRegGameServer& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:com.game.Protobuf.CMD_CS_UnRegGameServer)
}

void CMD_CS_UnRegGameServer::SharedCtor() {
  _cached_size_ = 0;
  wkindid_ = 0u;
  wserverid_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CMD_CS_UnRegGameServer::~CMD_CS_UnRegGameServer() {
  // @@protoc_insertion_point(destructor:com.game.Protobuf.CMD_CS_UnRegGameServer)
  SharedDtor();
}

void CMD_CS_UnRegGameServer::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void CMD_CS_UnRegGameServer::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const CMD_CS_UnRegGameServer& CMD_CS_UnRegGameServer::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_CMD_5fGame_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_CMD_5fGame_2eproto();
#endif
  return *default_instance_;
}

CMD_CS_UnRegGameServer* CMD_CS_UnRegGameServer::default_instance_ = NULL;

CMD_CS_UnRegGameServer* CMD_CS_UnRegGameServer::New() const {
  return new CMD_CS_UnRegGameServer;
}

void CMD_CS_UnRegGameServer::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<CMD_CS_UnRegGameServer*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(wkindid_, wserverid_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool CMD_CS_UnRegGameServer::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:com.game.Protobuf.CMD_CS_UnRegGameServer)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 wKindID = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &wkindid_)));
          set_has_wkindid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_wServerID;
        break;
      }

      // required uint32 wServerID = 2;
      case 2: {
        if (tag == 16) {
         parse_wServerID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &wserverid_)));
          set_has_wserverid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:com.game.Protobuf.CMD_CS_UnRegGameServer)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:com.game.Protobuf.CMD_CS_UnRegGameServer)
  return false;
#undef DO_
}

void CMD_CS_UnRegGameServer::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:com.game.Protobuf.CMD_CS_UnRegGameServer)
  // required uint32 wKindID = 1;
  if (has_wkindid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->wkindid(), output);
  }

  // required uint32 wServerID = 2;
  if (has_wserverid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->wserverid(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:com.game.Protobuf.CMD_CS_UnRegGameServer)
}

int CMD_CS_UnRegGameServer::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 wKindID = 1;
    if (has_wkindid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->wkindid());
    }

    // required uint32 wServerID = 2;
    if (has_wserverid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->wserverid());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CMD_CS_UnRegGameServer::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const CMD_CS_UnRegGameServer*>(&from));
}

void CMD_CS_UnRegGameServer::MergeFrom(const CMD_CS_UnRegGameServer& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_wkindid()) {
      set_wkindid(from.wkindid());
    }
    if (from.has_wserverid()) {
      set_wserverid(from.wserverid());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void CMD_CS_UnRegGameServer::CopyFrom(const CMD_CS_UnRegGameServer& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CMD_CS_UnRegGameServer::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void CMD_CS_UnRegGameServer::Swap(CMD_CS_UnRegGameServer* other) {
  if (other != this) {
    std::swap(wkindid_, other->wkindid_);
    std::swap(wserverid_, other->wserverid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string CMD_CS_UnRegGameServer::GetTypeName() const {
  return "com.game.Protobuf.CMD_CS_UnRegGameServer";
}


// ===================================================================

#ifndef _MSC_VER
const int CMD_CS_ServerOnLineCount::kWKindIDFieldNumber;
const int CMD_CS_ServerOnLineCount::kWServerIDFieldNumber;
const int CMD_CS_ServerOnLineCount::kDwOnLineCountFieldNumber;
#endif  // !_MSC_VER

CMD_CS_ServerOnLineCount::CMD_CS_ServerOnLineCount()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:com.game.Protobuf.CMD_CS_ServerOnLineCount)
}

void CMD_CS_ServerOnLineCount::InitAsDefaultInstance() {
}

CMD_CS_ServerOnLineCount::CMD_CS_ServerOnLineCount(const CMD_CS_ServerOnLineCount& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:com.game.Protobuf.CMD_CS_ServerOnLineCount)
}

void CMD_CS_ServerOnLineCount::SharedCtor() {
  _cached_size_ = 0;
  wkindid_ = 0u;
  wserverid_ = 0u;
  dwonlinecount_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CMD_CS_ServerOnLineCount::~CMD_CS_ServerOnLineCount() {
  // @@protoc_insertion_point(destructor:com.game.Protobuf.CMD_CS_ServerOnLineCount)
  SharedDtor();
}

void CMD_CS_ServerOnLineCount::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void CMD_CS_ServerOnLineCount::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const CMD_CS_ServerOnLineCount& CMD_CS_ServerOnLineCount::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_CMD_5fGame_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_CMD_5fGame_2eproto();
#endif
  return *default_instance_;
}

CMD_CS_ServerOnLineCount* CMD_CS_ServerOnLineCount::default_instance_ = NULL;

CMD_CS_ServerOnLineCount* CMD_CS_ServerOnLineCount::New() const {
  return new CMD_CS_ServerOnLineCount;
}

void CMD_CS_ServerOnLineCount::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<CMD_CS_ServerOnLineCount*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(wkindid_, dwonlinecount_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool CMD_CS_ServerOnLineCount::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:com.game.Protobuf.CMD_CS_ServerOnLineCount)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 wKindID = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &wkindid_)));
          set_has_wkindid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_wServerID;
        break;
      }

      // required uint32 wServerID = 2;
      case 2: {
        if (tag == 16) {
         parse_wServerID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &wserverid_)));
          set_has_wserverid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_dwOnLineCount;
        break;
      }

      // required uint64 dwOnLineCount = 3;
      case 3: {
        if (tag == 24) {
         parse_dwOnLineCount:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &dwonlinecount_)));
          set_has_dwonlinecount();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:com.game.Protobuf.CMD_CS_ServerOnLineCount)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:com.game.Protobuf.CMD_CS_ServerOnLineCount)
  return false;
#undef DO_
}

void CMD_CS_ServerOnLineCount::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:com.game.Protobuf.CMD_CS_ServerOnLineCount)
  // required uint32 wKindID = 1;
  if (has_wkindid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->wkindid(), output);
  }

  // required uint32 wServerID = 2;
  if (has_wserverid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->wserverid(), output);
  }

  // required uint64 dwOnLineCount = 3;
  if (has_dwonlinecount()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(3, this->dwonlinecount(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:com.game.Protobuf.CMD_CS_ServerOnLineCount)
}

int CMD_CS_ServerOnLineCount::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 wKindID = 1;
    if (has_wkindid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->wkindid());
    }

    // required uint32 wServerID = 2;
    if (has_wserverid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->wserverid());
    }

    // required uint64 dwOnLineCount = 3;
    if (has_dwonlinecount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->dwonlinecount());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CMD_CS_ServerOnLineCount::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const CMD_CS_ServerOnLineCount*>(&from));
}

void CMD_CS_ServerOnLineCount::MergeFrom(const CMD_CS_ServerOnLineCount& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_wkindid()) {
      set_wkindid(from.wkindid());
    }
    if (from.has_wserverid()) {
      set_wserverid(from.wserverid());
    }
    if (from.has_dwonlinecount()) {
      set_dwonlinecount(from.dwonlinecount());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void CMD_CS_ServerOnLineCount::CopyFrom(const CMD_CS_ServerOnLineCount& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CMD_CS_ServerOnLineCount::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void CMD_CS_ServerOnLineCount::Swap(CMD_CS_ServerOnLineCount* other) {
  if (other != this) {
    std::swap(wkindid_, other->wkindid_);
    std::swap(wserverid_, other->wserverid_);
    std::swap(dwonlinecount_, other->dwonlinecount_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string CMD_CS_ServerOnLineCount::GetTypeName() const {
  return "com.game.Protobuf.CMD_CS_ServerOnLineCount";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Protobuf
}  // namespace game
}  // namespace com

// @@protoc_insertion_point(global_scope)
