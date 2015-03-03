#include "public.sdk/source/vst2.x/audioeffectx.h"

#include <mruby.h>
#include <mruby/compile.h>

#define PROGRAMS_COUNT 10
#define PARAMETERS_COUNT 10
#define SCRIPT_EMPTY_NAME "---"

#define SCRIPTS_DIR "/Users/fazibear/dev/mrubyvst/examples/"

//-------------------------------------------------------------------------------------------------------
class MRubyVst : public AudioEffectX {

  public:
  MRubyVst(audioMasterCallback audioMaster);
  ~MRubyVst();

  void log(const char* txt);

  // Processing
  virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);
  virtual void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames);

  // Program
  void setProgramNames();
  virtual void setProgram(VstInt32 index);
  virtual void getProgramName(char* name);

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
  std::string programs[PROGRAMS_COUNT];

  mrb_state *mrb;
  mrb_value vst_instance;
};
