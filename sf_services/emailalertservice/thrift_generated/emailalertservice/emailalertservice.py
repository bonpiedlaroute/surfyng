#
# Autogenerated by Thrift Compiler (0.10.0)
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#
#  options string: py
#

from thrift.Thrift import TType, TMessageType, TFrozenDict, TException, TApplicationException
from thrift.protocol.TProtocol import TProtocolException
import sys
import logging
from .ttypes import *
from thrift.Thrift import TProcessor
from thrift.transport import TTransport


class Iface(object):
    def registerEmailAlert(self, userid, parameters):
        """
        Parameters:
         - userid
         - parameters
        """
        pass

    def notifyNewAnnounces(self, city):
        """
        Parameters:
         - city
        """
        pass

    def my_realestate_search(self, userid):
        """
        Parameters:
         - userid
        """
        pass

    def changeAlertStatus(self, alert_id, alert_status):
        """
        Parameters:
         - alert_id
         - alert_status
        """
        pass


class Client(Iface):
    def __init__(self, iprot, oprot=None):
        self._iprot = self._oprot = iprot
        if oprot is not None:
            self._oprot = oprot
        self._seqid = 0

    def registerEmailAlert(self, userid, parameters):
        """
        Parameters:
         - userid
         - parameters
        """
        self.send_registerEmailAlert(userid, parameters)
        return self.recv_registerEmailAlert()

    def send_registerEmailAlert(self, userid, parameters):
        self._oprot.writeMessageBegin('registerEmailAlert', TMessageType.CALL, self._seqid)
        args = registerEmailAlert_args()
        args.userid = userid
        args.parameters = parameters
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()

    def recv_registerEmailAlert(self):
        iprot = self._iprot
        (fname, mtype, rseqid) = iprot.readMessageBegin()
        if mtype == TMessageType.EXCEPTION:
            x = TApplicationException()
            x.read(iprot)
            iprot.readMessageEnd()
            raise x
        result = registerEmailAlert_result()
        result.read(iprot)
        iprot.readMessageEnd()
        if result.success is not None:
            return result.success
        raise TApplicationException(TApplicationException.MISSING_RESULT, "registerEmailAlert failed: unknown result")

    def notifyNewAnnounces(self, city):
        """
        Parameters:
         - city
        """
        self.send_notifyNewAnnounces(city)
        return self.recv_notifyNewAnnounces()

    def send_notifyNewAnnounces(self, city):
        self._oprot.writeMessageBegin('notifyNewAnnounces', TMessageType.CALL, self._seqid)
        args = notifyNewAnnounces_args()
        args.city = city
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()

    def recv_notifyNewAnnounces(self):
        iprot = self._iprot
        (fname, mtype, rseqid) = iprot.readMessageBegin()
        if mtype == TMessageType.EXCEPTION:
            x = TApplicationException()
            x.read(iprot)
            iprot.readMessageEnd()
            raise x
        result = notifyNewAnnounces_result()
        result.read(iprot)
        iprot.readMessageEnd()
        if result.success is not None:
            return result.success
        raise TApplicationException(TApplicationException.MISSING_RESULT, "notifyNewAnnounces failed: unknown result")

    def my_realestate_search(self, userid):
        """
        Parameters:
         - userid
        """
        self.send_my_realestate_search(userid)
        return self.recv_my_realestate_search()

    def send_my_realestate_search(self, userid):
        self._oprot.writeMessageBegin('my_realestate_search', TMessageType.CALL, self._seqid)
        args = my_realestate_search_args()
        args.userid = userid
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()

    def recv_my_realestate_search(self):
        iprot = self._iprot
        (fname, mtype, rseqid) = iprot.readMessageBegin()
        if mtype == TMessageType.EXCEPTION:
            x = TApplicationException()
            x.read(iprot)
            iprot.readMessageEnd()
            raise x
        result = my_realestate_search_result()
        result.read(iprot)
        iprot.readMessageEnd()
        if result.success is not None:
            return result.success
        raise TApplicationException(TApplicationException.MISSING_RESULT, "my_realestate_search failed: unknown result")

    def changeAlertStatus(self, alert_id, alert_status):
        """
        Parameters:
         - alert_id
         - alert_status
        """
        self.send_changeAlertStatus(alert_id, alert_status)
        return self.recv_changeAlertStatus()

    def send_changeAlertStatus(self, alert_id, alert_status):
        self._oprot.writeMessageBegin('changeAlertStatus', TMessageType.CALL, self._seqid)
        args = changeAlertStatus_args()
        args.alert_id = alert_id
        args.alert_status = alert_status
        args.write(self._oprot)
        self._oprot.writeMessageEnd()
        self._oprot.trans.flush()

    def recv_changeAlertStatus(self):
        iprot = self._iprot
        (fname, mtype, rseqid) = iprot.readMessageBegin()
        if mtype == TMessageType.EXCEPTION:
            x = TApplicationException()
            x.read(iprot)
            iprot.readMessageEnd()
            raise x
        result = changeAlertStatus_result()
        result.read(iprot)
        iprot.readMessageEnd()
        if result.success is not None:
            return result.success
        raise TApplicationException(TApplicationException.MISSING_RESULT, "changeAlertStatus failed: unknown result")


class Processor(Iface, TProcessor):
    def __init__(self, handler):
        self._handler = handler
        self._processMap = {}
        self._processMap["registerEmailAlert"] = Processor.process_registerEmailAlert
        self._processMap["notifyNewAnnounces"] = Processor.process_notifyNewAnnounces
        self._processMap["my_realestate_search"] = Processor.process_my_realestate_search
        self._processMap["changeAlertStatus"] = Processor.process_changeAlertStatus

    def process(self, iprot, oprot):
        (name, type, seqid) = iprot.readMessageBegin()
        if name not in self._processMap:
            iprot.skip(TType.STRUCT)
            iprot.readMessageEnd()
            x = TApplicationException(TApplicationException.UNKNOWN_METHOD, 'Unknown function %s' % (name))
            oprot.writeMessageBegin(name, TMessageType.EXCEPTION, seqid)
            x.write(oprot)
            oprot.writeMessageEnd()
            oprot.trans.flush()
            return
        else:
            self._processMap[name](self, seqid, iprot, oprot)
        return True

    def process_registerEmailAlert(self, seqid, iprot, oprot):
        args = registerEmailAlert_args()
        args.read(iprot)
        iprot.readMessageEnd()
        result = registerEmailAlert_result()
        try:
            result.success = self._handler.registerEmailAlert(args.userid, args.parameters)
            msg_type = TMessageType.REPLY
        except (TTransport.TTransportException, KeyboardInterrupt, SystemExit):
            raise
        except Exception as ex:
            msg_type = TMessageType.EXCEPTION
            logging.exception(ex)
            result = TApplicationException(TApplicationException.INTERNAL_ERROR, 'Internal error')
        oprot.writeMessageBegin("registerEmailAlert", msg_type, seqid)
        result.write(oprot)
        oprot.writeMessageEnd()
        oprot.trans.flush()

    def process_notifyNewAnnounces(self, seqid, iprot, oprot):
        args = notifyNewAnnounces_args()
        args.read(iprot)
        iprot.readMessageEnd()
        result = notifyNewAnnounces_result()
        try:
            result.success = self._handler.notifyNewAnnounces(args.city)
            msg_type = TMessageType.REPLY
        except (TTransport.TTransportException, KeyboardInterrupt, SystemExit):
            raise
        except Exception as ex:
            msg_type = TMessageType.EXCEPTION
            logging.exception(ex)
            result = TApplicationException(TApplicationException.INTERNAL_ERROR, 'Internal error')
        oprot.writeMessageBegin("notifyNewAnnounces", msg_type, seqid)
        result.write(oprot)
        oprot.writeMessageEnd()
        oprot.trans.flush()

    def process_my_realestate_search(self, seqid, iprot, oprot):
        args = my_realestate_search_args()
        args.read(iprot)
        iprot.readMessageEnd()
        result = my_realestate_search_result()
        try:
            result.success = self._handler.my_realestate_search(args.userid)
            msg_type = TMessageType.REPLY
        except (TTransport.TTransportException, KeyboardInterrupt, SystemExit):
            raise
        except Exception as ex:
            msg_type = TMessageType.EXCEPTION
            logging.exception(ex)
            result = TApplicationException(TApplicationException.INTERNAL_ERROR, 'Internal error')
        oprot.writeMessageBegin("my_realestate_search", msg_type, seqid)
        result.write(oprot)
        oprot.writeMessageEnd()
        oprot.trans.flush()

    def process_changeAlertStatus(self, seqid, iprot, oprot):
        args = changeAlertStatus_args()
        args.read(iprot)
        iprot.readMessageEnd()
        result = changeAlertStatus_result()
        try:
            result.success = self._handler.changeAlertStatus(args.alert_id, args.alert_status)
            msg_type = TMessageType.REPLY
        except (TTransport.TTransportException, KeyboardInterrupt, SystemExit):
            raise
        except Exception as ex:
            msg_type = TMessageType.EXCEPTION
            logging.exception(ex)
            result = TApplicationException(TApplicationException.INTERNAL_ERROR, 'Internal error')
        oprot.writeMessageBegin("changeAlertStatus", msg_type, seqid)
        result.write(oprot)
        oprot.writeMessageEnd()
        oprot.trans.flush()

# HELPER FUNCTIONS AND STRUCTURES


class registerEmailAlert_args(object):
    """
    Attributes:
     - userid
     - parameters
    """

    thrift_spec = (
        None,  # 0
        (1, TType.STRING, 'userid', 'UTF8', None, ),  # 1
        (2, TType.MAP, 'parameters', (TType.STRING, 'UTF8', TType.STRING, 'UTF8', False), None, ),  # 2
    )

    def __init__(self, userid=None, parameters=None,):
        self.userid = userid
        self.parameters = parameters

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.STRING:
                    self.userid = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 2:
                if ftype == TType.MAP:
                    self.parameters = {}
                    (_ktype1, _vtype2, _size0) = iprot.readMapBegin()
                    for _i4 in range(_size0):
                        _key5 = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                        _val6 = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                        self.parameters[_key5] = _val6
                    iprot.readMapEnd()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('registerEmailAlert_args')
        if self.userid is not None:
            oprot.writeFieldBegin('userid', TType.STRING, 1)
            oprot.writeString(self.userid.encode('utf-8') if sys.version_info[0] == 2 else self.userid)
            oprot.writeFieldEnd()
        if self.parameters is not None:
            oprot.writeFieldBegin('parameters', TType.MAP, 2)
            oprot.writeMapBegin(TType.STRING, TType.STRING, len(self.parameters))
            for kiter7, viter8 in self.parameters.items():
                oprot.writeString(kiter7.encode('utf-8') if sys.version_info[0] == 2 else kiter7)
                oprot.writeString(viter8.encode('utf-8') if sys.version_info[0] == 2 else viter8)
            oprot.writeMapEnd()
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class registerEmailAlert_result(object):
    """
    Attributes:
     - success
    """

    thrift_spec = (
        (0, TType.STRUCT, 'success', (EmailAlertResult, EmailAlertResult.thrift_spec), None, ),  # 0
    )

    def __init__(self, success=None,):
        self.success = success

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 0:
                if ftype == TType.STRUCT:
                    self.success = EmailAlertResult()
                    self.success.read(iprot)
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('registerEmailAlert_result')
        if self.success is not None:
            oprot.writeFieldBegin('success', TType.STRUCT, 0)
            self.success.write(oprot)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class notifyNewAnnounces_args(object):
    """
    Attributes:
     - city
    """

    thrift_spec = (
        None,  # 0
        (1, TType.STRING, 'city', 'UTF8', None, ),  # 1
    )

    def __init__(self, city=None,):
        self.city = city

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.STRING:
                    self.city = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('notifyNewAnnounces_args')
        if self.city is not None:
            oprot.writeFieldBegin('city', TType.STRING, 1)
            oprot.writeString(self.city.encode('utf-8') if sys.version_info[0] == 2 else self.city)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class notifyNewAnnounces_result(object):
    """
    Attributes:
     - success
    """

    thrift_spec = (
        (0, TType.STRUCT, 'success', (EmailAlertResult, EmailAlertResult.thrift_spec), None, ),  # 0
    )

    def __init__(self, success=None,):
        self.success = success

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 0:
                if ftype == TType.STRUCT:
                    self.success = EmailAlertResult()
                    self.success.read(iprot)
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('notifyNewAnnounces_result')
        if self.success is not None:
            oprot.writeFieldBegin('success', TType.STRUCT, 0)
            self.success.write(oprot)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class my_realestate_search_args(object):
    """
    Attributes:
     - userid
    """

    thrift_spec = (
        None,  # 0
        (1, TType.STRING, 'userid', 'UTF8', None, ),  # 1
    )

    def __init__(self, userid=None,):
        self.userid = userid

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.STRING:
                    self.userid = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('my_realestate_search_args')
        if self.userid is not None:
            oprot.writeFieldBegin('userid', TType.STRING, 1)
            oprot.writeString(self.userid.encode('utf-8') if sys.version_info[0] == 2 else self.userid)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class my_realestate_search_result(object):
    """
    Attributes:
     - success
    """

    thrift_spec = (
        (0, TType.LIST, 'success', (TType.MAP, (TType.STRING, 'UTF8', TType.STRING, 'UTF8', False), False), None, ),  # 0
    )

    def __init__(self, success=None,):
        self.success = success

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 0:
                if ftype == TType.LIST:
                    self.success = []
                    (_etype12, _size9) = iprot.readListBegin()
                    for _i13 in range(_size9):
                        _elem14 = {}
                        (_ktype16, _vtype17, _size15) = iprot.readMapBegin()
                        for _i19 in range(_size15):
                            _key20 = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                            _val21 = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                            _elem14[_key20] = _val21
                        iprot.readMapEnd()
                        self.success.append(_elem14)
                    iprot.readListEnd()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('my_realestate_search_result')
        if self.success is not None:
            oprot.writeFieldBegin('success', TType.LIST, 0)
            oprot.writeListBegin(TType.MAP, len(self.success))
            for iter22 in self.success:
                oprot.writeMapBegin(TType.STRING, TType.STRING, len(iter22))
                for kiter23, viter24 in iter22.items():
                    oprot.writeString(kiter23.encode('utf-8') if sys.version_info[0] == 2 else kiter23)
                    oprot.writeString(viter24.encode('utf-8') if sys.version_info[0] == 2 else viter24)
                oprot.writeMapEnd()
            oprot.writeListEnd()
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class changeAlertStatus_args(object):
    """
    Attributes:
     - alert_id
     - alert_status
    """

    thrift_spec = (
        None,  # 0
        (1, TType.STRING, 'alert_id', 'UTF8', None, ),  # 1
        (2, TType.STRING, 'alert_status', 'UTF8', None, ),  # 2
    )

    def __init__(self, alert_id=None, alert_status=None,):
        self.alert_id = alert_id
        self.alert_status = alert_status

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 1:
                if ftype == TType.STRING:
                    self.alert_id = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            elif fid == 2:
                if ftype == TType.STRING:
                    self.alert_status = iprot.readString().decode('utf-8') if sys.version_info[0] == 2 else iprot.readString()
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('changeAlertStatus_args')
        if self.alert_id is not None:
            oprot.writeFieldBegin('alert_id', TType.STRING, 1)
            oprot.writeString(self.alert_id.encode('utf-8') if sys.version_info[0] == 2 else self.alert_id)
            oprot.writeFieldEnd()
        if self.alert_status is not None:
            oprot.writeFieldBegin('alert_status', TType.STRING, 2)
            oprot.writeString(self.alert_status.encode('utf-8') if sys.version_info[0] == 2 else self.alert_status)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)


class changeAlertStatus_result(object):
    """
    Attributes:
     - success
    """

    thrift_spec = (
        (0, TType.STRUCT, 'success', (EmailAlertResult, EmailAlertResult.thrift_spec), None, ),  # 0
    )

    def __init__(self, success=None,):
        self.success = success

    def read(self, iprot):
        if iprot._fast_decode is not None and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None:
            iprot._fast_decode(self, iprot, (self.__class__, self.thrift_spec))
            return
        iprot.readStructBegin()
        while True:
            (fname, ftype, fid) = iprot.readFieldBegin()
            if ftype == TType.STOP:
                break
            if fid == 0:
                if ftype == TType.STRUCT:
                    self.success = EmailAlertResult()
                    self.success.read(iprot)
                else:
                    iprot.skip(ftype)
            else:
                iprot.skip(ftype)
            iprot.readFieldEnd()
        iprot.readStructEnd()

    def write(self, oprot):
        if oprot._fast_encode is not None and self.thrift_spec is not None:
            oprot.trans.write(oprot._fast_encode(self, (self.__class__, self.thrift_spec)))
            return
        oprot.writeStructBegin('changeAlertStatus_result')
        if self.success is not None:
            oprot.writeFieldBegin('success', TType.STRUCT, 0)
            self.success.write(oprot)
            oprot.writeFieldEnd()
        oprot.writeFieldStop()
        oprot.writeStructEnd()

    def validate(self):
        return

    def __repr__(self):
        L = ['%s=%r' % (key, value)
             for key, value in self.__dict__.items()]
        return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

    def __ne__(self, other):
        return not (self == other)
