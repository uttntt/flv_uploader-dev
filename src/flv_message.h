#pragma once
#include "flv_util.h"
#include <vector>
#include <list>

namespace castis {
namespace streamer {

typedef enum { video=1, audio=2 } media_es_type_t;
const uint64_t READY_TO_SEND_AUDIO_FRAME_COUNT = 4;
const uint64_t READY_TO_SEND_VIDEO_FRAME_COUNT = 4;

struct AudioPublishEsInit {
public:
  uint8_t trackId{2};
  std::string trackType{"audio"};
  uint32_t timescale{90000};
  std::string codec{"aac"};
  std::vector<unsigned char> codecInfo;
  
  std::string publish_id;
  uint32_t sample_rate; // big endian string
  uint32_t channel_count; // big endian string
};

struct VideoPublishEsInit {
public:
  uint8_t trackId{1};
  std::string trackType{"video"};
  uint32_t timescale{90000};
  std::string codec{"h264"};
  std::vector<unsigned char> codecInfo;
  
  std::string publish_id;
  std::vector<unsigned char> sps;
  std::vector<unsigned char> pps;
};

struct MediaPublishEs {
public:
  media_es_type_t type{video};
  uint64_t frameNumber{0};
  uint8_t trackId{2};
  uint8_t isKeyFrame{1};
  uint64_t pts{0};
  uint64_t dts{0};
  uint32_t size{0};
  
  std::string publish_id;
  flv_util::buffer_t* const data;

  MediaPublishEs(flv_util::buffer_t* const buffer) 
  : data(buffer) {
  }
  ~MediaPublishEs() {
    delete data;
  }
};

typedef std::shared_ptr<MediaPublishEs> media_publish_es_ptr;

struct MediaPublishEsContext {
public:  
  std::string publish_id_;
  AudioPublishEsInit audio_init_es_;
  VideoPublishEsInit video_init_es_;
  std::vector<unsigned char> video_eos_;
  
  uint8_t nal_startcode_len_{4};

  std::list<media_publish_es_ptr> media_es_;
  uint64_t frame_number_{0};
  uint64_t audio_frame_number_{0};
  uint64_t video_frame_number_{0};
};

std::string to_string(MediaPublishEsContext& context);

bool ready_to_send(castis::streamer::MediaPublishEsContext& context);
bool end_of_video_es(castis::streamer::MediaPublishEsContext& context);
media_publish_es_ptr make_media_es(
  media_es_type_t type,
  flv_util::buffer_t* const data,
  uint64_t frame_number,
  uint8_t is_key_frame,
  uint64_t pts,
  uint64_t dts,
  uint32_t size,
  int& ec
);


} // name space streamer
} // name space castis

namespace flv_message {

enum FLV_ES_TYPE {
  FLV_AUDIO_ES = 8,
  FLV_VIDEO_ES = 9
};

bool read_flv_es_dump_file(castis::streamer::MediaPublishEsContext& context, std::string const& file_path);
bool process_flv_es_message(
  castis::streamer::MediaPublishEsContext& context, 
  unsigned char* const buffer, 
  std::size_t const buffer_size,
  int& ec);
void read_flv_es_dump(castis::streamer::MediaPublishEsContext& context, unsigned int start, unsigned int end);


} // namespace flv_message
