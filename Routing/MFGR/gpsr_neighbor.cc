/*
*	desigened by pfhuang
*/

#include "gpsr_neighbor.h"

#define PI 3.141593

#define MAX(a, b) (a>=b?a:b)
#define MIN(a, b) (a>=b?b:a)

GPSRNeighbors::GPSRNeighbors(){
  my_id_ = -1;
  my_x_ = 0.0;
  my_y_ = 0.0;
  my_NAC_ = 0.0;
  hello_p_ = 1.0;
  w_ = 10.0;
  head_ = tail_ = NULL;
  nbSize_ = 0;

}

GPSRNeighbors::~GPSRNeighbors(){
  struct gpsr_neighbor *temp = head_;
  while(temp){
    temp = temp->next_;
    free(head_);
    head_ = temp;
  }
}

double 
GPSRNeighbors::getdis(double ax, double ay, double bx, double by){ //在某次调用这个函数的时候出错
  double tempx = ax - bx;
  double tempy = ay - by;

  tempx = tempx * tempx;
  tempy = tempy * tempy;

  double result = sqrt(tempx + tempy);
  return result;
}

int
GPSRNeighbors::nbsize(){
  return nbSize_;
}

void
GPSRNeighbors::myinfo(nsaddr_t mid, double mx, double my, double mNAC, double hello_p, double w){
  my_id_ = mid;
  my_x_ = mx;
  my_y_ = my;
  my_NAC_ = mNAC;
  hello_p_ = hello_p;
  w_ = w;
}

struct gpsr_neighbor*
GPSRNeighbors::getnb(nsaddr_t nid){
  struct gpsr_neighbor *temp = head_;
  while(temp){
    if(temp->id_ == nid){
      if((GPSR_CURRENT - temp->ts_) < DEFAULT_GPSR_TIMEOUT)
	return temp;
      else {
	delnb(temp); //if this entry expire, delete it and return NULL
	return NULL;
      }
      return temp;
    }
    temp = temp->next_;
  }
  return NULL;
}
/*
void GPSRNeighbors::update_count(){
  struct gpsr_neighbor *temp = head_;
  double current = GPSR_CURRENT;
  while(temp){
    if(current-temp->t1_>w_){
      
      if(temp->count_ > 0)
      temp->count_=temp->count_-1;
      else
	temp->count_ = temp->count_;
      
      temp->t1_ = temp->ts_-w_;
    }
      temp = temp->next_;
  }

}
*/

void GPSRNeighbors::update_count(){
  struct gpsr_neighbor *temp = head_;
  double current = GPSR_CURRENT;
  while(temp){
    if(current-temp->t1_>w_){
      
//      if(temp->count_ > 0)
//      temp->count_=temp->count_-1;
 //     else
	temp->count_ = temp->count_;
      
      temp->t1_ = temp->ts_-w_;
    }
      temp = temp->next_;
  }

}




double GPSRNeighbors::ETX(double t1, int count){
  double current = GPSR_CURRENT;
  double r = 0;
  double ETX = 1;
  int count_ = count;
//  int count_ = count - 1;
  if(current-t1>=w_){
    r=count_/(w_/hello_p_);
  }
  else if(1<current-t1<w_)
  {
    r=count_;
  }
  else
  {
    double dt = current-t1;
    r=count_/(dt/hello_p_);
  }
  
  ETX = 1/(r);
  return ETX;
}


/*

double GPSRNeighbors::ETX(double t1, int count){
  double current = GPSR_CURRENT;
  double r = 0;
  double ETX = 1;
  int count_ = count;
//  int count_ = count - 1;
  if(current-t1>=w_){
    r=count_/(w_/hello_p_);
  }
  else if(1<current-t1<w_)
  {
    r=count_
  }
 {
    double dt = current-t1;
    if(count_==0)
      count_ = 1;
//    if(dt<hello_p_)
//      dt = hello_p_;
    r=count_/(dt/hello_p_);
  }
  
  ETX = 1/(r);
  return ETX;
}


*/

void 
GPSRNeighbors::newNB(nsaddr_t nid, double nx, double ny, double nNAC){
  delalltimeout();
  struct gpsr_neighbor *temp = getnb(nid);

  if(temp==NULL){ //it is a new neighbor
    temp=(struct gpsr_neighbor*)malloc(sizeof(struct gpsr_neighbor));
    temp->id_ = nid;
    temp->x_ = nx;
    temp->y_ = ny;
	temp->NAC_ = nNAC;
    temp->ts_ = GPSR_CURRENT;
    temp->t1_ = GPSR_CURRENT;
    temp->count_ = 1;
    temp->next_ = temp->prev_ = NULL;

    if(tail_ == NULL){ //the list now is empty
      head_ = tail_  = temp;
      nbSize_ = 1;
    }
    else { //now the neighbors list is not empty
      tail_->next_ = temp;
      temp->prev_ = tail_;
      tail_ = temp;
      nbSize_++;
    }
  }
  else { //it is a already known neighbor
    temp->ts_ = GPSR_CURRENT;
    temp->x_ = nx; //the updating of location is allowed
    temp->y_ = ny;
	temp->NAC_ = nNAC;
//    if(temp->ts_-temp->t1_<w_)
    temp->count_++;
  }
}

void
GPSRNeighbors::delnb(nsaddr_t nid){
  struct gpsr_neighbor *temp = getnb(nid);
  if(temp==NULL) return;
  else delnb(temp);
}

void
GPSRNeighbors::delnb(struct gpsr_neighbor *nb){
  struct gpsr_neighbor *preffix = nb->prev_;

  if(preffix == NULL){
    head_ = nb->next_;
    nb->next_ = NULL;

    if(head_ == NULL) 
      tail_ = NULL;
    else head_->prev_ = NULL;

    free(nb);
  }
  else {
    preffix->next_ = nb->next_;
    nb->prev_ = NULL;
    if(preffix->next_ == NULL)
      tail_ = preffix;
    else (preffix->next_)->prev_ = preffix;
    free(nb);
  }
  
  nbSize_--;
  
}

void
GPSRNeighbors::delalltimeout(){
  struct gpsr_neighbor *temp = head_;
  struct gpsr_neighbor *dd;
  while(temp){
    if((GPSR_CURRENT - temp->ts_) > 2.0){
      dd = temp;
      temp = temp->next_;
      delnb(dd);
    }
    else temp = temp->next_;
  }
  
}



double GPSRNeighbors::NAC_count(){
	struct gpsr_neighbor *temp = head_;
	double NAC = 0;
	double count = 0;
	double hello_p_ = 1.0;
	double w_ = 10.0;
	double current;
	double r = 0;
	
	current = GPSR_CURRENT;
	while (temp){

	
		if (current - temp->t1_  > w_){
			r = r + temp->count_ / (current - temp->t1_);
		}
		else{

			//double dt = current - temp->t1_;
			//if (temp->count_ == 0)
			//	temp->count_ = 1;
			//if (dt<hello_p_)
			//	dt = hello_p_;

			r = r + temp->count_ / (w_);
		}
		
	    
	    //if ((gpsr_current - temp->t1_) <= 2.0)
		//{

		//	//		double dt = gpsr_current - temp->t1_;
		//	//		if (dt != 0)
		//	//	count = temp->count_;
		//	count = count + 1;
		//}
	
		temp = temp->next_;
	}
	NAC = r;
	return NAC;
}

/*
double GPSRNeighbors::ETX(double t1, int count){
	double current = GPSR_CURRENT;
	double r = 0;
	double ETX = 1;
	int count_ = count - 1;
	if (current - t1>w_){
		r = count_ / (w_ / hello_p_);
	}
	else{
		double dt = current - t1;
		if (count_ == 0)
			count_ = 1;
		if (dt<hello_p_)
			dt = hello_p_;

		r = count_ / (dt / hello_p_);
	}

	ETX = 1 / r;
	return ETX;
}
*/


nsaddr_t
GPSRNeighbors::gf_nexthop(double dx, double dy){
  struct gpsr_neighbor *temp = head_;
  //initializing the minimal distance as my distance to sink
  double mindis =getdis(my_x_, my_y_, dx, dy); 
  nsaddr_t nexthop = -1; //the nexthop result 

  while(temp){
    double tempdis = getdis(temp->x_, temp->y_, dx, dy);
    if(tempdis < mindis){
      mindis = tempdis;
      nexthop = temp->id_;
    }
    temp = temp->next_;
  }

  return nexthop;
}



//add NAC factor
nsaddr_t
GPSRNeighbors::lz_nexthop(double dx, double dy){
  struct gpsr_neighbor *temp = head_;
  double mydis = getdis(my_x_, my_y_, dx, dy);
  double mindis = getdis(my_x_, my_x_, dx, dy);
  nsaddr_t nexthop = -1;
  double EOA_max = 0;
  double nNAC_max = 0;
  double score_max = 0;
  double score_temp = 0;
  while(temp){
    double tempdis = getdis(temp->x_, temp->y_, dx, dy);
    if(tempdis==0)
      tempdis==1;
    double ETX_ = ETX(temp->t1_, temp->count_);
	double EOA = (mydis - tempdis) / (ETX_);//防止ETX为0的情况.

	//double EOA = 1/ ETX_;//防止ETX为0的情况.
	score_temp = ((mydis - tempdis)*3)/mydis + EOA*4 + temp->NAC_*3;
	//if (tempdis<mindis&&EOA>EOA_max&&nNAC_max<temp->NAC_){
	if (score_temp >= score_max){
	mindis = tempdis;
	nNAC_max = temp->NAC_;
	EOA_max = EOA;
	score_max = score_temp;
	nexthop = temp->id_;
    }
      temp = temp->next_;
  }  
  return nexthop;
}



struct gpsr_neighbor *
GPSRNeighbors::gg_planarize(){
    struct gpsr_neighbor *temp, *result, *index;
    index = head_;
    result = NULL;
    
    while(index){
      double midpx = my_x_ + (index->x_ - my_x_)/2.0;
      double midpy = my_y_ + (index->y_ - my_y_)/2.0;
      double mdis = getdis(my_x_, my_y_, midpx, midpy);
      
      temp = head_;
      while(temp){
	if(temp->id_ != index->id_){
	  double tempdis = getdis(midpx, midpy, temp->x_, temp->y_);
	  if(tempdis < mdis) break;
	}
	temp=temp->next_;
      }
      
      if(temp==NULL){
	temp = (struct gpsr_neighbor*)malloc(sizeof(struct gpsr_neighbor));
	temp->id_ = index->id_;
	temp->x_ = index->x_;
	temp->y_ = index->y_;
	temp->next_ = result;
	temp->prev_ = NULL;
	if(result) result->prev_ = temp;
	result = temp;
      }
      
      index=index->next_;
    }
  
    return result;

}

struct gpsr_neighbor *
GPSRNeighbors::rng_planarize(){
    struct gpsr_neighbor *temp, *result, *index;
    index = head_;
    result = NULL;
    
    while(index){
      double mdis = getdis(my_x_, my_y_, index->x_, index->y_);
      
      temp = head_;
      while(temp){
	if(temp->id_ != index->id_){
	  double tempdis1 = getdis(my_x_, my_y_, temp->x_, temp->y_);
	  double tempdis2 = getdis(index->x_, index->y_, temp->x_, temp->y_);
	  if(tempdis1 < mdis && tempdis2 < mdis) break;
	}
	temp=temp->next_;
      }
      
      if(temp==NULL){
	temp = (struct gpsr_neighbor*)malloc(sizeof(struct gpsr_neighbor));
	temp->id_ = index->id_;
	temp->x_ = index->x_;
	temp->y_ = index->y_;
	temp->next_ = result;
	temp->prev_ = NULL;
	if(result) result->prev_ = temp;
	result = temp;
      }
      
      index=index->next_;
    }
  
    return result;

}


double
GPSRNeighbors::angle(double x1, double y1, double x2, double y2){
  double line_len = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

  double sin_theta, cos_theta;
  double theta;

  if(line_len == 0.0){
//    printf("2 nodes are the same\n");
    return -1.0;
  }

  sin_theta = (y2-y1)/line_len;
  cos_theta = (x2-x1)/line_len;

  theta = acos(cos_theta);
  
  if(sin_theta<0){
    theta = 2*PI - theta;
  }

  return theta;
}


/* To check the line from me to theother, and the line from source
 * and destination is intersecting each other or not
 * Note: 2 line segments intersects each other if they have a common 
 *       point, BUT here, if the common point is the end point, 
 *       we don't count it.
 */
int
GPSRNeighbors::intersect(nsaddr_t theother, double sx, double sy,
			 double dx, double dy){
  //line 1 (x1,y1)--(x2,y2) is the segment
  //line 2 (x3,y3)--(x4,y4) is the xD 
  struct gpsr_neighbor *other = getnb(theother);
  double tdx, tdy ,tdz;
  if(other==NULL){
      MobileNode *other_;
      other_ = (MobileNode*)Node::get_node_by_address(theother);
            
            other_->getLoc(&tdx,&tdy,&tdz);
  }
  
  double x1 = my_x_; 
  double y1 = my_y_;
  double x2 = tdx;
  double y2 = tdy;
  double x3 = sx;
  double y3 = sy;
  double x4 = dx;
  double y4 = dy;

  double a1 = y2 - y1;
  double b1 = x1 - x2;
  double c1 = x2*y1 - x1*y2;

  double a2 = y4 - y3;
  double b2 = x3 - x4;
  double c2 = x4*y3 - x3*y4;

  double denom = a1*b2 - a2*b1;

  double x, y; //the result;

  if(denom == 0) {
    return 0; //parallel lines;
  }

  x = (b1*c2 - b2*c1)/denom;
  y = (a2*c1 - a1*c2)/denom;

  if(x > MIN(x1, x2) && x < MAX(x1, x2) &&
     x > MIN(x3, x4) && x < MAX(x3, x4))
    return 1;       
  else return 0;
}


int
GPSRNeighbors::num_of_neighbors(struct gpsr_neighbor *nblist){
  struct gpsr_neighbor *temp = nblist;
  int counter = 0;
  while(temp){
    counter++;
    temp = temp->next_;
  }
  return counter;
}

void
GPSRNeighbors::free_neighbors(struct gpsr_neighbor *nblist){
  struct gpsr_neighbor *temp, *head;
  head = nblist;
  while(head){
    temp = head;
    head = head->next_;
    free(temp);
  }
}

nsaddr_t
GPSRNeighbors::peri_nexthop(int type_, nsaddr_t last,
			    double sx, double sy,
			    double dx, double dy){
  struct gpsr_neighbor *planar_neighbors, *temp;
  double alpha, minangle;
  nsaddr_t nexthop=-1;
  
  if(type_){//GG planarizing
    planar_neighbors = gg_planarize();
  }else {//RNG planarizing
    planar_neighbors = rng_planarize();
  }

  if(last>-1){
      MobileNode *lastnb;
      lastnb = (MobileNode*)Node::get_node_by_address(last);
            double tdx, tdy ,tdz;
            lastnb->getLoc(&tdx,&tdy,&tdz);

//    if(lastnb == NULL) 
    alpha = angle(my_x_, my_y_, tdx, tdy);
  }
  else 
    alpha = angle(my_x_, my_y_, dx, dy); 
  if(num_of_neighbors(planar_neighbors)<1)
    temp = head_ ;
  else
  temp = planar_neighbors;
  
  minangle = angle(my_x_,my_y_,temp->x_,temp->y_);
  
  minangle = minangle - alpha;

  if(minangle<0.0){
    minangle = 2*PI + minangle ;
  }
  
  nexthop = temp->id_;
  
  while(temp){
    if(temp->id_ != last){
      double delta;
      delta = angle(my_x_, my_y_, temp->x_, temp->y_);
      delta = delta - alpha;
      if(delta < 0.0) {
	delta = 2*PI + delta;
      }
      
      if(delta < minangle){
	minangle = delta;
	nexthop = temp->id_;
      }
    }
    temp = temp->next_;
  }

  if(num_of_neighbors(planar_neighbors) > 1 &&
       intersect(nexthop, sx, sy, dx, dy)){
      free_neighbors(planar_neighbors);
//      return  peri_nexthop(type_, nexthop, sx, sy, dx, dy);
	return nexthop ;
  }
  return nexthop;
}

void
GPSRNeighbors::dump(){
  delalltimeout();
  
  FILE *fp = fopen(NB_TRACE_FILE, "a+"); 

  struct gpsr_neighbor *temp = head_;
  fprintf(fp, "%d:\t", my_id_);
  while(temp){
    fprintf(fp, "%d ", temp->id_);
    temp = temp->next_;
  }
  fprintf(fp,"\n");
  fclose(fp);
}


