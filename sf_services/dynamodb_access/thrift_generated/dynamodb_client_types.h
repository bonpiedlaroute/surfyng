/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef dynamodb_client_TYPES_H
#define dynamodb_client_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>




class OperationResultAsync;

typedef struct _OperationResultAsync__isset {
  _OperationResultAsync__isset() : success(false), error(false) {}
  bool success :1;
  bool error :1;
} _OperationResultAsync__isset;

class OperationResultAsync : public virtual ::apache::thrift::TBase {
 public:

  OperationResultAsync(const OperationResultAsync&);
  OperationResultAsync& operator=(const OperationResultAsync&);
  OperationResultAsync() : success(0), error() {
  }

  virtual ~OperationResultAsync() throw();
  bool success;
  std::string error;

  _OperationResultAsync__isset __isset;

  void __set_success(const bool val);

  void __set_error(const std::string& val);

  bool operator == (const OperationResultAsync & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(error == rhs.error))
      return false;
    return true;
  }
  bool operator != (const OperationResultAsync &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const OperationResultAsync & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(OperationResultAsync &a, OperationResultAsync &b);

inline std::ostream& operator<<(std::ostream& out, const OperationResultAsync& obj)
{
  obj.printTo(out);
  return out;
}



#endif
