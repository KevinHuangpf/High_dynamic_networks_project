/*	
*   desigened by pfhuang
*/

#ifndef GPSR_PACKET_H_
#define GPSR_PACKET_H_

#include "packet.h"
#include <math.h>

#define SINK_TRACE_FILE "sink_trace.tr"
#define NB_TRACE_FILE "gpsrnb_trace.tr"

#define GPSR_CURRENT Scheduler::instance().clock()
#define INFINITE_DELAY 5000000000000.0

#define GPSRTYPE_HELLO  0x01   //hello msg
#define GPSRTYPE_QUERY  0x02   //query msg from the sink
#define GPSRTYPE_DATA   0x04   //the CBR data msg

#define GPSR_MODE_GF    0x01   //greedy forwarding mode
#define GPSR_MODE_PERI  0x02   //perimeter routing mode

#define HDR_GPSR(p)   ((struct hdr_gpsr*)hdr_gpsr::access(p))
#define HDR_GPSR_HELLO(p) ((struct hdr_gpsr_hello*)hdr_gpsr::access(p))
#define HDR_GPSR_QUERY(p) ((struct hdr_gpsr_query*)hdr_gpsr::access(p))
#define HDR_GPSR_DATA(p) ((struct hdr_gpsr_data*)hdr_gpsr::access(p))

struct hdr_gpsr {
  u_int8_t type_;
  
  static int offset_;
  inline static int& offset() {return offset_;}
  inline static struct hdr_gpsr* access(const Packet *p){
    return (struct hdr_gpsr*) p->access(offset_);
  }
};


struct hdr_gpsr_hello {
  u_int8_t type_;
  float x_;     //My geo info
  float y_;
  float NAC_;
  inline int size(){
    int sz =
      sizeof(u_int8_t) +
      3*sizeof(float);
    return sz;
  }
};

struct hdr_gpsr_query {
  u_int8_t type_;
  float x_;      //The sink geo info
  float y_;
  float ts_;     //time stampe
  int hops_;
  u_int8_t seqno_;     //query sequence number
  inline int size(){
    int sz =
      2*sizeof(u_int8_t) +
      3*sizeof(float) +
      sizeof(int);
    return sz;
  }
};

struct hdr_gpsr_data {
  u_int8_t type_;
  u_int8_t mode_;  //Greedy forwarding or Perimeter Routing

  float sx_;      //the geo info of src
  float sy_;
  float dx_;      //the geo info of dst 
  float dy_;
//  int da[5];
  float ts_;      //the originating time stamp
  inline int size(){
    int sz =
      2*sizeof(u_int8_t) +
      5*sizeof(float) ;
    return sz;
  }
};

union hdr_all_gpsr {
  hdr_gpsr       gh;
  hdr_gpsr_hello ghh;
  hdr_gpsr_query gqh;
  hdr_gpsr_data  gdh;
};



#endif
