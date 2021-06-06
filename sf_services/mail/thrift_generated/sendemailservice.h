/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef sendemailservice_H
#define sendemailservice_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "sendmail_types.h"



#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class sendemailserviceIf {
 public:
  virtual ~sendemailserviceIf() {}
  virtual void sendemailtosurfyn(SendEmailResult& _return, const std::string& sender_email, const std::string& subject, const std::string& msg) = 0;
  virtual void sendaccountcreationemail(SendEmailResult& _return, const std::string& display_name, const std::string& new_account_email) = 0;
};

class sendemailserviceIfFactory {
 public:
  typedef sendemailserviceIf Handler;

  virtual ~sendemailserviceIfFactory() {}

  virtual sendemailserviceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(sendemailserviceIf* /* handler */) = 0;
};

class sendemailserviceIfSingletonFactory : virtual public sendemailserviceIfFactory {
 public:
  sendemailserviceIfSingletonFactory(const boost::shared_ptr<sendemailserviceIf>& iface) : iface_(iface) {}
  virtual ~sendemailserviceIfSingletonFactory() {}

  virtual sendemailserviceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(sendemailserviceIf* /* handler */) {}

 protected:
  boost::shared_ptr<sendemailserviceIf> iface_;
};

class sendemailserviceNull : virtual public sendemailserviceIf {
 public:
  virtual ~sendemailserviceNull() {}
  void sendemailtosurfyn(SendEmailResult& /* _return */, const std::string& /* sender_email */, const std::string& /* subject */, const std::string& /* msg */) {
    return;
  }
  void sendaccountcreationemail(SendEmailResult& /* _return */, const std::string& /* display_name */, const std::string& /* new_account_email */) {
    return;
  }
};

typedef struct _sendemailservice_sendemailtosurfyn_args__isset {
  _sendemailservice_sendemailtosurfyn_args__isset() : sender_email(false), subject(false), msg(false) {}
  bool sender_email :1;
  bool subject :1;
  bool msg :1;
} _sendemailservice_sendemailtosurfyn_args__isset;

class sendemailservice_sendemailtosurfyn_args {
 public:

  sendemailservice_sendemailtosurfyn_args(const sendemailservice_sendemailtosurfyn_args&);
  sendemailservice_sendemailtosurfyn_args& operator=(const sendemailservice_sendemailtosurfyn_args&);
  sendemailservice_sendemailtosurfyn_args() : sender_email(), subject(), msg() {
  }

  virtual ~sendemailservice_sendemailtosurfyn_args() throw();
  std::string sender_email;
  std::string subject;
  std::string msg;

  _sendemailservice_sendemailtosurfyn_args__isset __isset;

  void __set_sender_email(const std::string& val);

  void __set_subject(const std::string& val);

  void __set_msg(const std::string& val);

  bool operator == (const sendemailservice_sendemailtosurfyn_args & rhs) const
  {
    if (!(sender_email == rhs.sender_email))
      return false;
    if (!(subject == rhs.subject))
      return false;
    if (!(msg == rhs.msg))
      return false;
    return true;
  }
  bool operator != (const sendemailservice_sendemailtosurfyn_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const sendemailservice_sendemailtosurfyn_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class sendemailservice_sendemailtosurfyn_pargs {
 public:


  virtual ~sendemailservice_sendemailtosurfyn_pargs() throw();
  const std::string* sender_email;
  const std::string* subject;
  const std::string* msg;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _sendemailservice_sendemailtosurfyn_result__isset {
  _sendemailservice_sendemailtosurfyn_result__isset() : success(false) {}
  bool success :1;
} _sendemailservice_sendemailtosurfyn_result__isset;

class sendemailservice_sendemailtosurfyn_result {
 public:

  sendemailservice_sendemailtosurfyn_result(const sendemailservice_sendemailtosurfyn_result&);
  sendemailservice_sendemailtosurfyn_result& operator=(const sendemailservice_sendemailtosurfyn_result&);
  sendemailservice_sendemailtosurfyn_result() {
  }

  virtual ~sendemailservice_sendemailtosurfyn_result() throw();
  SendEmailResult success;

  _sendemailservice_sendemailtosurfyn_result__isset __isset;

  void __set_success(const SendEmailResult& val);

  bool operator == (const sendemailservice_sendemailtosurfyn_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const sendemailservice_sendemailtosurfyn_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const sendemailservice_sendemailtosurfyn_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _sendemailservice_sendemailtosurfyn_presult__isset {
  _sendemailservice_sendemailtosurfyn_presult__isset() : success(false) {}
  bool success :1;
} _sendemailservice_sendemailtosurfyn_presult__isset;

class sendemailservice_sendemailtosurfyn_presult {
 public:


  virtual ~sendemailservice_sendemailtosurfyn_presult() throw();
  SendEmailResult* success;

  _sendemailservice_sendemailtosurfyn_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _sendemailservice_sendaccountcreationemail_args__isset {
  _sendemailservice_sendaccountcreationemail_args__isset() : display_name(false), new_account_email(false) {}
  bool display_name :1;
  bool new_account_email :1;
} _sendemailservice_sendaccountcreationemail_args__isset;

class sendemailservice_sendaccountcreationemail_args {
 public:

  sendemailservice_sendaccountcreationemail_args(const sendemailservice_sendaccountcreationemail_args&);
  sendemailservice_sendaccountcreationemail_args& operator=(const sendemailservice_sendaccountcreationemail_args&);
  sendemailservice_sendaccountcreationemail_args() : display_name(), new_account_email() {
  }

  virtual ~sendemailservice_sendaccountcreationemail_args() throw();
  std::string display_name;
  std::string new_account_email;

  _sendemailservice_sendaccountcreationemail_args__isset __isset;

  void __set_display_name(const std::string& val);

  void __set_new_account_email(const std::string& val);

  bool operator == (const sendemailservice_sendaccountcreationemail_args & rhs) const
  {
    if (!(display_name == rhs.display_name))
      return false;
    if (!(new_account_email == rhs.new_account_email))
      return false;
    return true;
  }
  bool operator != (const sendemailservice_sendaccountcreationemail_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const sendemailservice_sendaccountcreationemail_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class sendemailservice_sendaccountcreationemail_pargs {
 public:


  virtual ~sendemailservice_sendaccountcreationemail_pargs() throw();
  const std::string* display_name;
  const std::string* new_account_email;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _sendemailservice_sendaccountcreationemail_result__isset {
  _sendemailservice_sendaccountcreationemail_result__isset() : success(false) {}
  bool success :1;
} _sendemailservice_sendaccountcreationemail_result__isset;

class sendemailservice_sendaccountcreationemail_result {
 public:

  sendemailservice_sendaccountcreationemail_result(const sendemailservice_sendaccountcreationemail_result&);
  sendemailservice_sendaccountcreationemail_result& operator=(const sendemailservice_sendaccountcreationemail_result&);
  sendemailservice_sendaccountcreationemail_result() {
  }

  virtual ~sendemailservice_sendaccountcreationemail_result() throw();
  SendEmailResult success;

  _sendemailservice_sendaccountcreationemail_result__isset __isset;

  void __set_success(const SendEmailResult& val);

  bool operator == (const sendemailservice_sendaccountcreationemail_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const sendemailservice_sendaccountcreationemail_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const sendemailservice_sendaccountcreationemail_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _sendemailservice_sendaccountcreationemail_presult__isset {
  _sendemailservice_sendaccountcreationemail_presult__isset() : success(false) {}
  bool success :1;
} _sendemailservice_sendaccountcreationemail_presult__isset;

class sendemailservice_sendaccountcreationemail_presult {
 public:


  virtual ~sendemailservice_sendaccountcreationemail_presult() throw();
  SendEmailResult* success;

  _sendemailservice_sendaccountcreationemail_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class sendemailserviceClient : virtual public sendemailserviceIf {
 public:
  sendemailserviceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  sendemailserviceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void sendemailtosurfyn(SendEmailResult& _return, const std::string& sender_email, const std::string& subject, const std::string& msg);
  void send_sendemailtosurfyn(const std::string& sender_email, const std::string& subject, const std::string& msg);
  void recv_sendemailtosurfyn(SendEmailResult& _return);
  void sendaccountcreationemail(SendEmailResult& _return, const std::string& display_name, const std::string& new_account_email);
  void send_sendaccountcreationemail(const std::string& display_name, const std::string& new_account_email);
  void recv_sendaccountcreationemail(SendEmailResult& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class sendemailserviceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<sendemailserviceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (sendemailserviceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_sendemailtosurfyn(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_sendaccountcreationemail(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  sendemailserviceProcessor(boost::shared_ptr<sendemailserviceIf> iface) :
    iface_(iface) {
    processMap_["sendemailtosurfyn"] = &sendemailserviceProcessor::process_sendemailtosurfyn;
    processMap_["sendaccountcreationemail"] = &sendemailserviceProcessor::process_sendaccountcreationemail;
  }

  virtual ~sendemailserviceProcessor() {}
};

class sendemailserviceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  sendemailserviceProcessorFactory(const ::boost::shared_ptr< sendemailserviceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< sendemailserviceIfFactory > handlerFactory_;
};

class sendemailserviceMultiface : virtual public sendemailserviceIf {
 public:
  sendemailserviceMultiface(std::vector<boost::shared_ptr<sendemailserviceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~sendemailserviceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<sendemailserviceIf> > ifaces_;
  sendemailserviceMultiface() {}
  void add(boost::shared_ptr<sendemailserviceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void sendemailtosurfyn(SendEmailResult& _return, const std::string& sender_email, const std::string& subject, const std::string& msg) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sendemailtosurfyn(_return, sender_email, subject, msg);
    }
    ifaces_[i]->sendemailtosurfyn(_return, sender_email, subject, msg);
    return;
  }

  void sendaccountcreationemail(SendEmailResult& _return, const std::string& display_name, const std::string& new_account_email) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->sendaccountcreationemail(_return, display_name, new_account_email);
    }
    ifaces_[i]->sendaccountcreationemail(_return, display_name, new_account_email);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class sendemailserviceConcurrentClient : virtual public sendemailserviceIf {
 public:
  sendemailserviceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  sendemailserviceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void sendemailtosurfyn(SendEmailResult& _return, const std::string& sender_email, const std::string& subject, const std::string& msg);
  int32_t send_sendemailtosurfyn(const std::string& sender_email, const std::string& subject, const std::string& msg);
  void recv_sendemailtosurfyn(SendEmailResult& _return, const int32_t seqid);
  void sendaccountcreationemail(SendEmailResult& _return, const std::string& display_name, const std::string& new_account_email);
  int32_t send_sendaccountcreationemail(const std::string& display_name, const std::string& new_account_email);
  void recv_sendaccountcreationemail(SendEmailResult& _return, const int32_t seqid);
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
