/*
    Michael Nguyen
    
    CPSC 351
    4/22/2021

    Bankers Algorithm
*/

#ifndef bank_h
#define bank_h

#include <queue>
#include "customer.h"

class bank {
public:
  bank() = default;
  bank(const ext_vector<int>& available) : avail(available), customers() { }
  
  ext_vector<int> get_avail() const { return avail; }
  bool is_avail(const ext_vector<int>& req) const { return req < avail; }
  
  bool is_safe(int id, const ext_vector<int>& req) 
  { 
      customer* holder = customers[id];
      std::queue<int> safeQ;
      std::queue<customer*> customerQ;
      ext_vector<int> saveAv = avail;
      ext_vector<int> max;

      bool allocation = true;

      // Case 1: If not available
      if (is_avail(req) == false)
          return false;
      // Casse 2: Ensure limited set of resources
      if (holder->needs_exceeded(req))
          return false;

      for (customer* c : customers)
      {
          if ((holder != c) && !c->needs_met())
          {
              customerQ.push(c);
          }
      }
      withdraw_resources(req);

      // If not finshed and allocation still needs to finish
      while (customer.empty() == false && allocation == true)
      {
          allocation = false;
          customer* c = customerQ.front();
          customerQ.pop();

          max = c->get_max();

          if (is_avail(max))
          {
              deposit_resources(max);
              safeQ.push(c->get_id());
              allocation = true;
          }
          else
              customerQ.push(c);
      }

      avail = saveAV;
      return customerQ.empty();
  }

// Completed
  bool req_approved(int id, const ext_vector<int>& req) 
  { 
      std::cout << "Request: " << req << endl;
      std::cout << "Available? " << Utils::yes_or_no(is_avail(req)) << endl;
      std::cout << "Safe? " << Utils::yes_or_no(is_safe(id, req)) << endl;

      if (is_safe(id, req))
      {
          std::cout << "APPROVED\n";
          return true;
      }
      else
      {
          std::count << "DENIED\n";
          return false;
      }
  }
  
  // Completed
  void add_customer(CustomerExample* c) { customers.push_back(c); }
  
  // Completed
  void withdraw_resources(const ext_vector<int>& req) {
    if (!is_avail(req)) {
      pthread_mutex_lock(&mutex_);
      std::cerr << "WARNING: req: " << req << " is not available for withdrawing\n";
      pthread_mutex_unlock(&mutex_);
      return;
    }
    if (is_avail(req)) { avail -= req; }
  }
  
  // Completed
  void deposit_resources(const ext_vector<int>& req) { avail += req; }

  // Completed
  ext_vector<CustomerExample*> get_customers() const { return customers; }
  
  // Completed
  void show() const {
    pthread_mutex_lock(&mutex_);
    std::cout << "avail: [" << avail << "]\n";
    pthread_mutex_unlock(&mutex_);
    
    for (CustomerExample* c : customers) {
      c->show();
    }
    std::cout << "\n";
  }
  
  // Completed
  friend std::ostream& operator<<(std::ostream& os, const bank& be) 
  {
    be.show();
    return os;
  }

  // Completed
private:
  ext_vector<int> avail;
  ext_vector<CustomerExample*> customers;
};

#endif /* bank_h */
