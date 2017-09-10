
#include <binder/BinderService.h>

#include "sniper.h"

namespace sniper {


IMPLEMENT_META_INTERFACE(SniperService, "android.ext.ISniperService");

status_t BnSniperService::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                     uint32_t flags) {
  switch (code) {
    case SHOOT_TARGET: {
      CHECK_INTERFACE(ISniperService, data, reply);
      ShootTarget(data.readString16(), data.readBool());
      return NO_ERROR;
    } break;
    case TARGET_LIST: {
      CHECK_INTERFACE(ISniperService, data, reply);
      std::list<TargetStatus>&& targets = TargetList();
      reply->writeUint32(targets.size());
      std::for_each(targets.begin(), targets.end(), [&](auto p) {
          reply->writeString16(p.pkgname);
          reply->writeBool(p.status);
        });
      return NO_ERROR;
    } break;
    default:
      return BBinder::onTransact(code, data, reply, flags);
  }
}

void SniperService::ShootTarget(const String16& pkgname, bool status) {
  shot_targets_[pkgname] = status;
}

std::list<TargetStatus>&& SniperService::TargetList() {
  std::list<TargetStatus> targets;
  std::for_each(shot_targets_.begin(), shot_targets_.end(), [&](auto p) {
      TargetStatus ts;
      ts.pkgname = p.first;
      ts.status = p.second;
      targets.push_back(ts);
    });
  return std::move(targets);
}

int Sniper::Main(int argc, char* argv[]) {
  BinderService<SniperService>::publishAndJoinThreadPool();
  return 0;
}

}  // namespace sniper
