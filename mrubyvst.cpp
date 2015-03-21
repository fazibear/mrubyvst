#include <string>
#include <fstream>
#include <streambuf>
#include <dirent.h>

#include "mrubyvst.h"

#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/variable.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
  return new MRubyVst(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
MRubyVst::MRubyVst(audioMasterCallback audioMaster): AudioEffectX(audioMaster, PROGRAMS_COUNT, PARAMETERS_COUNT) {

  setUniqueID('mrbv');
  canProcessReplacing();

  setNumInputs(2);
  setNumOutputs(2);

  mrb = mrb_open();

  FILE *file = fopen(SCRIPT_PATH, "r");
  if (file == NULL) {
  } else {
    mrb_load_file(mrb, file);
    mrb_value vst_class = mrb_vm_const_get(mrb, mrb_intern_lit(mrb, VST_CLASS));
    mrb_const_set(mrb, vst_class, mrb_intern_lit(mrb, "PROGRAMS_COUNT"), mrb_fixnum_value(PROGRAMS_COUNT));
    mrb_const_set(mrb, vst_class, mrb_intern_lit(mrb, "PARAMETERS_COUNT"), mrb_fixnum_value(PARAMETERS_COUNT));
    vst_instance = mrb_instance_new(mrb, vst_class);
    fclose(file);
  }

}

//-------------------------------------------------------------------------------------------------------
MRubyVst::~MRubyVst() {
  mrb_close(mrb);
}

//-------------------------------------------------------------------------------------------------------
void MRubyVst::setProgram(VstInt32 index) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "change_program"))){
    mrb_funcall(mrb, vst_instance, "change_program", 1, mrb_fixnum_value(index));
  }
}

//-----------------------------------------------------------------------------------------
bool MRubyVst::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "program_name"))){
    mrb_value mrb_name = mrb_funcall(mrb, vst_instance, "program_name", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_name)) {
      vst_strncpy(text, RSTRING_PTR(mrb_name), kVstMaxProgNameLen);
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------------------
void MRubyVst::setParameter(VstInt32 index, float value) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "set_parameter"))){
    mrb_funcall(mrb, vst_instance, "set_parameter", 2, mrb_fixnum_value(index), mrb_float_value(mrb, value));
  }
}

//-----------------------------------------------------------------------------------------
float MRubyVst::getParameter(VstInt32 index) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "parameter_value"))){
    mrb_value mrb_param_value = mrb_funcall(mrb, vst_instance, "parameter_value", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_param_value)) {
      return mrb_float(mrb_param_value);
    }
  }
  return 0;
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterName(VstInt32 index, char* label) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "parameter_name"))){
    mrb_value mrb_label = mrb_funcall(mrb, vst_instance, "parameter_name", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_label)) {
      vst_strncpy(label, RSTRING_PTR(mrb_label), kVstMaxParamStrLen);
    }
  }
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterDisplay(VstInt32 index, char* text) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "parameter_display_value"))){
    mrb_value mrb_display = mrb_funcall(mrb, vst_instance, "parameter_display_value", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_display)) {
      vst_strncpy(text, RSTRING_PTR(mrb_display), kVstMaxParamStrLen);
    }
  }
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterLabel(VstInt32 index, char* label) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "parameter_label"))){
    mrb_value mrb_label = mrb_funcall(mrb, vst_instance, "parameter_label", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_label)) {
      vst_strncpy(label, RSTRING_PTR(mrb_label), kVstMaxParamStrLen);
    }
  }
}

//------------------------------------------------------------------------
bool MRubyVst::getEffectName(char* name) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "effect_name"))){
    mrb_value mrb_name = mrb_funcall(mrb, vst_instance, "effect_name", 0);
    if (!mrb_nil_p(mrb_name)) {
      vst_strncpy(name, RSTRING_PTR(mrb_name), kVstMaxEffectNameLen);
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------
bool MRubyVst::getProductString(char* text) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "product"))){
    mrb_value mrb_product = mrb_funcall(mrb, vst_instance, "product", 0);
    if (!mrb_nil_p(mrb_product)) {
      vst_strncpy(text, RSTRING_PTR(mrb_product), kVstMaxProductStrLen);
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------
bool MRubyVst::getVendorString(char* text) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "vendor"))){
    mrb_value mrb_vendor = mrb_funcall(mrb, vst_instance, "vendor", 0);
    if (!mrb_nil_p(mrb_vendor)) {
      vst_strncpy(text, RSTRING_PTR(mrb_vendor), kVstMaxVendorStrLen);
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------------------
VstInt32 MRubyVst::getVendorVersion () {
  return 0;
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "version"))){
    mrb_value mrb_version = mrb_funcall(mrb, vst_instance, "version", 0);
    if (!mrb_nil_p(mrb_version)) {
      return mrb_fixnum(mrb_version);
    }
  }
  return 0;
}

//-----------------------------------------------------------------------------------------
void MRubyVst::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "process"))){
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    mrb_value mrb_inputs = mrb_ary_new(mrb);
    mrb_value mrb_input_1 = mrb_ary_new(mrb);
    mrb_value mrb_input_2 = mrb_ary_new(mrb);

    for (int i=0;i<sampleFrames;i++) {
      mrb_ary_push(mrb, mrb_input_1, mrb_float_value(mrb, in1[i]));
      mrb_ary_push(mrb, mrb_input_2, mrb_float_value(mrb, in2[i]));
    }

    mrb_ary_push(mrb, mrb_inputs, mrb_input_1);
    mrb_ary_push(mrb, mrb_inputs, mrb_input_2);

    mrb_value mrb_outputs = mrb_funcall(mrb, vst_instance, "process", 1, mrb_inputs);

    if (!mrb_nil_p(mrb_outputs)) {
      mrb_value mrb_output_1 = mrb_ary_shift(mrb, mrb_outputs);
      mrb_value mrb_output_2 = mrb_ary_shift(mrb, mrb_outputs);

      for (int i=0;i<sampleFrames;i++) {
        out1[i] = mrb_float(mrb_ary_shift(mrb, mrb_output_1));
        out2[i] = mrb_float(mrb_ary_shift(mrb, mrb_output_2));
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------------
void MRubyVst::log(const char* txt){
  FILE *f;
  f = fopen("/tmp/vst_log", "a");
  fprintf(f, "%s\n", txt);
  fclose(f);
}

