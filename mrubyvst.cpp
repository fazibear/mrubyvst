#include "mrubyvst.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
  return new MRubyVst(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
MRubyVst::MRubyVst(audioMasterCallback audioMaster): AudioEffectX(audioMaster, PROGRAMS_COUNT, PARAMETERS_COUNT) {
  setUniqueID(666);
  canProcessReplacing();

  setNumInputs(2);
  setNumOutputs(2);

  mrb = mrb_open();

  FILE *file = fopen(SCRIPT_PATH, "r");
  if (file != NULL) {
    mrb_load_file(mrb, file);

    mrb_value vst_class = mrb_vm_const_get(mrb, mrb_intern_lit(mrb, VST_CLASS));
    mrb_const_set(mrb, vst_class, mrb_intern_lit(mrb, "PROGRAMS_COUNT"), mrb_fixnum_value(PROGRAMS_COUNT));
    mrb_const_set(mrb, vst_class, mrb_intern_lit(mrb, "PARAMETERS_COUNT"), mrb_fixnum_value(PARAMETERS_COUNT));
    mrb_const_set(mrb, vst_class, mrb_intern_lit(mrb, "SAMPLE_RATE"), mrb_float_value(mrb, getSampleRate()));
    mrb_const_set(mrb, vst_class, mrb_intern_lit(mrb, "SCRIPT_PATH"), mrb_str_new_cstr(mrb, (SCRIPT_PATH)));

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
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "change_program"))){
    mrb_funcall(mrb, vst_instance, "change_program", 1, mrb_fixnum_value(index));
  }
  m.unlock();
  curProgram = index;
  updateDisplay();
}

//-----------------------------------------------------------------------------------------
bool MRubyVst::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "program_name"))){
    mrb_value mrb_name = mrb_funcall(mrb, vst_instance, "program_name", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_name)) {
      vst_strncpy(text, RSTRING_PTR(mrb_name), kVstMaxProgNameLen);
      m.unlock();
      return true;
    }
  }

  m.unlock();
  return false;
}

//-----------------------------------------------------------------------------------------
void MRubyVst::setParameter(VstInt32 index, float value) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "set_parameter"))){
    mrb_funcall(mrb, vst_instance, "set_parameter", 2, mrb_fixnum_value(index), mrb_float_value(mrb, value));
  }
  m.unlock();
}

//-----------------------------------------------------------------------------------------
float MRubyVst::getParameter(VstInt32 index) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "parameter_value"))){
    mrb_value mrb_param_value = mrb_funcall(mrb, vst_instance, "parameter_value", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_param_value)) {
      m.unlock();
      return mrb_float(mrb_param_value);
    }
  }
  m.unlock();
  return 0;
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterName(VstInt32 index, char* label) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "parameter_name"))){
    mrb_value mrb_label = mrb_funcall(mrb, vst_instance, "parameter_name", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_label)) {
      vst_strncpy(label, RSTRING_PTR(mrb_label), kVstMaxParamStrLen);
    }
  }
  m.unlock();
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterDisplay(VstInt32 index, char* text) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "parameter_display_value"))){
    mrb_value mrb_display = mrb_funcall(mrb, vst_instance, "parameter_display_value", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_display)) {
      vst_strncpy(text, RSTRING_PTR(mrb_display), kVstMaxParamStrLen);
    }
  }
  m.unlock();
}

//-----------------------------------------------------------------------------------------
void MRubyVst::getParameterLabel(VstInt32 index, char* label) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "parameter_label"))){
    mrb_value mrb_label = mrb_funcall(mrb, vst_instance, "parameter_label", 1, mrb_fixnum_value(index));
    if (!mrb_nil_p(mrb_label)) {
      vst_strncpy(label, RSTRING_PTR(mrb_label), kVstMaxParamStrLen);
    }
  }
  m.unlock();
}

//------------------------------------------------------------------------
bool MRubyVst::getEffectName(char* name) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "effect_name"))){
    mrb_value mrb_name = mrb_funcall(mrb, vst_instance, "effect_name", 0);
    if (!mrb_nil_p(mrb_name)) {
      vst_strncpy(name, RSTRING_PTR(mrb_name), kVstMaxEffectNameLen);
      m.unlock();
      return true;
    }
  }
  m.unlock();
  return false;
}

//------------------------------------------------------------------------
bool MRubyVst::getProductString(char* text) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "product"))){
    mrb_value mrb_product = mrb_funcall(mrb, vst_instance, "product", 0);
    if (!mrb_nil_p(mrb_product)) {
      vst_strncpy(text, RSTRING_PTR(mrb_product), kVstMaxProductStrLen);
      m.unlock();
      return true;
    }
  }
  m.unlock();
  return false;
}

//------------------------------------------------------------------------
bool MRubyVst::getVendorString(char* text) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "vendor"))){
    mrb_value mrb_vendor = mrb_funcall(mrb, vst_instance, "vendor", 0);
    if (!mrb_nil_p(mrb_vendor)) {
      vst_strncpy(text, RSTRING_PTR(mrb_vendor), kVstMaxVendorStrLen);
      m.unlock();
      return true;
    }
  }
  m.unlock();
  return false;
}

//-----------------------------------------------------------------------------------------
VstInt32 MRubyVst::getVendorVersion () {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "version"))){
    mrb_value mrb_version = mrb_funcall(mrb, vst_instance, "version", 0);
    if (!mrb_nil_p(mrb_version)) {
      m.unlock();
      return mrb_fixnum(mrb_version);
    }
  }
  m.unlock();
  return 0;
}

//-----------------------------------------------------------------------------------------
void MRubyVst::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) {
  m.lock();
  if(!mrb_nil_p(vst_instance) && mrb_respond_to(mrb, vst_instance, mrb_intern_lit(mrb, "process"))){
    int ai = mrb_gc_arena_save(mrb);

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

    mrb_value mrb_outputs = mrb_funcall(mrb, vst_instance, "process", 1, mrb_inputs);

    //mrb_value mrb_outputs = mrb_inputs;

    if (!mrb_nil_p(mrb_outputs)) {
      mrb_value mrb_output_1 = mrb_ary_shift(mrb, mrb_outputs);
      mrb_value mrb_output_2 = mrb_ary_shift(mrb, mrb_outputs);

      for (int i=0;i<sampleFrames;i++) {
        (*out1++) = mrb_float(mrb_ary_shift(mrb, mrb_output_1));
        (*out2++) = mrb_float(mrb_ary_shift(mrb, mrb_output_2));
      }
    }
    mrb_gc_arena_restore(mrb, ai);
  }
  m.unlock();
}

//-------------------------------------------------------------------------------------------------------
void MRubyVst::log(const char* txt){
  FILE *f;
  f = fopen("/tmp/vst_log", "a");
  fprintf(f, "%s\n", txt);
  fclose(f);
}
