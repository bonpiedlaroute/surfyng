/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef deposit_service_H
#define deposit_service_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "depositservice_types.h"



#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class deposit_serviceIf {
 public:
  virtual ~deposit_serviceIf() {}
  virtual void announce_deposit(DepositResult& _return, const std::string& user_id, const std::string& data) = 0;
  virtual void delete_announce(DepositResult& _return, const std::string& user_id, const std::string& announce_id) = 0;
  virtual void fetch_user_announces(std::vector<std::map<std::string, std::string> > & _return, const std::string& user_id) = 0;
};

class deposit_serviceIfFactory {
 public:
  typedef deposit_serviceIf Handler;

  virtual ~deposit_serviceIfFactory() {}

  virtual deposit_serviceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(deposit_serviceIf* /* handler */) = 0;
};

class deposit_serviceIfSingletonFactory : virtual public deposit_serviceIfFactory {
 public:
  deposit_serviceIfSingletonFactory(const boost::shared_ptr<deposit_serviceIf>& iface) : iface_(iface) {}
  virtual ~deposit_serviceIfSingletonFactory() {}

  virtual deposit_serviceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(deposit_serviceIf* /* handler */) {}

 protected:
  boost::shared_ptr<deposit_serviceIf> iface_;
};

class deposit_serviceNull : virtual public deposit_serviceIf {
 public:
  virtual ~deposit_serviceNull() {}
  void announce_deposit(DepositResult& /* _return */, const std::string& /* user_id */, const std::string& /* data */) {
    return;
  }
  void delete_announce(DepositResult& /* _return */, const std::string& /* user_id */, const std::string& /* announce_id */) {
    return;
  }
  void fetch_user_announces(std::vector<std::map<std::string, std::string> > & /* _return */, const std::string& /* user_id */) {
    return;
  }
};

typedef struct _deposit_service_announce_deposit_args__isset {
  _deposit_service_announce_deposit_args__isset() : user_id(false), data(false) {}
  bool user_id :1;
  bool data :1;
} _deposit_service_announce_deposit_args__isset;

class deposit_service_announce_deposit_args {
 public:

  deposit_service_announce_deposit_args(const deposit_service_announce_deposit_args&);
  deposit_service_announce_deposit_args& operator=(const deposit_service_announce_deposit_args&);
  deposit_service_announce_deposit_args() : user_id(), data() {
  }

  virtual ~deposit_service_announce_deposit_args() throw();
  std::string user_id;
  std::string data;

  _deposit_service_announce_deposit_args__isset __isset;

  void __set_user_id(const std::string& val);

  void __set_data(const std::string& val);

  bool operator == (const deposit_service_announce_deposit_args & rhs) const
  {
    if (!(user_id == rhs.user_id))
      return false;
    if (!(data == rhs.data))
      return false;
    return true;
  }
  bool operator != (const deposit_service_announce_deposit_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const deposit_service_announce_deposit_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class deposit_service_announce_deposit_pargs {
 public:


  virtual ~deposit_service_announce_deposit_pargs() throw();
  const std::string* user_id;
  const std::string* data;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _deposit_service_announce_deposit_result__isset {
  _deposit_service_announce_deposit_result__isset() : success(false) {}
  bool success :1;
} _deposit_service_announce_deposit_result__isset;

class deposit_service_announce_deposit_result {
 public:

  deposit_service_announce_deposit_result(const deposit_service_announce_deposit_result&);
  deposit_service_announce_deposit_result& operator=(const deposit_service_announce_deposit_result&);
  deposit_service_announce_deposit_result() {
  }

  virtual ~deposit_service_announce_deposit_result() throw();
  DepositResult success;

  _deposit_service_announce_deposit_result__isset __isset;

  void __set_success(const DepositResult& val);

  bool operator == (const deposit_service_announce_deposit_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const deposit_service_announce_deposit_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const deposit_service_announce_deposit_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _deposit_service_announce_deposit_presult__isset {
  _deposit_service_announce_deposit_presult__isset() : success(false) {}
  bool success :1;
} _deposit_service_announce_deposit_presult__isset;

class deposit_service_announce_deposit_presult {
 public:


  virtual ~deposit_service_announce_deposit_presult() throw();
  DepositResult* success;

  _deposit_service_announce_deposit_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _deposit_service_delete_announce_args__isset {
  _deposit_service_delete_announce_args__isset() : user_id(false), announce_id(false) {}
  bool user_id :1;
  bool announce_id :1;
} _deposit_service_delete_announce_args__isset;

class deposit_service_delete_announce_args {
 public:

  deposit_service_delete_announce_args(const deposit_service_delete_announce_args&);
  deposit_service_delete_announce_args& operator=(const deposit_service_delete_announce_args&);
  deposit_service_delete_announce_args() : user_id(), announce_id() {
  }

  virtual ~deposit_service_delete_announce_args() throw();
  std::string user_id;
  std::string announce_id;

  _deposit_service_delete_announce_args__isset __isset;

  void __set_user_id(const std::string& val);

  void __set_announce_id(const std::string& val);

  bool operator == (const deposit_service_delete_announce_args & rhs) const
  {
    if (!(user_id == rhs.user_id))
      return false;
    if (!(announce_id == rhs.announce_id))
      return false;
    return true;
  }
  bool operator != (const deposit_service_delete_announce_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const deposit_service_delete_announce_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class deposit_service_delete_announce_pargs {
 public:


  virtual ~deposit_service_delete_announce_pargs() throw();
  const std::string* user_id;
  const std::string* announce_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _deposit_service_delete_announce_result__isset {
  _deposit_service_delete_announce_result__isset() : success(false) {}
  bool success :1;
} _deposit_service_delete_announce_result__isset;

class deposit_service_delete_announce_result {
 public:

  deposit_service_delete_announce_result(const deposit_service_delete_announce_result&);
  deposit_service_delete_announce_result& operator=(const deposit_service_delete_announce_result&);
  deposit_service_delete_announce_result() {
  }

  virtual ~deposit_service_delete_announce_result() throw();
  DepositResult success;

  _deposit_service_delete_announce_result__isset __isset;

  void __set_success(const DepositResult& val);

  bool operator == (const deposit_service_delete_announce_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const deposit_service_delete_announce_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const deposit_service_delete_announce_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _deposit_service_delete_announce_presult__isset {
  _deposit_service_delete_announce_presult__isset() : success(false) {}
  bool success :1;
} _deposit_service_delete_announce_presult__isset;

class deposit_service_delete_announce_presult {
 public:


  virtual ~deposit_service_delete_announce_presult() throw();
  DepositResult* success;

  _deposit_service_delete_announce_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _deposit_service_fetch_user_announces_args__isset {
  _deposit_service_fetch_user_announces_args__isset() : user_id(false) {}
  bool user_id :1;
} _deposit_service_fetch_user_announces_args__isset;

class deposit_service_fetch_user_announces_args {
 public:

  deposit_service_fetch_user_announces_args(const deposit_service_fetch_user_announces_args&);
  deposit_service_fetch_user_announces_args& operator=(const deposit_service_fetch_user_announces_args&);
  deposit_service_fetch_user_announces_args() : user_id() {
  }

  virtual ~deposit_service_fetch_user_announces_args() throw();
  std::string user_id;

  _deposit_service_fetch_user_announces_args__isset __isset;

  void __set_user_id(const std::string& val);

  bool operator == (const deposit_service_fetch_user_announces_args & rhs) const
  {
    if (!(user_id == rhs.user_id))
      return false;
    return true;
  }
  bool operator != (const deposit_service_fetch_user_announces_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const deposit_service_fetch_user_announces_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class deposit_service_fetch_user_announces_pargs {
 public:


  virtual ~deposit_service_fetch_user_announces_pargs() throw();
  const std::string* user_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _deposit_service_fetch_user_announces_result__isset {
  _deposit_service_fetch_user_announces_result__isset() : success(false) {}
  bool success :1;
} _deposit_service_fetch_user_announces_result__isset;

class deposit_service_fetch_user_announces_result {
 public:

  deposit_service_fetch_user_announces_result(const deposit_service_fetch_user_announces_result&);
  deposit_service_fetch_user_announces_result& operator=(const deposit_service_fetch_user_announces_result&);
  deposit_service_fetch_user_announces_result() {
  }

  virtual ~deposit_service_fetch_user_announces_result() throw();
  std::vector<std::map<std::string, std::string> >  success;

  _deposit_service_fetch_user_announces_result__isset __isset;

  void __set_success(const std::vector<std::map<std::string, std::string> > & val);

  bool operator == (const deposit_service_fetch_user_announces_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const deposit_service_fetch_user_announces_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const deposit_service_fetch_user_announces_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _deposit_service_fetch_user_announces_presult__isset {
  _deposit_service_fetch_user_announces_presult__isset() : success(false) {}
  bool success :1;
} _deposit_service_fetch_user_announces_presult__isset;

class deposit_service_fetch_user_announces_presult {
 public:


  virtual ~deposit_service_fetch_user_announces_presult() throw();
  std::vector<std::map<std::string, std::string> > * success;

  _deposit_service_fetch_user_announces_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class deposit_serviceClient : virtual public deposit_serviceIf {
 public:
  deposit_serviceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  deposit_serviceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void announce_deposit(DepositResult& _return, const std::string& user_id, const std::string& data);
  void send_announce_deposit(const std::string& user_id, const std::string& data);
  void recv_announce_deposit(DepositResult& _return);
  void delete_announce(DepositResult& _return, const std::string& user_id, const std::string& announce_id);
  void send_delete_announce(const std::string& user_id, const std::string& announce_id);
  void recv_delete_announce(DepositResult& _return);
  void fetch_user_announces(std::vector<std::map<std::string, std::string> > & _return, const std::string& user_id);
  void send_fetch_user_announces(const std::string& user_id);
  void recv_fetch_user_announces(std::vector<std::map<std::string, std::string> > & _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class deposit_serviceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<deposit_serviceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (deposit_serviceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_announce_deposit(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_delete_announce(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_fetch_user_announces(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  deposit_serviceProcessor(boost::shared_ptr<deposit_serviceIf> iface) :
    iface_(iface) {
    processMap_["announce_deposit"] = &deposit_serviceProcessor::process_announce_deposit;
    processMap_["delete_announce"] = &deposit_serviceProcessor::process_delete_announce;
    processMap_["fetch_user_announces"] = &deposit_serviceProcessor::process_fetch_user_announces;
  }

  virtual ~deposit_serviceProcessor() {}
};

class deposit_serviceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  deposit_serviceProcessorFactory(const ::boost::shared_ptr< deposit_serviceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< deposit_serviceIfFactory > handlerFactory_;
};

class deposit_serviceMultiface : virtual public deposit_serviceIf {
 public:
  deposit_serviceMultiface(std::vector<boost::shared_ptr<deposit_serviceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~deposit_serviceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<deposit_serviceIf> > ifaces_;
  deposit_serviceMultiface() {}
  void add(boost::shared_ptr<deposit_serviceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void announce_deposit(DepositResult& _return, const std::string& user_id, const std::string& data) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->announce_deposit(_return, user_id, data);
    }
    ifaces_[i]->announce_deposit(_return, user_id, data);
    return;
  }

  void delete_announce(DepositResult& _return, const std::string& user_id, const std::string& announce_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->delete_announce(_return, user_id, announce_id);
    }
    ifaces_[i]->delete_announce(_return, user_id, announce_id);
    return;
  }

  void fetch_user_announces(std::vector<std::map<std::string, std::string> > & _return, const std::string& user_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->fetch_user_announces(_return, user_id);
    }
    ifaces_[i]->fetch_user_announces(_return, user_id);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class deposit_serviceConcurrentClient : virtual public deposit_serviceIf {
 public:
  deposit_serviceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  deposit_serviceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void announce_deposit(DepositResult& _return, const std::string& user_id, const std::string& data);
  int32_t send_announce_deposit(const std::string& user_id, const std::string& data);
  void recv_announce_deposit(DepositResult& _return, const int32_t seqid);
  void delete_announce(DepositResult& _return, const std::string& user_id, const std::string& announce_id);
  int32_t send_delete_announce(const std::string& user_id, const std::string& announce_id);
  void recv_delete_announce(DepositResult& _return, const int32_t seqid);
  void fetch_user_announces(std::vector<std::map<std::string, std::string> > & _return, const std::string& user_id);
  int32_t send_fetch_user_announces(const std::string& user_id);
  void recv_fetch_user_announces(std::vector<std::map<std::string, std::string> > & _return, const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif



#endif
