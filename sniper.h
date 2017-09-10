

#include <map>
#include <list>

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

using namespace android;

namespace sniper {


class Sniper {
public:

  static int Main(int argc, char* argv[]);

};  // class Sniper

struct TargetStatus {
  String16  pkgname;
  bool      status;
};

class ISniperService : public IInterface {
public:
  enum {
    SHOOT_TARGET = IBinder::FIRST_CALL_TRANSACTION,
    TARGET_LIST,
  };

  DECLARE_META_INTERFACE(SniperService);

  virtual void ShootTarget(const String16& pkgname, bool status) = 0;
  virtual std::list<TargetStatus>&& TargetList() = 0;
};  // class ISniperService


class BnSniperService : public BnInterface<ISniperService> {
public:
  BnSniperService()
      : BnInterface<ISniperService>() {}
  virtual status_t onTransact(uint32_t code,
                              const Parcel& data,
                              Parcel* reply,
                              uint32_t flags = 0);

};  // class BnSniperService

class BpSniperService : public BpInterface<ISniperService> {
public:
  explicit BpSniperService(const sp<IBinder>& impl)
      : BpInterface<ISniperService>(impl) {}
  
  virtual void ShootTarget(const String16& pkgname, bool status) {
    Parcel data, reply;
    data.writeInterfaceToken(ISniperService::getInterfaceDescriptor());
    data.writeString16(pkgname);
    data.writeBool(status);
    remote()->transact(BnSniperService::SHOOT_TARGET, data, &reply);
  }

  virtual std::list<TargetStatus>&& TargetList() {
    Parcel data, reply;
    data.writeInterfaceToken(ISniperService::getInterfaceDescriptor());
    remote()->transact(BnSniperService::TARGET_LIST, data, &reply);
    std::list<TargetStatus> targets;
    uint32_t len = reply.readUint32();
    for (uint32_t i = 0; i < len; ++i) {
      TargetStatus ts;
      ts.pkgname = reply.readString16();
      ts.status = reply.readBool();
      targets.push_back(ts);
    }
    return std::move(targets);
  }
};  // class BpSniperService

class SniperService : public BnSniperService {
public:
  static char const* getServiceName() { return "sniperservice"; }
  void ShootTarget(const String16& pkgname, bool status);
  std::list<TargetStatus>&& TargetList();
private:
  std::map<String16, bool> shot_targets_;
};  // class SniperService

}  // namespace sniper
