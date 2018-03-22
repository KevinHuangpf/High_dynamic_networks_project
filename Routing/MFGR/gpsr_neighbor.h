
/* 
 desigened by pfhuang
 */

#ifndef GPSR_NEIGHBOR_H_
#define GPSR_NEIGHBOR_H_

#include "gpsr_packet.h"
#include "mobilenode.h"

#define DEFAULT_GPSR_TIMEOUT   200.0   
        //the default time out period is 200.0 sec
        //If the hello messge was not received during this period
        //the entry in the neighbor list may be deleted

#define INIFINITE_DISTANCE   1000000000.0 //bigger than radio range is enough

/* The structure used for neighbor entry
 */
struct gpsr_neighbor {
  nsaddr_t id_; 
  double x_;    //the geo info
  double y_; 
  
  double NAC_;
  
  double ts_;     //the last time stamp of the hello msg from it
  double t1_;	//记录第一次建立该邻节点时的时间
  int count_;	//统计窗口时间内接收hello个数,如果统计时间长度小于窗口值,即为该时间段内收到的hello数目.
  struct gpsr_neighbor *next_;
  struct gpsr_neighbor *prev_;
};

class GPSRNeighbors {
private:
  //the neighbors list
  struct gpsr_neighbor *head_;
  struct gpsr_neighbor *tail_;
  int nbSize_; //number of neighbors

  nsaddr_t my_id_; //my id 
  double my_x_;    //my geo info
  double my_y_;    

  double my_NAC_; //my NAC
   
  double hello_p_; //hello发送间隔
  double w_;	//窗口值
  
  



  //计算r(t)值方法
  double ETX(double, int);
  //find the entry in neighbor list according to the provided id
  struct gpsr_neighbor *getnb(nsaddr_t);

  //delete the entry in neighbors list according to the provided id
  void delnb(nsaddr_t); 

  //delete the entry directly
  void delnb(struct gpsr_neighbor *);

  //delete all the entries time out
  void delalltimeout();

  //for a given neighbor list (such as planarized neighbors), return size
  int num_of_neighbors(struct gpsr_neighbor*);
  
  /* functions used for perimeter routing calculation */
  struct gpsr_neighbor *gg_planarize();  //GG planarize
  struct gpsr_neighbor *rng_planarize(); //RNG planarize
  void free_neighbors(struct gpsr_neighbor*); //free the given neighbor list

  double angle(double, double, double, double);
                                        //absolute angle of the given line
                                        //relative angle calculation based
  
  int intersect(nsaddr_t, double, double, double, double);
  //check the 2 lines are intersected locally
  
public:
  GPSRNeighbors();
  ~GPSRNeighbors();

  //using to update location information of myself
  void myinfo(nsaddr_t, double, double, double, double, double);
  //return the number of neighbors
  int nbsize();
  double xl[10];
  double yl[10];
  //calculate the distance between (x1, y1, x2, y2)
  double getdis(double, double, double, double);

  //add a possible new neighbor
  void newNB(nsaddr_t, double, double, double);

  //计算NAC值方法
  double NAC_count();

  //根据etx和距离值确定下一跳
  nsaddr_t lz_nexthop(double, double);
  //decide the next hop based on the destination(x, y)
  nsaddr_t gf_nexthop(double, double); //greedy forwarding mode
  nsaddr_t peri_nexthop(int, nsaddr_t, 	double, double, 
			double, double);//perimeter mode
  nsaddr_t setPrior();//设置邻节点的优先级用于选择侯选节点.
  nsaddr_t intersec_detect(double, double);
  int locDetection(double, double);//检测节点是否处在交叉路口处
  void update_count();//更新邻节点列表中的count数,使count数始终为窗口值.

  //dump the neighbor list 
  void dump();
};

#endif 
