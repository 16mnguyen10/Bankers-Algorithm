/*
    Michael Nguyen

    CPSC 351
    4/22/2021

    Bankers Algorithm
*/

#ifndef customer_h
#define customer_h

#include <random>
#include "utils.h"
#include "bank.h"

class bank;

extern pthread_mutex_t mutex_;


class customer {
public:
//  customer() = default;
  customer(int index, const ext_vector<int>& allocate, const ext_vector<int>& maximum,
                  bank* bank_=nullptr)
  : idx(index), alloc(allocate), max_(maximum), need(max_ - alloc), pBank(bank_) { }
  
  int get_id() const { return idx; }
  pthread_t* get_threadid() { return &thread_id; }
  
  bank* get_bank() const { return pBank; }
  ext_vector<int> get_max() const { return max_; }
  
  bool needs_met() const { return alloc == max_; }
  bool needs_overflow(const ext_vector<int>& req) const { return req > need; }

  void alloc_req(  const ext_vector<int>& req) { alloc += req;  need -= req; }
  void dealloc_req(const ext_vector<int>& req) { alloc -= req;  need += req; }
  
  void release_all_resources() {
    ext_vector<int> zero(alloc.size(), 0);
    alloc = max_ = need = zero;
    std::cout << "Customer p#" << idx << " has released all resources and is shutting down\n";
  }

  void show() const {
    pthread_mutex_lock(&mutex_);
    std::cout << "P#" << idx << "    " << alloc << "    " << max_ << "    " << need << "\n";
    pthread_mutex_unlock(&mutex_);
  }

  friend std::ostream& operator<<(std::ostream& os, const customer& c) {
    c.show();
    return os;
  }
  
  ext_vector<int> create_req() 
  { 
      ext_vector<int> req;
      
      for (size_t i = 0; i < alloc.size(); i++)
      {
          if (need[i] == 0)
          {
              req.push_back(0);
              continue;
          }

          int val = (rand() % need[i]) + 1;
          req.push_back(val);
      }

      return req;
  }
  
private:
  int idx;
  ext_vector<int> alloc;
  ext_vector<int> max_;
  ext_vector<int> need;
  pthread_t        thread_id;
  
  bank* pBank;
};

#endif /* customer_h */
