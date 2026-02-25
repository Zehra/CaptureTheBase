// CaptureTheBase.cpp
//

#include "bzfsAPI.h"

class CaptureTheBase : public bz_Plugin
{
public:
  virtual const char* Name(){return "CaptureTheBase";}
  virtual void Init (const char*config) {
    Register(bz_eAllowFlagGrab);
    Register(bz_eCaptureEvent);
  }
  virtual void Event(bz_EventData *eventData ){
    switch (eventData->eventType) {
      case bz_eAllowFlagGrab: {
        bz_AllowFlagGrabData_V1* allowGrab = (bz_AllowFlagGrabData_V1*)eventData;
        bz_eTeamType playerTeam =  bz_getPlayerTeam(allowGrab->playerID);
        bz_eTeamType flagTeam = flagToTeamValue(allowGrab->flagType);
          
        if ((playerTeam == flagTeam) || ((flagTeam != eNoTeam) && (playerTeam == eRogueTeam))) {
          allowGrab->allow = false;
        }
      }break;
     
      case bz_eCaptureEvent: {
        bz_CTFCaptureEventData_V1* captureData = (bz_CTFCaptureEventData_V1*)eventData;
        bz_killPlayer(captureData->playerCapping, true, BZ_SERVER);
        bz_incrementPlayerLosses(captureData->playerCapping, -1);
        bz_resetFlags(true, false);
      }break;
     
      default: {
      }break;
    }
  }
  virtual void Cleanup (void) {
    Flush();
  }
  // Utility functions
  bz_eTeamType flagToTeamValue(const char* flagType) {
    if (strcmp("R*", flagType) == 0) {
      return eRedTeam;
    } else if (strcmp("G*", flagType) == 0) {
      return eGreenTeam;
    } else if (strcmp("B*", flagType) == 0) {
      return eBlueTeam;
    } else if (strcmp("P*", flagType) == 0) {
      return ePurpleTeam;
    }  else {
      return eNoTeam;
    }
  }
//
};

BZ_PLUGIN(CaptureTheBase)
