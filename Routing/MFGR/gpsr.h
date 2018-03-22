/*  desigened by pfhuang
 *
 */

#ifndef GPSR_ROUTING_H_
#define GPSR_ROUTING_H_

#include "config.h"
#include "agent.h"
#include "ip.h"
#include "address.h"
//#include "scheduler.h"
#include "timer-handler.h"
#include "mobilenode.h"
#include "tools/random.h"
#include "packet.h"
#include "trace.h"
#include "classifier-port.h"
#include "cmu-trace.h"

#include "gpsr_packet.h"
#include "gpsr_neighbor.h"
#include "gpsr_sinklist.h"

class GPSRAgent;

/*
 * Hello timer which is used to fire the hello message periodically
 */
class GPSRHelloTimer : public TimerHandler {
public:
  GPSRHelloTimer(GPSRAgent *a) : TimerHandler() {a_=a;}
protected:
  virtual void expire(Event *e);
  GPSRAgent *a_;
};

/*
 * The Query Timer which is used by the data sink to fire the 
 * data query. It is not a part of the design of the GPSR routing.
 * Since the information of the data sink mostly is not able to be 
 * obtained directly (mostly, by DHT: distributed hash table), I 
 * just let the data sink to trigger the routing, like a common 
 * On-Demond routing.
 * 
 */
class GPSRQueryTimer : public TimerHandler {
public:
  GPSRQueryTimer(GPSRAgent *a) : TimerHandler() {a_=a;}
protected:
  virtual void expire(Event *e);
  GPSRAgent *a_;
};

//定时检查邻节点列表,更新count值.
class UpdateTimer : public TimerHandler {
public:
  UpdateTimer(GPSRAgent *a) : TimerHandler() {a_=a;}
protected:
  virtual void expire(Event *e);
  GPSRAgent *a_;
};

class GPSRAgent : public Agent {
private:

  friend class GPSRHelloTimer;
  friend class GPSRQueryTimer;
  friend class UpdateTimer;
  MobileNode *node_;             //the attached mobile node
  PortClassifier *port_dmux_;    //for the higher layer app de-multiplexing
  
  nsaddr_t my_id_;               //node id (address), which is NOT necessary
  double my_x_;                  //node location info, fatal info
  double my_y_;                  //     obtained from the attached node
  double my_NAC_;
  int flag_;			//控制turnon的运行次数

  Sinks *sink_list_;      //for multiple sinks

  GPSRNeighbors *nblist_; //neighbor list: routing table implemenation 
                          //               and planarizing implementation

  int recv_counter_;           
  u_int8_t query_counter_;

  GPSRHelloTimer hello_timer_;
  GPSRQueryTimer query_timer_;
  UpdateTimer	 update_timer;


  int planar_type_; //1=GG planarize, 0=RNG planarize

  double hello_period_;
  double query_period_;
  double update_period_;//更新count值的间隔
  double window_;	//窗口值,默认为10s

  void turnon();              //set to be alive
  void turnoff();             //set to be dead
  void updateInfo();		//更新当前节点自己的信息.
  void startSink();          
  void startSink(double);
  void startUpdate();
  void startUpdate(double);		//在仿真开始一个窗口值的时间后,开始更新邻节点列表中的count值.
  void GetLocation(double*, double*); //called at initial phase
  virtual void getLoc();
  void getNAC();

  void hellomsg();
  void query(nsaddr_t);

  void recvHello(Packet*);
  void recvQuery(Packet*);
  
  void sinkRecv(Packet*);
  void forwardData(Packet*);

  RNG randSend_;

  /**
   * The below variables and functions are used for 
   * localization algorithms
   */
  double localized_x_; 
  double localized_y_;
  void dvhop();

protected:
  Trace *tracetarget;              //for routing agent special trace
  void trace(char *fmt,...);       //   Not necessary 

  void hellotout();                //called by timer::expire(Event*)
  void querytout();
  void update();		   //更新邻节点列表中的count值
public:
  GPSRAgent();

  int command(int, const char*const*);
  void recv(Packet*, Handler*);         //inherited virtual function

};

#endif
