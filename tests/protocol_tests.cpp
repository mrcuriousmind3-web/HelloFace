#include "helloface/ipc_protocol.hpp"
#include <cassert>
#include <cstddef>
#include <iostream>
int main(){
 using namespace helloface::ipc;
 Message m{MessageType::MatchResult,42,{std::byte{0x2A},std::byte{0x00}}};auto wire=encode(m);Message parsed;assert(decode(wire,parsed));assert(parsed.type==m.type&&parsed.request_id==42&&parsed.payload==m.payload);
 MatchPayload match;match.selected_slot=2;match.slot_count=4;for(std::uint8_t i=0;i<4;++i){match.scores[i].slot_id=i;match.scores[i].cosine_score=0.7f-float(i)*0.1f;}
 auto bytes=encode_match(match);MatchPayload decoded;assert(decode_match(bytes,decoded));assert(decoded.selected_slot==2&&decoded.slot_count==4);assert(decoded.scores[0].cosine_score>decoded.scores[3].cosine_score);
 wire.pop_back();assert(!decode(wire,parsed));match.selected_slot=4;assert(encode_match(match).empty());std::cout<<"HelloFace IPC protocol tests passed\n";
}
