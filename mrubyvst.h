#include "public.sdk/source/vst2.x/audioeffectx.h"

#include <mutex>

#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/variable.h"


#define PROGRAMS_COUNT 10
#define PARAMETERS_COUNT 4
#define SCRIPT_PATH "/Users/fazibear/dev/mrubyvst/mrubyvst.rb"
#define VST_CLASS "MRubyVST"

//-------------------------------------------------------------------------------------------------------
class MRubyVst : public AudioEffectX {

public:
  MRubyVst(audioMasterCallback audioMaster);
  ~MRubyVst();

  void log(const char* txt);

  // Processing
  virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);

  // Program
  virtual void setProgram(VstInt32 index);
  virtual bool getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text);

  // Parameters
  virtual void setParameter(VstInt32 index, float value);
  virtual float getParameter(VstInt32 index);
  virtual void getParameterLabel(VstInt32 index, char* label);
  virtual void getParameterDisplay(VstInt32 index, char* text);
  virtual void getParameterName(VstInt32 index, char* text);

  virtual bool getEffectName(char* name);
  virtual bool getVendorString(char* text);
  virtual bool getProductString(char* text);
  virtual VstInt32 getVendorVersion();

protected:
  std::mutex m;
  mrb_state *mrb;
  mrb_value vst_instance;
};
