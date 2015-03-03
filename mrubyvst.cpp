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
  // 1 program, 1 parameter only
  setUniqueID('mrby');
  canProcessReplacing();

  setNumInputs(2);    // stereo in
  setNumOutputs(2);   // stereo out

  setProgramNames();
}

//-------------------------------------------------------------------------------------------------------
MRubyVst::~MRubyVst() {
  //mrb_close(mrb);
}

//-------------------------------------------------------------------------------------------------------
void MRubyVst::setProgramNames() {
  DIR *pDIR;
  struct dirent *entry;
  int i = 0;
  if( (pDIR=opendir(SCRIPTS_DIR)) ){
    while( (entry = readdir(pDIR)) ){
      if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && entry->d_type == DT_REG ) {
        programs[i++] = entry->d_name;
      }
    }
    closedir(pDIR);
  }
  while(i<PROGRAMS_COUNT) {
    programs[i++] = SCRIPT_EMPTY_NAME;
  }
}

//-------------------------------------------------------------------------------------------------------
void MRubyVst::setProgram(VstInt32 index) {
  if(strcmp(programs[index].c_str(), SCRIPT_EMPTY_NAME)){
    std::string script = SCRIPTS_DIR;
    script += programs[index];

    mrb = mrb_open();

    FILE *file = fopen(script.c_str(), "r");
    if (file == NULL) {
    } else {
      mrb_load_file(mrb, file);
      mrb_value vst_class = mrb_vm_const_get(mrb, mrb_intern_lit(mrb,"VST"));
      vst_instance = mrb_instance_new(mrb, vst_class);
      fclose(file);
    }
  }
  curProgram = index;
  updateDisplay();
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getProgramName(char* name) {
  strcpy(name, programs[curProgram].c_str());
}

//-----------------------------------------------------------------------------------------
void MRubyVst::setParameter(VstInt32 index, float value) {
  mrb_funcall(mrb, vst_instance, "parameter_set", 2, mrb_fixnum_value(index), mrb_float_value(mrb, value));
}

//-----------------------------------------------------------------------------------------
float MRubyVst::getParameter(VstInt32 index) {
  mrb_value mrb_index = mrb_fixnum_value(index);
  mrb_value mrb_param_value = mrb_funcall(mrb, vst_instance, "parameter_get", 1, mrb_index);
  if (!mrb_nil_p(mrb_param_value)) {
    return mrb_float(mrb_param_value);
  }else{
    return 0;
  }
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterName(VstInt32 index, char* label) {
  mrb_value mrb_label = mrb_funcall(mrb, vst_instance, "parameter_name", 1, mrb_fixnum_value(index));
  const char *return_label;
  if (!mrb_nil_p(mrb_label)) {
    return_label = RSTRING_PTR(mrb_label);
  }else{
    return_label = "";
  }
  vst_strncpy(label, return_label, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterDisplay(VstInt32 index, char* text) {
  mrb_value mrb_display = mrb_funcall(mrb, vst_instance, "parameter_display", 1, mrb_fixnum_value(index));
  const char *return_display;
  if (!mrb_nil_p(mrb_display)) {
    return_display = RSTRING_PTR(mrb_display);
  }else{
    return_display = "";
  }
  vst_strncpy(text, return_display, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterLabel(VstInt32 index, char* label) {
  mrb_value mrb_label = mrb_funcall(mrb, vst_instance, "parameter_label", 1, mrb_fixnum_value(index));
  const char *return_label;
  if (!mrb_nil_p(mrb_label)) {
    return_label = RSTRING_PTR(mrb_label);
  }else{
    return_label = "";
  }
  vst_strncpy(label, return_label, kVstMaxParamStrLen);
}

//------------------------------------------------------------------------
bool MRubyVst::getEffectName(char* name) {
  mrb_value mrb_name = mrb_funcall(mrb, vst_instance, "name", 0);
  const char *return_name;
  if (!mrb_nil_p(mrb_name)) {
    return_name = RSTRING_PTR(mrb_name);
  }else{
    return_name = "";
  }
  vst_strncpy(name, return_name, kVstMaxEffectNameLen);
  return true;
}

//------------------------------------------------------------------------
bool MRubyVst::getProductString(char* text) {
  mrb_value mrb_product = mrb_funcall(mrb, vst_instance, "product", 0);
  const char *return_product;
  if (!mrb_nil_p(mrb_product)) {
    return_product = RSTRING_PTR(mrb_product);
  }else{
    return_product = "";
  }
  vst_strncpy (text, return_product, kVstMaxVendorStrLen);
  return true;
}

//------------------------------------------------------------------------
bool MRubyVst::getVendorString(char* text) {
  mrb_value mrb_vendor = mrb_funcall(mrb, vst_instance, "vendor", 0);
  const char *return_vendor;
  if (!mrb_nil_p(mrb_vendor)) {
    return_vendor = RSTRING_PTR(mrb_vendor);
  }else{
    return_vendor = "";
  }
  vst_strncpy (text, return_vendor, kVstMaxVendorStrLen);
  return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 MRubyVst::getVendorVersion () {
  return 0;
  mrb_value mrb_version = mrb_funcall(mrb, vst_instance, "version", 0);
  if (!mrb_nil_p(mrb_version)) {
    return mrb_fixnum(mrb_version);
  }else{
    return 0;
  }
}

//-----------------------------------------------------------------------------------------
void MRubyVst::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "process_replacing"))){
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    mrb_value mrb_inputs = mrb_ary_new(mrb);
    mrb_value mrb_input_1 = mrb_ary_new(mrb);
    mrb_value mrb_input_2 = mrb_ary_new(mrb);

    for (int i=0;i<sampleFrames;i++) {
      mrb_ary_push(mrb, mrb_input_1, mrb_float_value(mrb, (*in1++)));
      mrb_ary_push(mrb, mrb_input_2, mrb_float_value(mrb, (*in2++)));
    }

    mrb_ary_push(mrb, mrb_inputs, mrb_input_1);
    mrb_ary_push(mrb, mrb_inputs, mrb_input_2);

    mrb_value mrb_outputs = mrb_funcall(mrb, vst_instance, "process_replacing", 1, mrb_inputs);

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
//-----------------------------------------------------------------------------------------
/*
void MRubyVst::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) {
  if(mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "stereo_process"))){
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    for (int i=0;i<sampleFrames;i++) {
      mrb_value mrb_left_in  = mrb_float_value(mrb, in1[i]);
      mrb_value mrb_right_in = mrb_float_value(mrb, in2[i]);

      mrb_value mrb_outputs = mrb_funcall(mrb, vst_instance, "stereo_process", 2, mrb_left_in, mrb_right_in);

      if (!mrb_nil_p(mrb_outputs)) {
        mrb_value mrb_left_out  = mrb_ary_shift(mrb, mrb_outputs);
        mrb_value mrb_right_out = mrb_ary_shift(mrb, mrb_outputs);

        out1[i] = mrb_float(mrb_left_out);
        out2[i] = mrb_float(mrb_right_out);
      }
    }
  }
}
*/
//-----------------------------------------------------------------------------------------
void MRubyVst::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames) {
}

//-------------------------------------------------------------------------------------------------------
void MRubyVst::log(const char* txt){
  FILE *f;
  f = fopen("/tmp/vst_log", "a");
  fprintf(f, "%s\n", txt);
  fclose(f);
}

