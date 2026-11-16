#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

namespace helloface::ipc {
inline constexpr std::uint32_t kMagic = 0x48464C31;
inline constexpr std::uint16_t kVersion = 1;
inline constexpr std::size_t kMaxPayload = 4096;
inline constexpr std::size_t kMaxSlots = 4;
enum class MessageType : std::uint16_t { Status=1, ScanRequest=2, MatchResult=3, Cancel=4, Error=5 };
struct SlotScore { std::uint8_t slot_id{}; std::uint8_t state{}; std::uint16_t reserved{}; float cosine_score{}; };
struct MatchPayload { std::uint8_t selected_slot{0xFF}; std::uint8_t slot_count{}; std::uint16_t flags{}; std::array<SlotScore,kMaxSlots> scores{}; };
struct Message { MessageType type{MessageType::Error}; std::uint32_t request_id{}; std::vector<std::byte> payload; };
namespace detail {
inline void put16(std::vector<std::byte>& b,std::uint16_t v){b.push_back(std::byte(v&255));b.push_back(std::byte((v>>8)&255));}
inline void put32(std::vector<std::byte>& b,std::uint32_t v){for(unsigned i=0;i<4;++i)b.push_back(std::byte((v>>(8*i))&255));}
inline bool get16(std::span<const std::byte>b,std::size_t&p,std::uint16_t&v){if(p+2>b.size())return false;v=std::uint16_t(std::to_integer<unsigned>(b[p]))|std::uint16_t(std::to_integer<unsigned>(b[p+1])<<8);p+=2;return true;}
inline bool get32(std::span<const std::byte>b,std::size_t&p,std::uint32_t&v){if(p+4>b.size())return false;v=0;for(unsigned i=0;i<4;++i)v|=std::uint32_t(std::to_integer<unsigned>(b[p+i]))<<(8*i);p+=4;return true;}
}
inline std::vector<std::byte> encode(const Message&m){
 if(m.payload.size()>kMaxPayload)return {};
 std::vector<std::byte>o;o.reserve(16+m.payload.size());detail::put32(o,kMagic);detail::put16(o,kVersion);detail::put16(o,static_cast<std::uint16_t>(m.type));detail::put32(o,m.request_id);detail::put32(o,static_cast<std::uint32_t>(m.payload.size()));o.insert(o.end(),m.payload.begin(),m.payload.end());return o;
}
inline bool decode(std::span<const std::byte>b,Message&o){
 if(b.size()<16||b.size()>16+kMaxPayload)return false;
 std::size_t p=0;std::uint32_t magic{},req{},size{};std::uint16_t ver{},type{};
 if(!detail::get32(b,p,magic)||!detail::get16(b,p,ver)||!detail::get16(b,p,type)||!detail::get32(b,p,req)||!detail::get32(b,p,size))return false;
 if(magic!=kMagic||ver!=kVersion||type<1||type>5||size>kMaxPayload||p+size!=b.size())return false;
 o.type=static_cast<MessageType>(type);o.request_id=req;o.payload.assign(b.begin()+static_cast<std::ptrdiff_t>(p),b.end());return true;
}
inline std::vector<std::byte> encode_match(const MatchPayload&m){
 if(m.slot_count>kMaxSlots||(m.selected_slot!=0xFF&&m.selected_slot>=kMaxSlots))return {};
 std::vector<std::byte>b;b.reserve(4+kMaxSlots*8);b.push_back(std::byte(m.selected_slot));b.push_back(std::byte(m.slot_count));detail::put16(b,m.flags);
 for(const auto&s:m.scores){if(s.slot_id>=kMaxSlots)return {};b.push_back(std::byte(s.slot_id));b.push_back(std::byte(s.state));detail::put16(b,s.reserved);std::uint32_t bits{};static_assert(sizeof(bits)==sizeof(s.cosine_score));std::memcpy(&bits,&s.cosine_score,sizeof(bits));detail::put32(b,bits);}return b;
}
inline bool decode_match(std::span<const std::byte>b,MatchPayload&m){
 if(b.size()!=4+kMaxSlots*8)return false;m.selected_slot=std::to_integer<std::uint8_t>(b[0]);m.slot_count=std::to_integer<std::uint8_t>(b[1]);std::size_t p=2;
 if(!detail::get16(b,p,m.flags)||m.slot_count>kMaxSlots||(m.selected_slot!=0xFF&&m.selected_slot>=kMaxSlots))return false;
 for(auto&s:m.scores){if(p+2>b.size())return false;s.slot_id=std::to_integer<std::uint8_t>(b[p++]);s.state=std::to_integer<std::uint8_t>(b[p++]);if(s.slot_id>=kMaxSlots||!detail::get16(b,p,s.reserved))return false;std::uint32_t bits{};if(!detail::get32(b,p,bits))return false;std::memcpy(&s.cosine_score,&bits,sizeof(bits));}return p==b.size();
}
}
