#ifndef DataFormats_GEMDigi_GEMVFATStatus_h
#define DataFormats_GEMDigi_GEMVFATStatus_h
#include "AMCdata.h"
#include "VFATdata.h"
#include <bitset>
#include <ostream>

class GEMVFATStatus {
public:
  union Errors {
    uint8_t codes;
    struct {
      uint8_t vc : 1;  // VFAT CRC error
      uint8_t InValidHeader : 1;
      uint8_t EC : 1;  // does not match AMC EC
      uint8_t BC : 1;  // does not match AMC BC
    };
  };
  union Warnings {
    uint8_t wcodes;
    struct {
      uint8_t basicOFW : 1;    // Basic overflow warning
      uint8_t zeroSupOFW : 1;  // Zero-sup overflow warning
    };
  };

  GEMVFATStatus() {}
  GEMVFATStatus(const gem::AMCdata& amc, const gem::VFATdata& vfat, uint16_t position, bool readMultiBX) {
    Errors error{0};
    Warnings warn{0};

    error.EC = vfat.ec() != amc.lv1Idt();
    if (!readMultiBX)
      error.BC = vfat.bc() != amc.bunchCrossing();

    if (vfat.version() > 2) {
      error.vc = vfat.vc();
      if (vfat.header() == 0x1E)
        warn.basicOFW = 0;
      else if (vfat.header() == 0x5E)
        warn.basicOFW = 1;
      else if (vfat.header() == 0x1A)
        warn.zeroSupOFW = 0;
      else if (vfat.header() == 0x56)
        warn.zeroSupOFW = 1;
      else
        error.InValidHeader = 1;
    }
    vfatPosition_ = position;

    errors_ = error.codes;
    warnings_ = warn.wcodes;
  }

  uint16_t vfatPosition() const { return vfatPosition_; }
  bool isBad() const { return errors_ != 0; }
  uint16_t errors() const { return errors_; }
  uint8_t warnings() const { return warnings_; }

private:
  uint16_t vfatPosition_;
  uint16_t errors_;
  uint8_t warnings_;
};

inline std::ostream& operator<<(std::ostream& out, const GEMVFATStatus& status) {
  out << "GEMVFATStatus errors " << std::bitset<8>(status.errors()) << " warnings "
      << std::bitset<8>(status.warnings());
  return out;
}
#endif
