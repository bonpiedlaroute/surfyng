/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Sf_service_H
#define Sf_service_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "sf_services_types.h"



#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class Sf_serviceIf {
 public:
  virtual ~Sf_serviceIf() {}
  virtual void getproperties(std::vector<Sf_struct> & _return) = 0;
};

class Sf_serviceIfFactory {
 public:
  typedef Sf_serviceIf Handler;

  virtual ~Sf_serviceIfFactory() {}

  virtual Sf_serviceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(Sf_serviceIf* /* handler */) = 0;
};

class Sf_serviceIfSingletonFactory : virtual public Sf_serviceIfFactory {
 public:
  Sf_serviceIfSingletonFactory(const boost::shared_ptr<Sf_serviceIf>& iface) : iface_(iface) {}
  virtual ~Sf_serviceIfSingletonFactory() {}

  virtual Sf_serviceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(Sf_serviceIf* /* handler */) {}

 protected:
  boost::shared_ptr<Sf_serviceIf> iface_;
};

class Sf_serviceNull : virtual public Sf_serviceIf {
 public:
  virtual ~Sf_serviceNull() {}
  void getproperties(std::vector<Sf_struct> & /* _return */) {
    return;
  }
};


class Sf_service_getproperties_args {
 public:

  Sf_service_getproperties_args(const Sf_service_getproperties_args&);
  Sf_service_getproperties_args& operator=(const Sf_service_getproperties_args&);
  Sf_service_getproperties_args() {
  }

  virtual ~Sf_service_getproperties_args() throw();

  bool operator == (const Sf_service_getproperties_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Sf_service_getproperties_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Sf_service_getproperties_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Sf_service_getproperties_pargs {
 public:


  virtual ~Sf_service_getproperties_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Sf_service_getproperties_result__isset {
  _Sf_service_getproperties_result__isset() : success(false) {}
  bool success :1;
} _Sf_service_getproperties_result__isset;

class Sf_service_getproperties_result {
 public:

  Sf_service_getproperties_result(const Sf_service_getproperties_result&);
  Sf_service_getproperties_result& operator=(const Sf_service_getproperties_result&);
  Sf_service_getproperties_result() {
  }

  virtual ~Sf_service_getproperties_result() throw();
  std::vector<Sf_struct>  success;

  _Sf_service_getproperties_result__isset __isset;

  void __set_success(const std::vector<Sf_struct> & val);

  bool operator == (const Sf_service_getproperties_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Sf_service_getproperties_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Sf_service_getproperties_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Sf_service_getproperties_presult__isset {
  _Sf_service_getproperties_presult__isset() : success(false) {}
  bool success :1;
} _Sf_service_getproperties_presult__isset;

class Sf_service_getproperties_presult {
 public:


  virtual ~Sf_service_getproperties_presult() throw();
  std::vector<Sf_struct> * success;

  _Sf_service_getproperties_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class Sf_serviceClient : virtual public Sf_serviceIf {
 public:
  Sf_serviceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  Sf_serviceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void getproperties(std::vector<Sf_struct> & _return);
  void send_getproperties();
  void recv_getproperties(std::vector<Sf_struct> & _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class Sf_serviceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<Sf_serviceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (Sf_serviceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_getproperties(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  Sf_serviceProcessor(boost::shared_ptr<Sf_serviceIf> iface) :
    iface_(iface) {
    processMap_["getproperties"] = &Sf_serviceProcessor::process_getproperties;
  }

  virtual ~Sf_serviceProcessor() {}
};

class Sf_serviceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  Sf_serviceProcessorFactory(const ::boost::shared_ptr< Sf_serviceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< Sf_serviceIfFactory > handlerFactory_;
};

class Sf_serviceMultiface : virtual public Sf_serviceIf {
 public:
  Sf_serviceMultiface(std::vector<boost::shared_ptr<Sf_serviceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~Sf_serviceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<Sf_serviceIf> > ifaces_;
  Sf_serviceMultiface() {}
  void add(boost::shared_ptr<Sf_serviceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void getproperties(std::vector<Sf_struct> & _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getproperties(_return);
    }
    ifaces_[i]->getproperties(_return);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class Sf_serviceConcurrentClient : virtual public Sf_serviceIf {
 public:
  Sf_serviceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  Sf_serviceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void getproperties(std::vector<Sf_struct> & _return);
  int32_t send_getproperties();
  void recv_getproperties(std::vector<Sf_struct> & _return, const int32_t seqid);
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
