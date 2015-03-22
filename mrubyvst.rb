# PROGRAMS_COUNT
# PARAMETERS_COUNT
# SCRIPT_PATH

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
  module Sandbox; end

  attr_reader :vendor, :product, :effect_name, :version

  def initialize
    @vendor = 'Mruby'
    @product = 'MrubyVST'
    @effect_name = 'MRubyEffects'
    @version = 0
    @programs_dir = "#{File.dirname(SCRIPT_PATH)}/programs"
  end

  def programs
    Dir.entries(@programs_dir) - ['.', '..']
  end

  def load_program(path)
    Module.new.instance_eval(File.open(path).read)
  end

  def change_program(index)
    @program = load_program("#{@programs_dir}/#{programs[index]}") if programs[index]
  end

  def program_name(index)
    programs[index].to_s
  end

  def set_parameter(index, value)
    @program.set_parameter(index, value) if @program
  end

  def parameter_name(index)
    @program.parameter_name(index) if @program
  end

  def parameter_value(index)
    @program.parameter_value(index) if @program
  end

  def parameter_display_value(index)
    @program.parameter_display_value(index) if @program
  end

  def parameter_label(index)
    @program.parameter_label(index) if @program
  end

  def process(data)
    @program.process(data) if @program
  end

  def log(str)
    io = File.open('/tmp/mrubyvst.log', 'a')
    io.write(str + "\n")
    io.close
  end
end
