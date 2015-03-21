# PROGRAMS_COUNT
# PARAMETERS_COUNT

# effect_name() -> String
# product()     -> String
# vendor()      -> String
# version()     -> Fixnum

# change_program(index) -> Nil
# program_name(index)   -> String

# set_paramter(index, value)     -> Nil
# parameter_name(index)          -> String
# parameter_value(index)         -> Float
# parameter_display_value(index) -> String
# parameter_label(index)         -> String
# process(data)                  -> Array

class MRubyVST
  attr_reader :vendor, :product, :effect_name, :version

  def initialize
    @vendor = ''
    @product = ''
    @effect_name = ''
    @version = 0
  end

  def change_program(index)
  end

  def program_name(index)
  end

  def set_parameter(index, value)
  end

  def parameter_name(index)
  end

  def parameter_value(index)
  end

  def parameter_display_value(index)
  end

  def parameter_label(index)
  end

  def process(data)
    data
  end
end
